#pragma once

#include "PlayerState.h"
#include "InteractObject.h"

BEGIN(Client)
BEGIN(Player)
class CCollectState final : public CPlayerState
{
public:
	CCollectState(class CPlayer* pPlayer, CInteractObject* pObject = nullptr);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	CInteractObject* m_pObject = nullptr;
};
END
END