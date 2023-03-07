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
	enum TYPE {ALPHEN, SION, RINWELL, LAW, PLAYER_END};

	enum STRIKESMASH{ALPHEN_SION , ALPHEN_RINWELL , ALPHEN_LAW , SION_RINWELL , SION_LAW , RINWELL_LAW};

	enum STRIKEPOSITION { ACTIVE1, ACTIVE2, AIPLAYER1, AIPLAYER2, LOCKON };

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
	vector<_vector> Get_StrikeLockOnPosition() { return m_vStrikeLockOnPos; }

	/* For Field - BattleZone level change & Comeback*/
	_vector	 Get_LastPosition() { return m_vLastPos; }
	void	 Save_LastPosition();

	PLAYER_MODE	 Check_ActiveMode(CPlayer* pPlayer);
	void	 Set_BattleMode(_bool bType) { m_bBattleMode = bType; }
	CPlayer* Get_EnumPlayer(_uint eID);

	void Set_Ai_Check();
	void Set_SmashAttack();
	void Update_StrikePosition(_tchar* FilePath, _int iMonsterPosNum = 1);

	void Set_StrikeLockOnPos(vector<_vector> pos) { m_vStrikeLockOnPos = pos; };

private:
	CPlayer* m_pActivePlayer = nullptr;
	vector<CPlayer*> m_pAIPlayers;
	_vector m_vLastPos = { 0.f,0.f,0.f,1.f };
	_bool	m_bBattleMode = false;

	CPlayer* m_AllPlayers[PLAYER_END] = { nullptr, nullptr, nullptr, nullptr };

	_vector m_vStrikePosition[5];
	vector<_vector> m_vStrikeLockOnPos;

public:
	virtual void Free() override;
};

END