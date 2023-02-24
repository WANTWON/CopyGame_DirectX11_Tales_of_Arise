#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CDodgeState final : public CPlayerState
{
public:
	CDodgeState(class CPlayer* pPlayer, DIRID eDir, _float fTime = 0.f);
	
	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* Late_Tick(_float ftimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Move(_float fTimeDelta);

private:
	DIRID m_eDirection = DIRID::DIR_END;

	CCollider* m_pDodgeCollider = nullptr;
};
END
END