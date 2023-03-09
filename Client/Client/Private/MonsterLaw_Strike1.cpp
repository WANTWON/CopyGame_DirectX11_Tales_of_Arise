#include "stdafx.h"
#include "..\Public\MonsterLaw_Strike1.h"
#include "GameInstance.h"
#include "Law.h"
#include "AICheckState.h"
#include "CameraManager.h"
#include "Effect.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "AlphenSkills.h"
#include "ParticleSystem.h"
#include "UI_Skillmessage.h"
#include "Monster_LawIdleState.h"
#include "PlayerHitState.h"
#include "UI_Dialogue_Caption.h"

using namespace MonsterLaw;
using namespace Player;
CMonsterLaw_Strike1::CMonsterLaw_Strike1(CMonsterLaw* pPlayer, CBaseObj* pTarget)
{

	m_pOwner = pPlayer;

	if (nullptr == pTarget)
	{
		m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());
	}
	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());

	StrikeTarget = pTarget;
}

CMonsterLawState * CMonsterLaw_Strike1::Tick(_float fTimeDelta)
{
	if (m_bStrikeBlur)
		StrikeBlur(fTimeDelta);

	m_fTimer += fTimeDelta;


		if (m_fTimer > 4.f)
		{
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_ALPHENLAWSTRIKE);
			m_fTimer = -100.f;
		}


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, false);
	if (!m_bIsAnimationFinished)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{

					if ((m_fEventStart1 != pEvent.fStartTime))
					{
						if (m_iEventIndex == 0)
							dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Open_Dialogue(1);
						else
							dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Next_Dialogueindex();

						++m_iEventIndex;
						m_fEventStart1 = pEvent.fStartTime;
					}

				}
					else if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime))
						{
							if (!strcmp(pEvent.szName, "Begin"))
							{
								/* Make Effect */
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								m_pEffects = CEffect::PlayEffectAtLocation(TEXT("LawAttack1_Begin.dat"), mWorldMatrix);

								_matrix mWorldMatrix2 = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix2.r[3] = m_vEffectPos[0];
								vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawAttack1_BeginFlash.dat"), mWorldMatrix2);
								_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix2.r[3]);

								for (auto& pEffect : Punch)
								{
									vPosition += vCamDir*0.1f;
									pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
							}
							else if (!strcmp(pEvent.szName, "Dash"))
							{
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[1];
								CEffect::PlayEffectAtLocation(TEXT("LawAttack1_Dash.dat"), mWorldMatrix);
							}
							else if (!strcmp(pEvent.szName, "Punch"))
							{
								
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[2];
								vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawAttack1_BeginPunch.dat"), mWorldMatrix);
								_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix.r[3]);

								for (auto& pEffect : Punch)
								{
									vPosition += vCamDir*0.1f;
									pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
								StrikeTarget->Set_State(CTransform::STATE_TRANSLATION, m_vStrikeLockOnPos[0]);
								dynamic_cast<CPlayer*>(StrikeTarget)->Set_HitState();
							}
							else if (!strcmp(pEvent.szName, "FootPunch"))
							{
								
								StrikeTarget->Set_State(CTransform::STATE_TRANSLATION, m_vStrikeLockOnPos[1]);
								dynamic_cast<CPlayer*>(StrikeTarget)->Set_HitState();
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[3];
								vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawAttack1_BeginPunch.dat"), mWorldMatrix);
								_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix.r[3]);
								
								for (auto& pEffect : Punch)
								{
									vPosition += vCamDir*0.1f;
									pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
							}
							else if (!strcmp(pEvent.szName, "Punch2"))
							{
								
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[4];
								vector<CEffect*> Punch = CEffect::PlayEffectAtLocation(TEXT("LawAttack1_BeginPunch.dat"), mWorldMatrix);
								_vector vPosition = Punch[0]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vCamDir = XMVector3Normalize(XMLoadFloat4(&CGameInstance::Get_Instance()->Get_CamPosition()) - mWorldMatrix.r[3]);

								for (auto& pEffect : Punch)
								{
									vPosition += vCamDir*0.1f;
									pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
							}
							else if (!strcmp(pEvent.szName, "FootReady"))
							{
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[5];
								CEffect::PlayEffectAtLocation(TEXT("LawAttack1_FootReady.dat"), mWorldMatrix);
							}
							else if (!strcmp(pEvent.szName, "Boom"))
							{
							
								StrikeTarget->Set_State(CTransform::STATE_TRANSLATION, m_vStrikeLockOnPos[2]);
								dynamic_cast<CPlayer*>(StrikeTarget)->Set_HitState();
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								mWorldMatrix.r[3] = m_vEffectPos[6];
								CEffect::PlayEffectAtLocation(TEXT("LawAttack1_Boom.dat"), mWorldMatrix);
								if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
									dynamic_cast<CCamera_Action*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 3.f, 0.02f);

								m_bStrikeBlur = true;
								m_bBullet = true;
							}

							m_fEventStart = pEvent.fStartTime;
						}
					
				
				}
			}
		}
	}


	return nullptr;
}

CMonsterLawState * CMonsterLaw_Strike1::LateTick(_float fTimeDelta)
{
	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	if (m_bIsAnimationFinished)
	{
	//	m_fWaitTimer += fTimeDelta;

		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
			CUI_Manager::Get_Instance()->Get_UIStrike() == false)
		{

				return new CMonster_LawIdleState(m_pOwner);
		}
	}

	if (m_bBullet && !m_bScreen)
	{
		m_fFadeTime += fTimeDelta;
		if (m_fFadeTime > .85f)
		{
			CUI_Manager::Get_Instance()->Set_UIStrike(true);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_StrikeFinish"), LEVEL_STATIC, TEXT("dddd"));
			m_bScreen = true;
			if (m_bStrikeBlur)
			{
				m_pOwner->Set_ResetStrikeBlur(true);
				m_bStrikeBlur = false;
			}
		}
	}

	return nullptr;
}

void CMonsterLaw_Strike1::Enter()
{
	m_pOwner->Get_Renderer()->Set_ZoomBlur(false);
	m_bStrikeBlur = false;

	m_pOwner->Set_IsActionMode(true);
	Set_EffectPosition();
	dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_RAINOUI);
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_MYSTIC_ZINRAIROUEIKYAKU);
	m_pOwner->Get_Model()->Reset();


	CGameInstance::Get_Instance()->PlaySounds(TEXT("AlphenLaw_Smash.wav"), SOUND_VOICE, 0.55f);
}

void CMonsterLaw_Strike1::Exit()
{
	if (m_bStrikeBlur)
	{
		/*m_pOwner->Set_ResetStrikeBlur(true);*/
		m_pOwner->Get_Renderer()->Set_ZoomBlur(false);
		m_bStrikeBlur = false;
	}

	m_pOwner->Get_Model()->Reset();


		if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
		{
			HITLAGDESC m_HitLagDesc;
			m_HitLagDesc.bHitLag = false;
			m_HitLagDesc.bLockOnChange = false;
			m_HitLagDesc.bShaking = false;


			if (!dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_LastStrikeAttack())
			{
				m_pTarget->Take_Damage(1000, CPlayerManager::Get_Instance()->Get_ActivePlayer());
			}
			
		}

	


	dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->fadeout();

	m_pTarget->Set_IsActionMode(false);
	m_pOwner->Set_IsActionMode(false);
	CBattleManager::Get_Instance()->Set_IsStrike(false);


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
	//CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	pCamera->Set_Zoom(false);
}

void CMonsterLaw_Strike1::StrikeBlur(_float fTimeDelta)
{
	_float fDuration = .45f;
	m_fResetTimer += fTimeDelta;

	/* Zoom Blur Lerp */
	_float fFocusPower = 10.f;

	_float fBlurInterpFactor = m_fResetTimer / fDuration;
	if (fBlurInterpFactor > 1.f)
		fBlurInterpFactor = 1.f;

	_int iDetailStart = 1;
	_int iDetailEnd = 10;
	_int iFocusDetailLerp = iDetailStart + fBlurInterpFactor * (iDetailEnd - iDetailStart);
	m_pOwner->Get_Renderer()->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);
}

void CMonsterLaw_Strike1::Set_EffectPosition()
{
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC Model;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/EffectPosition/LawAttack1EffectPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;

	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);


	for (int i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(Model), sizeof(NONANIMDESC), &dwByte, nullptr);
		m_vEffectPos.push_back(XMVectorSetW(XMLoadFloat3(&Model.vPosition), 1.f));
	}

	CloseHandle(hFile);

	m_vStrikeLockOnPos = CPlayerManager::Get_Instance()->Get_StrikeLockOnPosition();

}

