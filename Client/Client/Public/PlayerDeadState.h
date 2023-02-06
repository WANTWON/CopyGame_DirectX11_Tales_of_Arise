#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CPlayerDeadState final : public CPlayerState
{
public:
	CPlayerDeadState(class CPlayer* pPlayer, STATE_ID eStateType);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_bool m_bIsStop = false;
};
END
END