#pragma once
#include "Monster.h"


BEGIN(Client)
class COctorock final : public CMonster
{
public:
	enum STATE { ATTACK_ED, ATTACK_ST, DAMAGE, DEAD, DEAD_FIRE , FALL,
		PIYO, IDLE, WALK };

private:
	COctorock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~COctorock() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _uint Take_Damage(float fDamage, void* DamageType, CBaseObj* DamageCauser) override;


private:
	virtual void Change_Animation(_float fTimeDelta) override;
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderID() override;
	virtual _bool IsDead() override;
	virtual void Find_Target() override;
	virtual void Follow_Target(_float fTimeDelta) override;
	void Check_Navigation(_float fTimeDelta);

private:
	virtual void AI_Behaviour(_float fTimeDelta) override;
	void Patrol(_float fTimeDelta);
	void Make_Bullet();

private:
	STATE m_eState = IDLE;
	_bool m_bMakeBullet = false;
	_bool m_bFirst = false;

public:
	static COctorock* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	

};

END