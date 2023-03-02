#pragma once

#include "Bullet.h"

BEGIN(Client)
class CBossSkills final : public CBullet
{
public:
	enum TYPE {
		BULLET_SPEAR_MULTI_1,
		BULLET_SPEAR_MULTI_2,
		BULLET_SPEAR_MULTI_3,
		BULLET_SPEAR_MULTI_4,
		BULLET_SPEAR_MULTI_5,
		BULLET_SPEAR_MULTI_6,
		BULLET_END };

public:
	CBossSkills(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBossSkills(const CBossSkills& rhs);
	virtual ~CBossSkills() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	
protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual void Collision_Check() override;

private:
	void Tick_SpearMulti(_float fTimeDelta);
	void Dead_Effect();
	void Stop_Effect();

public:
	static CBossSkills* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:
	vector<CEffect*> m_SpearMulti;

	_float m_fBulletTimer = 0.f;
	_bool m_bCollided = false;
};
END