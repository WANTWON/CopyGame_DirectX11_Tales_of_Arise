#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)

class CPlayerChaseState final : public CPlayerState
{
public:
	CPlayerChaseState(class CPlayer* pPlayer, STATE_ID eStateType);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	CBaseObj* m_pTarget = nullptr;
};

END
END