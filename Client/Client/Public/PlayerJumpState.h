#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CJumpState final : public CPlayerState
{
public:
	enum JUMPTYPE { JUMP_IDLE, JUMP_RUN, JUMP_BATTLE, JUMP_END };

public:
	CJumpState(class CPlayer* pPlayer, _float fStartHeight, STATETYPE eType, _float fTime, JUMPTYPE eJumpType);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta);
	virtual CPlayerState* EventInput(void);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_bool Check_JumpEnd();

private:
	void Move(_float fTimeDelta);
	_float m_fStartHeight = 0.f;
	_float m_fEndHeight = 0.f;
	_float m_fTime = 0.f;
	_vector m_vecStartPos;
	_vector m_vecChangePos;
	_bool m_bIsJump = false;
	_bool m_bIsDrop = false;
	_bool m_bIsInput = false;

private:
	DIRID m_eDirection = DIRID::DIR_END;
	JUMPTYPE m_eJumpType = JUMP_END;
};
END
END