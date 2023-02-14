#include "stdafx.h"
#include "AICheckState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "AIAttackNormalState.h"
#include "AIRunawayfromTarget.h"
#include "Rinwell.h"
#include "AI_ChaseState.h"
#include "AI_BoostAttackState.h"
#include "AI_Alphen_NormalAttackState.h"
#include "AI_Alphen_SkillAttackState.h"
#include "AI_Sion_SkillState.h"

using namespace AIPlayer;

CAICheckState::CAICheckState(CPlayer* pPlayer, STATE_ID eStateType)
{
	m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}

CAIState * CAICheckState::Tick(_float fTimeDelta)
{
	
	//if(m_ePreStateID =

	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return nullptr;

	if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
	{
		m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
	}
	else
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}

	if (m_pTarget == nullptr)
		return nullptr;

	if (m_bLookatOnetime)
	{
		if (nullptr == m_pTarget)
		{
			m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
			(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		}
		else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		m_bLookatOnetime = false;
	}
	
	//m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	
//	if (m_eCurrentPlayerID == CPlayer::ALPHEN)
//	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

			//m_pOwner->Check_Navigation();
		}
//	}
//	else
//	{
//    	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

//		m_pOwner->Check_Navigation();


//	}
	

		m_pOwner->Check_Navigation();

	return nullptr;
}

CAIState * CAICheckState::LateTick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return nullptr;

	if (m_pTarget == nullptr)
		return nullptr;

	m_fTimer += fTimeDelta;

	
	if (m_fTimer > 0.5f)
	{
		if (Get_Target_Distance() <= 3.f)
		{
			m_bRangerRunaway = true; // for ranger runaway
		}
		else
			m_bRangerRunaway = false;

		


		switch (m_eCurrentPlayerID)
		{
		case CPlayer::ALPHEN:
			if (Get_Target_Distance() >= 3.f)
			{
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

					return new CAI_ChaseState(m_pOwner, STATE_RUN, m_eCurrentPlayerID, m_pTarget);
			}
				

			else if (m_pOwner->Get_Info().fCurrentMp <= 1.f)
			{
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();


			}
				

			else
				return  RandomAttackChoose();

			break;

		case CPlayer::SION:
			//return new CAI_Sion_BoostAttack(m_pOwner);
			if (Get_Target_Distance() >= 20.f)
			{
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

				return new CAI_ChaseState(m_pOwner, STATE_RUN, m_eCurrentPlayerID , m_pTarget);
			}
			if (m_bRangerRunaway)
			{
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

				return new CAIRunawayfromTarget(m_pOwner, STATE_RUN, m_eCurrentPlayerID);
			}
				
			else
			{
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
				return RandomAttackChoose_Sion();
			}
				
			break;

		case CPlayer::RINWELL:
			if (m_bRangerRunaway)
			{
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

				return new CAIRunawayfromTarget(m_pOwner, STATE_RUN, m_eCurrentPlayerID);
			}
				

			break;

		default:
			break;
		}
		

	}
	


	return nullptr;
}

void CAICheckState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;
	//m_iCurrentAnimIndex = CSion::ANIM::IDLE;
	

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_IDLE;
		break;
	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_MOVE_IDLE;
		break;
	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_LOOP;
		break;
	default:
		break;
	}
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);

	m_pOwner->Set_Manarecover(true);

	/*m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();


	m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));*/

}

void CAICheckState::Exit()
{
	__super::Exit();
}

CAIState * CAICheckState::RandomAttackChoose_Sion()
{
	
		if (nullptr == m_pTarget)
		{
			m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
			(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		}
		else
			m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

		if (m_pOwner->Get_Info().fCurrentMp < 1.f)
			return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);



		switch (rand() % 6)
		{

	case 0:
		return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
		
	case 1:
		return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE);
	
	case 2:
		return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_MAGNARAY);
	
	case 3:
		return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_BRAVE);

	case 4:
		return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET);

	case 5:
		return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_THUNDER_BOLT);
	
	}
		return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
}

CAIState * CAICheckState::RandomAttackChoose()
{
	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

	if (m_pOwner->Get_Info().fCurrentMp < 1.f)
		return new CAI_Alphen_NormalAttackState(m_pOwner, STATE_ATTACK, m_pTarget);

	switch (rand() % 4)
	{

	case 0:
		return new CAI_Alphen_NormalAttackState(m_pOwner, STATE_ATTACK, m_pTarget);

	case 1:
		return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_HIENZIN);

	case 2:
		return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_AKIZAME);

	case 3:
		return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN);

	}
	
		return new CAI_Alphen_NormalAttackState(m_pOwner, STATE_ATTACK, m_pTarget);
}
