#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)

class CLawAirRSkillState final : public CPlayerState
{
public:
	CLawAirRSkillState(class CPlayer* pPlayer, STATE_ID eStateType);
	
	virtual CPlayerState* HandleInput(void) override;
	virtual CPlayerState* Tick(_float fTimeDelta) override;
	virtual CPlayerState* LateTick(_float fTimeDelta) override;
	virtual CPlayerState* EventInput(void) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Update_Skill(void);
	void Remove_Skill(void);
	void Reset_Skill(void);

private:
	CCollider* m_pLeftHandCollider = nullptr;
	CCollider* m_pRightHandCollider = nullptr;
	CCollider* m_pLeftFootCollider = nullptr;

	_float m_fEventStartTime = -1.f;

	_bool m_bIsLoop = false;

	CBaseObj* m_pTarget = nullptr;

	/* Effect Booleans */
	_bool m_bSankamousyuukyaku_1 = false;				// R (Air)
	_bool m_bSankamousyuukyaku_2 = false;				// R (Air)
	_bool m_bSankamousyuukyaku_Punch_1 = false;			// R (Air)
	_bool m_bSankamousyuukyaku_Punch_2 = false;			// R (Air)

	_vector m_vPunchPosition_1;
	_vector m_vPunchPosition_2;

	/* Effect Vectors */
	vector<class CEffect*> m_Sankamousyuukyaku_1;
	vector<class CEffect*> m_Sankamousyuukyaku_Punch_1;
	vector<class CEffect*> m_Sankamousyuukyaku_Punch_2;

	//Sound
	_bool m_bSkill_R_Sound = false;
private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
	HITLAGDESC m_HitLagDesc;
};
END
END