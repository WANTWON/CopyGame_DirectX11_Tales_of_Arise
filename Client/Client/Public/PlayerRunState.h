#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CRunState final : public CPlayerState
{
public:
	CRunState(class CPlayer* pPlayer, DIRID eDir, _bool isDash = false);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Move(_float fTimeDelta);

private:
	DIRID m_eDirection = DIRID::DIR_END;
	_bool m_bIsDash = false;
	STATETYPE m_eStateType = STATETYPE_START;

	_float m_fRunTime = 0.f;

	//Sound
	_bool   m_bRunSound = false;
	_float	m_fSoundStart = -1.f;
	_float	m_fDashSoundStart = -1.f;
	_float  m_fBattleSoundStart = -1.f;
	_uint   m_iRunSoundCount = 0;
	_uint   m_iDashSoundCount = 0;
	_uint	m_iBattleSoundCount = 0;
	
};
END
END