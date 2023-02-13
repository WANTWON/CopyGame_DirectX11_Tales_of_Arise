#pragma once

#include "RinwellState.h"

BEGIN(Client)
BEGIN(AiRinwell)

class CPoseState final : public CRinwellState
{
	
public:
	CPoseState(class CAiRinwell* pPlayer, STATE_ID eStateID);

	virtual CRinwellState* Tick(_float fTimeDelta) override;
	virtual CRinwellState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_vector m_vTargetPosition = {0.f,0.f,0.f,0.f};
	_uint m_eDmgDir = FRONT;
	_bool m_bFinised = false;

};

END
END