#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_MENU_Back final : public CUI_Base
{
private:
	CUI_MENU_Back(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MENU_Back(const CUI_MENU_Back& rhs);
	virtual ~CUI_MENU_Back() = default;


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
	static CUI_MENU_Back* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }

public:
	HRESULT Render_MAINBACK();
	HRESULT Render_Inventory1();
	HRESULT Render_GALD();
	HRESULT Render_Player1_Status_MainMenu();
	HRESULT Render_Player2_Status_MainMenu();
	HRESULT Render_Player3_Status_MainMenu();
	HRESULT Render_Player4_Status_MainMenu();
	HRESULT Render_Player1_Status_useitem();
	HRESULT Render_Player2_Status_useitem();
	HRESULT Render_Player3_Status_useitem();
	HRESULT Render_Player4_Status_useitem();
	HRESULT Render_CP();
	HRESULT Render_Mainbottombutton();
	HRESULT Render_bottmline();
	
	HRESULT Render_Itembottombutton_useable();
	HRESULT Render_Itembottombutton_default();
	HRESULT Render_Inven_main();
	HRESULT Render_Inven_useable();
	HRESULT Render_Inven_material();
	HRESULT Render_Inven_weapon();
	HRESULT Render_Inven_armor();
	HRESULT Render_Inven_category();
	HRESULT Render_Flags();

	HRESULT Render_inven_bottom_item_forUseitem();

	void fadeinMain();

	void pingpong();

	void update_useable_inventory();
	void update_material_inventory();
	void update_weapon_inventory();
	void update_armor_inventory();

	void use_HPrecover_item();
	void RecoverPlayerHP(_uint index ){ m_fRecover_Power = m_PlayersMaxHP[index] - m_PlayersCurrentHP[index];}
	void Fill_HP(_uint playerindex);
	void check_HPrecover_item();

	void Enter_Main_to_Another_Menu();

	HRESULT Render_Questmenu();
	HRESULT Render_bottmline_Quest();
	HRESULT Render_Quest1Rewards();

	void Return_to_MainMenu();

private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*				m_pTextureCom2 = nullptr;
	CTexture*			    m_pTextureCom3 = nullptr;
	CTexture*               m_pTextureCom4 = nullptr;
	CTexture*               m_pTextureCom5 = nullptr;

	CTexture*               m_pTextureCom6 = nullptr;  // line 
	CTexture*               m_pTextureCom7 = nullptr;  // itemicon
	CTexture*               m_pTextureCom8 = nullptr;  // iteminfo
	CTexture*               m_pTextureCom9 = nullptr;  // gald

	CTexture*               m_pTextureCom10 = nullptr;  // hpbar
	CTexture*				m_pTextureCom11 = nullptr;  // hpgrad
	CTexture*               m_pTextureCom12 = nullptr;  // charactername
	CTexture*               m_pTextureCom13 = nullptr; // smallfont may be destroy
	CTexture*               m_pTextureCom14 = nullptr; // Cp

	CTexture*               m_pTextureCom15 = nullptr; // menubutton
	CTexture*               m_pTextureCom16 = nullptr; // menufont

	CTexture*               m_pTextureCom17 = nullptr; //categorynamefont

	CTexture*               m_pTextureCom18 = nullptr; //flag

	CTexture*               m_pTextureCom19 = nullptr; //itemuseportrait
	CTexture*               m_pTextureCom20 = nullptr; //lighteffect;
	CTexture*               m_pTextureCom21 = nullptr; //lighteffect;
	CTexture*               m_pTextureCom22 = nullptr; //lighteffect;
	CTexture*               m_pTextureCom23 = nullptr; //lighteffect;
	CTexture*               m_pTextureCom24 = nullptr; //lighteffect;
	CTexture*               m_pTextureCom25 = nullptr; //lighteffect;
	
	
	
	_bool m_bfadein = false;
	_bool m_bfadeout = false;

	_bool m_bmoveleft = true;

	_bool m_bRenderMain_back = true;

	_float m_fAngle = 0.f;

	_uint m_iCursor = 0;
	_uint m_iCursor_inventory1 = 0;
	_uint m_iCursor_itemuse = 0;

	_uint m_iCursor_Quest = 0;

	_bool m_bInventory1 = false;

	enum Renderpase {MENU_MAIN , MENU_INVENTORY1 , MENU_QUEST, MENU_END};
	enum Inventorytype {INVEN_MAIN , INVEN_USABLE, INVEN_MATERIAL , INVEN_WEAPON , INVEN_ARMOR , INVEN_ELSE , INVEN_END};
	_uint m_eInventory_type = INVEN_MAIN;
	Renderpase m_etype = MENU_MAIN;
	

	_float m_fCursorX = 0.f;

	_bool m_bright = false;
	//_bool m_bleft = false;
	_uint m_icount = 0;

	_float m_fFadeout_itemname = -24.f;
	_float m_fFadeout_itemnamex = -256.f;
	_float m_fFadeout_itemtype = -30.f;
	_float m_fFadeout_inventoryback = -300.f;
	_float m_fFadeout_inventorybackx = -914.f;
	_float m_fFadeout_iteminfo = -66.f;
	_float m_fFadeout_iteminfox = -350.f;
	_float m_fFadeout_cursor = -64.f;
	_float m_fFadeout_itemcount = -20.f;

	_float m_fFadeout_line = -1050.f;
	_float m_fFadeout_liney = -4.f;

	_float m_fFadeout_effect = -610.f;
	_float m_fFadeout_effecty = -52.f;

	_float m_fFadeout_categoryx = -32.f;

	_float m_fFadeout_categoryeffectx = -96.f;

	_float m_fScale_Mainback = 100.f;
	_bool m_bfadeout_inventory = false;
	_bool m_bfadein_inventory = false;
	_bool m_bgoback_menu = false;

	_float m_fgoback_menutimer = 0.f;

	_float m_fTopAlpha = 0.6f;

	_float m_fMainAlpha = 1.f;

	
	_bool m_bMainAlphaup = false;


	_bool m_biconfirstmove = true;
	_bool m_biconlastmove = false;
	_float m_ficonposition1 = 600.f;
	_float m_ficonposition2 = 600.f;
	_float m_ficonposition3 = 600.f;

	_uint m_igaldnum = 0;
	_uint m_ihpnum = 0;
	_uint m_icpnum = 0;

	_float m_fplayernameAlpha = 1.f;
	
	_float m_fmaxcp = 0.f;
	_float m_fcurrentcp = 0.f;

	_float m_fInventorymiddleAlpha = 0.f;

	_float m_fMain_Bottom_buttonY = 0.f;
	_float m_fInven_Bottom_buttonY = 160.f;

	_uint m_eCurrentItemtype = ITEMTYPE_END;

	_bool m_bupdate_inventory_once = true;

	_uint m_iInventory_category_cursor = 0;

	_float m_fAngle1 = 0.f;

	_uint m_iLeaderindex = 0;

	_bool m_busingiteminmenu = false;

	ITEM_NAME m_eSelected_UsableItem = ITEMNAME_END;

	_uint m_iRecover = 0;

	_uint m_iHpbarshaderpass = 0;

	_float m_fbackalpha = 0.f;

	_bool m_fbackfadeout = false;
	_bool m_fbackfadein = false;

	_float m_fbrightpos = 0.f;   //name
	_float m_fbrightpos_hpbar = 0.f;
	_float m_fbrightpos_icon = -2.f;

	_float m_fbrightpos_hpfont[9] = { 0.f , -0.5f, -1.f, -1.5f , -2.f , -2.5f, -3.f , -3.5f , -4.f };

	_bool m_breverselerp = false;


	_float m_PlayersCurrentHP[4] = { 0.f ,0.f , 0.f, 0.f };
	_float m_PlayersMaxHP[4] = { 0.f , 0.f, 0.f, 0.f };
	_float m_fRecover_Power = 0.f;
	_bool m_brecover_finsh = false; // for ++ --
	_bool m_brecover_effect = false;
	_bool m_brecovring = false;   //for finish effect

	_float m_fBluepower = 0.f;
	_float m_fRecoverItempower = 0.f;

	_float m_fRecoverFinish_HpbarEffectX = 0.f;

	_bool m_bQuestInformation = false;
	_bool m_bfadeout_quest = false;
	_float m_fQuestAlpha = 0.f;
	_bool m_bfadein_Quest = false;
//	_uint m_iRecoveringPlayer = 0;
};

END