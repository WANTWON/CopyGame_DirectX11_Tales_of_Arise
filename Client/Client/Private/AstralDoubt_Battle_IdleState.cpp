#include "stdafx.h"

#include "AstralDoubt_Battle_IdleState.h"
#include "GameInstance.h"
#include "AstralDoubt_Battle_TurnState.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_HeadBeamState.h"
#include "AstralDoubt_Battle_SpearMultiState.h"
#include "AstralDoubt_Battle_UpperState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"
#include "AstralDoubt_Battle_RushState.h"

using namespace Astral_Doubt;

CBattle_IdleState::CBattle_IdleState(CAstralDoubt* pAstralDoubt, STATE_ID  ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 100) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_IdleState::Tick(_float fTimeDelta)
{
 	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone", 0.f);

	if (m_ePreState_Id == STATE_ID::STATE_ADVENT)
	{ 
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

			m_pOwner->Check_Navigation();
		}
	}

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	CBaseObj* pOrigin_DamageCause = nullptr;
	pOrigin_DamageCause = m_pOwner->Get_OrginDamageCauser();


	if (m_pCurTarget == nullptr)
	{
		if (pOrigin_DamageCause == nullptr)
		{
			CBaseObj* pDamageCauser = nullptr;
			pDamageCauser = m_pOwner->Get_DamageCauser();

			if (pDamageCauser == nullptr)
			{
				m_pCurTarget = m_pOwner->Find_MinDistance_Target();

				m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
				m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);

				if (m_pCurTarget == nullptr)
				{
					return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_ALLPLAYER_DEAD);
				}
			}

			else if (pDamageCauser != nullptr)
			{
				CBaseObj* pDamageCauser = nullptr;
				pDamageCauser = m_pOwner->Get_DamageCauser();
				m_pOwner->Set_OrginDamageCauser(pDamageCauser);

				m_pCurTarget = pDamageCauser;

				m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
				m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
			}
		}

		else if (pOrigin_DamageCause != nullptr)
		{
			if (pOrigin_DamageCause->Get_Info().fCurrentHp <= 0)
			{
				CBaseObj* pDamageCauser = nullptr;
				pDamageCauser = m_pOwner->Get_DamageCauser();

				if (pDamageCauser == nullptr)
				{
					CBaseObj* pCorpseNearby = nullptr;
					pCorpseNearby = m_pOwner->Find_MinDistance_Target();

					if (pCorpseNearby == nullptr)
						return nullptr;

					if (pCorpseNearby->Get_Info().fCurrentHp > 0)
					{
						m_pCurTarget = pCorpseNearby;
						m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
					}

					else
						return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);

				}

				else if (pDamageCauser != nullptr)
				{
					if (pDamageCauser->Get_Info().fCurrentHp > 0)
					{
						pDamageCauser = m_pOwner->Get_DamageCauser();
						m_pOwner->Set_OrginDamageCauser(pDamageCauser);

						m_pCurTarget = pDamageCauser;

						m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
					}

					CBaseObj* pCorpseNearby = nullptr;
					pCorpseNearby = m_pOwner->Find_MinDistance_Target();

					if (pCorpseNearby->Get_Info().fCurrentHp > 0)
					{
						m_pCurTarget = pCorpseNearby;
						m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
					}
					else
						return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);
				}
			}

			else if (pOrigin_DamageCause->Get_Info().fCurrentHp > 0)
			{
				m_pCurTarget = pOrigin_DamageCause;
				m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
				m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
			}
		}
	}

	else
	{
		if (m_pCurTarget->Get_Info().fCurrentHp <= 0)
		{
			CBaseObj* pDamageCauser = nullptr;
			pDamageCauser = m_pOwner->Get_DamageCauser();

			if (pDamageCauser == nullptr)
			{
				CBaseObj* pCorpseNearby = nullptr;
				pCorpseNearby = m_pOwner->Find_MinDistance_Target();

				if (pCorpseNearby->Get_Info().fCurrentHp > 0)
				{
					m_pCurTarget = pCorpseNearby;
					m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
					m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
				}

				else
					return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);
			}

			else if (pDamageCauser != nullptr)
			{
				if (pDamageCauser->Get_Info().fCurrentHp > 0)
				{
					pDamageCauser = m_pOwner->Get_DamageCauser();
					m_pOwner->Set_OrginDamageCauser(pDamageCauser);

					m_pCurTarget = pDamageCauser;

					m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
					m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
				}

				else
					return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);
				//return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_LOOKOUT);
			}
		}

		else if (m_pCurTarget->Get_Info().fCurrentHp > 0)
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}


	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();



	return nullptr;
}

CAstralDoubt_State * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	
	m_pOwner->Check_Navigation();
	m_fTimeDeltaAcc += fTimeDelta;

	
	if (m_pCurTarget != nullptr)
	{
		
		m_b_IsTargetInsight = Is_TargetInFront(m_vCurTargetPos);

		m_b_IsTargetInsight = Is_TargetInSight(m_vCurTargetPos, 120);
	}

	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;

	if (m_ePreState_Id == STATE_ID::STATE_DOWN)
	{
		_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAt(vPosition);

		if (m_b_IsTargetInsight)
		{
		
			return new CBattle_RushState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_RUSH_START);
		}

	}

	else if (m_ePreState_Id == STATE_ID::STATE_ADVENT)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{
					if (m_bAdventSound == false)
					{
						m_fSoundStart = pEvent.fStartTime;
						CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Howling.wav"), SOUND_VOICE, 0.6f);
						m_bAdventSound = true;
					}
				}

				else
				{
					CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
				}
			}
		}

		if (m_bIsAnimationFinished)
		{
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_UPPER);
		}

	}

	else if (m_ePreState_Id == STATE_ID::STATE_BRAVE)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{
					if (m_bAdventSound == false)
					{
						
						CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Howling.wav"), SOUND_VOICE, 0.6f);
						m_bAdventSound = true;
					}
				}

				else
				{
					CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
				}
			}
		}

		if (m_bIsAnimationFinished)
		{
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_SPEARMULTI);
		}
	}

	else
	{
		if (m_pCurTarget == nullptr)
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_LOOKOUT);
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone", 0.f);
		}
		else
		{
			if (m_fTarget_Distance > 30.f)
			{
				if (m_ePreState_Id != CAstralDoubt_State::STATE_HEADBEAM)
				{
					if (m_b_IsTargetInsight == true)
					{
						if (m_bBeamTargetOn == false)
						{
							_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
							m_pOwner->Get_Transform()->LookAt(vPosition);
							m_bBeamTargetOn = true;
							return new CBattle_HeadBeamState(m_pOwner);
						}
					}
					else if (m_b_IsTargetInsight == false)
					{
						if (m_bBeamTargetOn == false)
						{
							_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
							m_pOwner->Get_Transform()->LookAt(vPosition);
							m_bBeamTargetOn = true;
							return new CBattle_HeadBeamState(m_pOwner);
						}
					}
				}
				else if (m_ePreState_Id == CAstralDoubt_State::STATE_HEADBEAM)
				{
					return new CBattleWalkState(m_pOwner);
				}
			}
			else if (m_fTarget_Distance <= 30.f)
			{
				_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
				m_pOwner->Get_Transform()->LookAt(vPosition);
				if (m_fTarget_Distance <= 12.f)
				{
					if (m_PreState_IsSpin == false)
					{
						return new CBattle_720Spin_FirstState(m_pOwner, CAstralDoubt_State::STATE_SPIN);
					}
					else
					{
						return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_FOOTPRESS);
					}
				}
				else
				{
					if (m_ePreState_Id == CAstralDoubt_State::STATE_UPPER)
						return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_SPEARMULTI);
					else if (m_ePreState_Id == CAstralDoubt_State::STATE_SPEARMULTI)
						return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_UPPER);
					else
					{
						switch (m_iRand)
						{
						case 0:
							return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_SPEARMULTI);
						case 1:
							return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_UPPER);
						default:
							break;
						}
					}
				}
			}
		}
	}

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	if (m_ePreState_Id == STATE_ID::STATE_DOWN)
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::MOVE_IDLE);
	}
	else if (m_ePreState_Id == STATE_ID::STATE_ADVENT)
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::EVENT_ADVENT);
	}
	else if (m_ePreState_Id == STATE_ID::STATE_BRAVE)
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_BRAVE);
		
		/*vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay && m_bAdventSound == false)
			{

				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Howling.wav"), SOUND_VOICE, 0.6f);
					m_bAdventSound = true;
				}
			}
		}*/
	}
	else if(m_ePreState_Id == STATE_ID::STATE_ALLPLAYER_DEAD)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_BRAVE);

	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::MOVE_IDLE);

	if (m_ePreState_Id == STATE_ID::STATE_SPIN)
		m_PreState_IsSpin = true;

	if (m_ePreState_Id == STATE_ID::STATE_FOOTPRESS)
		m_PreState_FootPress = true;
}

void CBattle_IdleState::Exit()
{
	if (m_ePreState_Id == STATE_ID::STATE_BRAVE || m_ePreState_Id == STATE_ID::STATE_ADVENT)
		CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}