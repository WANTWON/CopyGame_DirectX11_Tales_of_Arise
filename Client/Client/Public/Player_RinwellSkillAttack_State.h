#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)
class CPlayer_RinwellSkillAttack_State final : public CPlayerState
{
public:
	CPlayer_RinwellSkillAttack_State(class CPlayer* pPlayer, STATE_ID eStateType, _float fTime = 0.f);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	_bool m_bBulletMade = false;
	vector<CEffect*> m_pBlastEffect;
	vector<CEffect*> m_pSmokeEffect;

	_float m_fEventStart = -1.f;
	_uint m_iCount = 0;
};
END
END