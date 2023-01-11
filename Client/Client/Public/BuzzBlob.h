#pragma once
#include "Monster.h"


BEGIN(Client)
class CBuzzBlob final : public CMonster
{
public:
	enum STATE { DAMAGE, DEAD, IDLE, WALK};

private:
	CBuzzBlob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBuzzBlob() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual void Check_Navigation(_float fTimeDelta)  override;

public:
	virtual _uint Take_Damage(float fDamage, void* DamageType, CBaseObj* DamageCauser) override;
	virtual HRESULT SetUp_ShaderID() override;

private:
	virtual void Change_Animation(_float fTimeDelta) override;
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual _bool IsDead() override;
	virtual void Find_Target() override;
	virtual void Follow_Target(_float fTimeDelta) override;

private:
	virtual void AI_Behaviour(_float fTimeDelta) override;


private:
	STATE m_eState = IDLE;

public:
	static CBuzzBlob* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END