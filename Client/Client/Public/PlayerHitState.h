#pragma once
#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CHitState final : public CPlayerState
{
public:
	CHitState(class CPlayer* pPlayer, _float fTime = 0.f);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fTime = 0.f;

	_bool m_bIsBack = false;
};
END
END