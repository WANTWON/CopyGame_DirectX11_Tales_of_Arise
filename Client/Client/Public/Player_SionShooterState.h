#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CPlayer_SionShooterState final : public CPlayerState
{
public:
	CPlayer_SionShooterState(class CPlayer* pPlayer);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta);

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Move(_float fTimeDelta);
};
END
END