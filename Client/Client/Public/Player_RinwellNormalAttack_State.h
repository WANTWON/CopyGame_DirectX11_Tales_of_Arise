#pragma once

#include "PlayerState.h"
#include "Effect.h"

BEGIN(Client)
BEGIN(Player)

class CPlayer_RinwellNormalAttack_State final : public CPlayerState
{
public:
	CPlayer_RinwellNormalAttack_State(class CPlayer* pPlayer, STATE_ID eStateType, _float fTime = 0.f);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_matrix m_StartMatrix;

	_float m_fTime = 0.f;

	_bool m_bEffectSlashSpawned = false;
	_bool m_bSoundStart = false;
	vector<CEffect*> m_pEffects;

	_float m_fEventStart = 1.f;
};
END
END