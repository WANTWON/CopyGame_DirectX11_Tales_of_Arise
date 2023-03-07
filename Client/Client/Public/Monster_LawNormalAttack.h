#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
BEGIN(MonsterLaw)

class CMonster_LawNormalAttack final : public CMonsterLawState
{
public:
	CMonster_LawNormalAttack(class CMonsterLaw* pPlayer, STATE_ID state, CPlayer* pTarget , _int phase);//, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta) override;


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

	/* Effect Booleans */
	_bool m_bNormalAttack_1 = false;
	_bool m_bNormalAttack_2 = false;
	_bool m_bNormalAttack_3 = false;
	_bool m_bNormalAttack_4 = false;
	_bool m_bNormalAttack_5 = false;


private:
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
};
END
END