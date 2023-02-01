#pragma once

#include "RinwellState.h"

BEGIN(Client)
BEGIN(AiRinwell)

class CDamageState final : public CRinwellState
{
	enum DMG_DIR { FRONT, BACK, RIGHT, LEFT };

public:
	CDamageState(class CAiRinwell* pPlayer, _uint eDir);

	virtual CRinwellState* Tick(_float fTimeDelta) override;
	virtual CRinwellState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_vector m_vTargetPosition = {0.f,0.f,0.f,0.f};
	_uint m_eDmgDir = FRONT;

};

END
END