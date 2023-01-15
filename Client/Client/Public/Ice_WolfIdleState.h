#pragma once

#include "Ice_WolfState.h"

BEGIN(Client)
BEGIN(Ice_Wolf)
class CIdleState final : public CIce_WolfState
{
public:
	CIdleState(class CIce_Wolf* pPlayer);

	virtual CIce_WolfState* AI_Behavior() override;
	virtual CIce_WolfState* Tick(_float fTimeDelta) override;
	virtual CIce_WolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fIdleMoveTimer = 0.f;
	_float m_fIdleAttackTimer = 1.5f;
};
END
END