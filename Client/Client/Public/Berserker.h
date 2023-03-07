#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CBerserker final : public CMonster
{
public:
	enum ANIM {
		
		//ARISE_B,
		//ARISE_F,
		//ARISE_B_1,
		//ARISE_F_2,
		//ATTACK_DASH_CLAW_LOOP, 
		//ATTACK_DASH_CLAW_START,
		//ATTACK_DOUBLE_CLAW, 
		//ATTACK_DOUBLE_CROW, 
		//ATTACK_FIRE_BALL, 
		//ATTACK_HOWLING, 
		//ATTACK_MULTIPLE_FIRE_BALL, 
		//ATTACK_POUNCING,     
		//ATTACK_QUADRUPLE_CLAW, 
		//ATTACK_SHOCK_WAVE,
		//ATTACK_STEP_BACK, 
		//FLYING_ATTACK, 
		//DASH_LOOP,
		//DASH_START, 
		//ATTACK_DOUBLE_CLAW2,
		//ATTACK_DOUBLE_CROW2,
		//HOWING_AND_BACK,
		//HOWING,
		//HOWING_BACK_HOWING_BACK,
		//FLYING_SCRATCHES, //24
		//SCRATCHES_FOURTIMES,
		//ATTACK_SHOCK_WAVE2,
		//ATTACK_STEP_BACK2, 
		//FLYING_ATTACK2, //28 'DASH_START, DASH_LOOP�� �̾����� ������ ��' 
		//BLOW_DOWN_F1,
		//BLOW_UP_F1,
		//BLOW_DOWN_B1,
		//BLOW_DOWN_F2,
		//BLOW_UP_B2,
		//BLOW_UP_F2,
		//BLOW_UP_B3,
		//BLOW_UP_B4,
		//DAMAGE_AIR_SMALL_F1,
		//DAMAGE_AIR_SMALL_L1,
		//DAMAGE_AIR_SMALL_R1,
		//DAMAGE_AIR_LOOP,
		//DAMAGE_AIR_SMALL_B1,
		//DAMAGE_AIR_SMALL_F2,
		//DAMAGE_AIR_SMALL_L2,
		//DAMAGE_AIR_SMALL_R2,
		//DAMAGE_LARGE_B,
		//DAMAGE_LARGE_F,
		//DAMAGE_LARGE_L, 
		//DAMAGE_LARGE_R, 
		//DAMAGE_LOOP,
		//DAMAGE_SMALL_B, 
		//DAMAGE_SMALL_F, 
		//DAMAGE_SMALL_L, 
		//DAMAGE_SMALL_R,
		//DAMAGE_LARGE_B2,
		//DAMAGE_LARGE_F2,
		//DAMAGE_LARGE_L2,
		//DAMAGE_LARGE_R2,
		//DOWN_,
		//DOWN_B,
		//DOWN_F,
		//DAMAGE_LAND_LARGE_L3,
		//DAMAGE_LAND_LARGE_R3,
		//DAMAGE_LAND_LARGE_B3,
		//DAMAGE_LAND_LARGE_F3,
		//DAMAGE_LAND_LARGE_L4,
		//DAMAGE_LAND_LARGE_R4,
		//DOWN_KNEE,
		//DAMAGE_LAND_LARGE_B4,
		//DAMAGE_LAND_SMALL_F1,
		//DAMAGE_LAND_LARGE_L1,
		//DAMAGE_LAND_LARGE_R1,
		//DAMAGE_LAND_LARGE_B1,
		//SLOW_DEAD,
		//SLOW_DEAD2,
		//DEAD_B1,
		//DEAD_F1,
		//DEAD_B2,
		//DEAD_F2,
		//MOVE_WALK,
		//MOVE_IDLE,
		//MOVE_RUN,
		//MOVE_WALK_B,
		//MOVE_WALK_F,
		//MOVE_IDLE2,


		//////////////////////////
		ARISE_B,
		ARISE_F,
		ATTACK_DASH_CLAW_END,
		ATTACK_DASH_CLAW_LOOP,
		ATTACK_DASH_CLAW_START,
		ATTACK_DOUBLE_CLAW,
		ATTACK_DOUBLE_CROW,
		ATTACK_FIRE_BALL,
		ATTACK_HOWLING,
		ATTACK_MULTIPLE_FIRE_B,
		ATTACK_POUNCING,
		ATTACK_QUADRUPLE_CLAW,
		ATTACK_SHOCK_WAVE,
		ATTACK_STEP_BACK,
		BLOW_DOWN_B,
		BLOW_DOWN_F,
		BLOW_UP_B,
		BLOW_UP_F,
		DAMAGE_AIR_LARGE_B,
		DAMAGE_AIR_LARGE_F,
		DAMAGE_AIR_LARGE_L,
		DAMAGE_AIR_LARGE_R,
		DAMAGE_AIR_LOOP,
		DAMAGE_AIR_SMALL_B,
		DAMAGE_AIR_SMALL_F,
		DAMAGE_AIR_SMALL_L,
		DAMAGE_AIR_SMALL_R,
		DAMAGE_LARGE_B,
		DAMAGE_LARGE_F,
		DAMAGE_LARGE_L,
		DAMAGE_LARGE_R,
		DAMAGE_LOOP,
		DAMAGE_SMALL_B,
		DAMAGE_SMALL_F,
		DAMAGE_SMALL_L,
		DAMAGE_SMALL_R,
		DEAD,
		DOWN_B,
		DOWN_F,
		MOVE_IDLE,
		MOVE_RUN,
		MOVE_WALK_B,
		MOVE_WALK_F,
		TURN_L,
		TURN_R,
		SYMBOL_DETECT_IDLE,
		SYMBOL_DETECT_STOP_END,
		SYMBOL_DETECT_STOP_LOOP,
		SYMBOL_DETECT_STOP_START,
		SYMBOL_DETECT_STOP,
		SYMBOL_IDLE,
		SYMBOL_LOOKOUT,
		SYMBOL_RUN,
		SYMBOL_STOP,
		SYMBOL_TURN_LEFT,
		SYMBOL_TURN_RIGHT,
		SYMBOL_WALK,
		ADVENT,
	};



public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CBerserkerState* Get_State() { return m_pBerserkerState; }
	void Set_PlayerState(class CBerserkerState* pPlayerState) { m_pBerserkerState = pPlayerState; }
	void Set_OnGoingAnim() { m_bOnGoingAnim = true; }
	void Set_FinishAnim() { m_bOnGoingAnim = false; }
	void Set_OnGoingHowling() { m_bOnGoingHowLing = true; }
	void Set_FinishHowling() { m_bOnGoingHowLing = false; }
	void Set_OnBerserkerMode() { m_bBerserkerMode = true; }
	
public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser, HITLAGDESC HitDesc) override;
	HRESULT CBerserker::SetUp_ShaderID();
	virtual void	 Set_HitState() override;

private:
	CBerserker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBerserker(const CBerserker& rhs);
	virtual ~CBerserker() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render() override;
	virtual HRESULT Render_Glow() override;

public: /*For.State*/
	void AI_Behavior(_float fTimeDelta);
	void Tick_State(_float fTimeDelta);
	void LateTick_State(_float fTimeDelta);
	virtual void	 Set_BattleMode(_bool type) override;
		/*For Navigation*/
	virtual void Check_Navigation() override;
	
public:
	void Set_AngryOn() { m_bAngry = true; }
	_bool Get_IsAngry() { return m_bAngry; }
	

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CBerserkerState*  m_pBerserkerState = nullptr;

private:
	_bool m_bOnGoingAnim = false;
	_bool m_bOnGoingHowLing = false;
	_bool m_bBerserkerMode = false;
	_bool m_bDoneChangeState = false;
	_int  m_iBeDamaged_Cnt = 0;
	_bool m_bAngry = false;
public:
	static CBerserker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END