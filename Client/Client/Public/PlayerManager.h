#pragma once
#include "Base.h"
#include "Client_Defines.h"


BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CPlayer;
class CBaseObj;
class CPlayerManager : public CBase
{
	DECLARE_SINGLETON(CPlayerManager)
public:
	enum TYPE {ALPHEN, SION, RINWELL, PLAYER_END};

private:
	explicit CPlayerManager();
	virtual ~CPlayerManager() = default;

public: /* Getter Setter */
	CPlayer* Get_PlayerEnum(_uint iIndex) { return m_AllPlayers[iIndex]; }
	void	 Set_PlayerEnum(CPlayer* pPlayer, _uint iIndex) { m_AllPlayers[iIndex] = pPlayer; }
	CPlayer* Get_ActivePlayer() { return m_pActivePlayer; }
	void	 Set_ActivePlayer(CPlayer* pPlayer);
	void	 Set_ActivePlayer(_uint iIndex);
	vector<CPlayer*> Get_AIPlayers() { return m_pAIPlayers; } 

	/* For Field - BattleZone level change & Comeback*/
	_vector	 Get_LastPosition() { return m_vLastPos; }
	void	 Save_LastPosition();

	PLAYER_MODE	 Check_ActiveMode(CPlayer* pPlayer);
	void	 Set_BattleMode(_bool bType) { m_bBattleMode = bType; }

	CPlayer* Get_EnumPlayer(_uint eID);

private:
	CPlayer* m_pActivePlayer = nullptr;
	vector<CPlayer*> m_pAIPlayers;
	_vector m_vLastPos = { 0.f,0.f,0.f,1.f };
	_bool	m_bBattleMode = false;

	CPlayer* m_AllPlayers[PLAYER_END] = { nullptr, nullptr, nullptr };

public:
	virtual void Free() override;
};

END