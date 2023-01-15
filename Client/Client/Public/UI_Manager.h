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
	
public: /*Getter Setter*/
	void		Set_UI_OpenType(UITYPE eUIType);
	UITYPE		Get_UI_OpenType() { return m_eUIType; }

private:
	UITYPE				m_eUIType = UI_END;

public:
	void Free() override;
};

END