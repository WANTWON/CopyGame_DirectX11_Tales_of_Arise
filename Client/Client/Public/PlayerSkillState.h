#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)
class CSkillState final : public CPlayerState
{
public:
	CSkillState(class CPlayer* pPlayer, STATE_ID eStateType, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;

	vector<CEffect*> m_HienzinStartEffect;
	vector<CEffect*> m_HienzinEndEffect;
	vector<CEffect*> m_AkizameEffect;
	vector<CEffect*> m_HousyutigakuzinEffect;
};
END
END