#pragma once

#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CRunState final : public CPlayerState
{
public:
	CRunState(class CPlayer* pPlayer, DIRID eDir, _bool isDash = false);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* Late_Tick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Move(_float fTimeDelta);

private:
	DIRID m_eDirection = DIRID::DIR_END;
	_bool m_bIsDash = false;
	_bool m_bSoundStart = false;
	_uint m_iRunSoundCount = 0;
	
};
END
END