#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
BEGIN(MonsterLaw)
class CMonster_Law_StrikeTrigger2 final : public CMonsterLawState
{
public:
	CMonster_Law_StrikeTrigger2(class CMonsterLaw* pPlayer);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

	void Update_StrikePosition(_tchar * FilePath, _int iMonsterPosNum);

	vector<_vector> Get_strikepos() { return m_vStrikeLockOnPos; }

	enum STRIKEPOSITION { ACTIVE1, ACTIVE2, AIPLAYER1, AIPLAYER2, LOCKON };

private:

	_vector m_vStrikePosition[5];
	vector<_vector> m_vStrikeLockOnPos;


	_bool m_bbackstep = false;
	_bool m_bLookat = true;


};
END
END