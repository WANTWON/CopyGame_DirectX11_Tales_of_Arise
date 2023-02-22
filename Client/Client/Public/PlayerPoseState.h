#pragma once
#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CPlayerPoseState final : public CPlayerState
{
public:
	CPlayerPoseState(class CPlayer* pPlayer, STATE_ID eStateID);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:

};
END
END