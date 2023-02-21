#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CIce_Wolf final : public CMonster
{
public:
	enum ANIM {
		//TO14_MTN_CHR_WOL_000_BTL_ADVENT,
		//TO14_MTN_CHR_WOL_000_BTL_ARISE_B,
		//TO14_MTN_CHR_WOL_000_BTL_ARISE_F,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_BITE,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_BREATH_LEFT,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_CHARGE_ALL,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_ELEMENTAL_CHARGE_END,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_ELEMENTAL_CHARGE_LOOP,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_ELEMENTAL_CHARGE_START,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_HOWLING,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_JUMP_SCRATCH,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_SIDEWALK_R_END,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_SIDEWALK_R_LOOP,
		//TO14_MTN_CHR_WOL_000_BTL_ATTACK_SIDEWALK_R_START,

		ANIM_ADVENT,
		ANIM_ARISE_B,
		ANIM_ARISE_F,

		ANIM_ATTACK_BITE,
		ANIM_ATTACK_BREATH_LEFT,
		ANIM_ATTACK_ELEMENTAL_CHARGE_END,
		ANIM_ATTACK_ELEMENTAL_CHARGE_LOOP,
		ANIM_ATTACK_ELEMENTAL_CHARGE_START,
		ANIM_ATTACK_HOWLING, //9
		ANIM_ATTACK_NORMAL,
		ANIM_ATTACK_SOMERSAULT_END,
		ANIM_ATTACK_SOMERSAULT_LOOP,
		ANIM_ATTACK_SOMERSAULT_START,
		ANIM_ATTACK_STEP_BACK,

		ANIM_DEAD,
		ANIM_DOWN_B, ANIM_DOWN_F,
		ANIM_MOVE_IDLE,
		ANIM_MOVE_RUN,
		ANIM_MOVE_WALK_B,
		ANIM_MOVE_WALK_F,
		ANIM_TURN_L,
		ANIM_TURN_R,
		ANIM_BLOW_DOWN_B,
		ANIM_BLOW_DOWN_F,
		ANIM_BLOW_UP_B,
		ANIM_BLOW_UP_F,

		ANIM_DAMAGE_AIR_LARGE_B,
		ANIM_DAMAGE_AIR_LARGE_F,
		ANIM_DAMAGE_AIR_LARGE_L,
		ANIM_DAMAGE_AIR_LARGE_R,
		ANIM_DAMAGE_AIR_LOOP,
		ANIM_DAMAGE_AIR_SMALL_B,
		ANIM_DAMAGE_AIR_SMALL_F,
		ANIM_DAMAGE_AIR_SMALL_L,
		ANIM_DAMAGE_AIR_SMALL_R,
		ANIM_DAMAGE_LARGE_B,
		ANIM_DAMAGE_LARGE_F,
		ANIM_DAMAGE_LARGE_L,
		ANIM_DAMAGE_LARGE_R,
		ANIM_DAMAGE_LOOP,
		ANIM_DAMAGE_SMALL_B,
		ANIM_DAMAGE_SMALL_F,
		ANIM_DAMAGE_SMALL_L,
		ANIM_DAMAGE_SMALL_R,

		ANIM_SYMBOL_DETECT_IDLE,
		ANIM_SYMBOL_DETECT_STOP,
		ANIM_SYMBOL_IDLE,
		ANIM_SYMBOL_LOOKOUT,
		ANIM_SYMBOL_RUN,
		ANIM_SYMBOL_STOP,
		ANIM_SYMBOL_TURN_LEFT, //52
		ANIM_SYMBOL_TURN_RIGHT,
		ANIM_SYMBOL_WALK,
	};

public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CIceWolfState* Get_State() { return m_pState; }
	void Set_PlayerState(class CIceWolfState* pPlayerState) { m_pState = pPlayerState; }
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }
	void Set_Done_HitAnimState() { m_bDone_HitAnimState = false; }
	void Set_OnGoingBite() { m_bOnGoing_Bite= true; }
	void Set_FinishBite() { m_bOnGoing_Bite = false; }
	virtual void	 Set_BattleMode(_bool type) override;

public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser, _bool bLockOnChange = true) override;
	virtual HRESULT SetUp_ShaderID() override;

private:
	CIce_Wolf(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIce_Wolf(const CIce_Wolf& rhs);
	virtual ~CIce_Wolf() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render_Glow() override;

public: /*For.State*/
	void AI_Behavior(_float fTimeDelta);
	void Tick_State(_float fTimeDelta);
	void LateTick_State(_float fTimeDelta);

public:	/*For Navigation*/
	virtual void Check_Navigation() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CIceWolfState*  m_pState = nullptr;

	_float m_fSpeed = 3.f;
	
	_int m_iBeDamaged_Cnt = 0;
	_bool m_bDone_HitAnimState = false;
	_bool m_bSomeSauling = false;
	_bool m_bOnGoing_Bite = false;

public:
	static CIce_Wolf* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END