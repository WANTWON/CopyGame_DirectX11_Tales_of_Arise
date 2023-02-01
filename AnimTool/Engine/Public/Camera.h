#pragma once

#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract :	public CGameObject
{
public:
	typedef struct tagCameraDesc
	{
		_float3 vEye;
		_float3 vAt;

		_float fAspect;
		_float fFovY;
		_float fNear;
		_float fFar;

		CTransform::TRANSFORMDESC TransformDesc;
	}CAMERADESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render(void);

protected:
	static _tchar* m_pTransformTag;
	class CTransform* m_pTransform = nullptr;
	CAMERADESC	m_CameraDesc;
	class CPipeLine* m_pPipeLine = nullptr;

protected:
	HRESULT Bind_OnGraphicDev(void);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free(void) override;
};

END