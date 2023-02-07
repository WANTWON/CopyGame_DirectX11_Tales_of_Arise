#include "stdafx.h"
#include "AIAttackNormalState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "AICheckState.h"
#include "AIDeadState.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "Monster.h"

using namespace AIPlayer;

CAIAttackNormalState::CAIAttackNormalState(CPlayer* pPlayer , STATE_ID eStateType , CBaseObj* pTarget)
{
	m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	else
	m_pTarget = pTarget;
	
}

CAIState * CAIAttackNormalState::Tick(_float fTimeDelta)
{
	//if (m_pTarget == nullptr)
	//	m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

	//m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()) , "TransN");


	/*if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}*/

	return nullptr;
}

CAIState * CAIAttackNormalState::LateTick(_float fTimeDelta)
{
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::SION   :            //sion

		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{

					
					m_bIsStateEvent = true;
		
				}
				else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
							CBullet::BULLETDESC BulletDesc;
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.fVelocity = 100.f;
							BulletDesc.eBulletType = CSionSkills::NORMALATTACK;
							BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
							if (m_pTarget != nullptr)
								BulletDesc.vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
							else if (m_pTarget == nullptr)
							BulletDesc.vTargetPosition = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
							
							m_fEventStart = pEvent.fStartTime;
						
					}
				}
			
			}
		}
			if (m_iCurrentAnimIndex == CSion::ANIM::BTL_ATTACK_NORMAL_4 && m_bIsAnimationFinished)
				return new CAICheckState(m_pOwner, STATE_ID::STATE_IDLE);
		
		
		break;
	}














	if (m_bIsStateEvent)
	{
		__super::Exit();
		++m_iCurrentAnimIndex;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		m_bIsStateEvent = false;

		m_fEventStart = -1.f;

	}
	

	return nullptr;
}

void CAIAttackNormalState::Enter()
{
	m_eStateId = STATE_ID::STATE_ATTACK;
	m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_NORMAL_0;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);



}

void CAIAttackNormalState::Exit()
{
	__super::Exit();
}