#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
class CEffect;
BEGIN(MonsterLaw)

class CMonster_LawSkill final : public CMonsterLawState
{
public:
	CMonster_LawSkill(class CMonsterLaw* pPlayer, STATE_ID eStaetType);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	void Update_Skill(void);
	void Remove_Skill(void);
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

	/* Effect Booleans */
	_bool m_bRondsenpu = false;				// E 
	_bool m_bGaryoukuuha = false;			// R
	_bool m_bTyourengadan_1 = false;		// F
	_bool m_bTyourengadan_2 = false;		// F	
	_bool m_bTyourengadan_3 = false;		// F
	_bool m_bTyourengadan_4 = false;		// F
	_bool m_bTyourengadan_5 = false;		// F
	_bool m_bTyourengadan_Kick = false;		// F
	_bool m_bTyourengadan_Punch = false;	// F
	_vector m_vPunchPosition;				// F

	_bool m_bBurnKnuckle = false;			// E (Air)
	_bool m_bBurnKnuckleParticles = false;	// E (Air)
	_vector m_vBurnKnucklePosition;			// E (Air)
	_bool m_bEnhaBakusaiken = false;		// F (Air)

	_bool m_bLawBoost = false;
	_float m_fEventStart = -1.f;

											/* Effect Vectors */
	vector<class CEffect*> m_Garyoukuuha;
	vector<class CEffect*> m_TyourengadanPunch;
	vector<class CEffect*> m_BurnKnuckle;

	//Sound
	_bool m_bSkill_E_Sound = false;
	_bool m_bSkill_R_Sound = false;
	_bool m_bSkill_F_Sound = false;

private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
};
END
END