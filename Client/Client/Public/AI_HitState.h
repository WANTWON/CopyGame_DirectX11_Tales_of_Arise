#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAI_HitState final : public CAIState
{
public:
	CAI_HitState(class CPlayer* pPlayer, _vector vCauserPos, _bool isDown = false, _float fTime = 0.f);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fStartHeight = 0.f;

	_bool	m_bIsDown = false;
	_vector m_vCauserPos;
	_bool	m_bIsLook = false;
private:
	_bool Move(_float fTimeDelta);
};



END
END