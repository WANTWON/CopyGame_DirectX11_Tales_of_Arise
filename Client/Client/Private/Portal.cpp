#include "stdafx.h"
#include "..\Public\Portal.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Level.h"
#include "UI_InterectMsg.h"
#include "Effect.h"

CPortal::CPortal(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CPortal::CPortal(const CPortal & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CPortal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortal::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_PortalDesc, pArg, sizeof(PORTALDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&m_PortalDesc.m_ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(m_PortalDesc.m_ModelDesc.vScale);

	}

	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_TRIGGER, this);



	return S_OK;
}

int CPortal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	_vector vDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) -Get_TransformState(CTransform::STATE_TRANSLATION));
	vDir = XMVectorSetY(vDir, 0.f);
	m_pTransformCom->LookDir(vDir);

	if (!m_bFirst)
	{

		//_vector vOffset = { 0.f,4.f,0.f,0.f };
		_matrix mWorldMatrix = XMMatrixIdentity();
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		//mWorldMatrix.r[3] += vOffset;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Portal.dat"), mWorldMatrix);
		m_bFirst = true;
	}

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr)
		{
			//iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
			iter->Set_State(CTransform::STATE_LOOK, Get_TransformState(CTransform::STATE_LOOK));
			iter->Set_State(CTransform::STATE_RIGHT, Get_TransformState(CTransform::STATE_RIGHT));
			iter->Set_State(CTransform::STATE_UP, Get_TransformState(CTransform::STATE_UP));
		}

	}

	return OBJ_NOEVENT;
}

void CPortal::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (Check_IsinFrustum(2.f) == false)
		return;

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	
	if (m_pSPHERECom->Collision(pPlayer->Get_Collider()))
	{
		dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Open_sysmsg(3);
		if (CGameInstance::Get_Instance()->Key_Up(DIK_E))
		{
			CLevel* pCurrentLevel = CGameInstance::Get_Instance()->Get_CurrentLevel();
			pCurrentLevel->Set_NextLevel(true, m_PortalDesc.iNextLevel);
		}
	}
	else
		dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();

}

HRESULT CPortal::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(7.f, 7.f, 7.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPortal::SetUp_ShaderResources()
{
	return S_OK;
}

CPortal * CPortal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPortal* pInstance = new CPortal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPortal::Clone(void * pArg)
{
	CPortal* pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPortal"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPortal::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_TRIGGER, this);

}
