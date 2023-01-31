#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Get_item_Popup final : public CUI_Base
{
private:
	CUI_Get_item_Popup(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Get_item_Popup(const CUI_Get_item_Popup& rhs);
	virtual ~CUI_Get_item_Popup() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CUI_Get_item_Popup* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }

	
	//wchar_t* CharToWChar(const char* pstrSrc);


	void Open_Dialogue(_uint index);//, _uint index1);l


private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*               m_pTextureCom2 = nullptr;
	_bool m_bfadein = true;
	_bool m_bfadein1 = false;
	_bool m_bfadeout = false;

	_bool m_bmoveleft = true;

	_float m_fFadeX = -50.f;
	_float m_fFadeY = -20.f;
	_float m_fFade1X = -50.f;
	_float m_fFade1Y = -20.f;



	_float m_fFontsize = 0.9f;
	_float m_fFontOffsetY = 35.f;



	/*_float2					m_fFontPos = { 0.f,0.f };
	_float2                 m_fFontPos1 = { 0.f , 0.f };

	_float m_FontR, m_FontG, m_FontB = 0.f;

	_tchar					m_szTXT[MAX_PATH] = TEXT("");
	char fuck[256];
	vector<_tchar*> m_vDialoguepopup[2];
	vector<_tchar*> m_vDialoguepopup1[2];
	vector<vector<vector<_tchar*>>> m_vCurrentDialogue;*/
	_uint m_iDialogueindex = 0;

	/*_uint m_iVectorIndex = 0;
	_uint m_iVectorIndex1 = 1;*/

	_bool m_bgoup = false;
	_float m_fAlpha1 = 0.f;
	_bool m_bseconton = false;
	//vector<vector>
	_float timer = 0.f;
	_float m_fDietimer = 0.f;
	_bool m_bfirstarrived = false;
	_bool m_bDeadtimeron = false;

	_uint vectorsize = 0;

	_bool m_btick = true;

	_float m_fIndexOffsetY = 0.f;


	

public:
	typedef struct tagitempopup
	{
		//_float2 position = { 0.f,0.f };
		_uint iIndex = 0;
		_uint iCount = 0;
		ITEM_NAME eName = ITEMNAME_END;
	    ITEM_TYPE eType= ITEMTYPE_END;

	}POPUPDESC;

	POPUPDESC m_popupdesc;
};

END