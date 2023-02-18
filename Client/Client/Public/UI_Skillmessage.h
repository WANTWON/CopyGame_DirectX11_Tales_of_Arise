#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Skillmessage final : public CUI_Base
{
private:
	CUI_Skillmessage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Skillmessage(const CUI_Skillmessage& rhs);
	virtual ~CUI_Skillmessage() = default;


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
	static CUI_Skillmessage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }


	//wchar_t* CharToWChar(const char* pstrSrc);


	void Skillmsg_on(_uint index , _bool itemuse = false);//, _uint index1);l


	enum SKILLNAME {
		SKILLNAME_CHOOSAWOO,
		SKILLNAME_BEEYEONIN,
		SKILLNAME_BOONGSUPGEEAKJIN,
		SKILLNAME_SUMGONGSANGEULPA,
		SKILLNAME_YOUSEONGJIN,
		SKILLNAME_ONEWALL,
		SKILLNAME_GRAVITY,
		SKILLNAME_MAGNARAY,
		SKILLNAME_EXPLODE,
		SKILLNAME_AQUARUINA,
		SKILLNAME_GLACIA,
		SKILLNAME_TRESVENTUS,
		SKILLNAME_THUNDERFIELD,
		SKILLNAME_GALEFORCE,
		SKILLNAME_METEORSWARM,
		SKILLNAME_DIVINESABER,
		SKILLNAME_BANGJEON,
		SKILLNAME_HOLYRANCE,
		SKILLNAME_WARYUNGGONGPA,
		SKILLNAME_YOONMUSUNPOONG,
		SKILLNAME_CHOYUNATAN,
		SKILLNAME_SANHWAMANGSUP,
		SKILLNAME_YUENSOKWAN,
		SKILLNAME_YUMPAEFUCKSAY
	};

private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*               m_pTextureCom2 = nullptr;

	_bool m_bfadein = true;
	_bool m_bfadeout = false;

	_bool m_bmoveleft = true;

	_float m_fFadeX = 180.f;

	_uint m_iNum = 5;
	//0추사우
	//1비연인
	//2원월
    //3유성진
	//4붕습지악진
	//5
	

	

	_float m_fFontsize = 0.9f;
	_float m_fFontOffsetY = 35.f;







	_bool m_bgoup = false;
	_float m_fAlpha1 = 0.f;
	_bool m_bseconton = false;
	//vector<vector>
	_float timer = 0.f;
	_float m_fDietimer = 0.f;
	_bool m_bfirstarrived = false;
	_bool m_bDeadtimeron = false;

	_uint vectorsize = 0;

	_bool m_btick = false;

	_float m_fIndexOffsetY = 0.f;

	_bool m_bUseItem = false;




public:
	typedef struct tagitempopup
	{
		//_float2 position = { 0.f,0.f };
		_uint iIndex = 0;
		_uint iCount = 0;
		ITEM_NAME eName = ITEMNAME_END;
		ITEM_TYPE eType = ITEMTYPE_END;

	}POPUPDESC;

	POPUPDESC m_popupdesc;
};

END