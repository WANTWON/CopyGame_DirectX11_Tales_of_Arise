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
#include "Effect.h"

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
	Update_Effects();
	Update_Blur(fTimeDelta);

	Find_Target();
 	
	if (m_ePreState_Id == STATE_ID::STATE_ADVENT)
	{
		if(!m_bIsAnimationFinished)
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, false);
	}
	else if (m_ePreState_Id == STATE_ID::STATE_HALF)
	{
		if (!m_bIsAnimationFinished)
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, false);
	}
	else
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone", 0.f);


		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

			m_pOwner->Check_Navigation();
		}
	}
	

	////////////////////////////////현재 코드 - ACTIVE_PLAYER만을 타겟으로 함 ////////////////////
	Find_Target();
	m_vActiveTargetPos = m_pActiveTarget->Get_TransformState(CTransform::STATE_TRANSLATION);





	
	return nullptr;
}

CAstralDoubt_State * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	Remove_Effects();

	m_fTimeDeltaAcc += fTimeDelta;

	////////////////////////////////현재 코드 - ACTIVE_PLAYER만을 타겟으로 함 ////////////////////
	

	if (m_pActiveTarget != nullptr)
	{
		m_vActiveTargetPos = m_pActiveTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_b_IsTargetInsight = Is_TargetInFront(m_vActiveTargetPos);

		m_b_IsTargetInsight = Is_TargetInSight(m_vActiveTargetPos, 120);
	}

	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;

	if (m_ePreState_Id == STATE_ID::STATE_DOWN)
	{
		/*_vector vPosition = XMVectorSetY(m_vActiveTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAt(vPosition);*/
		TurnToTarget(fTimeDelta);

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
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{
					if (m_bLandSound == false)
					{
						dynamic_cast<CCamera_Action*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 2.f, 0.05f);
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Boss_Asu_FootPress.wav"), SOUND_VOICE, 0.5f);
						m_bLandSound = true;
					}
				}

				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{
					if (m_bAdventSound == false)
					{
						m_fSoundStart = pEvent.fStartTime;
						dynamic_cast<CCamera_Action*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 1.f, 0.1f);
						CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Howling.wav"), SOUND_VOICE, 0.6f);
						m_bAdventSound = true;
					}
				}

				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
				{
					if (!m_bRoar && !strcmp(pEvent.szName, "Roar"))
					{
						_matrix WorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Roar_Wind.dat"), WorldMatrix);
						m_Roar = CEffect::PlayEffectAtLocation(TEXT("Boss_Roar.dat"), WorldMatrix);
						Update_Effects();
						Update_Blur(fTimeDelta);

						m_bRoar = true;
						m_bRoarBlur = true;
					}
				}
			}
		}

		if (m_bIsAnimationFinished && CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
		{
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_UPPER);
		}

	}
	else if (m_ePreState_Id == STATE_ID::STATE_BRAVE)
	{
		if (m_bIsAnimationFinished)
		{
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_SPEARMULTI);
		}
	}
	else if (m_ePreState_Id == STATE_ID::STATE_HALF)
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
				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
				{
					if (!m_bRoar && !strcmp(pEvent.szName, "Roar"))
					{
						_matrix WorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Roar_Wind.dat"), WorldMatrix);
						m_Roar = CEffect::PlayEffectAtLocation(TEXT("Boss_Roar.dat"), WorldMatrix);
						Update_Effects();

						m_bRoar = true;
						m_bRoarBlur = true;
					}
				}
			}
		}

		if (m_bIsAnimationFinished && CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
		{
			CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
			pPlayer->Set_IsActionMode(false);

			vector<CPlayer*> pAIList = CPlayerManager::Get_Instance()->Get_AIPlayers();
			for (auto& iter : pAIList)
			{
				iter->Set_IsActionMode(false);
			}

			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_SPEARMULTI);
		}
			
	}
	else
	{
		if (m_pActiveTarget == nullptr)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_LOOKOUT);
		else
		{
			if (m_fActiveTarget_Distance > 30.f) //if (m_fActiveTarget_Distance > 30.f)
			{
				if ((STATE_SPEARMULTI != m_ePreState_Id) && (STATE_HEADBEAM != m_ePreState_Id))
				{
					switch (m_iRand)
					{
					case 0:
						return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_SPEARMULTI);
					case 1:
						if (!m_bBeamTargetOn)
						{
							/*_vector vPosition = XMVectorSetY(m_vActiveTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
							m_pOwner->Get_Transform()->LookAt(vPosition);*/
							TurnToTarget(fTimeDelta);
							m_bBeamTargetOn = true;
							return new CBattle_HeadBeamState(m_pOwner);
						}
						break;
					}
				}
				else
					return new CBattleWalkState(m_pOwner, m_ePreState_Id);
			}
			else if (m_fActiveTarget_Distance <= 30.f)   //(m_fActiveTarget_Distance <= 30.f)
			{
				/*_vector vPosition = XMVectorSetY(m_vActiveTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
				m_pOwner->Get_Transform()->LookAt(vPosition);*/
				TurnToTarget(fTimeDelta);
				if (m_fActiveTarget_Distance <= 12.f)  //(m_fActiveTarget_Distance <= 15.f)
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
					if (m_fActiveTarget_Distance <= 22.f)
					{
						if (m_ePreState_Id == CAstralDoubt_State::STATE_UPPER)
							return new CBattleWalkState(m_pOwner, m_ePreState_Id);
						else
							return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_UPPER);
					}
					else
						return new CBattleWalkState(m_pOwner, m_ePreState_Id);
				}
			}
		}
	}


	///////////////////////////////////////기존의 코드- 나를 때린 대상, 근접 대상을 찾아 공격.//////////////////

	/*if (m_pCurTarget != nullptr)
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
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{
					if (m_bLandSound == false)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Boss_Asu_FootPress.wav"), SOUND_VOICE, 0.6f);
						m_bLandSound = true;
					}
				}

				else if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
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
				if (m_fTarget_Distance <= 15.f)
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
	}*/

	return nullptr;
}

void CBattle_IdleState::Update_Effects()
{
	for (auto& pEffect : m_Roar)
	{
		if (!pEffect)
			continue;

		_float4x4 PivotMatrix = m_pOwner->Get_Model()->Get_PivotFloat4x4();
		_matrix ParentWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

		CHierarchyNode* pBone;

		pBone = m_pOwner->Get_Model()->Get_BonePtr("HEAD1_C");
		_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

		_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
		_vector vPosition = SocketMatrix.r[3] + (XMVector4Normalize(vLook) * 2);

		pEffect->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
	}
}

void CBattle_IdleState::Remove_Effects()
{
	for (auto& pEffect : m_Roar)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;
	}
}

void CBattle_IdleState::Update_Blur(_float fTimeDelta)
{
	if (!m_bRoarBlur)
		return;

	_float fDuration = .75f;
	m_fBlurTimer += fTimeDelta;

	if (m_fBlurTimer > m_fBlurResetAfter && !m_bBlurResetted)
	{
		m_fBlurTimer = 0.f;
		m_bBlurResetted = true;
	}

	// Blur On
	if (!m_bBlurResetted)
	{
		/* Zoom Blur Lerp */
		_float fFocusPower = 10.f;

		_float fBlurInterpFactor = m_fBlurTimer / fDuration;
		if (fBlurInterpFactor > 1.f)
			fBlurInterpFactor = 1.f;

		_int iDetailStart = 1;
		_int iDetailEnd = 4;
		_int iFocusDetailLerp = iDetailStart + fBlurInterpFactor * (iDetailEnd - iDetailStart);
		m_pOwner->Get_Renderer()->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);
	}
	// Blur Off
	else
	{
		/* Zoom Blur Lerp */
		_float fFocusPower = 10.f;

		_float fBlurInterpFactor = m_fBlurTimer / fDuration;
		if (fBlurInterpFactor > 1.f)
			fBlurInterpFactor = 1.f;

		_int iDetailStart = 4;
		_int iDetailEnd = 1;
		_int iFocusDetailLerp = iDetailStart + fBlurInterpFactor * (iDetailEnd - iDetailStart);
		m_pOwner->Get_Renderer()->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);

		if (m_fBlurTimer > fDuration)
			m_bRoarBlur = false;
	}
}

void CBattle_IdleState::Enter()
{
	m_bRoar = false;
	
	m_bRoarBlur = false;
	m_fBlurTimer = 0.f;
	m_pOwner->Get_Renderer()->Set_ZoomBlur(false);

	if (m_ePreState_Id == STATE_ID::STATE_DOWN)
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::MOVE_IDLE);
	}
	else if (m_ePreState_Id == STATE_ID::STATE_ADVENT)
	{
		m_pOwner->Set_IsActionMode(true);
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::EVENT_ADVENT);
	}
	else if (m_ePreState_Id == STATE_ID::STATE_BRAVE)
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_BRAVE);

	}
	else if (m_ePreState_Id == STATE_ID::STATE_ALLPLAYER_DEAD)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_BRAVE);

	else if (STATE_ID::STATE_HALF == m_ePreState_Id)
	{
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("Boss50Down.dat"), XMMatrixIdentity());
		m_pOwner->Set_IsActionMode(true);
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_DETECT_IDLE);
		m_pOwner->Get_Model()->Reset();
	}

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
	{
		m_pOwner->Set_IsActionMode(false);
		CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
	}
		
}