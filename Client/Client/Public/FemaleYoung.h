#pragma once

#include "Client_Defines.h"
#include "Npc.h"


BEGIN(Client)

class CFemaleYoung final : public CNpc
{
public:
	enum ANIM { AVOID_ED, AVOID_LP, AVOID_ST, COLD, CROSS1_ED, CROSS1_LP, CROSS2_ED,
		CROSS2_LP, CROSS2_ST, CROSS1_ST, CROSS_ANS_TALK, CRY, DOWN, GET_DOWN_KNESS, GREET,
		HELP, HI, HURT, LAUGH, LOOK_AROUND, LOOK_AROUND2, PLEASE, POSE_STAND, RUN, SCARED, SIT, STAND, 
		TURNBACK, TURNLEFT, TURNRIGJT, WALK, WALK2
	};

private:
	CFemaleYoung(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFemaleYoung(const CFemaleYoung& rhs);
	virtual ~CFemaleYoung() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public: /*For.State*/
	virtual _bool	Is_AnimationLoop(_uint eAnimId);
	void			Tick_State(_float fTimeDelta);
	void			LateTick_State(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components(void* pArg);

	ANIM m_eState = HURT;

public:
	static CFemaleYoung* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END