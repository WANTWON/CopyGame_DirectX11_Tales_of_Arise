#pragma once

#include "PlayerState.h"
#include "Effect.h"

BEGIN(Client)
BEGIN(Player)

class CPlayer_SionMiniAttackState final : public CPlayerState
{
public:
	CPlayer_SionMiniAttackState(class CPlayer* pPlayer, STATE_ID eStateType);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	vector<CEffect*> m_pEffects;
	
	_bool m_bIsShot = false;
};
END
END