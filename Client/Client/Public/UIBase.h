#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "UI_Manager.h"

BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

END

BEGIN(Client)

class CUI_Base abstract : public CGameObject
{
public:
	enum SHADERUI_ID { UI_ALPHADEFAULT, UI_ALPHABLEND, UI_PICKED, UI_ALPHASET, UI_SCREEN, UI_EFFECTFADEOUT, UI_EFFECTSCREEN };

protected:
	CUI_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Base(const CUI_Base& rhs);
	virtual ~CUI_Base() = default;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_float2 Get_Position() { return m_fPosition; }
	_float2 Get_Scale() { return m_fSize; }
	void Set_Position(_float2 fPostion) { m_fPosition = fPostion; };
	void Set_Scale(_float2 fScale) { m_fSize = fScale; };

protected:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

protected:
	_float2					m_fPosition, m_fSize;
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	_uint					m_eShaderID = UI_ALPHADEFAULT;
	_float					m_fAlpha = 1.f;

	_bool                   m_bisinLoading = false;


public:
	virtual HRESULT Ready_Components(void * pArg = nullptr) = 0;
	virtual HRESULT SetUp_ShaderResources() = 0; /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() { return S_OK; };


public:
	virtual void Free() override;
};

END