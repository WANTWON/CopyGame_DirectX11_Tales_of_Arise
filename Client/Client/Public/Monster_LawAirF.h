#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
BEGIN(MonsterLaw)

class CMonster_LawAirF final : public CMonsterLawState
{
public:
	CMonster_LawAirF(class CMonsterLaw* pPlayer);//, _float fStartHeight = 0.f, _float fTime = 0.f);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta) override;


	virtual void Enter() override;
	virtual void Exit() override;


private:
	_matrix m_StartMatrix;

	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;
	_float m_fEventStart = -1.f;

private:

	CCollider* m_pLandCollider = nullptr;

	_matrix m_ColliderMatrix = XMMatrixIdentity();

	//CCollider* m_pLeftHandCollider = nullptr;
	//CCollider* m_pRightHandCollider = nullptr;
	//CCollider* m_pLeftFootCollider = nullptr;
	CCollider* Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition);
	//_bool m_bIsLoop = false;


};
END
END