#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"
#include "PlayerManager.h"

BEGIN(Engine)
class CNavigation;
class CModel;
END

BEGIN(Client)
class CPlayerState;
class CAIState;
class CPlayer abstract : public CBaseObj,
	public PxUserControllerHitReport,
	public PxControllerBehaviorCallback
{	
public:
	enum PARTS { PARTS_WEAPON, PARTS_END };
	enum SKILL {SKILL1, SKILL2, SKILL3, SKILL4, SKILL5, SKILL_END };
	enum PLAYERID {ALPHEN, SION};

protected:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() override;


public: /* Getter &  Setter */
	CModel*			Get_Model() { return m_pModelCom; }
	CTransform*		Get_Transform() { return m_pTransformCom; }
	CNavigation*	Get_Navigation(void) { return m_pNavigationCom; }
	CPlayerState*	Get_State() { return m_pPlayerState; }
	_uint			Get_SkillAnimIndex(SKILL SkillIndex) { return m_eSkills[SkillIndex]; }
	OBJINFO			Get_Info() { return m_tInfo; }
	PLAYERID			Get_PlayerID() { return m_ePlayerID; }
	CGameObject*	Get_Parts(_int iIndex) { return m_Parts[iIndex]; }


	void Set_PlayerState(class CPlayerState* pPlayerState) { m_pPlayerState = pPlayerState; }
	

public: /*For.State*/
	virtual _bool	Is_AnimationLoop(_uint eAnimId) PURE;
	void			HandleInput();
	void			Tick_State(_float fTimeDelta);
	void			Tick_AIState(_float fTimeDelta);
	void			LateTick_State(_float fTimeDelta);
	void			LateTick_AIState(_float fTimeDelta);


public: /*For.Navigation*/
	void Change_Navigation(LEVEL eLevel);
	void Check_Navigation();
	void Compute_CurrentIndex(LEVEL eLevel);



protected: /* For Component */
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	vector<CNavigation*>	m_vecNavigations;
	_bool	m_bIsFirst = true;
	

protected: /* for 4 Player */
	OBJINFO			m_tInfo;
	PLAYERID			m_ePlayerID = ALPHEN;
	CPlayerState*	m_pPlayerState = nullptr;
	CAIState*		m_pAIState = nullptr;
	_uint			m_eSkills[SKILL_END] = { 0 , 0 , 0, 0 , 0 };
	CPlayerManager*	m_pPlayerManager = nullptr;

protected:
	vector<class CGameObject*> m_Parts;

protected:
	HRESULT SetUp_Controller();
	//캡슐 컨트롤러 충돌처리 함수
	virtual void onShapeHit(const PxControllerShapeHit& hit) override;
	virtual void onControllerHit(const PxControllersHit& hit) override;
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) override;
	//캡슐 컨트롤러 충돌 방식 플래그
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor)override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller)override;
	virtual PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle)override;


	PxCapsuleController* gController = nullptr;// 캡슐 컨트롤서 포인터
	PxRigidDynamic* ControllerActor = nullptr;// 캡슐 연동 엑터
	//ControllerActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
	 //->컨트롤러 액터 물리효과 받도록 만드는 함수
	//(이때 컨트롤러의 움직이니 아닌 액터만 물리효과를 받기때문에 
	//액터의 포지션으로 컨트롤러를 움직여줘야함;
	_float fJump = 50.f;
	PxRigidDynamic* Px = nullptr;//일반 물리 객체

protected:
	virtual HRESULT Ready_Parts() { return S_OK; };
	virtual HRESULT Ready_Components(void* pArg) PURE;
	virtual HRESULT SetUp_ShaderResources();

public:
	virtual void Free() override;
};
END