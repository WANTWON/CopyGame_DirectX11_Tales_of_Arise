#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
class CEffect;
BEGIN(MonsterLaw)
class CMonster_LawAirR final : public CMonsterLawState
{
public:
	CMonster_LawAirR(class CMonsterLaw* pPlayer);//, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta) override;


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
	CCollider* m_pLeftHandCollider = nullptr;
	CCollider* m_pRightHandCollider = nullptr;
	CCollider* m_pLeftFootCollider = nullptr;
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
	_bool m_bIsLoop = false;

	/* Effect Booleans */
	_bool m_bSankamousyuukyaku_1 = false;				// R (Air)
	_bool m_bSankamousyuukyaku_2 = false;				// R (Air)
	_bool m_bSankamousyuukyaku_Punch_1 = false;			// R (Air)
	_bool m_bSankamousyuukyaku_Punch_2 = false;			// R (Air)
	_bool m_bSankamousyuukyaku_Particles_1 = false;		// R (Air)
	_bool m_bSankamousyuukyaku_Particles_2 = false;		// R (Air)

	_vector m_vPunchPosition_1;
	_vector m_vPunchPosition_2;

	/* Effect Vectors */
	vector<CEffect*> m_Sankamousyuukyaku_1;
	vector<CEffect*> m_Sankamousyuukyaku_Punch_1;
	vector<CEffect*> m_Sankamousyuukyaku_Punch_2;
};
END
END