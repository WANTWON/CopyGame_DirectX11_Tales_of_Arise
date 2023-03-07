#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CAstralDoubt final : public CMonster
{
public:
	enum ANIM {
		ARISE_F,
		ATTACK_BRAVE,
		ATTACK_HEAD_BEAM,
		ATTACK_IN_OUT_UPPER,
		ATTACK_JUMP_SHOOT_DROP_CEILING,
		ATTACK_JUMP_SHOOT_DROP_DESCEND_LOOP,
		ATTACK_JUMP_SHOOT_DROP_END,
		ATTACK_JUMP_SHOOT_DROP_RISE_LOOP,
		ATTACK_JUMP_SHOOT_DROP_START,
		ATTACK_SPEAR_HANDSTAND_FOOTPRESS,
		ATTACK_SPEAR_MULTI,
		ATTACK_SPEAR_RUSH_END,
		ATTACK_SPEAR_RUSH_LOOP,
		ATTACK_SPEAR_RUSH_START,
		ATTACK_SPEAR_SWING_END,
		ATTACK_SPEAR_SWING_LOOP,
		ATTACK_SPEAR_SWING_START,
		ATTACK_SWING_360,
		ATTACK_SWORD_720_FIRST,
		ATTACK_SWORD_720_LAST,
		DEAD,
		DOWN_B,
		DOWN_F,
		DOWN_UNIQUE,
		EVENT_ADVENT,
		MOVE_IDLE,
		MOVE_WALK_B,
		MOVE_WALK_F,
		SYMBOL_DETECT_IDLE,
		SYMBOL_DETECT_STOP,
		SYMBOL_IDLE,
		SYMBOL_LOOKOUT,
		SYMBOL_RUN,
		SYMBOL_STOP,
		SYMBOL_TURN_LEFT,
		SYMBOL_TURN_POSE,
		SYMBOL_TURN_RIGHT,
		SYMBOL_WALK,
		TURN_L,
		TURN_POSE,
		TURN_R,
		ARISE_B
	};

public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CAstralDoubt_State* Get_State() { return m_pState; }
	void Set_PlayerState(class CAstralDoubt_State* pPlayerState) { m_pState = pPlayerState; }
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }
	void Set_Done_HitAnimState() { m_bDone_HitAnimState = true; }
	void Set_Finish_HitAnimState() { m_bDone_HitAnimState = false; }
	void Set_OnGoing320Spin() { m_bOnGoing_320Spin = true; }
	void Set_Finish320Spin() { m_bOnGoing_320Spin = false; }
	virtual void	 Set_BattleMode(_bool type) override;
public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser, HITLAGDESC HitDesc) override;
	virtual HRESULT SetUp_ShaderID() override;
	virtual void	 Set_HitState() override;
private:
	CAstralDoubt(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAstralDoubt(const CAstralDoubt& rhs);
	virtual ~CAstralDoubt() = default;

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
	virtual _float ForTheBossTarget_Distance(CBaseObj*);

	void Check_Navigation_Jump(void);

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CAstralDoubt_State*  m_pState = nullptr;

	_float m_fSpeed = 3.f;
	
	_int m_iBeDamaged_Cnt = 0;
	_bool m_bDone_HitAnimState = false;
//	_bool m_bSomeSauling = false;
	_bool m_bOnGoing_320Spin = false;

public:
	static CAstralDoubt* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END