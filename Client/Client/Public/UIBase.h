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
	enum SHADERUI_ID { UI_ALPHADEFAULT, UI_ALPHABLEND, UI_PICKED, UI_ALPHASET, UI_SCREEN, UI_EFFECTFADEOUT,UI_HPBAR,UI_COMBOLINE,UI_GOLDEN, UI_CP_GUAGE,
		UI_CP_GUAGE_BLACK,UI_MP_GUAGE, UI_COLOR_BLACK, UI_POTRAIT_DARK , UI_POTRAIT_READY, UI_POTRAIT_ALLBLUE, UI_RUNECOLOR,
		UI_LOCKON_REVERSE ,UI_UVROT , UI_INVENICON, UI_INVENBACK,UI_MENULINE, UI_INVENTOPBOTTOM,UI_INVENTOPBOTTOMALPHA, UI_GALDBACK, UI_UVROTSTRONG,UI_ALPHAPORTRAIT,
		UI_LIGHTEFFECT, UI_BRIGHT,UI_BLUEHPBAR,UI_DIALOGUEBOX,UI_DIALOGUELINE,UI_DIALOGUECURSOR, UI_DIALOGUECURSORNONMOVE,UI_ALPHATESTSET,
		UI_BRIGHTBLACK,UI_BRIGHTDIALOGUELINE, UI_OUTLINE, UI_OUTLINE2, UI_BOOSTGUAGE,
		UI_EFFECTSCREEN ,UI_END };

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
	_float3	Get_Scale_float3() { return m_vScale; };
	void Set_Position(_float2 fPostion) { m_fPosition = fPostion; };
	void Set_Scale(_float2 fScale) { m_fSize = fScale; };
	void Set_Scale(_float3 vScale);
	_bool Get_Isdead() { return m_bIsdead; }
	void SetUp_BillBoard();

	

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


	_float3			m_vScale = _float3(1.f, 1.f, 1.f);

	_bool                   m_bisinLoading = false;

	_bool m_bIsRender = false;
	_bool m_bIsdead = false;
	

public:
	virtual HRESULT Ready_Components(void * pArg = nullptr) = 0;
	virtual HRESULT SetUp_ShaderResources() = 0; /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() { return S_OK; };


public:
	virtual void Free() override;
};

END