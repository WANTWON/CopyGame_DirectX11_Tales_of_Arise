#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CAttack2NormalState final : public CPlayerState
{
public:
	CAttack2NormalState(class CPlayer* pPlayer);
	
	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	_matrix m_StartMatrix;
};
END
END
