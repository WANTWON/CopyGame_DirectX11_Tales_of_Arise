#pragma once

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	_float Get_CamDistance() const {return m_fCamDistance;}
	_bool Get_Dead() { return m_bDead; }
	void Set_Dead(_bool bDead) { m_bDead = bDead; }

public:
	class CComponent* Find_Component(const _tchar* pComponentTag);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() { return NOERROR; }

	virtual _bool Picking(_float3* PickingPoint) { return false; }
	virtual void PickingTrue() { return; }

protected:
	ID3D11Device* m_pDevice = nullptr; 
	ID3D11DeviceContext* m_pContext = nullptr;
	_bool m_bDead = false;
	float m_fCamDistance = 0.f;
	_float4x4 m_CollisionMatrix;

protected:
	map<const _tchar*, class CComponent*> m_Components;

protected:
	HRESULT Add_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);
	void Compute_CamDistance(_fvector vWorldPos);

public:	
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};
END

