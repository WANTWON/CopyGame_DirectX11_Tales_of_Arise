#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
class CEffect;
BEGIN(MonsterLaw)

class CFascinate final : public CMonsterLawState
{

public:
	CFascinate(class CMonsterLaw* pPlayer);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_vector m_vTargetPosition = { 0.f,0.f,0.f,0.f };
	_uint m_eDmgDir = FRONT;
	_bool m_bFinised = false;
	vector<class CEffect*> m_pEffects;

};

END
END