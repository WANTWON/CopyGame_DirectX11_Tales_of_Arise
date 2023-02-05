#pragma once

#include "AIState.h"
#include "Sion.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAICheckState final : public CAIState
{
public:
	CAICheckState(class CPlayer* pPlayer, STATE_ID eStateType);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_vector m_vTargetPosition = { 0.f,0.f,0.f,0.f }; 

};
END
END