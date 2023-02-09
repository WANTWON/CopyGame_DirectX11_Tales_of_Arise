#pragma once
#include "PlayerState.h"

BEGIN(Client)
BEGIN(Player)

class CAlphenSkillState final : public CPlayerState
{
public:
	CAlphenSkillState(class CPlayer* pPlayer, STATE_ID eStateType, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;
};

END
END