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
	enum MONSTER_ID { MONSTER_OCTOROCK, MONSTER_MOBLINSWORD, MONSTER_ROLA, MONSTER_PAWN, 
		MONSTER_TAIL, MONSTER_TOGEZO,
		MONSTER_CUCCO , MONSTER_ALBATROSS, MONSTER_END };
	enum DMG_DIRECTION {FRONT, BACK, RIGHT, LEFT};

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
	virtual _uint Take_Damage(float fDamage, void* DamageType, CBaseObj* DamageCauser);
	virtual void Check_Navigation(_float fTimeDelta);
	
protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) = 0;
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID();
	
public: // Get& Set
	MONSTER_ID Get_MonsterID(void) { return m_eMonsterID; }
	_bool Get_Aggro(void) { return m_bAggro; }
	_bool Get_Hited(void) { return m_bHit; }

protected:
	DMG_DIRECTION Calculate_Direction();
	_vector Calculate_PosDirction();
	virtual void AI_Behaviour(_float fTimeDelta) { };
	virtual void Find_Target();
	virtual void Follow_Target(_float fTimeDelta) { };
	virtual HRESULT Drop_Items();
	virtual _bool IsDead() = 0;
	virtual void Make_GetAttacked_Effect(CBaseObj* DamageCauser = nullptr);
	virtual void Make_DeadEffect(CBaseObj* Target = nullptr);


protected:
	_float					m_fAttackRadius = 0.5f;
	_float					m_fPatrolRadius = 3.f;
	_float3					m_fPatrolPosition = _float3(0.f, 0.f, 0.f);

	_float					m_fDistanceToTarget = 0.f;

	_bool					m_bAggro = false;
	_bool					m_bIsAttacking = false;
	_bool					m_bHit = false;
	_bool					m_bMove = true;
	_bool					m_bMakeEffect = false;
	
	//For Move Time
	DWORD m_dwDeathTime = GetTickCount();
	DWORD m_dwAttackTime = GetTickCount();
	DWORD m_dwIdleTime = GetTickCount();
	DWORD m_dwWalkTime = GetTickCount();
	DWORD m_dwHitTime = GetTickCount();


	MONSTER_ID	m_eMonsterID = MONSTER_END;
	OBJINFO  m_tInfo;
	DMG_DIRECTION	m_eDmg_Direction = FRONT;


protected: /* For.Components */
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	CBaseObj*				m_pTarget = nullptr;
	CTexture*				m_pDissolveTexture = nullptr;
	_float					m_fAnimSpeed = 1.f;

public:
	virtual void Free() override;
};
END