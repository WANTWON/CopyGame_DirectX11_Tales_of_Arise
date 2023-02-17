#include "stdafx.h"

#include "AlphenSkills.h"
#include "Monster.h"
#include "ParticleSystem.h"

CAlphenSkills::CAlphenSkills(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBullet(pDevice, pContext)
{
}

CAlphenSkills::CAlphenSkills(const CAlphenSkills & rhs)
	: CBullet(rhs)
{
}

HRESULT CAlphenSkills::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlphenSkills::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	_vector vOffset = { 0.f,0.f,0.f,0.f };
	_vector vLocation = { 0.f,0.f,0.f,0.f };
	_matrix mWorldMatrix = XMMatrixIdentity();
	_vector vDir = { 0.f,0.f,0.f,0.f };

	switch (m_BulletDesc.eBulletType)
	{
		case STRIKE_1:
		{
			vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
			mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation;
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Alphen_Strike_1.dat"), mWorldMatrix);
			break;
		}
		case STRIKE_2:
		{
			vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
			mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation;
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Alphen_Strike_2.dat"), mWorldMatrix);
			break;
		}
	}

	return S_OK;
}

int CAlphenSkills::Tick(_float fTimeDelta)
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
		case STRIKE_1:
		case STRIKE_2:
		{
			Tick_Strike(fTimeDelta);
			break;
		}
	}

	if (m_pAABBCom != nullptr)
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pOBBCom != nullptr)
		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pSPHERECom != nullptr)
		m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());


	return OBJ_NOEVENT;
}

void CAlphenSkills::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	switch (m_BulletDesc.eBulletType)
	{
		case STRIKE_1:
		{
			if (m_fTime >= m_BulletDesc.fDeadTime)
				m_bDead = true;
			break;
		}
	}
}

void CAlphenSkills::Collision_Check()
{
	CBaseObj* pCollisionTarget = nullptr;
	switch (m_BulletDesc.eBulletType)
	{
		case STRIKE_1:
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
			break;
	}
}

void CAlphenSkills::Dead_Effect()
{
	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	switch (m_BulletDesc.eBulletType)
	{
		case STRIKE_1:
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
			m_pEffects.clear();
			m_pSmoke.clear();
			break;
		}
	}
}

HRESULT CAlphenSkills::Ready_Components(void * pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	switch (m_BulletDesc.eBulletType)
	{
		case STRIKE_1:
		{
			ColliderDesc.vScale = _float3(30, 30, 30);
			ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
			ColliderDesc.vPosition = _float3(0.f, 0.f, 15.f);

			if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
				return E_FAIL;

			break;
		}
	}

	return S_OK;
}
void CAlphenSkills::Tick_Strike(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}
}

CAlphenSkills * CAlphenSkills::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAlphenSkills* pInstance = new CAlphenSkills(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CAlphenSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAlphenSkills::Clone(void * pArg)
{
	CAlphenSkills* pInstance = new CAlphenSkills(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CAlphenSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAlphenSkills::Free()
{
	__super::Free();
}