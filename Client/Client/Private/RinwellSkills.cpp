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
		vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
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
		break;
	case DIVINE_SABER:
		vLocation = m_BulletDesc.vTargetPosition;
		//mWorldMatrix.r[]
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);
		break;
	case DIVINE_SABER_BULLET:
		vLocation = m_BulletDesc.vTargetPosition;
		m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vLocation);

		vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION);
		mWorldMatrix = m_BulletDesc.pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		//m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("AquaImpact.dat"), mWorldMatrix);
		break;


	default:
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

	}

	if(m_BulletDesc.eBulletType == THUNDER_FIELD)
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	else
	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());



	return OBJ_NOEVENT;
}

void CRinwellSkills::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (XMVectorGetY(Get_TransformState(CTransform::STATE_TRANSLATION)) <= 0)
		m_bDead = true;
}

void CRinwellSkills::Collision_Check()
{
	CBaseObj* pCollisionTarget = nullptr;
	switch (m_BulletDesc.eBulletType)
	{
	case PHOTON_FLASH:
	case METEOR:
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

	//m_pTransformCom->LookAt(m_BulletDesc.vTargetPosition);
	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);
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
}

void CRinwellSkills::Tick_DivineSaber(_float fTimeDelta)
{
	if (m_bDead == true)
		return;

	//m_fDivineTimer += fTimeDelta;

	//if (m_fAquaTImer > 0.1f)
	//{
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

	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
		return;
	bulletcount -= 1;
	m_fDivineOffset -= 1;
	//m_fDivineTimer = 0.f;

	if (bulletcount <= 0)
		m_bDead = true;
}

void CRinwellSkills::Tick_DivineSaberBullet(_float fTimeDelta)
{
	m_fDivineTimer += fTimeDelta;
	if (m_fDivineTimer> 0.5f)
		m_bDead = true;

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
