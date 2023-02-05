#include "stdafx.h"
#include "..\Public\SionSkills.h"

CSionSkills::CSionSkills(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBullet(pDevice, pContext)
{
}

CSionSkills::CSionSkills(const CSionSkills & rhs)
	: CBullet(rhs)
{
}

HRESULT CSionSkills::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSionSkills::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

int CSionSkills::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;

	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
		Tick_NormalAttack(fTimeDelta);
		break;
	}


	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CSionSkills::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CSionSkills::Ready_Components(void * pArg)
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



void CSionSkills::Tick_NormalAttack(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	m_pTransformCom->Go_PosTarget(fTimeDelta, m_BulletDesc.vTargetPosition);
	//m_pTransformCom->Go_Straight(fTimeDelta);
	return;
}

CSionSkills * CSionSkills::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSionSkills* pInstance = new CSionSkills(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSionSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSionSkills::Clone(void * pArg)
{
	CSionSkills* pInstance = new CSionSkills(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSionSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSionSkills::Free()
{
	__super::Free();
}
