#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)

class CLawSkillState final : public CPlayerState
{
public:
	CLawSkillState(class CPlayer* pPlayer, STATE_ID eStaetType, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Reset_Skill(void);

private:
	_float m_fStartHeight;
	_float m_fTime;
};
END
END