#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)

class CAI_Alphen_SkillAttackState final : public CAIState
{
public:
	CAI_Alphen_SkillAttackState(class CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget, _uint skillindex);//, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;


	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Reset_Skill();

private:
	_matrix m_StartMatrix;

	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;

	/* Effects */
	/* E */
	_bool m_bHienzinFirstEffect = false;
	_bool m_bHienzinSecondEffect = false;

	/* R */
	_bool m_bAkizameEffect = false;

	/* F */
	_bool m_bHousyutigakuzinFirstEffect = false;
	_bool m_bHousyutigakuzinSecondEffect = false;

	vector<CEffect*> m_HousyutigakuzinStart;
};
END
END