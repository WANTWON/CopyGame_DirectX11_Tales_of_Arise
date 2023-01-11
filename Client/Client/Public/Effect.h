#pragma once
#include "Client_Defines.h"
#include "BaseObj.h"
#include "GameInstance.h"

BEGIN(Engine)
class CModel;
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEffect abstract : public CBaseObj
{
public:
	enum EFFECTTYPE { MODEL, VIBUFFER_RECT};


	typedef struct EffectTag
	{
		EFFECTTYPE eEffectType = MODEL;
		_uint	   eEffectID = 0;
		_uint	   iTextureNum = 0;

		_vector	   vInitPositon = XMVectorSet(0.f,0.f,0.f,1.f);
		_float3	   vInitScale = _float3(0.f, 0.f, 0.f);
		_vector	   vColor = XMVectorSet(255.f, 255.f, 255.f, 255.f);

		_vector	   vDistance = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		_vector	   vLook = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		
		_float	   fDeadTime = 0.f;
		_float	   fStartTime = 0.f;

		CBaseObj*  pTarget = nullptr;
	}EFFECTDESC;

protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Target(CBaseObj* pObject) { m_pTarget = pObject; }


protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID() { return S_OK; };
	virtual void Change_Animation(_float fTimeDelta) {};
	virtual void Change_Texture(_float fTimeDelta) {};

protected: /* For.Components */
	CModel*					m_pModelCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CBaseObj*				m_pTarget = nullptr;

	_float					m_fDeadtime = 0.f;
	_float					m_fAnimSpeed = 1.f;
	_float					m_fTexUV = 0.f;
	_float3					m_vDirection = _float3(0.f, 0.f, 0.f);

	_vector					m_vColorBack = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_vector					m_vColorFront = XMVectorSet(1.f, 1.f, 1.f, 1.f);

	EFFECTDESC				m_EffectDesc;
	_float4x4				m_CombinedWorldMatrix;

public:
	virtual void Free() override;
};

END