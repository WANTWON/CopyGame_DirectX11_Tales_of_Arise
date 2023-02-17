#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)

class CAI_Alphen_NormalAttackState final : public CAIState
{
public:
	CAI_Alphen_NormalAttackState(class CPlayer* pPlayer, STATE_ID eStateType , CBaseObj* pTarget);//, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;


	virtual void Enter() override;
	virtual void Exit() override;

private:
	_matrix m_StartMatrix;

	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;

	_bool m_bEffectSlashSpawned = false;
	vector<CEffect*> m_SlashEffect;
};
END
END