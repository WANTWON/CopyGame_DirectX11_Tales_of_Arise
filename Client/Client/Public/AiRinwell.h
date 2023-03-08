#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CRinwellState;
class CAiRinwell final : public CMonster
{
public:
	enum ANIM {
		BTL_ADVENT,
		BTL_ARISE_B,
		BTL_ARISE_F,
		BTL_ATTACK_BRAVE,
		BTL_ATTACK_DENGEKISYOUHEKI,
		BTL_ATTACK_DENZIHOU,
		BTL_ATTACK_PHOTONFLASH,
		BTL_ATTACK_GALEFORCE,
		BTL_ATTACK_FUZIN,
		BTL_ATTACK_HIEN,
		BTL_ATTACK_HOUDEN,
		BTL_ATTACK_HYOUROU,
		BTL_ATTACK_MIZUTAMARI_END,
		BTL_ATTACK_MIZUTAMARI_LOOP,
		BTL_ATTACK_MIZUTAMARI_START,
		BTL_ATTACK_NORMAL_0,
		BTL_ATTACK_NORMAL_1,
		BTL_ATTACK_NORMAL_2,
		BTL_ATTACK_NORMAL_3,
		BTL_ATTACK_NORMAL_AIR_0,
		BTL_ATTACK_NORMAL_AIR_1,
		BTL_ATTACK_NORMAL_AIR_2,
		BTL_ATTACK_SENKOU,
		BTL_ATTACK_STRIKE,
		BTL_ATTACK_STRIKE_AIR,
		BTL_ATTACK_SUIHOU,
		BTL_ATTACK_TUMUZIKAZE,
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
		BTL_FALL,
		BTL_GUARD_HIT,
		BTL_GUARD_LOOP,
		BTL_GUARD_START,
		BTL_GUARD_WALK_B,
		BTL_GUARD_WALK_F,
		BTL_GUARD_WALK_L,
		BTL_GUARD_WALK_R,
		BTL_ITEM,
		BTL_JUMP,

		DASH,
		DASH_BRAKE_000,
		DASH_BRAKE_001,
		IDLE,
		IDLE_CHARA,
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
		LADDER_DOWN_TRANS_LADDER_IDLE,
		LADDER_IDLE,
		LADDER_TRANS_JUMP_RUN_LOOP,
		LADDER_UP_END,
		LADDER_UP_LOOP,
		LADDER_UP_START,
		LADDER_UP_TRANS_LADDER_IDLE,
		RUN,
		RUN_BRAKE_000,
		RUN_BRAKE_001,
		TOUCH_STAND,
		TREASURE_OPEN,
		WALK,
		WALK_TRANS_IDLE_000,
		WALK_TRANS_IDLE_001,


		BTL_STEP_AIR,
		BTL_STEP_LAND,
		BTL_STEP_LAND_BACK,

		BTL_ATTACK_MAGIC_EMIT_AIR,
		BTL_ATTACK_MAGIC_EMIT,
		BTL_ATTACK_MAGIC_STACK,
		BTL_MAGIC_EMIT,
		BTL_MAGIC_END,
		BTL_MAGIC_LOOP,
		BTL_MAGIC_START,
		BTL_MAGIC_WALK_B,
		BTL_MAGIC_WALK_F,
		BTL_MAGIC_WALK_L,
		BTL_MOVE_WALK_R,

		BTL_MOVE_IDLE,
		BTL_MOVE_RUN,
		BTL_LAND,
		SYS_DOOR_OPEN_HAND_RIGHT,
		IDLE_TO_IDLE_SIDE
	};


	enum SKILL_TYPE
	{
		PHOTONFLASH,
		GALEFORCE,
		CROSSBLADE,
		AIR_DUST,
		SKILL_END
	};

public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CRinwellState* Get_State() { return m_pState; }
	_uint Get_SkillIndex() { return m_eSkillIndex; }
	void Set_PlayerState(class CRinwellState* pPlayerState) { m_pState = pPlayerState; }
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }
	void Set_SkillIndex(_uint iIndex) { m_eSkillIndex = iIndex; }
	virtual void	 Set_BattleMode(_bool type) override;
	_bool Get_AirMode(void) { return m_bIsAir; }
	void Set_AirMode(_bool bAir) { m_bIsAir = bAir; }
	_float Get_Methor(void) { return m_fMethor; }

	void Kill_Boss_Rinwell();

public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser, HITLAGDESC HitDesc) override;
	virtual HRESULT SetUp_ShaderID() override;

private:
	CAiRinwell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAiRinwell(const CAiRinwell& rhs);
	virtual ~CAiRinwell() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render() override;
	virtual HRESULT Render_Glow() override;
	virtual void	 Set_HitState() override;
public: /*For.State*/
	void AI_Behavior(_float fTimeDelta);
	void Tick_State(_float fTimeDelta);
	void LateTick_State(_float fTimeDelta);
	void Battle_Animation(_float fTimeDelta);
	_bool Get_NpcMode() { return m_bNpcMode; }

		/*For Navigation*/
	virtual void Check_Navigation() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CRinwellState*  m_pState = nullptr;

	_float   m_fSpeed = 3.f;
	_uint	 m_eAnim = IDLE_CHARA;
	_uint	 m_ePreAnim = IDLE_CHARA;

	_bool	m_bMotion_Finished = false;
	_bool	m_bNpcMode = false;
	_uint	 m_eSkillIndex = PHOTONFLASH;

	_int	m_iDamage = 0;
	_bool	m_bIsAir = false;

	_float	m_fMethor = 1000.f;

public:
	static CAiRinwell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END