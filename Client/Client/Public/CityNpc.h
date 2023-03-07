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
		NPC_NMM_MHB_000, NPC_NMM_SLV_000, MAN_GLD, MAN_PLC, NPC_NFY_WAC, NPC_NMY_GNL, DOG, DUCK};


public:
	enum NPC_NMM_DIM_ANIM {
		DIM_TURN_LEFT,
		DIM_IDLE,
		DIM_TURN_RIGHT,
		DIM_TALK,
		DIM_WALK
	};

	enum DOG_ANIM {
		DOG_IDLE,
		DOG_PEE,
		DOG_BARK,
		DOG_SIT,
		DOG_WALK,
		DOG_WALK_TWO,
		DOG_RUN,
		DOG_WAG_A_TAIL
	};

	enum DUCK_ANIM{
		DUCK_WALK,
		DUCK_READY_FLY,
		DUCK_LOOKAT_BUG
	};
	
	enum NPC_NFY_WAC_ANIM {
		WAC_Sit,
		WAC_Walk
	};

	enum NPC_NFY_FIA_ANIM {
		FIA_Idle,
		FIA_Walk_leisurely,
		FIA_Laugh,
		FIA_Talk,
		FIA_whisper,
		FIA_OKIve_decided,
		FIA_Hands_onbothwaists,
		FIA_Walk,
	};

	enum NPC_NFM_SLV_ANIM {
		NFM_IDLE,
		NFM_HI,
		NFM_SIT
	};

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

	//enum NPC_NFY_FIA_000_ANIM {
	//	FIA_Idle,
	//	FIA_Laugh,
	//	FIA_Walk
	//};

	enum NPC_MAN_PLC {
		PLC_TurnRight,
		PLC_Idle,
		PLC_TurnLeft,
		FlyIntoRage_Start,
		FlyIntoRage_Loop,
		FlyIntoRage_End,
		Talk,
		Hi_Start,
		Hi_Loop,
		Hi_End,
		Secret_Start,
		Secret_Loop,
		Secret_End,
		GiveitToYou,
		Shrug,
		AreyouSerious,
		PLC_Walk,

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
	void Set_eState(_uint eState) { m_eState = eState; }

public:
	_float		Find_Friend();
	_float		Find_MAN_GLD();
	_float		Find_NMM_SLV();
	_float		Find_NFM();

	//_float		Find_PLC_Target(_uint iIndex);
	
	CCityNpc*	Find_FIA_Target();
	CCityNpc*	Find_GNL_Target();


	CCityNpc*	Get_FIA(_uint iIndex);
	CCityNpc*	Get_PLC(_uint iIndex);
	CCityNpc*	Get_GLD();
	CCityNpc*	Get_Duck(_uint iIndex);

private:
	virtual HRESULT Ready_Components(void* pArg);
	_uint m_eState = 0;
	_uint m_ePreState = 0;
	class CCityNpc * m_pChildFriend = nullptr;
	class CCityNpc * m_pGld = nullptr;
	class CCityNpc * m_pNMM_SLV = nullptr;
	class CCityNpc * m_pNFM = nullptr;
	class CCityNpc * m_pDuck_One = nullptr;
	class CCityNpc * m_pDuck_Two = nullptr;

	class CCityNpc * m_pFIA_Target = nullptr;
	class CCityNpc * m_pDIM_Target = nullptr;
	class CCityNpc * m_pGNL_Target = nullptr;

	_float			m_fFriendDistance = 0.f;
	_float			m_fGLD_Distance = 0.f;
	_float			m_fNMM_SLV_Distance = 0.f;
	_float			m_fNFM_Distance = 0.f;

	_float			m_fChild_TimeDleta = 0.f;
	_float			m_fChild_2thTimeDleta = 0.f;
	_float			m_fTimeDelta = 0.f;
	_float			m_fTimeDelta_Two = 0.f;
	_float			m_fPLC_TimeDelta = 0.f;
	_float			m_fChildSpeed = 0.f;
	_float			m_fActiveTarget_Distance = 0.f;

	_bool			m_bchaseGLDFinish = false;
	_bool			m_bchaseNMM_SLV_Start = false;
	_bool			m_bChaseNFMStart = false;
	_bool			m_bChaseFriend_Start= false;
	
	_bool			m_bFriendBye = false;
	_bool			m_bListenFinish = false;
	_bool			m_bTurn = false;
	_bool			m_bTired = false;
	_bool			m_bHappyStart = false;
	_bool			m_bHappyLoop = false;
	_bool			m_bHappyEnd = false;
	_bool			m_bIsAnimationFinished = false;
	_bool			m_bTurnFinish = false;
	_bool			m_bCrowdSound = false;

	//DOG
	_bool			m_bIdle = false;
	_bool			m_bPee = false;
	_bool			m_bWag_a_Tail = false;

	//GNL
	_float			m_fGNL_Target_Distance = 0.f;
	_bool			m_bLookAt = false;

	//DIM
	_bool			 m_bTalkAnim = false;
	_float			 m_fDIM_Target_Distance = 0.f;
	

	//PLC
	_float			m_fPLC_One_Target_Distance = 0.f;
	_float			m_fPLC_Two_Target_Distance = 0.f;

	_bool			m_bPLC_Anim_Hi_End_Begin= false;

	_bool			m_bPLC_SayGoodbye = false;
	_bool			m_bPLC_Anim_Turn_Left_Begin = false;
	_bool			m_bPLC_GoToHer = true;

	//FIA
	
	_float			m_fFIA_Target_Distance = 0.f;
	_float			m_fFIA_Two_Target_Distance = 0.f;

	_bool			m_bTalk = false;
	_bool			m_bLaugh = false;
	_bool			m_bHands_onbothwaists = false;

public:
	static CCityNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END