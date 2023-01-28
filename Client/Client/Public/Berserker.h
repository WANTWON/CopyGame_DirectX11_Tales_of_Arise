#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CBerserker final : public CMonster
{
public:
	enum ANIM {
		
		ARISE_B,
		ARISE_F,
		ARISE_B_1,
		ARISE_F_2,
		ATTACK_DASH_CLAW_LOOP, 
		ATTACK_DASH_CLAW_START,
		ATTACK_DOUBLE_CLAW, 
		ATTACK_DOUBLE_CROW, 
		ATTACK_FIRE_BALL, 
		ATTACK_HOWLING, 
		ATTACK_MULTIPLE_FIRE_BALL, 
		ATTACK_POUNCING,     
		ATTACK_QUADRUPLE_CLAW, 
		ATTACK_SHOCK_WAVE,
		ATTACK_STEP_BACK, 
		FLYING_ATTACK, 
		DASH_LOOP,
		DASH_START, 
		ATTACK_DOUBLE_CLAW2,
		ATTACK_DOUBLE_CROW2,
		HOWING_AND_BACK,
		HOWING,
		HOWING_BACK_HOWING_BACK,
		FLYING_SCRATCHES, //24
		SCRATCHES_FOURTIMES,
		ATTACK_SHOCK_WAVE2,
		ATTACK_STEP_BACK2, 
		FLYING_ATTACK2, //28 'DASH_START, DASH_LOOP랑 이어지는 동작인 듯' 
		BLOW_DOWN_F1,
		BLOW_UP_F1,
		BLOW_DOWN_B1,
		BLOW_DOWN_F2,
		BLOW_UP_B2,
		BLOW_UP_F2,
		BLOW_UP_B3,
		BLOW_UP_B4,
		DAMAGE_AIR_SMALL_F1,
		DAMAGE_AIR_SMALL_L1,
		DAMAGE_AIR_SMALL_R1,
		DAMAGE_AIR_LOOP,
		DAMAGE_AIR_SMALL_B1,
		DAMAGE_AIR_SMALL_F2,
		DAMAGE_AIR_SMALL_L2,
		DAMAGE_AIR_SMALL_R2,
		DAMAGE_LARGE_B,
		DAMAGE_LARGE_F,
		DAMAGE_LARGE_L, 
		DAMAGE_LARGE_R, 
		DAMAGE_LOOP,
		DAMAGE_SMALL_B, 
		DAMAGE_SMALL_F, 
		DAMAGE_SMALL_L, 
		DAMAGE_SMALL_R,
		DAMAGE_LARGE_B2,
		DAMAGE_LARGE_F2,
		DAMAGE_LARGE_L2,
		DAMAGE_LARGE_R2,
		DOWN_,
		DOWN_B,
		DOWN_F,
		DAMAGE_LAND_LARGE_L3,
		DAMAGE_LAND_LARGE_R3,
		DAMAGE_LAND_LARGE_B3,
		DAMAGE_LAND_LARGE_F3,
		DAMAGE_LAND_LARGE_L4,
		DAMAGE_LAND_LARGE_R4,
		DOWN_KNEE,
		DAMAGE_LAND_LARGE_B4,
		DAMAGE_LAND_SMALL_F1,
		DAMAGE_LAND_LARGE_L1,
		DAMAGE_LAND_LARGE_R1,
		DAMAGE_LAND_LARGE_B1,
		SLOW_DEAD,
		SLOW_DEAD2,
		DEAD_B1,
		DEAD_F1,
		DEAD_B2,
		DEAD_F2,
		MOVE_WALK,
		MOVE_IDLE,
		MOVE_RUN,
		MOVE_WALK_B,
		MOVE_WALK_F,
		MOVE_IDLE2,


		//////////////////////////우선 이 주석 위쪽에 있는 애니메이션은 
		//애님이름과 애니메이션 모션 일치하고
		//밑에는 정확하지 않아서 확인 필요. 추후에 진행하겠음 
		QUIK_WALK,// 뛰기 혹은 빠르게 걷기인 듯. 
		BACK_WALK,
		TURN_L,  
		TURN_R,
		TURN_L2,
		TURN_R2,
		HOWLING2,
		INTHE_AIR,
		UP_KNEE,
		LEAF,
		SYMBOL_DETECT_IDLE,
		LOOK_AROUND,
		QUIK_WALK2,
		LOOK_AROUND_DOWN_KNEE,
		ROTATE180_L,
		ROTATE180_R,
		FIELD_WALK,
		HOWLING_,
		THREAT,
		INTHE_AIR2,
		JUMPING,
		DOWN_KNEE_LEAP,
		SYMBOL_IDLE,
		SYMBOL_IDLE2_LOOK_AROUND,
		TURN_POSE,
		SYMBOL_DETECT_STOP, //SYM1
		SYMBOL_DETECT_STOP_END, //SYM2
		SYMBOL_DETECT_STOP_LOOP, //SYM3
		SYMBOL_DETECT_STOP_START, //SYM4
		SYMBOL_IDLE_, //SYM5
		SYMBOL_LOOKOUT, //SYM6
		SYMBOL_RUN, //SYM7
		SYMBOL_STOP, //SYM8
		SYMBOL_TURN_LEFT, //SYM9
		SYMBOL_TURN_POSE, //SYM10
		SYMBOL_TURN_RIGHT, //SYM11
		SYMBOL_WALK, //SYM12
		ADVENT,
	};



public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CBerserkerState* Get_State() { return m_pBerserkerState; }
	void Set_PlayerState(class CBerserkerState* pPlayerState) { m_pBerserkerState = pPlayerState; }

public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual _int Take_Damage(float fDamage, CBaseObj* DamageCauser) override;
	
private:
	CBerserker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBerserker(const CBerserker& rhs);
	virtual ~CBerserker() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public: /*For.State*/
	void AI_Behavior(_float fTimeDelta);
	void Tick_State(_float fTimeDelta);
	void LateTick_State(_float fTimeDelta);
		/*For Navigation*/
	virtual void Check_Navigation() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CBerserkerState*  m_pBerserkerState = nullptr;

public:
	static CBerserker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END