#pragma once

#include "AIState.h"

BEGIN(Client)
class CEffect;
BEGIN(AIPlayer)
class CAI_Overlimit_State final : public CAIState
{
public:
	CAI_Overlimit_State(class CPlayer* pPlayer, CBaseObj* pTarget, _bool FinalCameraMode = false);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	vector<CEffect*> m_pEffects;

	_float m_fTime = 0.f;
	_bool  m_bBullet = false;
	_bool m_bFinalMode = false;

	_float m_fEventStart = -1.f;

	//Sound
	_bool m_bAlphenSound = false;
	_bool m_bSionSound = false;
	_bool m_bRinwellSound = false;
	_bool m_bLawSound = false;
};
END
END