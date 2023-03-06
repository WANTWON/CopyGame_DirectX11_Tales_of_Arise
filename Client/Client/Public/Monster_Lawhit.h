#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
BEGIN(MonsterLaw)

class CMonster_Lawhit final : public CMonsterLawState
{
	enum DMG_DIR { FRONT, BACK, RIGHT, LEFT };

public:
	CMonster_Lawhit(class CMonsterLaw* pPlayer, _uint eDir, STATE_ID eStateID);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_vector m_vTargetPosition = { 0.f,0.f,0.f,0.f };
	_uint m_eDmgDir = FRONT;

private:
	void Move(void);
};

END
END