#pragma once

#include "AIState.h"

BEGIN(Client)
class CEffect;
BEGIN(AIPlayer)
class CAI_AlphenSion_Smash final : public CAIState
{
public:
	CAI_AlphenSion_Smash(class CPlayer* pPlayer, CBaseObj* pTarget);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	vector<CEffect*> m_pEffects;

	_float m_fTime = 0.f;
	_bool  m_bBullet = false;
	_bool  m_bScreen = false;
	_float m_fScreenTimer = 0.f;

	_bool m_bAlphenStrike_1 = false;
	_bool m_bAlphenStrike_2 = false;

	_float m_fEventStart = -1.f;
};
END
END