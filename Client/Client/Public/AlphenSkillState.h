#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)

class CAlphenSkillState final : public CPlayerState
{
public:
	CAlphenSkillState(class CPlayer* pPlayer, STATE_ID eStateType, _float fTime = 0.f);

	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Reset_Skill();

private:
	/* Effects */
	/* E */
	_bool m_bHienzinFirstEffect = false;
	_bool m_bHienzinSecondEffect = false;
	_bool m_bRyuuseizinFirstEffect = false;
	_bool m_bRyuuseizinSecondEffect = false;
	_bool m_bSkillSoundStart = false;
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

	CCollider* m_pSwordCollider = nullptr;
	_float m_fColEventStartTime = 0.f;

	//Sound

	//_bool m_bSkillJump_R_Sound = false;
	_bool m_bSkill_JumpF_Sound = false;
	_bool m_bSkill_F_Sound = false;
	_bool m_bSkill_R_Sound = false;
	_bool m_bSkill_E_Sound = false;
private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
	HITLAGDESC m_HitLagDesc;

};
END
END