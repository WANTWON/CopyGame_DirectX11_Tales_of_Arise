#pragma once
#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CHitState final : public CPlayerState
{
public:
	CHitState(class CPlayer* pPlayer, _vector vCauserPos, _bool isDown = false, _float fTime = 0.f, STATETYPE eStateType = STATETYPE_DEFAULT);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	STATETYPE m_eStateType;

	_float m_fStartHeight = 0.f;

	_bool m_bIsDown = false;
	_vector m_vCauserPos;
	_bool m_bIsLook = false;
	_bool m_bIsMove = false;

private:
	_bool Move(_float fTimeDelta);
};
END
END