#include "stdafx.h"
#include "..\Public\RinwellSkills.h"

CRinwellSkills::CRinwellSkills(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBullet(pDevice, pContext)
{
}

CRinwellSkills::CRinwellSkills(const CRinwellSkills & rhs)
	: CBullet(rhs)
{
}

HRESULT CRinwellSkills::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRinwellSkills::Initialize(void * pArg)
{
	__super::Initialize(pArg);


	_vector vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
	_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

	_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	mWorldMatrix.r[3] = vLocation;

	m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBall.dat"), mWorldMatrix);
	
	return S_OK;
}

int CRinwellSkills::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;

	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
		Tick_PhotonFlash(fTimeDelta);
		break;
	}
	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CRinwellSkills::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CRinwellSkills::Ready_Components(void * pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 3.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



void CRinwellSkills::Tick_PhotonFlash(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	m_pTransformCom->Go_PosTarget(fTimeDelta, m_BulletDesc.vTargetPosition);
	//m_pTransformCom->Go_Straight(fTimeDelta);

	for (auto& iter : m_pEffects)
	{
		iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}

	return;
}

CRinwellSkills * CRinwellSkills::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRinwellSkills* pInstance = new CRinwellSkills(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRinwellSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRinwellSkills::Clone(void * pArg)
{
	CRinwellSkills* pInstance = new CRinwellSkills(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRinwellSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRinwellSkills::Free()
{
	__super::Free();

	for (auto& iter : m_pEffects)
	{
		iter->Set_Dead(true);
	}
}
