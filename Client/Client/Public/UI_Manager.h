#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Client)

class CObj_UI;
class CNpc;
class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)
public:
	enum UITYPE { UI_INVEN, UI_MAP, UI_OPTION, UI_END };

	/* For Message*/
	enum MSG_TYPE { PASSABLE, MUST_CHOICE };
	
	enum CHOICE_TYPE { BUY_NOBUY, TALK_NOTTALK, MARIN_PERFUME, 
		MARIN_CHANGE, MARIN_VECTOR, MARIN_DOT, MARIN_CHARM, 
		MARIN_DATEME, DO_UNDO};
	
	typedef struct MessageTag
	{
		MSG_TYPE	eMsgType = PASSABLE;
		_uint		iTextureNum = 0;
		CHOICE_TYPE eChoiceType = BUY_NOBUY;
	}MSGDESC;

	enum EQUIP_BT { EQUIP_X, EQUIP_Y, EQUIP_END };
	enum ROOMTYPE {MARINHOUSE, SHOP, CRANEGAME, TELEPHONE};



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
	void Tick_Message();
	void Tick_Name(_float fTimeDelta);
	void Setting_ChoiceButton();
	
public: /*Getter Setter*/
	void		Set_UI_OpenType(UITYPE eUIType);
	void		Set_EquipItem(EQUIP_BT eEquipBt, CObj_UI* pObj);
	void		Set_NextLevelIndex(LEVEL eLevel) { m_eNextLevel = eLevel; }
	void		Set_NextLevel(_bool type) 
	{ m_bNextLevel = type; m_bFinishedReady = false; 
	if (type == false) { m_bFirst = false; m_fNameTime = 0.f; }}
	void		Set_NextLevelFinished(_bool type) { m_bFinishedReady = type;  }
	void		Set_RoomType(ROOMTYPE eType) { m_eRoomType = eType; }
	void		Set_Talking(_bool type) { m_bTalking = type; }
	void		Set_PlayGame(_bool type) { m_bPlayGame = type; }
	void		Set_NextMessage();
	void		Set_TellEnd(_bool type) { m_bTellEnd = type; }
	void		Set_NameTimeZero() { m_fNameTime = 0.f;}
	void		Set_Get_DgnMap(_bool type) { m_bGetMap = type; }

	void		Get_Key();
	_uint		Get_KeySize() { return (_uint)m_KeyList.size(); }
	_uint		Get_MessageSize() { return (_uint)m_vecMsgDecs.size(); }
	UITYPE		Get_UI_OpenType() { return m_eUIType; }
	_bool		Get_OpenMessage() { return m_bOpenMessage; }
	_bool		Get_Talking() { return m_bTalking; }
	_bool		Get_NpcGet() { return m_bNpcGet; }
	_bool 		Get_PlayGame() { return m_bPlayGame; }
	_bool		Get_Is_HaveItem(LEVEL eLevel, _tchar* LayerTag, _uint ItemtexTureNum);
	_bool		Get_TellEnd() { return m_bTellEnd; }
	_bool		Get_NextLevel() { return m_bNextLevel; }
	_bool		Get_NextLevelFinished() { return m_bFinishedReady; }
	_bool		Get_DgnMap() { return m_bGetMap; }

	CNpc*		Get_TalkingNpc() { return m_pTalkingNpc; }
	CObj_UI*	Get_Button() { return m_pInteractButton; }
	CObj_UI*	Get_NameBox() { return m_pNameBox; }
	CObj_UI*	Get_EquipItem(EQUIP_BT eEquipBt) { return m_EquipTile[eEquipBt]; }

	LEVEL		Get_NextLevelIndex() { return m_eNextLevel; }
	ROOMTYPE	Get_RoomType() { return m_eRoomType; }
	MSGDESC		Get_MessageType() { return m_vecMsgDecs.front(); }

public:  /* Adder */
	void		Add_Button(CObj_UI* pObj) { m_pInteractButton = pObj; }
	void		Add_NameBox(CObj_UI* pObj) { m_pNameBox = pObj; }
	void		Add_ChoiceButton(CObj_UI* pObj) { m_vecChoiceButton.push_back(pObj); }
	void		Add_InvenGroup(CObj_UI* pObj) { m_InvenTile.push_back(pObj); }
	void		Add_WarpGroup(CObj_UI* pObj) { m_WarpGroup.push_back(pObj);  }
	void		Add_HpGroup(CObj_UI* pObj) { m_HpList.push_back(pObj); }
	void		Add_KeyGroup(CObj_UI* pObj) { m_KeyList.push_back(pObj); }
	void		Add_RubyGroup(CObj_UI* pObj) { m_RubyList.push_back(pObj); }
	void		Add_TalkingNpc(CNpc* Obj) { m_pTalkingNpc = Obj; }
	void		Add_MessageDesc(MSGDESC eType) { m_vecMsgDecs.push_back(eType); }
	void		Open_Message(_bool type) { m_bOpenMessage = type;}
	void		Clear_ChoiceButton();
	void		Clear_WarpGroup();

	void		Use_Key();
private:
	LEVEL				m_eNextLevel = LEVEL_GAMEPLAY;
	ROOMTYPE			m_eRoomType = MARINHOUSE;
	UITYPE				m_eUIType = UI_END;
	_bool				m_bNextLevel = false;
	_bool				m_bFinishedReady = false;
	_bool				m_bTellEnd = false;
	
	/*For Message */
	_bool				m_bOpenMessage = false;
	_bool				m_bTalking = false;
	_bool				m_bChoice = false;
	list<MSGDESC>		m_vecMsgDecs;
	vector<CObj_UI*>	m_vecChoiceButton;
	_int				m_iChoiceIndex = 0;
	CNpc*				m_pTalkingNpc = nullptr;
	_bool				m_bNpcGet = false;
	_bool				m_bPlayGame = false;
	
	/*For Inventory*/
	_int				m_iInvenIndex = 0;
	CObj_UI*			m_pPickedTile = nullptr;
	CObj_UI*			m_EquipTile[EQUIP_END] = { nullptr };
	vector<CObj_UI*>	m_InvenTile;

	/*For Player State*/
	vector<CObj_UI*>	m_HpList;
	vector<CObj_UI*>	m_KeyList;
	vector<CObj_UI*>	m_RubyList;
	_int				m_iRubyShareSize = 0;

	/*For Button */
	CObj_UI*			m_pInteractButton = nullptr;

	/*For Name*/
	CObj_UI*			m_pNameBox = nullptr;
	_float				m_fNameTime = 0.f;
	_bool				m_bFirst = false;
	_bool				m_bBossNameBox = false;

	/* For Warp */
	vector<CObj_UI*>	m_WarpGroup;
	_int				m_iWarpIndex = 0;
	_int				m_iMInMaxIndex = 0;
	CObj_UI*			m_pPickedWarp = nullptr;
	_bool				m_bGetMap = false;
public:
	void Free() override;
};

END