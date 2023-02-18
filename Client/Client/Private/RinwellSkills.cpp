#include "stdafx.h"
#include "..\Public\RinwellSkills.h"
#include "Monster.h"
#include "Player.h"

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

	_vector vOffset = { 0.f,0.f,0.f,0.f };
	_vector vLocation = { 0.f,0.f,0.f,0.f };
	_matrix mWorldMatrix = XMMatrixIdentity();

	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
		vOffset = m_BulletDesc.pOwner->Get_TransformState(CTransform::STATE_LOOK);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset*2.f;
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBall.dat"), mWorldMatrix);
		break;
	case GALE_FORCE:
		vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("GaleForce.dat"), mWorldMatrix);

		vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffects = CEffect::PlayEffectAtLocation(TEXT("GaleForceParticle.dat"), mWorldMatrix);

		vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlast2Effects = CEffect::PlayEffectAtLocation(TEXT("GaleForceParticle2.dat"), mWorldMatrix);
		break;
	case METEOR:
		vLocation = m_BulletDesc.vInitPositon;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("MeteorBall.dat"), mWorldMatrix);

		break;
	case DIVINE_SABER:
		vOffset = XMVectorSet(0.f, m_fRadius + 1.f, 0.f, 0.f);
		vLocation = m_BulletDesc.vTargetPosition + vOffset;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberRing.dat"), mWorldMatrix);

		vOffset = XMVectorSet(0.f, m_fRadius + 0.5f, 0.f, 0.f);
		vLocation = m_BulletDesc.vTargetPosition + vOffset;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffects = CEffect::PlayEffectAtLocation(TEXT("DivineFloor.dat"), mWorldMatrix);

		OriginLightDesc = *CGameInstance::Get_Instance()->Get_LightDesc(0);
		OriginLightDesc2 = *CGameInstance::Get_Instance()->Get_LightDesc(10);

		memcpy(&DevineLightDesc, &OriginLightDesc, sizeof(LIGHTDESC));
		memcpy(&DevineLightDesc2, &OriginLightDesc2, sizeof(LIGHTDESC));
		break;
	case DIVINE_SABER_BULLET:
	{
		vLocation = m_BulletDesc.vTargetPosition;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(rand() % 180));

		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;

		_int iRandNum = rand() % 4;
		if (iRandNum == 0)
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberBullet0.dat"), mWorldMatrix);
		if (iRandNum == 1)
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberBullet1.dat"), mWorldMatrix);
		if (iRandNum == 2)
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberBullet2.dat"), mWorldMatrix);
		if (iRandNum == 2)
			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("DivineSaberBullet3.dat"), mWorldMatrix);


		//m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("AquaImpact.dat"), mWorldMatrix);
		break;
	}
	case HOlY_RANCE:

		/*vLocation = m_BulletDesc.vInitPositon;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);*/

		break;
	case HOLY_RANCE_BULLET:
		vLocation = m_BulletDesc.vInitPositon;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		//m_BulletDesc.vTargetPosition = 

		break;
	case THUNDER_FIELD:
		vOffset = XMVectorSet(0.f, m_fRadius + 5.f, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(rand() % 180));
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("ThunderFieldBullet.dat"), mWorldMatrix);

		vOffset = XMVectorSet(0.f, m_fRadius + 4, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix.r[3] = vLocation;
		m_pBlastEffects = CEffect::PlayEffectAtLocation(TEXT("ThunderFieldAura.dat"), mWorldMatrix);
		vOffset = XMVectorSet(0.f, m_fRadius + 2.f, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		mWorldMatrix.r[3] = vLocation;
		m_pBlast2Effects = CEffect::PlayEffectAtLocation(TEXT("ThunderFieldFloor.dat"), mWorldMatrix);

		break;
	case METEORDEAD:
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("MeteorDeadFlash.dat"), mWorldMatrix);
		break;
	}

	
	return S_OK;
}

int CRinwellSkills::Tick(_float fTimeDelta)
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
	case PHOTON_FLASH:
		Tick_PhotonFlash(fTimeDelta);
		break;
	case METEORDEAD:
	case GALE_FORCE:
		Tick_GaleForce(fTimeDelta);
		break;
	case THUNDER_FIELD:
		Tick_ThunderField(fTimeDelta);
		break;
	case METEOR:
		Tick_Meteor(fTimeDelta);
		break;
	case DIVINE_SABER:
		Tick_DivineSaber(fTimeDelta);
		break;
	case DIVINE_SABER_BULLET:
		Tick_DivineSaberBullet(fTimeDelta);
		break;

	case HOlY_RANCE:
		Tick_HolyRance(fTimeDelta);
		break;

	case HOLY_RANCE_BULLET:
		Tick_HolyRanceBullet(fTimeDelta);
		break;

	}


	if(m_pAABBCom != nullptr)
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pSPHERECom != nullptr)
		m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pOBBCom != nullptr)
		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CRinwellSkills::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	switch (m_BulletDesc.eBulletType)
	{
	case METEOR:
		if (XMVectorGetY(Get_TransformState(CTransform::STATE_TRANSLATION)) <= 2.f)
			m_bDead = true;
		break;
	}

	if (XMVectorGetY(Get_TransformState(CTransform::STATE_TRANSLATION)) <= 0)
		m_bDead = true;
}

void CRinwellSkills::Collision_Check()
{
	CBaseObj* pCollisionTarget = nullptr;
	switch (m_BulletDesc.eBulletType)
	{
	case METEOR:
	case PHOTON_FLASH:
	case HOLY_RANCE_BULLET:
		__super::Collision_Check();
		break;
	case THUNDER_FIELD:
		if (m_BulletDesc.eCollisionGroup == PLAYER)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pAABBCom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
		}
		else
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAABBCom, &pCollisionTarget))
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
		}
		break;
	case GALE_FORCE:
		if (m_BulletDesc.eCollisionGroup == PLAYER)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
		}
		else
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionTarget))
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
		}
		break;

	case DIVINE_SABER_BULLET:
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pAABBCom, &pCollisionTarget))
		{
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
			m_bDead = true;
		}
		break;
	}
}

void CRinwellSkills::Dead_Effect()
{
	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

	}
	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
	{
		_vector vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
		_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pDeadEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashDead.dat"), mWorldMatrix);
		break;
	}
	case METEOR:
	{
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 1.f;
		BulletDesc.eBulletType = METEORDEAD;
		BulletDesc.iDamage = 300;
		BulletDesc.fDeadTime = 1.f;
		BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_TRANSLATION);
		BulletDesc.pOwner = m_BulletDesc.pOwner;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
		break;
	}
	case THUNDER_FIELD:
	{
		if (!m_pBlast2Effects.empty())
		{
			for (auto& iter : m_pBlast2Effects)
			{
				if (iter != nullptr)
				{
					iter->Set_Dead(true);
				}
			}
		}
		break;
	}

	}
}


HRESULT CRinwellSkills::Ready_Components(void * pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
		ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		break;
	case GALE_FORCE:
		ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 1.f);
		break;

	case METEOR:
		ColliderDesc.vScale = _float3(15.f, 5.f, 15.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 1.f);
		break;
	
	case THUNDER_FIELD:
		ColliderDesc.vScale = _float3(1.f, 10.f, 1.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 1.f);
		break;

	case DIVINE_SABER:
		ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 1.f);
		break;

	case DIVINE_SABER_BULLET:
		ColliderDesc.vScale = _float3(0.5f, 7.f, 0.5f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

	case HOlY_RANCE:
		ColliderDesc.vScale = _float3(0.5f, 7.f, 0.5f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

	case HOLY_RANCE_BULLET:
		ColliderDesc.vScale = _float3(0.5f, 7.f, 0.5f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		
		break;

	}

	if (m_BulletDesc.eBulletType == THUNDER_FIELD || m_BulletDesc.eBulletType == DIVINE_SABER || m_BulletDesc.eBulletType == DIVINE_SABER_BULLET)
	{
		if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
	}
	

	return S_OK;
}



void CRinwellSkills::Tick_PhotonFlash(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	
	return;
}

void CRinwellSkills::Tick_GaleForce(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fTime >= m_BulletDesc.fDeadTime)
		m_bDead = true;
}

void CRinwellSkills::Tick_Meteor(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;
	 
	//_vector vDir =  m_BulletDesc.vTargetDir;

	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}

}

void CRinwellSkills::Tick_ThunderField(_float fTimeDelta)
{
	if (m_bDeadEffect)
		m_bDead = true;

	//_vector vDir =  m_BulletDesc.vTargetDir;

	//m_pTransformCom->LookAt(m_BulletDesc.vTargetPosition);
	m_fThunderStopTimer += fTimeDelta;

	if(m_fThunderStopTimer < 4.f)
	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
		{
			_vector vOffset = XMVectorSet(0.f, m_fRadius + 6.f, 0.f, 0.f);
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION) + vOffset);
		}
			
	}

	for (auto& iter : m_pBlast2Effects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		_vector vOffset = XMVectorSet(0.f, m_fRadius + 0.5f, 0.f, 0.f);
		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION) + vOffset);
	}

}

void CRinwellSkills::Tick_DivineSaber(_float fTimeDelta)
{
	if (m_bDead == true)
		return;

	LightOffset.x += 0.01f;
	LightOffset.y -= 0.01f;
	LightOffset.z += 0.01f;

	if (LightOffset.x >= 0.1f)
		LightOffset.x = 0.1f;
	if (LightOffset.y <= -0.1f)
		LightOffset.y = -0.1f;
	if (LightOffset.z >= 0.1f)
		LightOffset.z = 0.1f;

	DevineLightDesc.vDiffuse.x = OriginLightDesc.vDiffuse.x + LightOffset.x;
	DevineLightDesc.vDiffuse.y = OriginLightDesc.vDiffuse.y + LightOffset.y;
	DevineLightDesc.vDiffuse.z = OriginLightDesc.vDiffuse.z + LightOffset.z;
	DevineLightDesc2.vDiffuse.x = OriginLightDesc2.vDiffuse.x + LightOffset.x*2.f;
	DevineLightDesc2.vDiffuse.y = OriginLightDesc2.vDiffuse.y + LightOffset.x*2.f;
	DevineLightDesc2.vDiffuse.z = OriginLightDesc2.vDiffuse.z + LightOffset.z*2.f;

	CGameInstance::Get_Instance()->Set_LightDesc(0,&DevineLightDesc);
	m_fDivineTimer += fTimeDelta;

	if (m_fDivineTimer > 0.1f)
	{
		float offsetx = (_float)(rand() % m_fDivineOffset)*(rand() % 2 == 0 ? 1.f : -1.f) / 10;
		float offsetz = (_float)(rand() % m_fDivineOffset)*(rand() % 2 == 0 ? 1.f : -1.f) / 10;
		CBullet::BULLETDESC BulletDesc;
		_vector vLocation = { 0.f,0.f,0.f,0.f };
		BulletDesc.iDamage = rand() % 150 + 1;
		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		BulletDesc.vTargetPosition.m128_f32[0] += offsetx;
		BulletDesc.vTargetPosition.m128_f32[2] += offsetz;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.pOwner = m_BulletDesc.pOwner;
		BulletDesc.eBulletType = CRinwellSkills::DIVINE_SABER_BULLET;
		BulletDesc.fDeadTime = 0.5f;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
		bulletcount -= 1;
		m_fDivineOffset -= 1;
		m_fDivineTimer = 0.f;
	}

	if (bulletcount < 20)
	{
		LightOffset.x -= 0.01f;
		LightOffset.y += 0.01f;
		LightOffset.z -= 0.01f;

		if (LightOffset.x <= 0.f)
			LightOffset.x = 0.0f;
		if (LightOffset.y >= 0.f)
			LightOffset.y = 0.f;
		if (LightOffset.z <= 0.f)
			LightOffset.z = 0.f;

		DevineLightDesc.vDiffuse.x = OriginLightDesc.vDiffuse.x + LightOffset.x;
		DevineLightDesc.vDiffuse.y = OriginLightDesc.vDiffuse.x + LightOffset.x;
		DevineLightDesc.vDiffuse.z = OriginLightDesc.vDiffuse.z + LightOffset.z;
		DevineLightDesc2.vDiffuse.x = OriginLightDesc2.vDiffuse.x + LightOffset.x*2.f;
		DevineLightDesc2.vDiffuse.y = OriginLightDesc2.vDiffuse.y + LightOffset.y*2.f;
		DevineLightDesc2.vDiffuse.z = OriginLightDesc2.vDiffuse.z + LightOffset.z*2.f;

		CGameInstance::Get_Instance()->Set_LightDesc(0, &DevineLightDesc);
		CGameInstance::Get_Instance()->Set_LightDesc(10, &DevineLightDesc2);
	}

	if (bulletcount <= 0)
	{
		m_bDead = true;
		CGameInstance::Get_Instance()->Set_LightDesc(0, &OriginLightDesc);
		CGameInstance::Get_Instance()->Set_LightDesc(10, &OriginLightDesc2);
	}
		
}

void CRinwellSkills::Tick_DivineSaberBullet(_float fTimeDelta)
{
	m_fDivineTimer += fTimeDelta;
	if (m_fDivineTimer> 0.5f)
		m_bDead = true;

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;

		if (iter != nullptr)
			iter->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}

}

void CRinwellSkills::Tick_HolyRance(_float fTimeDelta)
{
	m_fholytimer += fTimeDelta;
    
	if (m_fholytimer > 0.5f && m_bholybullet[0])
	{
		m_bholybullet[0] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 15.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] += 4.f;

		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
		
	}

	if (m_fholytimer > 0.8f && m_bholybullet[1])
	{
		m_bholybullet[1] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 15.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] += 4.f;

		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}

	if (m_fholytimer > 1.1f && m_bholybullet[2])
	{
		m_bholybullet[2] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 15.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] -= 8.f;

		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}

	if (m_fholytimer > 1.4f && m_bholybullet[3])
	{
		m_bholybullet[3] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 15.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] += 6.f;

		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}

	if (m_fholytimer > 1.7f && m_bholybullet[4])
	{
		m_bholybullet[4] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 15.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] -= 6.f;

		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}

	if (m_fholytimer > 2.f && m_bholybullet[5])
	{
		m_bholybullet[5] = false;
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = PLAYER;
		BulletDesc.fVelocity = 15.f;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE_BULLET;
		BulletDesc.vInitPositon = m_BulletDesc.vInitPositon;
		BulletDesc.vInitPositon.m128_f32[0] += 2.f;

		BulletDesc.vTargetPosition = m_BulletDesc.vTargetPosition;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;
	}


	if (m_fholytimer > 2.3f)
	{

		m_bDead = true;
	}


}

void CRinwellSkills::Tick_HolyRanceBullet(_float fTimeDelta)
{
	_vector vDir = XMVector3Normalize(m_BulletDesc.vTargetPosition - m_BulletDesc.vInitPositon);

	m_pTransformCom->LookAt(m_BulletDesc.vTargetPosition);
	m_pTransformCom->Go_PosDir(fTimeDelta, vDir);
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
}
