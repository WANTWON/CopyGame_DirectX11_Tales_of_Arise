#pragma once

#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "Law.h"
#include "Weapon.h"
#include "BattleManager.h"

BEGIN(Client)
class CPlayerState
{
public:
	enum STATE_ID_PLAYER
	{
		
		STATE_COLLECT,
		STATE_PLAYER_END
	};

public:
	virtual ~CPlayerState() {};

public:
	virtual CPlayerState* HandleInput() PURE;
	virtual CPlayerState* Tick(_float fTimeDelta) PURE;
	virtual CPlayerState* Late_Tick(_float fTimeDelta) PURE;
	virtual CPlayerState* EventInput(void) PURE;
	virtual void Exit() PURE;
};
END