#pragma once

#include "Client_Defines.h"
#include "Npc.h"


BEGIN(Client)

class CSnowFieldNpc final : public CNpc
{
public:
	enum NPCTYPE { FEMALE_YOUNG, MAN_OLD, MAN_GLD, MAN_PLC};

	enum FEMALE_YOUNG_ANIM { AVOID_ED, AVOID_LP, AVOID_ST, COLD, CROSS1_ED, CROSS1_LP, CROSS2_ED,
		CROSS2_LP, CROSS2_ST, CROSS1_ST, CROSS_ANS_TALK, CRY, DOWN, GET_DOWN_KNESS, GREET,
		HELP, HI, HURT, LAUGH, LOOK_AROUND, LOOK_AROUND2, PLEASE, POSE_STAND, RUN, SCARED, SIT, STAND, 
		TURNBACK, TURNLEFT, TURNRIGJT, WALK, WALK2
	};

	enum ANIM_OLD_MAN {
		WEEP, THANK, HEADACHE_START, HEADACHE_LOOP, HEADACHE_END, CROSSONESARMS_ST , CROSSONESARMS_LP,
		CROSSONESARMS_ED, NODAGREETING, THISISASECRET, KEEPASECRET, SHAKEONESHEAD, DEATH
	};

	enum ANIM_MAN_PLC {
		IDLE, FLYINTORAGE_START, FLYINTORAGE_LOOP, TALK, HI_START, HI_LOOP, HI_END,
		SECRET_START, SECRET_LOOP, SECRET_END, GIVEITTOYOU, SHRUG, AREYOUSERIOUS, WALKING
	};

	enum ANIM_MAN_GLD {
		GETHURT, HANDS_ONTHEWAIST, KUNGFU, CARRYBOX, LAUGH2, CROSSONESARMS_START, CROSSONESARMS_LOOP,
		CROSSONESARMS_END, SIGHED_START, SIGHED_LOOP, SIGHED_END, TALKING, BYE_START, BYE_LOOP, BYE_END, BOW,
		PLEASEKEEPASECRET, SHAKEHEAD, THRUST_ONES_FIST 
	};

private:
	CSnowFieldNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSnowFieldNpc(const CSnowFieldNpc& rhs);
	virtual ~CSnowFieldNpc() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public: /*For.State*/
	virtual _bool	Is_AnimationLoop(_uint eAnimId);
	void			Tick_State(_float fTimeDelta);
	void			LateTick_State(_float fTimeDelta);
	void			Talk_with_Npc();

private:
	virtual HRESULT Ready_Components(void* pArg);
	_uint m_eState = HURT;

public:
	static CSnowFieldNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END