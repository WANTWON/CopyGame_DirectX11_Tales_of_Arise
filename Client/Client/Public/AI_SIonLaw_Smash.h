#pragma once

#include "AIState.h"

BEGIN(Client)
class CEffect;
BEGIN(AIPlayer)
class CAI_SionLaw_Smash final : public CAIState
{
public:
	CAI_SionLaw_Smash(class CPlayer* pPlayer, CBaseObj* pTarget);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void StrikeBlur(_float fTimeDelta);

private:
	vector<CEffect*> m_pEffects;

	_float m_fTime = 0.f;
	_bool  m_bBullet = false;
	_float  m_fFadeTime = 0.f;
	
	_bool m_bAlphenStrike_1 = false;
	_bool m_bAlphenStrike_2 = false;

	_float m_fEventStart = -1.f;

	/* Strike Screen Blur */
	_bool m_bStrikeBlur = false;
	_float m_fEffectEventEndTime = 0.f;
	_float m_fEffectEventCurTime = 0.f;
	_float m_fResetTimer = 0.f;

	_float m_fShakingTime = 0.f;
	_bool m_bShaking = false;
};
END
END