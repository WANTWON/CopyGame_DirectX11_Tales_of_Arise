#include "stdafx.h"
#include "..\Public\ShootingObject.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Level_Restaurant.h"
#include "Level_WorkTool.h"

CShootingObject::CShootingObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInteractObject(pDevice, pContext)
{
	ZeroMemory(&m_tShootingDesc, sizeof(SHOOTINGDEC));
}

CShootingObject::CShootingObject(const CShootingObject & rhs)
	: CInteractObject(rhs)
	, m_tShootingDesc(rhs.m_tShootingDesc)
{
}

HRESULT CShootingObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CShootingObject::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tShootingDesc, pArg, sizeof(SHOOTINGDEC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_tShootingDesc.tNonDesc.vScale.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_tShootingDesc.tNonDesc.vScale.y);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, m_tShootingDesc.tNonDesc.vScale.z);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&m_tShootingDesc.tNonDesc.vPosition), 1.f));

	return S_OK;
}

int CShootingObject::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);


	if (!m_bhit)
	{
		if (m_tShootingDesc.m_bGoRight)
			m_pTransformCom->Go_PosDir(fTimeDelta*m_tShootingDesc.fVelocity, XMVectorSet(1.f, 0.f, 0.f, 0.f));

		else
			m_pTransformCom->Go_PosDir(fTimeDelta*m_tShootingDesc.fVelocity, XMVectorSet(-1.f, 0.f, 0.f, 0.f));
	}
	



	_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	WorldMatrix.r[0] = XMVector4Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector4Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector4Normalize(WorldMatrix.r[2]);

	m_pSPHERECom->Update(WorldMatrix);

	return OBJ_NOEVENT;
}

void CShootingObject::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	__super::Late_Tick(fTimeDelta);

	if (XMVectorGetX(Get_TransformState(CTransform::STATE_TRANSLATION)) > 42.f)
		m_bDead = true;


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EDGE_DETECTION, this);

	CBaseObj* pCollisionTarget = nullptr;
	CBaseObj* pBullet = nullptr;

	if (!CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MINIGAME2, m_pSPHERECom, &pCollisionTarget))
	{
		//Collision
		int a = 0;
	}

	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PBULLET, m_pSPHERECom, &pBullet))
	{
		//Collision
		_vector dir;
		dir =  Get_TransformState(CTransform::STATE_TRANSLATION) - pBullet->Get_TransformState(CTransform::STATE_TRANSLATION);
		//
		m_vDir = XMVector3Normalize(dir);
		pBullet->Set_Dead(true);
		//m_pTransformCom->Go_PosDir(fTimeDelta, Get_TransformState(CTransform::STATE_TRANSLATION) - pBullet->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_bhit = true;
		//m_tShootingDesc.m_iScore;
		dynamic_cast<CLevel_WorkTool*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Increase_Score(m_tShootingDesc.m_iScore);

		//int a = 0;
	}

	if (m_bhit)
	{
		m_fDeadtimer += fTimeDelta;
		if (m_fDeadtimer > 4.f)
			m_bDead = true;
			
		switch (m_tShootingDesc.eType)
		{
		case TYPE_A:
	//		m_pTransformCom->Go_PosDir(fTimeDelta*2.5, m_vDir);
			m_pTransformCom->Go_PosDir(fTimeDelta*2.5, m_vDir);
			m_pTransformCom->Jump(fTimeDelta, m_tShootingDesc.fVelocity, 2.f, Get_TransformState(CTransform::STATE_TRANSLATION).m128_f32[1]);
			m_pTransformCom->Turn(Get_TransformState(CTransform::STATE_UP), fTimeDelta*4.f);
			break;

		case TYPE_B:
			m_pTransformCom->Go_PosDir(fTimeDelta*2.5, m_vDir);
			m_pTransformCom->Jump(fTimeDelta, m_tShootingDesc.fVelocity, 2.f, Get_TransformState(CTransform::STATE_TRANSLATION).m128_f32[1]);
	//		m_pTransformCom->Go_PosDir(fTimeDelta*2.5, m_vDir);
			m_pTransformCom->Turn(Get_TransformState(CTransform::STATE_RIGHT), fTimeDelta*4.f);
			break;

		case TYPE_C:
			m_pTransformCom->Go_PosDir(fTimeDelta*2.5, m_vDir);
			m_pTransformCom->Jump(fTimeDelta, m_tShootingDesc.fVelocity, 2.f, Get_TransformState(CTransform::STATE_TRANSLATION).m128_f32[1]);
        	m_pTransformCom->Turn(Get_TransformState(CTransform::STATE_LOOK), fTimeDelta*4.f);
			break;
				
		}
	}
	
}

HRESULT CShootingObject::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CShootingObject::Render_EdgeDetection()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 7)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShootingObject::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_tShootingDesc.tNonDesc.pModeltag, (int)strlen(m_tShootingDesc.tNonDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

_bool CShootingObject::Is_AnimationLoop(_uint eAnimId)
{
	return false;
}

HRESULT CShootingObject::SetUp_ShaderID()
{
	
	m_eShaderID = SHADER_NONANIMDEFAULT;

	return S_OK;
}

CShootingObject * CShootingObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CShootingObject* pInstance = new CShootingObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CShootingObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShootingObject::Clone(void * pArg)
{
	CShootingObject* pInstance = new CShootingObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CShootingObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShootingObject::Free(void)
{
	__super::Free();
}
