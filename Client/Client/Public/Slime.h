#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)
class CSlime final : public CMonster
{
public:
	enum ANIM {
		ADVENT,
		ARISE_B,
		ARISE_F,
		ATTACK_FALL_END,
		ATTACK_FALL_LOOP,
		ATTACK_FALL_START,
		ATTACK_POISON_RAIN,
		ATTACK_POISON_SHOT,
		ATTACK_TACKLE,
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
		DOWN_UNIQUE,
		MAGIC_EMIT,
		MAGIC_END,
		MAGIC_LOOP,
		MAGIC_START,
		MOVE_IDLE,
		MOVE_RUN,
		MOVE_WALK_B,
		MOVE_WALK_F,
		TURN_L,
		TURN_POSE,
		TURN_R,
		SYMBOL_DETECT_IDLE,
		SYMBOL_DETECT_STOP_END,
		SYMBOL_DETECT_STOP_LOOP,
		SYMBOL_DETECT_STOP_START,
		SYMBOL_IDLE,
		SYMBOL_LOOKOUT,
		SYMBOL_RUN,
		SYMBOL_STOP,
		SYMBOL_TURN_LEFT,
		SYMBOL_TURN_POSE,
		SYMBOL_TURN_RIGHT,
		SYMBOL_WALK

		
	};



public:
	CModel* Get_Model() { return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	class CSlimeState* Get_State() { return m_pSlimeState; }
	void Set_PlayerState(class CSlimeState* pHawkState) { m_pSlimeState = pHawkState; }

public:
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;

	virtual _int Take_Damage(int fDamage, CBaseObj* DamageCauser) override;
	virtual HRESULT SetUp_ShaderID() override;
private:
	CSlime(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSlime(const CSlime& rhs);
	virtual ~CSlime() = default;

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
		/*For Navigation*/
	virtual void Check_Navigation() override;

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	class CSlimeState*  m_pSlimeState = nullptr;

private:
	_bool   m_bDoneChangeState = false;

public:
	static CSlime* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END