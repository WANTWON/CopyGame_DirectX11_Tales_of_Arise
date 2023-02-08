#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)
class CPlayer_SionSkillAttack final : public CPlayerState
{
public:
	CPlayer_SionSkillAttack(class CPlayer* pPlayer, STATE_ID eStateType, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;

	_bool m_bHienzinFirstEffect = false;
	_bool m_bHienzinSecondEffect = false;
	_bool m_bAkizameEffect = false;
	_bool m_bHousyutigakuzinFirstEffect = false;
	_bool m_bHousyutigakuzinSecondEffect = false;

	vector<CEffect*> m_HousyutigakuzinStart;
	void CallbackFunction(_uint iIndex);
};
END
END