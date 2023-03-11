#pragma once

#include "PlayerState.h"
BEGIN(Client)
class CEffect;
BEGIN(Player)
class CPlayerBoostAttackState final : public CPlayerState
{
public:
	CPlayerBoostAttackState(class CPlayer* pPlayer, CBaseObj* pTarget);

	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	vector<CEffect*> m_pEffects;
	vector<CEffect*> m_pEffects2;
	vector<CEffect*> m_pEffects3;

	_float m_fTime = 0.f;
	_bool  m_bBullet = false;

	_bool m_bAlphenBoost_1 = false;
	_bool m_bAlphenBoost_2 = false;
	_bool m_bLawBoost = false;

	_bool m_bAlphenFloor = false;
	_bool m_bAlphenStrike_1 = false;
	_bool m_bAlphenStrike_2 = false;

	_float m_fEventStart = -1.f;

	_int m_iCurrentAnimIndex = 0;
	_int m_eCurrentPlayerID = 0;
	CBaseObj* m_pTarget = nullptr;


};
END
END