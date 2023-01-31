#include "..\Public\PhysX.h"

#include "GameObject.h"
IMPLEMENT_SINGLETON(CPhysX)

CPhysX::CPhysX()
{
}


HRESULT CPhysX::Initialize(HWND hWnd, _uint iWinCX, _uint iWinCY, ID3D11Device*	pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice); Safe_AddRef(m_pContext);
	m_hWnd = hWnd;
	m_iWinCX = iWinCX;
	m_iWinCY = iWinCY;

	/* 1. Make PxFoundation */
	/* Every PhysX module requires a PxFoundation instance to be available */
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_AllocatorCallback,
		m_ErrorCallback);
	if (!m_Foundation)
	{
		ERR_MSG(TEXT("PxCreateFoundation failed!"));
		return E_FAIL;
	}


	/* 2. 최상위 PxPhysics 객체를 만든다*/
	bool  recordMemoryAllocations = true; //메모리 프로파일링을 수행할지 여부 지정
	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport*  transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	/*PhysX는 입력이 해당 단위를 일관되게 사용하는 한 길이 또는 질량 단위에 관계없이 올바른 결과를 생성하도록 설계되었습니다.
	그러나 단위에 따라 기본값을 조정해야 하는 특정 공차 값이 있습니다.
	이러한 허용 오차가 적절한 값으로 기본 설정되도록 하려면 PxPhysics 및 PxCooking 인터페이스를 생성할 때
	PxTolerancesScale의 값을 조정하십시오. 개체에 대한 공차는 생성 시 설정되며 응용 프로그램에서 재정의할 수 있습니다.*/
	PxTolerancesScale scale = PxTolerancesScale();
	scale.length = 1.f;
	scale.speed = 10.f;
	m_Physics = customCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation,scale, recordMemoryAllocations, m_Pvd);
	if (!m_Physics)
	{
		ERR_MSG(TEXT("PxCreatePhysics failed!"));
		return E_FAIL;
	}

	/* 3. PxCreateCooking  */
	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxCookingParams(scale));
	if (!m_Cooking)
	{
		ERR_MSG(TEXT("PxCreateCooking failed!"));
		return E_FAIL;
	}

	/* 4. 확장 라이브러리 초기화 */
	if (!PxInitExtensions(*m_Physics, m_Pvd))
	{
		ERR_MSG(TEXT("PxInitExtensions failed!"));
		return E_FAIL;
	}
	

	return S_OK;
}

HRESULT CPhysX::CreateScene()
{
	// mNbThreads 작업자 스레드인 CPU 디스패처 생성 
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	m_CpuDispatcher = PxDefaultCpuDispatcherCreate(info.dwNumberOfProcessors); //CPU의 스레드 개수
	if(!m_CpuDispatcher)
	{
		ERR_MSG(TEXT("PxDefaultCpuDispatcherCreate failed!"));
		return E_FAIL;
	}

	// Scene을 Set 해줍니다.
	PxSceneDesc  sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); //default gravity
	sceneDesc.cpuDispatcher = m_CpuDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	
	//default materials   //static friction, dynamic friction, restitution
	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.5f);// 충돌체 마찰력, dynamic 마찰력, 탄성력

	//create a static plane (XZ)
	m_groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material); // 평지를 만들어 줍니다
	m_Scene->addActor(*m_groundPlane);

	m_ControllerManager = PxCreateControllerManager(*m_Scene);

	return S_OK;
}

HRESULT CPhysX::CleanScene()
{
	PX_RELEASE(m_ControllerManager);
	PX_RELEASE(m_Scene);
	PX_RELEASE(m_CpuDispatcher);
	return S_OK;
}

void CPhysX::Tick(_float TimeDelta)
{
	if (m_Scene)
	{
		m_Scene->simulate(TimeDelta);
		m_Scene->fetchResults(true);
	}
}

void CPhysX::Add_Actor(PxActor * actor)
{
	m_Scene->addActor(*actor);
}

PxPhysics * CPhysX::customCreatePhysics(PxU32 version, PxFoundation & foundation, const PxTolerancesScale & scale, bool trackOutstandingAllocations, PxPvd * pvd)
{
	PxPhysics* physics = PxCreateBasePhysics(version, foundation, scale,trackOutstandingAllocations, pvd);

	if (!physics)
		return nullptr;

	PxRegisterHeightFields(*physics);


	return physics;
}

void CPhysX::Free()
{
	PX_RELEASE(m_ControllerManager);
	PX_RELEASE(m_Scene);
	PX_RELEASE(m_CpuDispatcher);

	PxCloseExtensions();
	PX_RELEASE(m_Physics);
	PX_RELEASE(m_Cooking);

	if (m_Pvd)
	{
		PxPvdTransport* transport = m_Pvd->getTransport();
		m_Pvd->release();	m_Pvd = nullptr;
		PX_RELEASE(transport);

	}
	PX_RELEASE(m_Foundation);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
