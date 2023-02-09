#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)

class CCloseChaseState final : public CPlayerState
{
public:
	CCloseChaseState(class CPlayer* pPlayer, STATE_ID eStateType, STATE_ID eNextStateType);
	
	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	CBaseObj* m_pTarget = nullptr;
	STATE_ID m_eNextState = STATE_END;
};

END
END