#pragma once

#include "RinwellState.h"

BEGIN(Client)
BEGIN(AiRinwell)

class CMoveState final : public CRinwellState
{
public:
	CMoveState(class CAiRinwell* pPlayer, STATETYPE eType, _int iCount);

	virtual CRinwellState* Tick(_float fTimeDelta) override;
	virtual CRinwellState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Move(_float fTimeDelta, _int iDir);
	void LookAtPlayer(_float fTimeDelta);

private:
	_vector m_vTargetPosition = {0.f,0.f,0.f,0.f};
	_int	m_iChangeCount = 0;
	_bool	m_ChangedDir = false;
	_bool	m_bAirMove = false;
	_int	m_iDir = 0;
	_float  m_fTurnDegree = 0.f;
};

END
END