#pragma once

#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)
class CLawAttackState final : public CPlayerState
{
public:
	CLawAttackState(class CPlayer* pPlayer, STATE_ID eStateType, _float fTime = 0.f);

	virtual CPlayerState* HandleInput() override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Reset_Skill(void);

private:
	CCollider* m_pLeftHandCollider = nullptr;
	CCollider* m_pRightHandCollider = nullptr;
	CCollider* m_pLeftFootCollider = nullptr;
	CCollider* m_pRightFootCollider = nullptr;

	_float m_fEventStartTime = -1.f;

	/* Effect Booleans */
	_bool m_bNormalAttack_1 = false;			
	_bool m_bNormalAttack_2 = false;
	_bool m_bNormalAttack_3 = false;
	_bool m_bNormalAttack_4 = false;
	_bool m_bNormalAttack_5 = false;
	_bool m_bNormalAttackAir_1 = false;
	_bool m_bNormalAttackAir_2 = false;
	_bool m_bNormalAttackAir_3 = false;
	_bool m_bNormalAttackAir_4 = false;
	_bool m_bNormalAttackAir_5 = false;

private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
};
END
END