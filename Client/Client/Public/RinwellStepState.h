#pragma once

#include "RinwellState.h"

BEGIN(Client)
BEGIN(AiRinwell)
class CRinwellStepState final : public CRinwellState
{
public:
	CRinwellStepState(class CAiRinwell* pRinwell, STATETYPE eStateType);
	
	virtual CRinwellState* Tick(_float fTimeDelta) override;
	virtual CRinwellState* LateTick(_float fTimeDelta);

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Move(_float fTimeDelta);

private:
	STATETYPE m_eStateType;
	_vector m_vCurrentPos;
	_vector m_vTargetPos;
	_int m_iDistance;
	_float m_fRatio = 0.f;
};
END
END