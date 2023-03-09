#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)

class CAI_LAW_AIRSKILLR final : public CAIState
{
public:
	CAI_LAW_AIRSKILLR(class CPlayer* pPlayer, STATE_ID eStateType);//, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;


	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Update_Skill(void);
	void Remove_Skill(void);
	void Reset_Skill(void);

private:
	_matrix m_StartMatrix;

	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;

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
	CCollider* m_pLeftHandCollider = nullptr;
	CCollider* m_pRightHandCollider = nullptr;
	CCollider* m_pLeftFootCollider = nullptr;
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
	_bool m_bIsLoop = false;
	HITLAGDESC m_HitLagDesc;

};
END
END