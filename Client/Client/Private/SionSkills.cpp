#include "stdafx.h"
#include "..\Public\SionSkills.h"
#include "Monster.h"
#include "Player.h"


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
		m_BulletDesc.vTargetPosition = XMVectorSetY(m_BulletDesc.vTargetPosition, XMVectorGetY(m_BulletDesc.vTargetPosition) + 3.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalbullet.dat"), mWorldMatrix);
		break;
	case GLACIA:
		vLocation = m_BulletDesc.vInitPositon;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		m_pTransformCom->LookAt(m_BulletDesc.vTargetPosition);

		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("GlacioBullet.dat"), mWorldMatrix);
		break;
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

	case AQUA_LUINA:
		vLocation = m_BulletDesc.vTargetPosition;
		//mWorldMatrix.r[]
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		break;
	case AQUA_LUINA_BULLET:
		vLocation = m_BulletDesc.vTargetPosition;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);

		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("AquaImpact.dat"), mWorldMatrix);
		break;

	case EXPLOSION:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		
		mWorldMatrix.r[3] = vLocation + vOffset;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("ExplosionBall.dat"), mWorldMatrix);
		break;
	case EXPLOSIONGROUND:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("ExplosionGround.dat"), mWorldMatrix);
		break;
	}

	return S_OK;
}

int CSionSkills::Tick(_float fTimeDelta)
{
	_int iSuperTick = __super::Tick(fTimeDelta);

	if (iSuperTick == OBJ_DEAD)
	{
		Dead_Effect();
		return OBJ_DEAD;
	}

	if (iSuperTick == OBJ_NOSHOW)
		return OBJ_NOSHOW;


	_float fTime = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));

	if (Check_Exception() == false)
		return OBJ_NOEVENT;

	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
		Tick_NormalAttack(fTime);
		break;
	case GLACIA:
		Tick_GlaciaAttack(fTime);
		break;
	case BOOST:
	case GRAVITY_DEAD:
	case GLACIA_DEAD:
	case MAGNA_RAY:
		Tick_BoostAttack(fTime);
		break;
	case GRAVITY:
		Tick_GravityAttack(fTime);
		break;
	case TRESVENTOS:
		Tick_TresVentos(fTime);
		break;
	case AQUA_LUINA:
		Tick_AQUA_LUINA(fTime);
		break;
	case AQUA_LUINA_BULLET:
		Tick_AQUA_LUINA_BULLET(fTime);
		break;
	case EXPLOSION:
		Tick_Explosion(fTime);
		break;
	case NAILBULLET:
		Tick_NailBullet(fTime);
		break;
	}

	if (m_pAABBCom != nullptr)
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pOBBCom != nullptr)
		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pSPHERECom != nullptr)
		m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CSionSkills::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;


	__super::Late_Tick(fTimeDelta);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	if (Check_Exception() == false)
		return;

	switch (m_BulletDesc.eBulletType)
	{
	case NORMALATTACK:
		if (m_fTime >= m_BulletDesc.fDeadTime || XMVectorGetY(Get_TransformState(CTransform::STATE_TRANSLATION)) < 0.5f)
			m_bDead = true;
		break;
	case TRESVENTOS:
		break;
	case BOOST:
	case MAGNA_RAY:
	case GRAVITY:
	case AQUA_LUINA_BULLET:
	case EXPLOSIONGROUND:
	case NAILBULLET:
		if (m_fTime >= m_BulletDesc.fDeadTime)
			m_bDead = true;
		break;
	case GLACIA:
		if (!m_bMadeSmoke&& XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - m_BulletDesc.vTargetPosition)) < 1.f)
		{
			m_bMadeSmoke = true;
			_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
			_vector vRight = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_RIGHT));
			_matrix mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
			_vector vDir = XMVectorSetY(XMVector3Normalize(vLocation - XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition())), 0.f);
			mWorldMatrix.r[3] = vLocation + vDir* 2.f;
			m_pSmoke = CEffect::PlayEffectAtLocation(TEXT("GlacioDeadSmoke.dat"), mWorldMatrix);
		}
		break;
	case EXPLOSION:
		if (XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - m_BulletDesc.vTargetPosition)) < 1.f)
		{
			m_fTime += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));;

			if (m_fTime > 3.f)
				m_bDead = true;
		}
		else
		{
			m_fExplosionGroundTimer += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));;

			if (m_fExplosionGroundTimer > 5.f)
			{
				m_fExplosionGroundTimer = 0.f;
				CBullet::BULLETDESC BulletDesc;
				_vector vLocation = { 0.f,0.f,0.f,0.f };
				BulletDesc.iDamage = rand() % 150 + 1;
				BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
				BulletDesc.eCollisionGroup = PLAYER;
				BulletDesc.pOwner = m_BulletDesc.pOwner;
				BulletDesc.eBulletType = CSionSkills::EXPLOSIONGROUND;
				BulletDesc.fDeadTime = 0.5f;

				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
					return;
			}

		}
		break;
	}

}

void CSionSkills::Collision_Check()
{
	
	if (Check_Exception() == false)
		return;

	CBaseObj* pCollisionTarget = nullptr;
	switch (m_BulletDesc.eBulletType)
	{
	case TRESVENTOS:
	case NORMALATTACK:
		m_HitLagDesc.bLockOnChange = true;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.1f;
		m_HitLagDesc.bShaking = false;
		m_HitLagDesc.fShakingPower = 2.f;
		m_HitLagDesc.fShakingMinusPower = 0.2f;
		__super::Collision_Check();
		break;
	case GRAVITY_DEAD:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.3f;
		m_HitLagDesc.bShaking = true;
		m_HitLagDesc.fShakingPower = 2.f;
		m_HitLagDesc.fShakingMinusPower = 0.2f;
		__super::Collision_Check();
		break;
	case GLACIA_DEAD:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.3f;
		m_HitLagDesc.bShaking = true;
		m_HitLagDesc.fShakingPower = 2.f;
		m_HitLagDesc.fShakingMinusPower = 0.2f;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
		{
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
			m_bDead = true;
		}
		break;
	case AQUA_LUINA_BULLET:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bShaking = true;
		m_HitLagDesc.fShakingPower = 0.4f;
		m_HitLagDesc.fShakingMinusPower = 0.1f;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pAABBCom, &pCollisionTarget))
		{
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
			m_bDead = true;
		}
		break;
	case EXPLOSION:
	case GLACIA:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.1f;
		m_HitLagDesc.bShaking = true;
		m_HitLagDesc.fShakingPower = 0.4f;
		m_HitLagDesc.fShakingMinusPower = 0.1f;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
		break;
	case BOOST:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.1f;
		m_HitLagDesc.bShaking = true;
		m_HitLagDesc.fShakingPower = 2.f;
		m_HitLagDesc.fShakingMinusPower = 0.2f;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
		break;
	case MAGNA_RAY:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.1f;
		m_HitLagDesc.bShaking = false;

		switch (m_BulletDesc.eCollisionGroup)
		{
		case PLAYER:
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pOBBCom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
			break;

		default:
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pOBBCom, &pCollisionTarget))
				dynamic_cast<CPlayer*> (pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
		}
		break;
	case GRAVITY:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = true;
		m_HitLagDesc.fHitLagTimer = 0.1f;
		m_HitLagDesc.bShaking = false;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
		{
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, m_HitLagDesc);
			_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - pCollisionTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

			if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
				vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
			else
				vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);

			pCollisionTarget->Get_Transform()->Go_PosDir(CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_Object"))*0.5f, vDirection, dynamic_cast<CMonster*>(pCollisionTarget)->Get_Navigation());
		}
		break;
	case NAILBULLET:
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bShaking = false;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
		{
			_vector vDirection = pCollisionTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

			pCollisionTarget->Get_Transform()->Go_PosDir(0.05f, vDirection, dynamic_cast<CMonster*>(pCollisionTarget)->Get_Navigation());
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
	case NAILBULLET:
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
					//if (m_bTresventos == false)
					//{
					//	CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillSound_Jump_E_Hit.wav"), SOUND_EFFECT, 0.1f);
					//	m_bTresventos = true;
					//}
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
	case GLACIA:
	{
		_vector vHeightOffset = { 0.f,3.f,0.f,0.f };
		_vector vOffset = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION));
		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vHeightOffset;// -vOffset*1.5 ;
		_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();

		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("GlacioDead.dat"), mWorldMatrix);

		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 1.f;
		BulletDesc.eBulletType = CSionSkills::GLACIA_DEAD;
		BulletDesc.iDamage = 300;
		BulletDesc.fDeadTime = 1.f;
		BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_TRANSLATION);
		BulletDesc.pOwner = m_BulletDesc.pOwner;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		break;
	}
	case BOOST:
	case GRAVITY_DEAD:
	{
		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 3.f, 0.1f);
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
	case EXPLOSION:
		if (m_bExplosionSound == false)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillSound_Jump_E_Hit.wav"), SOUND_EFFECT, 0.1f);
			m_bExplosionSound = true;
		}

		if(CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 3.f, 0.1f);
		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		_vector vRight = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE::STATE_RIGHT));
		_matrix mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("ExplosionBurst.dat"), mWorldMatrix);
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
	{
		ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
	}
	case GRAVITY_DEAD:
	case GRAVITY:
	case GLACIA_DEAD:

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

	case AQUA_LUINA:
		ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
			return E_FAIL;
		break;

	case AQUA_LUINA_BULLET:
		ColliderDesc.vScale = _float3(0.5f, 6.f, 0.5f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
			return E_FAIL;
		break;

	case EXPLOSION:
		ColliderDesc.vScale = _float3(20.f, 20.f, 20.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
		break;

	case NAILBULLET:
		ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;

		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Nail"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
	}


void CSionSkills::Tick_NormalAttack(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	_vector vDir = XMVector3Normalize(m_BulletDesc.vTargetPosition - m_BulletDesc.vInitPositon);

	m_pTransformCom->LookAt(m_BulletDesc.vTargetPosition);
	m_pTransformCom->Go_PosDir(fTimeDelta, vDir);
	//m_pTransformCom->Go_Straight(fTimeDelta);

	for (auto& iter : m_pEffects)
	{
		if(iter != nullptr)
		iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	return;
}

void CSionSkills::Tick_GlaciaAttack(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	m_pTransformCom->Go_PosTarget(fTimeDelta, m_BulletDesc.vTargetPosition);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}
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

void CSionSkills::Tick_AQUA_LUINA(_float fTimeDelta)
{
	if (m_bDead == true)
		return;

	m_fAquaTImer += fTimeDelta;

	//if (m_fAquaTImer > 0.1f)
	//{
		float offsetx = (_float)(rand() % 70)*(rand()%2 == 0 ? 1.f : -1.f) / 10;
		float offsetz = (_float)(rand() % 70)*(rand() % 2 == 0 ? 1.f : -1.f) / 10;
		CBullet::BULLETDESC BulletDesc;
		_vector vLocation = { 0.f,0.f,0.f,0.f };
		BulletDesc.iDamage = rand() % 150 + 1;
		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.vTargetPosition.m128_f32[0] += offsetx;
		BulletDesc.vTargetPosition.m128_f32[2] += offsetz;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		BulletDesc.eBulletType = CSionSkills::AQUA_LUINA_BULLET;
		BulletDesc.fDeadTime = 0.5f;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
		bulletcount -= 1;
		m_fAquaTImer = 0.f;

		if (bulletcount <= 0)
			m_bDead = true;
	//}
	
}

void CSionSkills::Tick_AQUA_LUINA_BULLET(_float fTimeDelta)
{
	m_fAquaTImer += fTimeDelta;
	if (m_fAquaTImer > 0.5f)
		m_bDead = true;

	m_fTime += fTimeDelta;

}

void CSionSkills::Tick_Explosion(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	m_pTransformCom->Go_PosTarget(fTimeDelta, m_BulletDesc.vTargetPosition);


	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
		{
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
		}

	}
}

void CSionSkills::Tick_NailBullet(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	m_pTransformCom->LookDir(m_BulletDesc.vTargetDir);
	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}
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
