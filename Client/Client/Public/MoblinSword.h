#pragma once
#include "Monster.h"


BEGIN(Client)
class CMoblinSword final : public CMonster
{
public:
	enum STATE {
		DAMAGE_B, DAMAGE_F, DEAD_B, DEAD_F, DEAD_FIRE, FALL, FIND,
		GUARD, KYOROKYORO, PIYO, STAGGER, STANCE_WAIT, STANCE_WALK, IDLE, WALK};

private:
	CMoblinSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMoblinSword() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _uint Take_Damage(float fDamage, void* DamageType, CBaseObj* DamageCauser) override;
	virtual void Sound_By_State(_float fTimeDelta) override;

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
	
private:
	STATE m_eState = IDLE;
	STATE m_ePreState = FIND;

	_bool					m_bFirst = false;
	_float					m_fEffectTime = 0.f;
	_float					m_fEffectEndTime = 0.f;

public:
	static CMoblinSword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END