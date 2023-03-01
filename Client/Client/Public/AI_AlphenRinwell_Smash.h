#pragma once

#include "AIState.h"

BEGIN(Client)
class CEffect;
BEGIN(AIPlayer)
class CAI_AlphenRinwell_Smash final : public CAIState
{
public:
	CAI_AlphenRinwell_Smash(class CPlayer* pPlayer, CBaseObj* pTarget);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	vector<CEffect*> m_pEffects;
	vector<CEffect*> m_pEffects2;
	vector<CEffect*> m_pEffects3;

	_float m_fFadeTime = 0.f;
	_float m_fTime = 0.f;
	_bool  m_bBullet = false;

	

	_float m_fEventStart = -1.f;
};
END
END