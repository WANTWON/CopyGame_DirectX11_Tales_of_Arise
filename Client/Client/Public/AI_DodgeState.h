#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAI_DodgeState final : public CAIState
{
public:
	CAI_DodgeState(class CPlayer* pPlayer, CBaseObj* pTarget, _bool back = false ,_bool lookat = true);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;


private:
	_bool m_bbackstep = false;
	_bool m_bLookat = true;

	CCollider* m_pDodgeCollider = nullptr;

};
END
END