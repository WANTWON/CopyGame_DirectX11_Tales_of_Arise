#pragma once

#include "RinwellState.h"

BEGIN(Client)
BEGIN(AiRinwell)

class CMoveState final : public CRinwellState
{
public:
	CMoveState(class CAiRinwell* pPlayer);

	virtual CRinwellState* Tick(_float fTimeDelta) override;
	virtual CRinwellState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Move(_float fTimeDelta);

private:
	_vector m_vTargetPosition = {0.f,0.f,0.f,0.f};
	_int	m_ChangeDirCount = 0;
	_bool	m_ChangedDir = false;

};

END
END