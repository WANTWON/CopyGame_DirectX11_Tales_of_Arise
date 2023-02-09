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
	_vector vDir = { 0.f,0.f,0.f,0.f };

	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalbullet.dat"), mWorldMatrix);
		break;
	case GLACIA:
		vLocation = m_BulletDesc.vInitPositon;
		//mWorldMatrix.r[]
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);



	case BOOST:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Sion_BoostBlast.dat"), mWorldMatrix);
		break;
	case GRAVITY:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("GravitasFieldBlast.dat"), mWorldMatrix);
	

		m_pSmoke = CEffect::PlayEffectAtLocation(TEXT("GravitasFieldSmoke.dat"), mWorldMatrix);

		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("GravitasFieldBall.dat"), mWorldMatrix);
		break;
	case GRAVITY_DEAD:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		vDir = XMVectorSetY(XMVector3Normalize( vLocation - XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition())), 0.f);
		vDir = XMVectorSetW(vDir, 0.f);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation + vDir*2.f;
		m_pSmoke = CEffect::PlayEffectAtLocation(TEXT("GravitasDeadSmoke.dat"), mWorldMatrix);

		mWorldMatrix.r[3] = vLocation + vOffset;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("GravitasDead.dat"), mWorldMatrix);

		break;
	case MAGNA_RAY:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("MagnaRayBurst.dat"), mWorldMatrix);


		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("MagnaRayBullet.dat"), mWorldMatrix);
		m_pTransformCom->LookDir(m_BulletDesc.vTargetDir);
		break;
	case TRESVENTOS:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("TresVentusStart.dat"), mWorldMatrix);
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("TresVentosBullet.dat"), mWorldMatrix);
		m_pSmoke = CEffect::PlayEffectAtLocation(TEXT("TresVentosSmoke.dat"), mWorldMatrix);
		m_pTransformCom->LookDir(m_BulletDesc.vTargetDir);
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
	case GLACIA:
		Tick_GlaciaAttack(fTimeDelta);
	case BOOST:
	case GRAVITY_DEAD:
	case MAGNA_RAY:
		Tick_BoostAttack(fTimeDelta);
		break;
	case GRAVITY:
		Tick_GravityAttack(fTimeDelta);
		break;
	case TRESVENTOS:
		Tick_TresVentos(fTimeDelta);
		break;
	}

#ifdef _DEBUG
	if (m_pAABBCom != nullptr)
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pOBBCom != nullptr)
		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pSPHERECom != nullptr)
		m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
#endif

	return OBJ_NOEVENT;
}

void CSionSkills::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
	case TRESVENTOS:
		break;
	case BOOST:
	case MAGNA_RAY:
	case GRAVITY:
		if (m_fTime >= m_BulletDesc.fDeadTime)
			m_bDead = true;
		break;
	}

	
}

void CSionSkills::Collision_Check()
{
	CBaseObj* pCollisionTarget = nullptr;
	switch (m_BulletDesc.eBulletType)
	{
	case TRESVENTOS:
	case NORMALATTACK:
	case GRAVITY_DEAD:
		__super::Collision_Check();
		break;
	case BOOST:
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
		break;
	case MAGNA_RAY:
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pOBBCom, &pCollisionTarget))
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
		break;
	case GRAVITY:
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
		{
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
			_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - pCollisionTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

			if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
				vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
			else
				vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);

			pCollisionTarget->Get_Transform()->Go_PosDir(0.1f, vDirection);
		}
		break;
	}
}

void CSionSkills::Dead_Effect()
{
	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
	{
		if (!m_pEffects.empty())
		{
			for (auto& iter : m_pEffects)
			{
				if (iter != nullptr)
				{
					iter->Set_Dead(true);
				}
			}
		}
		m_pEffects.clear();

		_vector vOffset = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION));
		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset*1.5;
		_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pDeadEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalBulletDead.dat"), mWorldMatrix);
		break;
	}
	case TRESVENTOS:
	{
		if (!m_pEffects.empty())
		{
			for (auto& iter : m_pEffects)
			{
				if (iter != nullptr)
				{
					iter->Set_Dead(true);
				}
			}
		}

		if (!m_pSmoke.empty())
		{
			for (auto& iter : m_pSmoke)
			{
				if (iter != nullptr)
				{
					iter->Set_Dead(true);
				}
			}
		}
		m_pEffects.clear();
		m_pSmoke.clear();
		break;
	}
	case BOOST:
	case GRAVITY_DEAD:
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
	case GRAVITY:
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

		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 1.f;
		BulletDesc.eBulletType = CSionSkills::GRAVITY_DEAD;
		BulletDesc.iDamage = 300;
		BulletDesc.fDeadTime = 1.f;
		BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_TRANSLATION);
		BulletDesc.pOwner = m_BulletDesc.pOwner;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		break;

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
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
		break;
	case TRESVENTOS:
		ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
		break;
	case BOOST:
		ColliderDesc.vScale = _float3(30, 30, 30);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 15.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
		break;
	case GLACIA:
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	case GRAVITY_DEAD:
	case GRAVITY:
		ColliderDesc.vScale = _float3(10.f, 10.f, 10.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
		break;
	case MAGNA_RAY:
		ColliderDesc.vScale = _float3(5.f, 5.f, 30.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 15.f);
		if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
			return E_FAIL;
		break;
	}


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

void CSionSkills::Tick_GlaciaAttack(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	m_pTransformCom->Go_PosTarget(fTimeDelta, m_BulletDesc.vTargetPosition);
}

void CSionSkills::Tick_BoostAttack(_float fTimeDelta)
{
	m_fTime += fTimeDelta;


	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}
}

void CSionSkills::Tick_GravityAttack(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	m_fTime += fTimeDelta;
	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}

	for (auto& iter : m_pSmoke)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
		{
			_vector vEffectPosition = Get_TransformState(CTransform::STATE_TRANSLATION);
			_vector vDir = XMVectorSetY(XMVector3Normalize(vEffectPosition - XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition())), 0.f);
			iter->Set_State(CTransform::STATE_TRANSLATION, vEffectPosition + vDir*2.f);
		}
		
	}

}

void CSionSkills::Tick_TresVentos(_float fTimeDelta)
{
	CBaseObj* pTarget = m_BulletDesc.pTarget;

	if (pTarget == nullptr)
		pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
	if (pTarget == nullptr)
	{
		m_bDead = true;
		return;
	}
		
	CTransform* pTargetTransform = pTarget->Get_Transform();

	//_vector vTargetDir = XMVector3Normalize( XMVectorSetY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION),3.f) - Get_TransformState(CTransform::STATE_TRANSLATION));
	//
	//
	//_vector vLook = XMVector3Normalize(pTargetTransform->Get_State(CTransform::STATE_LOOK));

	//m_pTransformCom->Go_PosDir( fTimeDelta,XMVectorSet(0.f, XMVectorGetY(vTargetDir), 0.f, 0.f));

	//vLook = XMVectorSetY(vLook, 0.f);
	//vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	//_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	//if (fDot < 0.9f)
	//	pTargetTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.5f);

	//m_pTransformCom->Go_PosTarget(fTimeDelta, pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	
	m_pTransformCom->LookAt(XMVectorSetY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f));
	m_pTransformCom->Go_Straight(fTimeDelta);


	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
		{
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
		//	iter->Set_State(CTransform::STATE_RIGHT, Get_TransformState(CTransform::STATE_RIGHT));
		//	iter->Set_State(CTransform::STATE_UP, Get_TransformState(CTransform::STATE_UP));
		//	iter->Set_State(CTransform::STATE_LOOK, Get_TransformState(CTransform::STATE_LOOK));
		}
			
	}

	for (auto& iter : m_pSmoke)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
		{
			_vector vEffectPosition = Get_TransformState(CTransform::STATE_TRANSLATION);
			_vector vDir = XMVectorSetY(XMVector3Normalize(vEffectPosition - XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition())), 0.f);
			iter->Set_State(CTransform::STATE_TRANSLATION, vEffectPosition + vDir*2.f);
		}

	}
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
