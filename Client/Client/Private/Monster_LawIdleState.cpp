#include "stdafx.h"
#include "..\Public\Monster_LawIdleState.h"

#include "RinwellMoveState.h"
#include "RinwellSkillState.h"
#include "Monster_LawNormalAttack.h"
#include "Monster_LawSkill.h"
#include "Monster_LawDodge.h"
#include "MonsterLaw_StrikeTrigger.h"
#include "Monster_Law_Move.h"



using namespace MonsterLaw;

CMonster_LawIdleState::CMonster_LawIdleState(CMonsterLaw * pLaw)
{
	m_pOwner = pLaw;
	//m_fWaitingTime = fTime;
}

CMonsterLawState * CMonster_LawIdleState::Tick(_float fTimeDelta)
{

	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());

	if (m_bAirMove)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 1.5f);
	else
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;
		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation();

	m_fTime += fTimeDelta;

	return nullptr;
}

CMonsterLawState * CMonster_LawIdleState::LateTick(_float fTimeDelta)
{

	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());


	
	/*if (m_fWaitingTime < m_fTime)
	{
		_float fDistance = XMVectorGetX(XMVector3Length(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION)));
		if (fDistance < 0.5f)
		{
			_uint iSkill = rand() % 2;

			switch (iSkill)
			{
			case 0:
				m_pOwner->Set_SkillIndex(CRinwellState::GALEFORCE);
				break;
			case 1:
				m_pOwner->Set_SkillIndex(CRinwellState::THUNDERFIELD);
				break;
			}

			return new CSkillState(m_pOwner, m_pOwner->Get_SkillIndex());
		}
		else
			return new CMoveState(m_pOwner, STATETYPE_MAIN, 0);
	}*/
	//return new CMonster_LawSkill(m_pOwner, SKILL_E);

	
	//

	if (m_pOwner->Get_Phase() == 1 || m_pOwner->Get_Phase() == 2)
	{
		switch (rand() % 6)
		{
		case 0:
			return new CMonster_LawSkill(m_pOwner, SKILL_E);

		case 1:
			return new CMonster_LawDodge(m_pOwner, true);

		case 2:
			return new CMonster_LawSkill(m_pOwner, SKILL_STRIKE);



		}
	}

	if (Find_EnumTarget(m_pOwner->Get_Phase()) > 6.f)
	{
		return new CMonster_Law_Move(m_pOwner);
	}


	

	if (m_pOwner->Get_Debug())
	{
		//return new CMonsterLaw_StrikeTrigger(m_pOwner);
		m_pOwner->Set_Debug(false);
	}
		

	switch (rand() % 6)
	{
	case 0:
		return new CMonster_LawSkill(m_pOwner, SKILL_R);

	case 1:
		return new CMonster_LawSkill(m_pOwner, SKILL_E);
	case 2:
		return new CMonster_LawSkill(m_pOwner, SKILL_F);
	case 3:
		return new CMonster_LawNormalAttack(m_pOwner , NORMALATTACK_1 , m_pTarget , m_pOwner->Get_Phase());
	case 4:
		return new CMonster_LawDodge(m_pOwner);
	case 5:
		return new CMonsterLaw_StrikeTrigger(m_pOwner);
	default:
		return new CMonster_LawSkill(m_pOwner, SKILL_STRIKE);
	}
	
	return nullptr;
}

void CMonster_LawIdleState::Enter(void)
{
	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());

	m_eStateId = STATE_ID::STATE_IDLE;

	
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CMonsterLaw::ANIM::BTL_MOVE_IDLE);

	//Find_ActiveTarget();
	m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
}

void CMonster_LawIdleState::Exit(void)
{
	m_fTime = 0.f;
}
