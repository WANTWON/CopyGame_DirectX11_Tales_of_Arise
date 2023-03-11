#pragma once

#include "PlayerState.h"
#include "InteractObject.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)
class CPlayerOverlimit final : public CPlayerState
{
public:
	CPlayerOverlimit(class CPlayer* pPlayer, _bool FinalCameraMode = false);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	_bool m_bFinalMode = false;

	//Sound
	_bool m_bAlphenSound = false;
	_bool m_bSionSound = false;
	_bool m_bRinwellSound = false;
	_bool m_bLawSound = false;

	
};
END
END