#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)

class AI_LAW_NomalAttack_State final : public CAIState
{
public:
	AI_LAW_NomalAttack_State(class CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget);//, _float fStartHeight = 0.f, _float fTime = 0.f);

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

private:

	CCollider* m_pLeftHandCollider = nullptr;
	CCollider* m_pRightHandCollider = nullptr;
	CCollider* m_pLeftFootCollider = nullptr;
	CCollider* m_pRightFootCollider = nullptr;

	_float m_fEventStartTime = -1.f;

private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
	HITLAGDESC m_HitLagDesc;
};
END
END