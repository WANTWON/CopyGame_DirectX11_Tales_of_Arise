#pragma once
#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)
class CHitState final : public CPlayerState
{
public:
	CHitState(class CPlayer* pPlayer, _vector vCauserPos, _float fMoveLength = 2.f, HITTYPE eHitType = HIT_NORMAL, STATETYPE eStateType = STATETYPE_DEFAULT);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	STATETYPE m_eStateType;
	HITTYPE m_eHitType = HIT_END;

	_float m_fStartHeight = 0.f;
	_float m_fMoveLength = 0.f;

	_vector m_vStartPos;
	_vector m_vCauserPos;
	_vector m_vDir;
	_vector m_vGoalPos;
	_float	m_fRatio = 0.f;

	_bool m_bIsMove = false;

private:
	_bool Move(_float fTimeDelta);
};
END
END