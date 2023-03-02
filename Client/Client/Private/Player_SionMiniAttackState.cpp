#include "stdafx.h"
#include "..\Public\Player_SionMiniAttackState.h"

#include "GameInstance.h"
#include "EffectMesh.h"
#include "Player_SionShooterState.h"
#include "SionSkills.h"

using namespace Player;

CPlayer_SionMiniAttackState::CPlayer_SionMiniAttackState(CPlayer * pPlayer, STATE_ID eStateType)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
}

CPlayerState * CPlayer_SionMiniAttackState::HandleInput()
{
	return nullptr;
}

CPlayerState * CPlayer_SionMiniAttackState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		if (!m_bIsFly)
			m_pOwner->Check_Navigation();
	}
	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if ((ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType) && !m_bIsShot)
			{
				CBullet::BULLETDESC BulletDesc;
				BulletDesc.eCollisionGroup = PLAYER;
				BulletDesc.fVelocity = 40.f;
				BulletDesc.eBulletType = CSionSkills::NAILBULLET;
				BulletDesc.vInitPositon = XMLoadFloat4x4(&CGameInstance::Get_Instance()->Get_CamWorldMatrix()).r[3] + XMLoadFloat4x4(&CGameInstance::Get_Instance()->Get_CamWorldMatrix()).r[2];
				
				BulletDesc.pOwner = m_pOwner;
				BulletDesc.vTargetDir = XMLoadFloat4x4(&CGameInstance::Get_Instance()->Get_CamWorldMatrix()).r[2];

				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_WORKTOOL, TEXT("Layer_Bullet"), &BulletDesc)))
					return nullptr;

				//รั น฿ป็ 
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Shot.wav"), SOUND_EFFECT, 0.13f);
				
				_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
				_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir * 3;
				_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
				mWorldMatrix.r[3] = vLocation;
				m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalBulletBlast.dat"), mWorldMatrix);

				m_bIsShot = true;
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
			{
				if (GetKeyState(VK_LBUTTON) < 0)
				{
					return new CPlayer_SionMiniAttackState(m_pOwner, STATE_NORMAL_ATTACK1);
				}
			}
		}
	}

	return nullptr;
}

CPlayerState * CPlayer_SionMiniAttackState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
		return new CPlayer_SionShooterState(m_pOwner);

	return nullptr;
}

CPlayerState * CPlayer_SionMiniAttackState::EventInput(void)
{
	return nullptr;
}

void CPlayer_SionMiniAttackState::Enter()
{
	__super::Enter();

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_0);

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_pOwner->Set_Manarecover(false);
}

void CPlayer_SionMiniAttackState::Exit()
{
	__super::Exit();

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT_SION);
}
