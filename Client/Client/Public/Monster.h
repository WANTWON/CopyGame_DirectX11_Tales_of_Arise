#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"
#include "GameInstance.h"
#include "Cell.h"
#include "BattleManager.h"
#include "CameraManager.h"

BEGIN(Engine)
class CModel;
class CTexture;
class CNavigation;
END

BEGIN(Client)
class CMonster abstract : public CBaseObj
{
public:
	enum DMG_DIR {FRONT, BACK, RIGHT, LEFT};

	typedef struct tagCharacterStats
	{
		_float m_fMaxHp = 10.f;
		_float m_fCurrentHp = 10.f;
		_float m_fAttackPower = 10.f;
		_float m_fWalkSpeed = 3.f;
		_float m_fRunSpeed = 5.f;
		_float m_fLockonSmashGuage = 0.f;
		_uint  m_iHitcount = 0;
		_uint  m_iBedamagedCount = 0;
	}STATS;

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;
	virtual HRESULT Render_EdgeDetection() override;
	virtual _bool ExceptionHanding();

public:
	virtual void AI_Behaviour(_float fTimeDelta) { };
	virtual void Find_Target();
	virtual CBaseObj* Find_MinDistance_Target();
	virtual CBaseObj* Check_FiledTarget();
	virtual _float Target_Distance(CBaseObj* pTarget);
	virtual void Follow_Target(_float fTimeDelta) { };
	virtual void Make_GetAttacked_Effect(CBaseObj* DamageCauser = nullptr);
	virtual void Make_DeadEffect(CBaseObj* Target = nullptr);
	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser, _bool bLockOnChange = true);
	virtual void	 Set_BattleMode(_bool type)				PURE;
public:
	void Collision_Object(_float fTimeDelta);
	void Change_Navigation(LEVEL eLevel);
	void Compute_CurrentIndex(LEVEL eLevel);
	void Reset_Lockonguage() { m_tStats.m_fLockonSmashGuage = 0.f; }

protected:
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID();
	virtual HRESULT Drop_Items();
	
	virtual void Compute_CurrentIndex();

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr)	PURE;
	virtual _bool Is_AnimationLoop(_uint eAnimId)			PURE;
	virtual void Check_Navigation()							PURE;

	
	void Check_NearTrigger();
	
public: // Get & Set
	MONSTER_ID Get_MonsterID(void) { return m_eMonsterID; }
	_bool Get_Aggro(void) { return m_bAggro; }
	_bool Get_Hited(void) { return m_bHit; }
	_bool Get_Dissolve(void) { return m_bDissolve; }
	_bool Get_DownState(void) { return m_bDownState; }
	_float Get_AggroRadius() { return m_fAggroRadius; }
	_float Get_PatrolRadius() { return m_fPatrolRadius; }
	_float Get_AttackRadius() { return m_fAttackRadius; }
	_float Get_Attack_BiteRadius() { return m_fAttack_BiteRadius; }
	STATS Get_Stats() { return m_tStats; }
	CNavigation* Get_Navigation(void) { return m_pNavigationCom; }
	void Set_GlowUp() { m_bGlowUp = true; }
	void Set_Dissolve() { m_bDissolve = true; }
	void Set_OnGoingDown() { m_bOnGoingDown = true; }
	void Set_FinishGoingDown() { m_bOnGoingDown = false; }
	void Set_FinishDownState() { m_bDownState = false; }
	void SetOff_BedamagedCount() { m_bBedamageAnim = false; }
	void Set_BedamageCount_Delay() { m_bBedamageAnim_Delay = true; }
	void SetOff_BedamageCount_Delay() { m_bBedamageAnim_Delay = false; }
	void Set_OrginDamageCauser(CBaseObj* pOrginDamageCauser) { m_pOrginDamageCauser = pOrginDamageCauser; }
	CBaseObj* Get_Trigger() { return m_pTrigger; }
	CBaseObj* Get_DamageCauser() { return m_pTarget; }
	CBaseObj* Get_OrginDamageCauser() { return m_pOrginDamageCauser; }
	void	 Save_LastPosition();
	_vector	 Get_LastPosition() { return m_vLastPos; }
	void	Set_LastStrikeAttack(_bool type) { m_bLastStrikeAttack = type; }
	_bool Get_LastStrikeAttack() { return m_bLastStrikeAttack; }
	



protected:
	DMG_DIR Calculate_DmgDirection();
	_vector	Calculate_DirectionByPos();
	_bool Check_AmILastMoster();
	
protected:
	STATS m_tStats;
	_float m_fAttackRadius = 0.5f;
	_float m_fAttack_BiteRadius = 5.f;
	_float m_fAggroRadius = 15.f;
	_float m_fPatrolRadius = 5.f;
	_float m_fDistanceToTarget = 0.f;
	_float m_fTime_TakeDamageDeltaAcc = 0.f;

	_float m_fAnimSpeed = 1.f;

	_float m_fTimeDeltaAcc = 0.f;
	_float m_fCntChanceTime = 0.f;

	_bool m_bAggro = false;
	_bool m_bIsAttacking = false;
	_bool m_bHit = false;
	_bool m_bMove = true;
	_bool m_bMakeEffect = false;
	_bool m_bTakeDamage = false;
	_bool m_bBattleMode = false;
	_bool m_bDownState = false;
	_bool m_bOnGoingDown = false;
	_bool m_bBedamageAnim = false;
	_bool m_bBedamageAnim_Delay = false;
	_bool  m_bLastStrikeAttack = false;
	_bool m_bKillerSkill = false;

	_uint m_eCurLevel = LEVEL_END;
	_uint m_iRand = 0;

	_float m_fMinLengh = MAXDISTANCE;
	CBaseObj* m_pTrigger = nullptr;

	//For Move Time
	DWORD m_dwDeathTime = GetTickCount();
	DWORD m_dwAttackTime = GetTickCount();
	DWORD m_dwIdleTime = GetTickCount();
	DWORD m_dwWalkTime = GetTickCount();
	DWORD m_dwHitTime = GetTickCount();

	MONSTER_ID	m_eMonsterID = MONSTER_END;
	DMG_DIR	m_eDmg_Direction = FRONT;
	

protected: /* For.Components */
	CModel* m_pModelCom = nullptr;
	CNavigation* m_pNavigationCom = nullptr;
	CBaseObj* m_pTarget = nullptr;
	CBaseObj* m_pOrginDamageCauser = nullptr;
	CNavigation* m_vecNavigation[LEVEL_END] = { nullptr };

	/* Dissolve */
	CTexture* m_pDissolveTexture = nullptr;
	
protected:
	_bool m_bGlowUp = false; /* Used to increase Glow Power and spawn Diffuse Particles. */
	_float m_fGlowUpTimer = 0.f;
	_float m_fGlowUpLifespan = 2.f;
	_bool m_bDissolve = false;
	_float m_fDissolveTimer = 0.f;
	_float m_fDissolveLifespan = 3.f;
	vector<class CEffect*> m_pDissolveParticles;
	_vector m_vLastPos = { 0.f,0.f,0.f,1.f };
	LEVEL m_eLevel = LEVEL_END;
public:
	virtual void Free() override;
};
END