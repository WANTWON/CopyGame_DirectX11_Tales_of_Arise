#pragma once
#include "Monster.h"


BEGIN(Client)
class CRola final : public CMonster
{
public:
	enum STATE { DAMAGE,  DEAD, DEAD_ST, JUMP, JUMP_ED, JUMP_ST, PUSH, IDLE};
	enum DIR { RIGHT, LEFT };

private:
	CRola(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRola() = default;

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
	virtual _bool IsDead() override;
	virtual void Find_Target() override;
	virtual void Follow_Target(_float fTimeDelta) override;

private:
	virtual void AI_Behaviour(_float fTimeDelta) override;
	virtual void Check_Navigation(_float fTimeDelta) override;
	virtual HRESULT SetUp_ShaderID() override;
	
	void	Patrol(_float fTimeDelta);
	_bool	Moving_AttackPosition(_float fTimeDelta);
	void	Jumping_Attack(_float fTimeDelta);
	void	Make_PushEffect();
	

private:
	STATE m_eState = IDLE;
	STATE m_ePreState = PUSH;

	_bool m_bJump = false;
	_bool m_bSpecialAttack = false;
	_bool m_bBackStep = false;
	_bool m_bSoundPlay = false;
	_int  m_iDmgCount = 3;

	_vector m_vAttackPos = { 109.7f, 0.f, 33.75f, 1.f };

	DIR		m_eAttackDir = RIGHT;
	_vector m_vDirection = { 0.f,0.f,0.f,0.f };

	_float					m_fEffectTime = 0.f;
	_float					m_fEffectEndTime = 0.f;
	_float					m_fZoomValue = 0.f;
public:
	static CRola* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END