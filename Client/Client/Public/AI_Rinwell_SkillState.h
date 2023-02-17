#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)

class CAI_Rinwell_SkillState final : public CAIState
{
public:
	CAI_Rinwell_SkillState(class CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget);//, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;


	virtual void Enter() override;
	virtual void Exit() override;

private:
	_matrix m_StartMatrix;

	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;
	_float m_fEventStart = -1.f;

	_bool m_bEffectSlashSpawned = false;
	_bool m_bIsStateEvent = false;
	_bool m_bBulletMake = false;
	_bool m_bStateFinish = false;
	_bool m_bCollideFinsh = false;
	vector<CEffect*> m_pBlastEffect;
	vector<CEffect*> m_pSmokeEffect;
};
END
END