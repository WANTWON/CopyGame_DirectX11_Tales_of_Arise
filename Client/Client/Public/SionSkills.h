#pragma once
#include "Bullet.h"

BEGIN(Client)
class CSionSkills final : public CBullet
{
public:

	enum TYPE { NORMALATTACK, BOOST, GRAVITY, GRAVITY_DEAD, MAGNA_RAY, TRESVENTOS, GLACIA };


public:
	CSionSkills(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSionSkills(const CSionSkills& rhs);
	virtual ~CSionSkills() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual void Collision_Check() override;
	void Dead_Effect();

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;

private:
	void Tick_NormalAttack(_float fTimeDelta);
	void Tick_GlaciaAttack(_float fTimeDelta);
	void Tick_BoostAttack(_float fTimeDelta);
	void Tick_GravityAttack(_float fTimeDelta);
	void Tick_TresVentos(_float fTimeDelta);

private:
	vector<CEffect*> m_pBlastEffect;
	vector<CEffect*> m_pSmoke;

public:
	static CSionSkills* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

};

END