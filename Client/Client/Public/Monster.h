#pragma once
#include "Client_Defines.h"
#include "BaseObj.h"
#include "GameInstance.h"
#include "Cell.h"

BEGIN(Engine)
class CModel;
class CTexture;
class CNavigation;
END

BEGIN(Client)
class CMonster abstract : public CBaseObj
{
public:
	enum MONSTER_ID { ICE_WOLF, MONSTER_END };

	enum DMG_DIR {FRONT, BACK, RIGHT, LEFT};

	typedef struct tagCharacterStats
	{
		_float m_fMaxHp = 10.f;
		_float m_fCurrentHp = 10.f;
		_float m_fAttackPower = 10.f;
		_float m_fWalkSpeed = 3.f;
		_float m_fRunSpeed = 5.f;
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

public:
	virtual void AI_Behaviour(_float fTimeDelta) { };
	virtual void Find_Target();
	virtual void Follow_Target(_float fTimeDelta) { };
	virtual void Make_GetAttacked_Effect(CBaseObj* DamageCauser = nullptr);
	virtual void Make_DeadEffect(CBaseObj* Target = nullptr);

protected:
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID();
	virtual HRESULT Drop_Items();
	virtual _int Take_Damage(float fDamage, CBaseObj* DamageCauser);
	virtual void Compute_CurrentIndex();

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr)	PURE;
	virtual _bool Is_AnimationLoop(_uint eAnimId)			PURE;
	virtual void Check_Navigation()							PURE;

	
	
public: // Get & Set
	MONSTER_ID	Get_MonsterID(void) { return m_eMonsterID; }
	_bool		Get_Aggro(void) { return m_bAggro; }
	_bool		Get_Hited(void) { return m_bHit; }
	_float		Get_AggroRadius() { return m_fAggroRadius; }
	_float		Get_PatrolRadius() { return m_fPatrolRadius; }
	_float		Get_AttackRadius() { return m_fAttackRadius; }
	_float		Get_Attack_BiteRadius() { return m_fAttack_BiteRadius; }
	STATS		Get_Stats() { return m_tStats; }
	_bool		Get_BattleMode = false;
	CNavigation* Get_Navigation(void) { return m_pNavigationCom; }
	void		Set_Dissolve() { m_bDissolve = true; }
	

protected:
	DMG_DIR Calculate_DmgDirection();
	_vector	Calculate_DirectionByPos();
	
protected:
	STATS					m_tStats;
	_float					m_fAttackRadius = 0.5f;
	_float					m_fAttack_BiteRadius = 5.f;
	_float					m_fAggroRadius = 15.f;
	_float					m_fPatrolRadius = 5.f;
	_float					m_fDistanceToTarget = 0.f;
	_float					m_fTime_TakeDamageDeltaAcc = 0.f;

	_float					m_fAnimSpeed = 1.f;

	_bool					m_bAggro = false;
	_bool					m_bIsAttacking = false;
	_bool					m_bHit = false;
	_bool					m_bMove = true;
	_bool					m_bMakeEffect = false;
	_bool					m_bTakeDamage = false;
	
	//For Move Time
	DWORD m_dwDeathTime = GetTickCount();
	DWORD m_dwAttackTime = GetTickCount();
	DWORD m_dwIdleTime = GetTickCount();
	DWORD m_dwWalkTime = GetTickCount();
	DWORD m_dwHitTime = GetTickCount();


	MONSTER_ID	m_eMonsterID = MONSTER_END;
	DMG_DIR	m_eDmg_Direction = FRONT;


protected: /* For.Components */
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	CBaseObj*				m_pTarget = nullptr;

	/* Dissolve */
	CTexture*				m_pDissolveTexture = nullptr;
	
protected:
	_bool		m_bDissolve = false;


public:
	virtual void Free() override;
};
END