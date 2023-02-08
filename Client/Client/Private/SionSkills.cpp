#include "stdafx.h"
#include "..\Public\SionSkills.h"
#include "Monster.h"
#include "ParticleSystem.h"

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

	_vector vOffset = { 0.f,0.f,0.f,0.f };
	_vector vLocation = { 0.f,0.f,0.f,0.f };
	_matrix mWorldMatrix = XMMatrixIdentity();

	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalbullet.dat"), mWorldMatrix);
		break;
	case BOOST:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Sion_BoostBlast.dat"), mWorldMatrix);
		break;
	}

	return S_OK;
}

int CSionSkills::Tick(_float fTimeDelta)
{
	
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	if (m_bDead)
	{
		Dead_Effect();
		return OBJ_DEAD;
	}


	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
		Tick_NormalAttack(fTimeDelta);
		break;
	case BOOST:
		Tick_BoostAttack(fTimeDelta);
		break;
	}


	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CSionSkills::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

void CSionSkills::Collision_Check()
{
	CBaseObj* pCollisionTarget = nullptr;
	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
		__super::Collision_Check();
		break;
	case BOOST:
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
		break;
	}
}

void CSionSkills::Dead_Effect()
{
	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
	{
		_vector vOffset = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION));
		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset*1.5;
		_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pDeadEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalBulletDead.dat"), mWorldMatrix);
		break;
	}
	case BOOST:
	{
		if (!m_pEffects.empty())
		{
			for (auto& iter : m_pEffects)
			{
				if (iter != nullptr)
				{
					CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
					if (pParticleSystem != nullptr)
						pParticleSystem->Set_Stop(true);

				}
					
			}
		}
		break;
	}

	}
}

HRESULT CSionSkills::Ready_Components(void * pArg)
{

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));


	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		break;
	case BOOST:
		ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 5.f);
		break;
	}

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

	for (auto& iter : m_pEffects)
	{
		iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	return;
}

void CSionSkills::Tick_BoostAttack(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime >= m_BulletDesc.fDeadTime)
		m_bDead = true;
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
