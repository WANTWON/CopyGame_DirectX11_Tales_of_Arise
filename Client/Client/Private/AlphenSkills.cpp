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

	_matrix mWorldMatrix = XMMatrixIdentity();
	_vector vOffset =	{ 0.f, 0.f, 0.f, 0.f };
	_vector vLocation = { 0.f, 0.f, 0.f, 0.f };
	_vector vDir =		{ 0.f, 0.f, 0.f, 0.f };

	

	switch (m_BulletDesc.eBulletType)
	{
		case BOOST_1:
		{
			m_bIsActiveAtActionCamera = true;

			mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();

			CEffect::PlayEffectAtLocation(TEXT("Alphen_Boost_Wind_1.dat"), mWorldMatrix);

			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Alphen_Boost_1.dat"), mWorldMatrix);			
			for (auto& pEffect : m_pEffects)
			{
				if (!pEffect)
					continue;

				if (!wcscmp(pEffect->Get_PrototypeId(), TEXT("Akizame")))
				{
					_float4 vPosition;
					XMStoreFloat4(&vPosition, pEffect->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));

					mWorldMatrix.r[3] = XMLoadFloat4(&vPosition);

					CEffect::PlayEffectAtLocation(TEXT("Alphen_Boost_Particles_1.dat"), mWorldMatrix);
					break;
				}
			}

			break;
		}
		case BOOST_2:
		{
			m_bIsActiveAtActionCamera = true;

			mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Alphen_Boost_2.dat"), mWorldMatrix);

			for (auto& pEffect : m_pEffects)
			{
				if (!pEffect)
					continue;

				if (!wcscmp(pEffect->Get_PrototypeId(), TEXT("Akizame")))
				{
					_float4 vPosition;
					XMStoreFloat4(&vPosition, pEffect->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));

					mWorldMatrix.r[3] = XMLoadFloat4(&vPosition);

					CEffect::PlayEffectAtLocation(TEXT("Alphen_Boost_Particles_2.dat"), mWorldMatrix);
					break;
				}
			}
			break;
		}
	}

	return S_OK;
}

int CAlphenSkills::Tick(_float fTimeDelta)
{
	_int iSuperTick = __super::Tick(fTimeDelta);

	if (iSuperTick == OBJ_DEAD)
	{
		Dead_Effect();
		return OBJ_DEAD;
	}

	if (iSuperTick == OBJ_NOSHOW)
		return OBJ_NOSHOW;

	switch (m_BulletDesc.eBulletType)
	{
		case BOOST_1:
		case BOOST_2:
		{
			Tick_Boost(fTimeDelta);
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
		case BOOST_1:
		case BOOST_2:
		{
			if (m_fTime >= m_BulletDesc.fDeadTime)
				m_bDead = true;
			break;
		}
	}
}

void CAlphenSkills::Collision_Check()
{
	if (Check_Exception() == false)
		return;

	CBaseObj* pCollisionTarget = nullptr;
	switch (m_BulletDesc.eBulletType)
	{
		case BOOST_1:
		case BOOST_2:
			m_HitLagDesc.bLockOnChange = false;
			m_HitLagDesc.bHitLag = true;
			m_HitLagDesc.fHitLagTimer = 0.1f;
			m_HitLagDesc.bShaking = true;
			m_HitLagDesc.fShakingPower = 2.f;
			m_HitLagDesc.fShakingMinusPower = 0.2f;
			m_HitLagDesc.bZoom = true;
			m_HitLagDesc.fZoomDistance = 3.f;
			m_HitLagDesc.fZoomSpeed = 0.5f;
			m_HitLagDesc.fBlurPower = 6.f;
			m_HitLagDesc.fBlurDetail = 10.f;

			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
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
		case BOOST_1:
		case BOOST_2:
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
		case BOOST_1:
		case BOOST_2:
		{
			ColliderDesc.vScale = _float3(20.f, 20.f, 20.f);
			ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
			ColliderDesc.vPosition = _float3(0.f, 0.f, 10.f);

			if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
				return E_FAIL;

			break;
		}
	}

	return S_OK;
}
void CAlphenSkills::Tick_Boost(_float fTimeDelta)
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
		ERR_MSG(TEXT("Failed to Create: CAlphenSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAlphenSkills::Clone(void * pArg)
{
	CAlphenSkills* pInstance = new CAlphenSkills(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Clone: CAlphenSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAlphenSkills::Free()
{
	__super::Free();
}