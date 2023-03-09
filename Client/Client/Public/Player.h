#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"
#include "PlayerManager.h"
#include "BattleManager.h"

BEGIN(Engine)
class CNavigation;
class CModel;
END

BEGIN(Client)
class CPlayerState;
class CAIState;
class CPlayer abstract : public CBaseObj
{	
public:
	enum PARTS { PARTS_WEAPON, PARTS_END };
	enum GROUNDSKILL { GROUND_SKILL1, GROUND_SKILL2, GROUND_SKILL3, GROUND_SKILL_END };
	enum FLYSKILL { FLY_SKILL1, FLY_SKILL2, FLY_SKILL3, FLY_SKILL_END };
	enum PLAYERID { ALPHEN, SION, RINWELL, LAW };

	enum STRIKESMASH { ALPHEN_SION, ALPHEN_RINWELL, ALPHEN_LAW, SION_RINWELL, SION_LAW, RINWELL_LAW };

protected:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() override;
	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser, _float fMoveLength = 1.f, HITTYPE eHitType = HIT_NORMAL);
	virtual HRESULT Render_EdgeDetection() override;

public: /* Getter &  Setter */
	CModel*			Get_Model() { return m_pModelCom; }
	CNavigation*	Get_Navigation(void) { return m_pNavigationCom; }
	CPlayerState*	Get_State() { return m_pPlayerState; }
	PLAYERID		Get_PlayerID() { return m_ePlayerID; }
	CGameObject*	Get_Parts(_int iIndex) { return m_Parts[iIndex]; }
	LEVEL			Get_Level(void) { return m_eLevel; }
	void			Set_TakeDamage_DelayFinish() { m_bTakeDamage_Delay = false; }
	PLAYER_MODE		Get_PlayerMode(void) { return m_ePlayerMode; }

	/* 지상 스킬 */
	_uint			Get_GroundSkillAnimIndex(GROUNDSKILL SkillIndex) { return m_eGroundSkills[SkillIndex]; }
	/* 공중 스킬 */
	_uint			Get_FlySkillAnimIndex(FLYSKILL SkillIndex) { return m_eFlySkills[SkillIndex]; }

	/* 공중 판단 */
	_bool			Get_IsFly(void) { return m_bIsFly; }
	void			On_IsFly(void) { m_bIsFly = true; }
	void			Off_IsFly(void) { m_bIsFly = false; }

	/* 저스트 회피 판단 */
	_bool			Get_IsJustDodge(void) { return m_bIsJustDodge; }
	void			On_JustDodge(void) { m_bIsJustDodge = true; }
	void			Off_JustDodge(void) { m_bIsJustDodge = false; }
	/* OverLimit */
	void            Set_Overlimit(_bool bOverlimit) { m_bOverLimit = bOverlimit; }
	_bool           Get_Overlimit() { return m_bOverLimit; }
	void			Get_AuraColor();
	void			EffectSpawn_Overlimit();
	void			EffectUpdate_Overlimit();
	void			EffectRemove_Overlimit();
	void			EffectStop_Overlimit();

	/* STRIKEATTACK */
	void			Set_StrikeAttack(_bool tof) { m_bStrikeAttack = tof; }
	_bool           Get_StrikeAttack() { return m_bStrikeAttack; }

	void Set_PlayerState(class CPlayerState* pPlayerState) { m_pPlayerState = pPlayerState; }
	void Set_PlayerCollectState(class CInteractObject* pObject = nullptr);
	void Play_AISkill(PLAYERID ePlayer);

	void AI_RINWELL_Event();

	//expup
	void Plus_EXP(_uint exp);

	//levelup
	_bool Get_Levelup() { return m_bLevelup; }
	void Set_Levelup(_bool tof) { m_bLevelup = tof; }
	//manarecover control
	void Set_Manarecover(_bool tof) { m_bManaRecover = tof; }
	// manause
	void Use_Mana(_float mana) { m_tInfo.fCurrentMp -= mana; }

	//revive
	void Revive();
	//ai to checkstate
	void AI_check();

	void RecoverHP(_float hp);

public: /*For.State*/
	virtual _bool	Is_AnimationLoop(_uint eAnimId) PURE;
	void			HandleInput();
	void			Tick_State(_float fTimeDelta);
	void			Tick_AIState(_float fTimeDelta);
	void			LateTick_State(_float fTimeDelta);
	void			LateTick_AIState(_float fTimeDelta);
	void            SmashAttack(_uint smashtype);
	void            BoostAttack();
	CPlayerState* Get_PlayerState() { return m_pPlayerState; }
	//void Set_PlayerState(CPlayerState* state) { m_pPlayerState = state; }
	
	void Set_BoostGuage(_float boostguage) { m_tInfo.fCurrentBoostGuage = boostguage; }
	_float Get_BoostGuage() { return m_tInfo.fCurrentBoostGuage; }
	void Plus_Overcount() { ++m_tInfo.idodgecount; }
	void Set_Overcount(_uint count) { m_tInfo.idodgecount = count; }

	_bool Get_DodgeEffect() { return m_bDodgeEffect; }
	void Set_DodgeEffect(_bool bDodgeEffect) { m_bDodgeEffect = bDodgeEffect; }	
	void Reset_DodgeEffect(_float fTimeDelta);

	_bool Get_ResetStrikeBlur() { return m_bResetStrikeBlur; }
	void Set_ResetStrikeBlur(_bool bResetStrikeBlur) { m_bResetStrikeBlur = bResetStrikeBlur; }
	void Reset_StrikeBlur(_float fTimeDelta);

	void Set_HitState();
	void Set_OverLimitState(); 

public: /*For.Navigation*/
	void Change_Navigation(LEVEL eLevel);
	void Check_Navigation();
	_bool Check_Navigation_Jump(void);
	void Compute_CurrentIndex(LEVEL eLevel);

public:	/* For.Weapon */
	virtual void Change_Level(LEVEL eLevel);

public: /* For. MiniGame2 */
	void Change_ShootState(void);

protected: /* For Component */
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	vector<CNavigation*>	m_vecNavigations;
	_bool	m_bIsFirst = true;

protected: /* for 4 Player */
	PLAYERID			m_ePlayerID = ALPHEN;
	CPlayerState*	m_pPlayerState = nullptr;
	CAIState*		m_pAIState = nullptr;
	CPlayerManager*	m_pPlayerManager = nullptr;
	PLAYER_MODE		m_ePlayerMode;
	
	/* 스킬 */
	_uint			m_eGroundSkills[GROUND_SKILL_END] = { GROUND_SKILL_END, };
	_uint			m_eFlySkills[FLY_SKILL_END] = { FLY_SKILL_END, };
	/* 공중 판단 변수*/
	_bool			m_bIsFly = false;
	/* 현재 레벨 변수 */
	LEVEL			m_eLevel = LEVEL_END;
	/* 저스트 회피 판단 변수 */
	_bool			m_bIsJustDodge = false;
	/* mana recover */
	_bool           m_bManaRecover = true;

	/* Over Limit */
	_bool			m_bOverLimit = false;
	_float			m_fOverLimitTimer = 0.f;
	_float			m_fFresnelTimer = 0.f;
	_bool			m_bIsOverlimiEffectSpawned = false;
	_float3			m_vAuraColor;
	_float			m_fAuraTimer = 0.f;
	vector<class CEffect*> m_Aura;

	/* STIRKEATTACK */
	_bool			m_bStrikeAttack = false;
	

	/* Pose */
	_bool m_bIsPose = false;

	/* Dodge Screen Effect */
	_bool m_bDodgeEffect = false;
	_float m_fResetDuration = .45f;
	_float m_fResetTimer = 0.f;

	/* Strike Screen Effect */
	_bool m_bResetStrikeBlur = false;
	_float m_fStrikeBlurResetDuration = .45f;
	_float m_fStrikeBlurResetTimer = 0.f;

	/* 무한피격 방지 */
	_bool m_bTakeDamage_Delay = false;

private:
	_bool m_bLevelup = false;
	_bool m_bIsSionWeapon = true;

protected:
	vector<class CGameObject*> m_Parts;
	
protected:
	HRESULT SetUp_Controller();

protected:
	virtual HRESULT Ready_Parts() { return S_OK; };
	virtual HRESULT Ready_Components(void* pArg) PURE;
	virtual HRESULT SetUp_ShaderResources();

public:
	virtual void Free() override;
};
END