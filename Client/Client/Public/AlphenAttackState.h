#pragma once

#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)
class CAlphenAttackState final : public CPlayerState
{
public:
	CAlphenAttackState(class CPlayer* pPlayer, STATE_ID eStateType, _float fStartHeight = 0.f, _float fTime = 0.f);
	
	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;

	_bool m_bEffectSlashSpawned = false;
	_bool m_bEffectKickSpawned = false;
	vector<CEffect*> m_SlashEffect;
};
END
END