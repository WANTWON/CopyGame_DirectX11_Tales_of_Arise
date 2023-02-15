#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)

class CAlphenSkillState final : public CPlayerState
{
public:
	CAlphenSkillState(class CPlayer* pPlayer, STATE_ID eStateType, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Reset_Skill();

private:
	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;

	/* Effects */
	/* E */
	_bool m_bHienzinFirstEffect = false;
	_bool m_bHienzinSecondEffect = false;
	_bool m_bRyuuseizinFirstEffect = false;
	_bool m_bRyuuseizinSecondEffect = false;

	/* R */
	_bool m_bAkizameFirstEffect = false;
	_bool m_bAkizameSecondEffect = false;
	_bool m_bSenkusyourepaParticle = false;
	_bool m_bSenkusyourepaFirstEffect = false;
	_bool m_bSenkusyourepaSecondEffect = false;

	/* F */
	_bool m_bHousyutigakuzinFirstEffect = false;
	_bool m_bHousyutigakuzinSecondEffect = false;
	_bool m_bHousyutigakuzinThirdEffect = false;
	_bool m_bEngetuFirstEffect = false;
	_bool m_bEngetuSecondEffect = false;

	vector<CEffect*> m_SenkusyourepaParticles;
};
END
END