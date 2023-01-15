#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CIce_Wolf final : public CMonster
{
public:
	enum ANIM {
		ANIM_ADVENT, 
		ANIM_ARISE_B, 
		ANIM_ARISE_F,
		
		ANIM_ATTACK_BITE, 
		ANIM_ATTACK_BREATH_LEFT, 
		ANIM_ATTACK_ELEMENTAL_CHARGE_END, 
		ANIM_ATTACK_ELEMENTAL_CHARGE_LOOP,
		ANIM_ATTACK_ELEMENTAL_CHARGE_START,
		ANIM_ATTACK_HOWLING,
		ANIM_ATTACK_NORMAL,
		ANIM_ATTACK_SOMERSAULT_END,
		ANIM_ATTACK_SOMERSAULT_LOOP,
		ANIM_ATTACK_SOMERSAULT_START,
		ANIM_ATTACK_STEP_BACK,

		ANIM_DEAD,
		ANIM_DOWN_B,ANIM_DOWN_F,
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
		ANIM_SYMBOL_TURN_LEFT,
		ANIM_SYMBOL_TURN_RIGHT,
		ANIM_SYMBOL_WALK,
	};



public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CIceWolfState* Get_State() { return m_pIce_WolfState; }
	void Set_PlayerState(class CIceWolfState* pPlayerState) { m_pIce_WolfState = pPlayerState; }

public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual _float Take_Damage(float fDamage, CBaseObj* DamageCauser) override;
	
private:
	CIce_Wolf(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIce_Wolf(const CIce_Wolf& rhs);
	virtual ~CIce_Wolf() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public: /*For.State*/
	void AI_Behavior();
	void TickState(_float fTimeDelta);
	void LateTickState(_float fTimeDelta);
		/*For Navigation*/
	virtual void Check_Navigation() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CIceWolfState*  m_pIce_WolfState = nullptr;

public:
	static CIce_Wolf* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END