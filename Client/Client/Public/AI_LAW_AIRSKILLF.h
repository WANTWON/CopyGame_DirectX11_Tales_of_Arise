#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)

class CAI_LAW_AIRSKILLF final : public CAIState
{
public:
	CAI_LAW_AIRSKILLF(class CPlayer* pPlayer, STATE_ID eStateType);//, _float fStartHeight = 0.f, _float fTime = 0.f);

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


private:
	CCollider* m_pLandCollider = nullptr;

	_matrix m_ColliderMatrix = XMMatrixIdentity();

	/* Effect Booleans */
	_bool m_bEnhaBakusaiken_1 = false;	// F (Air)
	_bool m_bEnhaBakusaiken_2 = false;	// F (Air)

										/* Effect Vectors */
	vector<class CEffect*> m_EnhaBakusaiken_1;

private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
	HITLAGDESC m_HitLagDesc;
};
END
END