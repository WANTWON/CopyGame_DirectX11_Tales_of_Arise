#pragma once
#include "Client_Defines.h"
#include "Base.h"

//#include "UI_Dialoguepopup.h"
BEGIN(Client)

//class UI_Dialogue;
class CUI_Base;
class CUI_Get_item_Popup;
class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)
public:
	enum UITYPE { UI_INVEN, UI_MAP, UI_OPTION, UI_END };

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	void Initialize_PlayerState();
	void Tick_UI(_float fTimeDelta);
	void Tick_Inventory();
	void Tick_Map();
	void Tick_PlayerState();
	void Tick_Coin();


//public:
//	_uint get_player0hp() { return iplayer0_hp; }
//	_uint get_player1hp() { return iplayer1_hp; }
//	_uint get_player2hp() { return iplayer2_hp; }
//	_uint get_player3hp() { return iplayer3_hp; }
//
//	void set_player0hp(_uint hp) { iplayer0_hp = hp; }
//	void set_player1hp(_uint hp) { iplayer1_hp = hp; }
//	void set_player2hp(_uint hp) { iplayer2_hp = hp; }
//	void set_player3hp(_uint hp) { iplayer3_hp = hp; }
	
public: /*Getter Setter*/
	void		Set_UI_OpenType(UITYPE eUIType);
	UITYPE		Get_UI_OpenType() { return m_eUIType; }

	_uint Get_Arrived_Count() { return iPortrait_Arrived_Count; }
	void plus_Arrived_Count() { ++iPortrait_Arrived_Count; }

	void ReSet_Arrived_Count() { iPortrait_Arrived_Count = 0; }

	vector<ITEMINFO*>* Get_Inventory() { return &m_vInventory; }
	vector<ITEMINFO*>* Get_Inventory_useable() { return &m_vInventory_useable; }
	vector<ITEMINFO*>* Get_Inventory_material() { return &m_vInventory_material; }
	vector<ITEMINFO*>* Get_Inventory_weapon() { return &m_vInventory_weapon; }
	vector<ITEMINFO*>* Get_Inventory_armor() { return &m_vInventory_armor; }
	vector<ITEMINFO*>* Get_Inventory_else() { return &m_vInventory_else; }

	list<CUI_Base*>* Get_Itempopup_list() {return &m_pItempopup;}

	void Erase_Itempopup_list(CUI_Base* point);

	/*CUI_Dialogue* Get_Dialogue() { return m_pDialogue; }
	CUI_Dialoguepopup* Get_Dialoguepopup() { return m_pDialoguepopup; }*/
	CUI_Base*	 Get_Dialogue() { return m_pDialogue; }
	CUI_Base*    Get_Dialoguepopup() { return m_pDialoguepopup; }
	CUI_Base*    Get_DialogueCaption() { return m_pDialogueCaption; }
	CUI_Base*    Get_System_msg() { return m_pSystemmsg; }
	CUI_Base*    Get_Skill_msg() { return m_pSkillmsg; }
	CUI_Base*    Get_LockOn() { return m_pLockOn; }

	CUI_Base*    Get_HitMsg() { return m_pHitmsg; }
	CUI_Base*    Get_Hitfont() { return m_pHitfont; }

	CUI_Base*    Get_DMGNUM() { return m_pDamagenumber; }
	CUI_Base*    Get_DMGfont() { return m_pDamages; }
	CUI_Base*    Get_Comboline() { return m_pComboline; }

	
		

	void Set_Dialogue(CUI_Base* pointer) { m_pDialogue = pointer; }
	void Set_Dialoguepopup(CUI_Base* pointer) { m_pDialoguepopup = pointer; }
	void Set_DialogueCaption(CUI_Base* pointer) { m_pDialogueCaption = pointer; }

	void Set_SystemMsg(CUI_Base* pointer) { m_pSystemmsg = pointer; }
	void Set_SkillMsg(CUI_Base* pointer) { m_pSkillmsg = pointer; }
	void Set_LockOn(CUI_Base* pointer) { m_pLockOn = pointer; }
	void Set_HitMsg(CUI_Base* pointer) { m_pHitmsg = pointer; }
	void Set_Hitfont(CUI_Base* pointer) { m_pHitfont = pointer; }

	void Set_Damagenumber(CUI_Base* pointer) { m_pDamagenumber = pointer; }
	void Set_Damages(CUI_Base* pointer) { m_pDamages = pointer; }
	void Set_comboline(CUI_Base* pointer) { m_pComboline = pointer; }



	void AddItem(ITEM_NAME name, ITEM_TYPE type , _bool popup, _bool msgbox, _uint count = 1);

	
	void PlusGald(_uint gald) { m_iGald += gald; }
	void MinusGald(_uint gald) { m_iGald -= gald; }
	_uint Get_Gald() { return m_iGald; }

	void PlusCP(_uint CP) { m_iCP += CP; }
	void MinusCP(_uint CP) { m_iCP -= CP; }
	_uint Get_CP() { return m_iCP; }
	_uint Get_MAXCP() { return m_iMAXCP; }

	void Set_Mainmenuon(_bool onoff) { m_bMainMenuon = onoff; }
	_bool Get_Mainmenuon() { return m_bMainMenuon; }

	void Set_StopTick(_bool onoff) { m_bStopTick = onoff; }
	_bool Get_StopTick() { return m_bStopTick; }

	void Set_UIStrike(_bool onoff) { m_bUIStrikeFade = onoff; }
	_bool Get_UIStrike() { return m_bUIStrikeFade; }

	void Set_UIQuestScreen(_bool onoff) { m_bUIQuestScreen = onoff; }
	_bool Get_UIQuestScreen() { return m_bUIQuestScreen; }

	_uint Get_QuestIndex() { return m_iQuestIndex; }
	_uint Get_Quest1_Lettuce() { return m_iQuest1_Lettuce; }
	_uint Get_Quest1_Herb() { return m_iQuest1_Herb; }
	
	void Plus_Quest1_Lettuce() { ++m_iQuest1_Lettuce; }
	void Plus_Quest1_Herb() { ++m_iQuest1_Herb; }
	void Set_QuestIndex(_uint num) { m_iQuestIndex = num; }

	_bool Get_QuestComplete(_uint num) { return m_bQuestComplete[num]; }
	void Set_QuestComplete(_uint num, _bool tof) { m_bQuestComplete[num] = tof; }

	_uint Get_Dialogue_section() { return m_iDialogueSection;  }
	void Set_Dialogue_section(_uint num) { m_iDialogueSection = num; }

	_int Get_Minigamecaptionindex() { return m_iMinigame2Caption; }
	void Plus_Minigamecaptionindex(_int index) {		m_iMinigame2Caption += index	;	}

	void Reset_Minigamecaptionindex() { m_iMinigame2Caption = 0; }


	//void Read_TextFiles_for_dialogue();

	//wchar_t* ConverCtoWC(char* str);


private:
	UITYPE				m_eUIType = UI_END;

	_uint iPortrait_Arrived_Count;

	vector<ITEMINFO*> m_vInventory;

	vector<ITEMINFO*> m_vInventory_useable;
	vector<ITEMINFO*> m_vInventory_material;
	vector<ITEMINFO*> m_vInventory_weapon;
	vector<ITEMINFO*> m_vInventory_armor;
	vector<ITEMINFO*> m_vInventory_else;
	//INVEN_MAIN, INVEN_USABLE, INVEN_MATERIAL, INVEN_WEAPON, INVEN_ARMOR, INVEN_ELSE, INVEN_END

	CUI_Base* m_pDialogue = nullptr;
	CUI_Base* m_pDialoguepopup = nullptr;
	CUI_Base* m_pDialogueCaption = nullptr;
	CUI_Base* m_pSystemmsg = nullptr;
	CUI_Base* m_pSkillmsg = nullptr;
	CUI_Base* m_pLockOn = nullptr;
	CUI_Base* m_pHitmsg = nullptr;
	CUI_Base* m_pHitfont = nullptr;

	CUI_Base* m_pDamagenumber = nullptr;
	CUI_Base* m_pDamages = nullptr;
	CUI_Base* m_pComboline = nullptr;

	list<CUI_Base*>m_pItempopup;
	//CUI_Base* m_pDialogue
//	CUI_Dialogue* m_pDialogue = nullptr;
//	CUI_Dialoguepopup* m_pDialoguepopup = nullptr;
	
	_uint m_iGald;
	_uint m_iCP;
	_uint m_iMAXCP;

	_bool m_bMainMenuon = false; 
	_bool m_bStopTick = false;;

	_uint m_iQuestIndex = 0; // que

	_uint m_iQuest1_Lettuce = 0;
	_uint m_iQuest1_Herb = 0;

	_bool m_bQuestComplete[8] = { false,false, false, false, false, false, false, false };

	_bool m_bUIStrikeFade = false;
	_bool m_bUIQuestScreen = false;

	_uint m_iDialogueSection = 0;
//private:
//	_uint iplayer0_hp = 0;
//	_uint iplayer1_hp = 0;
//	_uint iplayer2_hp = 0;
//	_uint iplayer3_hp = 0;
	
	_int m_iMinigame2Caption = 0;

public:
	void Free() override;
};

END