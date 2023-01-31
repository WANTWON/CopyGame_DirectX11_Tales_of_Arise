#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CJumpState final : public CPlayerState
{
public:
	CJumpState(class CPlayer* pPlayer, _float fStartHeight, STATETYPE eType, _float fTime);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta);

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

private:
	DIRID m_eDirection = DIRID::DIR_END;
};
END
END