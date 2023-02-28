#pragma once

#include "AIState.h"

BEGIN(Client)
class CEffect;
BEGIN(AIPlayer)
class CAI_BoostAttack final : public CAIState
{
public:
	CAI_BoostAttack(class CPlayer* pPlayer, CBaseObj* pTarget);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	vector<CEffect*> m_pEffects;
	vector<CEffect*> m_pEffects2;
	vector<CEffect*> m_pEffects3;

	_float m_fTime = 0.f;
	_bool  m_bBullet = false;

	_bool m_bAlphenBoost_1 = false;
	_bool m_bAlphenBoost_2 = false;
	_bool m_bLawBoost = false;

	_float m_fEventStart = -1.f;

	_bool m_bAlphenStrike_1 = false;
	_bool m_bAlphenStrike_2 = false;
};
END
END