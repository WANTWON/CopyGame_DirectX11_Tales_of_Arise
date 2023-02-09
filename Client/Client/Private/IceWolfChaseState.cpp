#include "stdafx.h"
#include "..\Public\IceWolfChaseState.h"
#include "IceWolfHowLingState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfIdleState.h"
using namespace IceWolf;

CChaseState::CChaseState(CIce_Wolf * pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CChaseState::AI_Behaviour(_float fTimeDelta)
{
	

	return nullptr;
}

CIceWolfState * CChaseState::Tick(_float fTimeDelta)
{
	
	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	return nullptr;
}

CIceWolfState * CChaseState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());

	if (m_fTarget_Distance >= 15.f)
		return new CIdleState(m_pOwner, CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE);
	
	return nullptr;
}

void CChaseState::Enter()
{
	m_eStateId = STATE_ID::STATE_DISCOVER;


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_RUN);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Chase.wav"), SOUND_VOICE, 0.1f);
}

void CChaseState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}
