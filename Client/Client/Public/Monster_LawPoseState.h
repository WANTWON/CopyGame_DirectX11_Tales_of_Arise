#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
BEGIN(MonsterLaw)

class CPoseState final : public CMonsterLawState
{
	
public:
	CPoseState(class CMonsterLaw* pPlayer, STATE_ID eStateID);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_vector m_vTargetPosition = {0.f,0.f,0.f,0.f};
	_uint m_eDmgDir = FRONT;
	_bool m_bFinised = false;

};

END
END