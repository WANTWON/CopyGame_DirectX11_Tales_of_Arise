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

	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()) , "TransN");


	/*if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}*/

	m_pOwner->Check_Navigation();
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
							BulletDesc.fVelocity = 20.f;
							BulletDesc.eBulletType = CSionSkills::NORMALATTACK;
							BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
							if (m_pTarget != nullptr)
								BulletDesc.vTargetPosition = (m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
							else if (m_pTarget == nullptr)
							BulletDesc.vTargetPosition =CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
							BulletDesc.pOwner = m_pOwner;
							BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
							BulletDesc.fDeadTime = 5.f;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
							//총 발사 사운드
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Shot.wav"), SOUND_EFFECT_SION, 0.07f);
							m_fEventStart = pEvent.fStartTime;

							_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
							_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir*3;
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = vLocation;
							m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalBulletBlast.dat"), mWorldMatrix);
							
							m_bSoundStart = false;
					}
				}
				
				else if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{
					m_fReloadTimeDelta += fTimeDelta;

					
					CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT_SION);
						if (!m_bSoundStart)
						{
							/*if (m_fReloadTimeDelta > 0.005f)
								CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);*/
							
							m_bSoundStart = true;
							CGameInstance::Get_Instance()->PlaySounds(TEXT("SionReload.wav"), SOUND_EFFECT, 0.1f);
							

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
	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_pOwner->Set_Manarecover(false);

}

void CAIAttackNormalState::Exit()
{
	__super::Exit();
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}