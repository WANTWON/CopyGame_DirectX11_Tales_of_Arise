#pragma once
#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)

class CSkillState final : public CPlayerState
{
public:
	CSkillState(class CPlayer* pPlayer, STATE_ID eStateType);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;
};

END
END