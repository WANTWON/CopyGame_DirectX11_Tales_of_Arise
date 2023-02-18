#pragma once
#include "AIState.h"

BEGIN(Client)
class CEffect;
BEGIN(AIPlayer)

class CAI_LAW_SkillAttack_State final : public CAIState
{
public:
	CAI_LAW_SkillAttack_State(class CPlayer* pPlayer, STATE_ID eStaetType);

	
	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Reset_Skill(void);

private:
	_float m_fStartHeight;
	_float m_fTime;

	CCollider* m_pLeftHandCollider = nullptr;
	CCollider* m_pRightHandCollider = nullptr;
	CCollider* m_pLeftFootCollider = nullptr;
	CCollider* m_pRightFootCollider = nullptr;

	_float m_fEventStartTime = -1.f;

	CBaseObj* m_pTarget = nullptr;

private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
};
END
END