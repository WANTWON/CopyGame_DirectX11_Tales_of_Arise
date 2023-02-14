#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_BattleResult final : public CUI_Base
{
private:
	CUI_BattleResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BattleResult(const CUI_BattleResult& rhs);
	virtual ~CUI_BattleResult() = default;


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
	HRESULT Render_Player1();
	HRESULT Render_Player2();
	HRESULT Render_Player3();
	HRESULT Render_Player4();
	HRESULT Render_Cp();
	HRESULT Render_Exp();

public:
	static CUI_BattleResult* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }


	//wchar_t* CharToWChar(const char* pstrSrc);


	void Skillmsg_on(_uint index);//, _uint index1);l


	/*enum SKILLNAME {
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
		SKILLNAME_TRESVENTUS
	};*/

private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*               m_pTextureCom2 = nullptr;
	CTexture*               m_pTextureCom3 = nullptr;
	CTexture*               m_pTextureCom4 = nullptr;
	CTexture*               m_pTextureCom5 = nullptr;
	CTexture*               m_pTextureCom6 = nullptr;
	CTexture*               m_pTextureCom7 = nullptr;
	CTexture*               m_pTextureCom8 = nullptr;
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

	_uint m_ihpnum = 0;
	_float m_fbrightpos_hpfont[9] = { 0.f , -0.5f, -1.f, -1.5f , -2.f , -2.5f, -3.f , -3.5f , -4.f };

	_float m_PlayersCurrentHP[4] = { 0.f ,0.f , 0.f, 0.f };
	_float m_PlayersMaxHP[4] = { 0.f , 0.f, 0.f, 0.f };

	_uint m_PlayersCurrentExp[4] = { 0 ,0 , 0, 0 };
	_uint m_PlayersMaxExp[4] = { 0 , 0, 0, 0 };
	_uint m_PlayersLevel[4] = { 0, 0 , 0, 0 };

	_float m_PlayersHPAlpha[4] = { 0.f , 0.f , 0.f, 0.f };


	_float m_fbrightpos = 0.f;
	_float m_fbrightpos_hpbar = 0.f;
	_float m_fMainAlpha = 0.f;

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

	_bool m_PlayersHPAlphadown[4] = { false , false , false , false };
	_bool m_PlayersHPAlphaup[4] = { false , false , false , false };

	_float m_fLevelactiontimer[4] = { 0.f, 0.f, 0.f , 0.f };
	_bool m_bLevelaction[4] = { false ,false,  false , false };
	_bool m_bRuneOn[4] = { true , true , true , true };
	_bool m_bLevelupOn[4] = { true , true , true , true };
	_float m_fGainExp = 0.f;
	_bool m_bExpFinish = false;
	_bool m_bFirst = true;
	_float m_fmaxcp = 0.f;
	_float m_fcurrentcp = 0.f;
	_uint m_icpnum = 0;
	_float m_fGainExp1 = 0.f;
		

	
//public:
//	typedef struct tagitempopup
//	{
//		//_float2 position = { 0.f,0.f };
//		_uint iIndex = 0;
//		_uint iCount = 0;
//		ITEM_NAME eName = ITEMNAME_END;
//		ITEM_TYPE eType = ITEMTYPE_END;
//
//	}POPUPDESC;
//
//	POPUPDESC m_popupdesc;
};

END