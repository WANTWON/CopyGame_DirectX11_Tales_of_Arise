#pragma once
#include "Client_Defines.h"
#include "Base.h"
BEGIN(Client)

class CUI_Base;
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

private:
	UITYPE				m_eUIType = UI_END;
	

//private:
//	_uint iplayer0_hp = 0;
//	_uint iplayer1_hp = 0;
//	_uint iplayer2_hp = 0;
//	_uint iplayer3_hp = 0;


public:
	void Free() override;
};

END