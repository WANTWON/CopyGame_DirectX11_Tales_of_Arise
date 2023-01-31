#pragma once

#include "Base.h"

/* 게임객체들의 부모가되는 클래스다. */

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagPivotMatrix
	{
		_float3 vPosition;
		_float3 vScale;
		_float3 vRotation;
	}PIVOT;
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	_float Get_CamDistance(void) const { return m_fCamDistance; }

	_bool Get_Dead(void) { return m_bDead;}
	void Set_Dead(void) { m_bDead = true; }

public:
	class CComponent* Find_Component(const _tchar* pComponentTag);

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render(void);

protected:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	_float m_fCamDistance = 0.f;

	_bool m_bDead = false;
	
protected:
	map<const _tchar*, class CComponent*> m_Components;

protected:
	HRESULT Add_Components(const _tchar* pComponentTag, _uint iLevelIndex, const _tchar* pPrototypeTag, CComponent** ppOut, void* pArg = nullptr);
	//void Compute_CamDistance(_float3 vWorldPos);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free(void) override;
};

END