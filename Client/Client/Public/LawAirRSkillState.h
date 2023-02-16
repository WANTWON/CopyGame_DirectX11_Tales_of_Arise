#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)

class CLawAirRSkillState final : public CPlayerState
{
public:
	CLawAirRSkillState(class CPlayer* pPlayer, STATE_ID eStateType, _float fStartHeight = 0.f, _float fTime = 0.f);
	
	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	_float m_fStartHeight;
	_float m_fTime;

	CCollider* m_pLeftHandCollider = nullptr;
	CCollider* m_pRightHandCollider = nullptr;
	CCollider* m_pLeftFootCollider = nullptr;

	_float m_fEventStartTime = -1.f;

	_bool m_bIsLoop = false;

private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
};
END
END