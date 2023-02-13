#pragma once
#include "Player.h"

BEGIN(Client)
class CSion final : public CPlayer
{
public:
	enum ANIM {
		BTL_ADVENT,
		BTL_ARISE_B,
		BTL_ARISE_F,
		BTL_ATTACK_BLASTER,
		BTL_ATTACK_BRAVE,
		BTL_ATTACK_BURNING_BOMB,
		BTL_ATTACK_BURNING_FORCE,
		BTL_ATTACK_CRESCENT_BULLET,
		BTL_ATTACK_DIVIDE_STAR,
		BTL_ATTACK_FLARE_BURST,
		BTL_ATTACK_FLASH,
		BTL_ATTACK_GRAVITY_FORCE,
		BTL_ATTACK_ICE_BLOCK,
		BTL_ATTACK_ICE_BREAK,
		BTL_ATTACK_MIDORINOSETSUNA,
		BTL_ATTACK_MOON_BULLET,
		BTL_ATTACK_NORMAL_0,
		BTL_ATTACK_NORMAL_1,
		BTL_ATTACK_NORMAL_2,
		BTL_ATTACK_NORMAL_3,
		BTL_ATTACK_NORMAL_4,
		BTL_ATTACK_NORMAL_AIR_0,
		BTL_ATTACK_NORMAL_AIR_1,
		BTL_ATTACK_NORMAL_AIR_2,
		BTL_ATTACK_NORMAL_AIR_3,
		BTL_ATTACK_NORMAL_AIR_4,
		BTL_ATTACK_PARALYZE_SMOKE,
		BTL_ATTACK_POISON_BUBBLE,
		BTL_ATTACK_RAILGUN,
		BTL_ATTACK_RELOAD,
		BTL_ATTACK_STRIKE,
		BTL_ATTACK_STRIKE_AIR,
		BTL_ATTACK_THUNDER_BOLT,
		BTL_ATTACK_TRESVENTOS,
		BTL_ATTACK_WIDE_SHOT,
		BTL_DAMAGE_AIR_LARGE_B,
		BTL_DAMAGE_AIR_LARGE_F,
		BTL_DAMAGE_AIR_LARGE_L,
		BTL_DAMAGE_AIR_LARGE_R,
		BTL_DAMAGE_AIR_LOOP,
		BTL_DAMAGE_AIR_SMALL_B,
		BTL_DAMAGE_AIR_SMALL_F,
		BTL_DAMAGE_AIR_SMALL_L,
		BTL_DAMAGE_AIR_SMALL_R,
		BTL_DAMAGE_LARGE_B,
		BTL_DAMAGE_LARGE_F,
		BTL_DAMAGE_LARGE_L,
		BTL_DAMAGE_LARGE_R,
		BTL_DAMAGE_LOOP,
		BTL_DAMAGE_SMALL_B,
		BTL_DAMAGE_SMALL_F,
		BTL_DAMAGE_SMALL_L,
		BTL_DAMAGE_SMALL_R,
		BTL_DEAD,
		BTL_DOWN_B,
		BTL_DOWN_F,
		BTL_GUARD_HIT,
		BTL_GUARD_LOOP,
		BTL_GUARD_START,
		BTL_GUARD_WALK_B,
		BTL_GUARD_WALK_F,
		BTL_GUARD_WALK_L,
		BTL_GUARD_WALK_R,
		BTL_ITEM,
		BTL_JUMP,
		BTL_LAND,
		BTL_MOVE_BRAKE,
		BTL_MOVE_IDLE,
		BTL_MOVE_RUN,
		BTL_MOVE_WALK_B,
		BTL_MOVE_WALK_F,
		BTL_MOVE_WALK_L,
		BTL_MOVE_WALK_R,

		DASH,
		DASH_BRAKE_000,
		DASH_BRAKE_001,
		DOOR_OPEN_HAND_BOTH,
		IDLE,
		IDLE_TRANS_WALK,
		JUMP_LANDING,
		JUMP_LANDING_HIGH,
		JUMP_LOOP,
		JUMP_RUN_END,
		JUMP_RUN_LANDING,
		JUMP_RUN_LOOP,
		JUMP_RUN_START,
		JUMP_START,
		LADDER_DOWN_END,
		LADDER_DOWN_LOOP,
		LADDER_DOWN_START,
		LADDER_IDLE,
		LADDER_TRANS_JUMP_RUN_LOOP,
		LADDER_UP_END,
		LADDER_UP_LOOP,
		LADDER_UP_START,
		RUN_BRAKE_000,
		TREASURE_OPEN,
		WALK,
		WALK_TRANS_IDLE_000,
		WALK_TRANS_IDLE_001,


		BTL_STEP_AIR,
		BTL_STEP_LAND,
		BTL_STEP_LAND_BACK,
		SYS_DOOR_OPEN_HAND_RIGHT,
		SYS_RUN,

		BTL_MAGNARAY,

		IDLE_SIDE,
		IDLE_TO_IDLE_SIDE,

	};

private:
	CSion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSion(const CSion& rhs);
	virtual ~CSion() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public: /*For.State*/
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;

private:
	virtual HRESULT Ready_Parts() override;
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual void Change_Level(LEVEL eLevel) override;
	HRESULT SetUp_ShaderResources();

public:
	static CSion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END