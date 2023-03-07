#pragma once

#include "Bullet.h"

BEGIN(Client)
class CBossSkills final : public CBullet
{
public:
	enum TYPE
	{
		BULLET_SPEAR_MULTI_1,
		BULLET_SPEAR_MULTI_2,
		BULLET_SPEAR_MULTI_3,
		BULLET_SPEAR_MULTI_4,
		BULLET_SPEAR_MULTI_5,
		BULLET_SPEAR_MULTI_6,
		BULLET_SPEAR_DYNAMIC,
		BULLET_LASER,
		BULLET_END 
	};

private:
	typedef struct tagFocusPointDescription
	{
		_float3 vFocusPosition = _float3(0.f, 0.f, 0.f);
		_bool bDidFocus = false;
	} FOCUSPOINTDESC;

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
	void Tick_Laser(_float fTimeDelta);
	void LateTick_Laser(_float fTimeDelta);

	/* Effects Functions */
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

	vector<FOCUSPOINTDESC> m_LaserFocusPoints;
	_float3 m_vFocusPoint;
	_float3 m_vNextFocusPoint;
	_float m_fFocusTimer = 0.f;
	_uint m_iCurrentFocusPoint = 0;
};
END