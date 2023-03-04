#include "stdafx.h"
#include "HawkBattle_DeadState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"

using namespace Hawk;

CBattle_DeadState::CBattle_DeadState(CHawk* pHawk)
{
	m_pOwner = pHawk;
}

CHawkState * CBattle_DeadState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CHawkState * CBattle_DeadState::Tick(_float fTimeDelta)
{
	if (false == m_bDeadAnimFinish)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta*1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	m_pOwner->Check_Navigation();

	return nullptr;
}

CHawkState * CBattle_DeadState::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_bIsAnimationFinished && false == m_bDeadAnimFinish)
	{
		CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

		pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
		m_pAtkColliderCom = nullptr;

		pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);
		m_pOwner->Set_GlowUp();
		m_pOwner->Set_Dissolve();
		m_bDeadAnimFinish = true;
		m_fTimeDeltaAcc = 0.f;
	}

	return nullptr;
}

void CBattle_DeadState::Enter()
{
	m_eStateId = STATE_ID::STATE_DEAD;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::DEAD);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Hawk_Dead.wav"), SOUND_VOICE, 1.0f);
}

void CBattle_DeadState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}