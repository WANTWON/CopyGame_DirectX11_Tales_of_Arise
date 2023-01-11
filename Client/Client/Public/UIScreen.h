#pragma once
#include "Obj_UI.h"

BEGIN(Client)

class CUIScreen final : public CObj_UI
{
public:
	enum VISIBLESCENE { VISIBLE_LOGO, VISIBLE_LOADING, VISIBLE_PLAYGAME, VISIBLE_EFFECT, VISIBLE_SCREEN };
	enum TEXNUM { INVEN, FIELD_MAP, NO_MAP, DGN_MAP_MASK, DGN_MAP_OPEN };

	typedef struct backgroundTag
	{
		_tchar* pTextureTag = nullptr;
		VISIBLESCENE eVisibleScreen = VISIBLE_PLAYGAME;
	}BACKGROUNDESC;

private:
	CUIScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIScreen(const CUIScreen& rhs);
	virtual ~CUIScreen() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources()override; /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() override;

private:


	BACKGROUNDESC m_BackgroundDesc;
	_float		  m_fAlpha = 1.f;
	_float2		  m_fTexUV = _float2(0.f, 0.f);
	CTexture*	  m_pFogtexture = nullptr;

public:
	static CUIScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END