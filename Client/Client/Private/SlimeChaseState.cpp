#include "stdafx.h"

#include "SlimeChaseState.h"
#include "GameInstance.h"
#include "SlimeIdleState.h"

using namespace Slime;

CChaseState::CChaseState(CSlime* pSlime)
{
	m_pOwner = pSlime;
	
}

CSlimeState * CChaseState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CSlimeState * CChaseState::Tick(_float fTimeDelta)
{


	m_fTarget_Distance = Find_BattleTarget();


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_float fTranslationLength, fRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &fTranslationLength, &fRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((fTranslationLength * 0.01f), fRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	return nullptr;
}

CSlimeState * CChaseState::LateTick(_float fTimeDelta)
{
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);


	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);

	if (m_fTarget_Distance >= 8.f)
		//return new CIdleState(m_pOwner);


	return nullptr;
}

void CChaseState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::MOVE_WALK_F);


}

void CChaseState::Exit()
{

}