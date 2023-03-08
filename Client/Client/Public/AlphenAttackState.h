#pragma once

#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)
class CAlphenAttackState final : public CPlayerState
{
public:
	CAlphenAttackState(class CPlayer* pPlayer, STATE_ID eStateType, _float fTime = 0.f);
	
	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_bool m_bEffectSlashSpawned = false;
	_bool m_bEffectKickSpawned = false;
	vector<CEffect*> m_SlashEffect;

	CCollider* m_pSwordCollider = nullptr;
	CCollider* m_pFootCollider = nullptr;
	_float m_fColEventStartTime = 0.f;

private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
	HITLAGDESC m_HitLagDesc;

	//Sound
	_uint	   m_iFruitCutSoundCount = 0;
	
};
END
END