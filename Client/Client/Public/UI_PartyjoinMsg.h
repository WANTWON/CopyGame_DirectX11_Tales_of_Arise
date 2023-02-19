#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_PartyjoinMsg final : public CUI_Base
{
private:
	CUI_PartyjoinMsg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PartyjoinMsg(const CUI_PartyjoinMsg& rhs);
	virtual ~CUI_PartyjoinMsg() = default;


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
	static CUI_PartyjoinMsg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;


	//wchar_t* CharToWChar(const char* pstrSrc);

public:


	void Open_sysmsg(_uint index);//, _uint index1);l
								  //void Close_sysmsg() { m_bInterect = false; }


private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*               m_pTextureCom2 = nullptr;
	CTexture*				m_pTextureCom3 = nullptr;
	CTexture*               m_pTextureCom4 = nullptr;
	CTexture*				m_pTextureCom5 = nullptr;

	_bool m_bfadein = true;
	_bool m_bfadein1 = true;
	_bool m_bfadeout = false;


public:
	typedef struct tagitemmsgbox
	{
		//_float2 position = { 0.f,0.f };
		_uint iIndex = 0;
		_uint iCount = 0;
		ITEM_NAME eName = ITEMNAME_END;
		ITEM_TYPE eType = ITEMTYPE_END;

	}MSGBOXDESC;

	MSGBOXDESC m_msgboxdesc;


	//vector<vector>



};

END