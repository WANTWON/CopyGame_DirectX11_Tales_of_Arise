#pragma once

#include "AIState.h"

BEGIN(Client)
class CEffect;
BEGIN(AIPlayer)
class CAI_AlphenLaw_Smash final : public CAIState
{
public:
	CAI_AlphenLaw_Smash(class CPlayer* pPlayer, CBaseObj* pTarget);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void StrikeBlur(_float fTimeDelta);
	void Set_EffectPosition();

private:
	vector<CEffect*> m_pEffects;

	vector<_vector> m_vStrikeLockOnPos;
	vector<_vector> m_vEffectPos;

	_float m_fTime = 0.f;
	_bool  m_bBullet = false;
	_bool m_bScreen = false;

	_float m_fEventStart = -1.f;
	_float m_fEventStart1 = -2.f;
	_float m_fTimer = 0.f;
	_float m_fFadeTime = 0.f;

	/* Strike Screen Blur */
	_bool m_bStrikeBlur = false;
	_float m_fEffectEventEndTime = 0.f;
	_float m_fEffectEventCurTime = 0.f;
	_float m_fResetTimer = 0.f;

	_int m_iEventIndex = 0;

	//Sound 
	_bool m_bSoundStart = false;
};
END
END