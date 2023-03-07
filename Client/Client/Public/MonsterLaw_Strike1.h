#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
class CEffect;
BEGIN(MonsterLaw)
class CMonsterLaw_Strike1 final : public CMonsterLawState
{
public:
	CMonsterLaw_Strike1(class CMonsterLaw* pPlayer, CBaseObj* pTarget);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void StrikeBlur(_float fTimeDelta);
	void Set_EffectPosition();

private:
	vector<CEffect*> m_pEffects;

	vector<_vector> m_vStrikeLockOnPos;
	vector<_vector> m_vEffectPos;

	_float m_fTime = 0.f;
	_bool  m_bBullet = false;

	_float m_fEventStart = -1.f;
	_float m_fTimer = 0.f;
	_float m_fFadeTime = 0.f;

	/* Strike Screen Blur */
	_bool m_bStrikeBlur = false;
	_float m_fEffectEventEndTime = 0.f;
	_float m_fEffectEventCurTime = 0.f;
	_float m_fResetTimer = 0.f;
};
END
END