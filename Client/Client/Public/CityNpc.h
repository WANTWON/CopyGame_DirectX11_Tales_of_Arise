#pragma once

#include "Client_Defines.h"
#include "Npc.h"
#include "GameInstance.h"

BEGIN(Client)

class CCityNpc final : public CNpc
{
public:
	enum NPCTYPE { NPC_NFC_SLV_000, NPC_NFC_SLV_000_2th, NPC_NFM_SLV_000, 
		NPC_NFY_FIA_000, NPC_NMM_BEF_000
	, NPC_NMM_BLS_000, NPC_NMM_DIM_000,
		NPC_NMM_MHB_000, NPC_NMM_SLV_000, MAN_GLD, MAN_PLC};


public:
	enum NPC_NFC_SLV_000_ANIM {
		Run,
		Idle,
		qHappyStart,
		qHappyLoop,
		qHappyEnd,
		ImHugry,
		Squat,
		Walk
	};

	enum NPC_NFC_SLV_000_2th_ANIM {
		Run_2th,
		Idle_2th,
		qHappyStart_2th,
		qHappyLoop_2th,
		qHappyEnd_2th,
		ImHugry_2th,
		Cry_Start_2th,
		Cry_Loop_2th,
		Cry_End_2th,
		Whispe_Start_2th,
		Whispe_Loop_2th,
		Whispe_End_2th,
		Squat_2th,
		Walk_2th
	};

	enum NPC_NFY_FIA_000_ANIM {
		FIA_Idle,
		FIA_Laugh,
		FIA_Walk
	};

private:
	CCityNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCityNpc(const CCityNpc& rhs);
	virtual ~CCityNpc() = default;

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

public:
	_float Find_Target();
	_float Find_MAN_GLD();

private:
	virtual HRESULT Ready_Components(void* pArg);
	_uint m_eState = 0;
	class CCityNpc * m_pTarget = nullptr;
	class CCityNpc * m_pGld = nullptr;
	_float			m_fDistance = 0.f;
	_float			m_fGldDistance = 0.f;
	_float			m_fChild_TimeDleta = 0.f;
	_float			m_fChild_2thTimeDleta = 0.f;
	_float			m_fTimeDelta = 0.f;
	_float			m_fChildSpeed = 0.f;

	_bool			m_chaseGldFinish = false;
	_bool			m_bTired = false;

	_bool			m_bIsAnimationFinished = false;
public:
	static CCityNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END