#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CIdleState final : public CPlayerState
{
public:
	enum IDLETYPE { IDLE_MAIN, IDLE_MAIN_TO_SIDE, IDLE_SIDE, IDLE_END };

public:
	CIdleState(class CPlayer* pPlayer, IDLETYPE eIdleType);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* Late_Tick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	IDLETYPE m_eIdleType = IDLE_END;
};
END
END