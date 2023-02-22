#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAI_Item_Use_State final : public CAIState
{
public:
	CAI_Item_Use_State(class CPlayer* pPlayer, ITEM_NAME itemname);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

	void Use_Item(ITEM_NAME item);



private:
	_bool m_bStopRunning = false;

	ITEM_NAME m_eItemname = ITEMNAME_END;
	_float m_fEventStart = -1.f;
};
END
END