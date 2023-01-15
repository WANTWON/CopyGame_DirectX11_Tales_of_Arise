#include "stdafx.h"
#include "..\Public\Collision_Manger.h"
#include "BaseObj.h"
#include "Player.h"
#include "Monster.h"


IMPLEMENT_SINGLETON(CCollision_Manager);


CCollision_Manager::CCollision_Manager()
{
}

void CCollision_Manager::Add_CollisionGroup(COLLSIONGROUP CollisionGroup, CBaseObj * pGameObject)
{
	if (nullptr == pGameObject)
		return;
	m_GameObjects[CollisionGroup].push_back(pGameObject);
}

void CCollision_Manager::Out_CollisionGroup(COLLSIONGROUP CollisionGroup, CBaseObj * pGameObject)
{
	auto& iter = m_GameObjects[CollisionGroup].begin();
	while (iter != m_GameObjects[CollisionGroup].end())
	{
		if (*iter == pGameObject)
			iter = m_GameObjects[CollisionGroup].erase(iter);
		else
			++iter;
	}
}

void CCollision_Manager::Clear_CollisionGroup(COLLSIONGROUP CollisionGroup)
{
	m_GameObjects[CollisionGroup].clear();
}

void CCollision_Manager::Clear_CollisionGroupExpect(COLLSIONGROUP CollisionGroup)
{
	for (_uint i = 0; i < COLLISION_END; ++i)
	{
		if (i == CollisionGroup)
			continue;

		m_GameObjects[i].clear();
	}
}

_bool CCollision_Manager::CollisionwithGroup(COLLSIONGROUP CollisionGroup, CCollider* pCollider, CBaseObj** pOut)
{
	for (auto& iter : m_GameObjects[CollisionGroup])
	{
		if (iter == nullptr || iter->Get_Collider() == pCollider)
			continue;

		CCollider* pTargetCollider = iter->Get_Collider();

		if (pCollider->Collision(pTargetCollider))
		{
			if (pOut != nullptr)
				*pOut = iter;
			return true;
		}
			
	}

	return false;
}

_bool CCollision_Manager::CollisionwithGroup(COLLSIONGROUP SourGroup, COLLSIONGROUP DestGroup, CBaseObj** outSour, CBaseObj** outDest)
{
	for (auto& Sour : m_GameObjects[SourGroup])
	{
		CCollider* pSourCollider = Sour->Get_Collider();
		if (pSourCollider == nullptr)
			continue;

		for (auto& Dest : m_GameObjects[DestGroup])
		{
			CCollider* pDestCollider = Dest->Get_Collider();
			if (pDestCollider == nullptr || Dest == Sour)
				continue;

		

			if (pDestCollider->Collision(pSourCollider))
			{
				*outSour = Sour;
				*outDest = Dest;
				return true;
			}	
		}
	}

	*outSour = nullptr;
	*outDest = nullptr;
	return false;

}

void CCollision_Manager::CollisionwithBullet()
{
	///* Player가 Monster Bullet에 충돌 했을 때*/
	//CBaseObj* pPlayer = nullptr;
	//CBaseObj* pMonsterBullet = nullptr;

	//if (CollisionwithGroup(COLLISION_PLAYER, COLLISION_MBULLET, &pPlayer, &pMonsterBullet))
	//{
	//	pMonsterBullet->Set_Dead(true);

	//	CPlayer* Player = dynamic_cast<CPlayer*>(pPlayer);
	//	if (Player->Get_AnimState() == CPlayer::SHIELD || Player->Get_AnimState() == CPlayer::SHIELD_HOLD_LP
	//		|| Player->Get_AnimState() == CPlayer::SHIELD_HOLD_B || Player->Get_AnimState() == CPlayer::SHIELD_HOLD_F
	//		|| Player->Get_AnimState() == CPlayer::SHIELD_HOLD_L || Player->Get_AnimState() == CPlayer::SHIELD_HOLD_R )
	//	{
	//		Player->Set_AnimState(CPlayer::SHIELD_HIT);
	//		Player->Make_GuardEffect(pMonsterBullet);
	//	}
	//	else
	//	{

	//		CMonsterBullet::BULLETDESC BulletDesc = dynamic_cast<CMonsterBullet*>(pMonsterBullet)->Get_BulletDesc(); 
	//		dynamic_cast<CPlayer*>(Player)->Take_Damage(1.f, &BulletDesc, pMonsterBullet);

	//	}
	//	
	//}

	///* Monster가 Player Bullet에 충돌 했을 때*/
	//CBaseObj* pMonster = nullptr;
	//CBaseObj* pPlayerBullet = nullptr;
	//if (CollisionwithGroup(COLLISION_MONSTER, COLLISION_PBULLET, &pMonster, &pPlayerBullet))
	//{
	//	CPlayer::ANIM ePlayerState = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")))->Get_AnimState();
	//	if (ePlayerState == CPlayer::SLASH || ePlayerState == CPlayer::SLASH_HOLD_ED || ePlayerState == CPlayer::S_SLASH)
	//		dynamic_cast<CMonster*>(pMonster)->Take_Damage(1, nullptr, pPlayerBullet);
	//}
}


void CCollision_Manager::Update_Collider()
{
	for (_uint i = 0; i < COLLISION_END; ++i)
	{
		for (auto& iter : m_GameObjects[i])
		{
			if(iter->Get_IsSocket() == false)
				iter->Update_Collider();
		}
			
	}
}

_float CCollision_Manager::Calculate_DmgDirection(CBaseObj* Sour, CBaseObj* Dest, _float* fCross)
{
	_vector BulletLook = Sour->Get_TransformState(CTransform::STATE_LOOK);
	_vector PlayerLook = Dest->Get_TransformState(CTransform::STATE_LOOK);
	_vector fDot = XMVector3Dot(BulletLook, PlayerLook);
	_float fAngleRadian = acos(XMVectorGetX(fDot));
	_float fAngleDegree = XMConvertToDegrees(fAngleRadian);
	_vector vCross = XMVector3Cross(BulletLook, PlayerLook);

	if(fCross != nullptr)
		*fCross = XMVectorGetY(vCross);

	return fAngleDegree;
}



void CCollision_Manager::Free()
{
	for (_uint i = 0; i < COLLISION_END; ++i)
	{
		m_GameObjects[i].clear();
	}
}
