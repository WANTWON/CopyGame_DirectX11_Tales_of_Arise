#pragma once
#include "Monster.h"


BEGIN(Client)
class CCucco final : public CMonster
{
public:
	enum STATE { FLY_ATTACK, LANDING, LIFTED, RUN, THROWN, IDLE, WALK };

private:
	CCucco(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCucco() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _uint Take_Damage(float fDamage, void* DamageType, CBaseObj* DamageCauser) override;
	void	Set_Angry(_bool type);
	void	Set_State(STATE eState) { m_eState = eState; };
	virtual HRESULT SetUp_ShaderID() override;
private:
	virtual void Change_Animation(_float fTimeDelta) override;
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual _bool IsDead() override;
	virtual void Find_Target() override;
	virtual void Follow_Target(_float fTimeDelta) override;
	virtual void Sound_By_State(_float fTimeDelta) override;
	void Check_Navigation(_float fTimeDelta);

private:
	virtual void AI_Behaviour(_float fTimeDelta) override;
	void Patrol(_float fTimeDelta);

private:
	STATE m_eState = IDLE;
	_bool m_bAngry = false;
	_float m_fHeight = 0.f;
	_float m_fTime = 0.0f;
	_float m_fVoiceTime = 0.f;

	
public:
	static CCucco* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	

};

END