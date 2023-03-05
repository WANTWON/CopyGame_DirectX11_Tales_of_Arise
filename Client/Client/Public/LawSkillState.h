#pragma once
#include "PlayerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Player)

class CLawSkillState final : public CPlayerState
{
public:
	CLawSkillState(class CPlayer* pPlayer, STATE_ID eStaetType, _float fTime = 0.f);

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

	/* Effect Vectors */
	vector<class CEffect*> m_Garyoukuuha;
	vector<class CEffect*> m_TyourengadanPunch;
	vector<class CEffect*> m_BurnKnuckle;

private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
	HITLAGDESC m_HitLagDesc;
};
END
END