#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CPhysX final : public CBase
{
	DECLARE_SINGLETON(CPhysX)
public:
	enum GEOMETRY { GEOMETRY_SPHERE, GEOMETRY_BOX, GEOMETRY_CAPSULE, GEOMETRY_END };

private:
	CPhysX();
	virtual ~CPhysX() = default;
	
public:
	HRESULT Initialize(HWND hWnd, _uint iWinCX, _uint iWinCY, ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	HRESULT CreateScene();
	HRESULT CleanScene();
	void Tick(_float TimeDelta);

public:
	void Add_Actor(PxActor* actor);

public:
	PxControllerManager* Get_ControllerManager() { return m_ControllerManager; }
	PxMaterial* Get_Material() { return m_Material; }
	PxScene* Get_Scene() { return m_Scene; }
	PxPhysics* Get_Physics() { return m_Physics; }
	PxCooking* Get_Cooking() { return m_Cooking; }

private:
	/* heightField가 필요할 때*/
	PxPhysics* customCreatePhysics(PxU32 version, PxFoundation& foundation, const PxTolerancesScale& scale, bool trackOutstandingAllocations, PxPvd* pvd);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	HWND				m_hWnd;
	_uint				m_iWinCX, m_iWinCY;

	/* 1. Initialize시 Foundation 생성을 위해 필요한 변수*/
	PxDefaultErrorCallback  m_ErrorCallback;
	PxDefaultAllocator  m_AllocatorCallback;
	PxFoundation*		m_Foundation = nullptr;
	// PxFoundation이 있어야 gPhysics를 생성할 수 있다

	/* 2. 최상위 PxPhysics 를 만들기 위해 필요한 변수 */
	PxPhysics*			m_Physics = nullptr;
	PxPvd*				m_Pvd = nullptr;

	/* 3. PxCooking 를 만들기 위한 변수*/
	PxCooking*			m_Cooking = nullptr;

	/* 4. CPU리소스를 효율적으로 공유할 수 있도록 하기 위해 구현하는 것을 추천 */
	PxDefaultCpuDispatcher* m_CpuDispatcher = nullptr;		
	PxControllerManager*	m_ControllerManager = nullptr;
	PxScene*				m_Scene = nullptr;

	// gPhysics를 사용해 createMaterial해서 사용
	// 충돌제 마찰력, Dynamic 마찰력, 탄성력을 지정하여 사용
	PxMaterial* m_Material = NULL;

private: /* test */
	PxRigidStatic* m_groundPlane = nullptr;

public:
	virtual void Free() override;
};

END