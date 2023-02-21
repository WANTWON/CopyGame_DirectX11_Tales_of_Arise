#pragma once

#include "PlayerState.h"
#include "InteractObject.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)
class CPlayerOverlimit final : public CPlayerState
{
public:
	CPlayerOverlimit(class CPlayer* pPlayer);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	CInteractObject* m_pObject = nullptr;
	vector<CEffect*> m_pEffects;
};
END
END