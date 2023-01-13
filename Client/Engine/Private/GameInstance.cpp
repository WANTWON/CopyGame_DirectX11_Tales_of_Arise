#include "..\Public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pObject_Manager(CObject_Manager::Get_Instance())
	, m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())	
	, m_pPipeLine(CPipeLine::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
	, m_pFont_Manager(CFont_Manager::Get_Instance())
	, m_pFrustum(CFrustum::Get_Instance())
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
	, m_pSound_Manager(CSound_Manager::Get_Instance())
	, m_pPicking(CPicking::Get_Instance())
{	
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pSound_Manager);
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pGraphic_Device); 
	
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, _uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	if (nullptr == m_pGraphic_Device || 
		nullptr == m_pObject_Manager)
		return E_FAIL;

	/* 그래픽 디바이스를 초기화한다. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, ppDevice, ppContext)))
		return E_FAIL;

	/* 입력 디바이스를 초기화한다. */
	if (FAILED(m_pInput_Device->Initialize(hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	/* 픽킹 초기화한다 */
	if (FAILED(m_pPicking->Initialize(GraphicDesc.hWnd, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, *ppDevice, *ppContext)))
		return E_FAIL;

	/* 사운드 디바이스를 초기화한다. */
	if (FAILED(m_pSound_Manager->Initialize()))
		return E_FAIL;

	/* 컨테이너의 공간을 확보해둔다. */
	if (FAILED(m_pObject_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Container(iNumLevels)))
		return E_FAIL;

	m_pFrustum->Initialize();
		

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || 
		nullptr == m_pObject_Manager)
		return;

	m_pInput_Device->Update();

	m_pLevel_Manager->Tick(fTimeDelta);
	m_pObject_Manager->Tick(fTimeDelta);

	m_pPipeLine->Update();
	m_pPicking->Tick();
	m_pFrustum->Transform_ToWorldSpace();

	m_pLevel_Manager->Late_Tick(fTimeDelta);
	m_pObject_Manager->Late_Tick(fTimeDelta);
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager || 
		nullptr == m_pComponent_Manager)
		return;

	m_pComponent_Manager->Clear(iLevelIndex);
	m_pObject_Manager->Clear(iLevelIndex);
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);

	return S_OK;
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_DepthStencil_View();
	
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

_char CGameInstance::Get_DIKState(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKState(eKeyID);	
}

_char CGameInstance::Get_DIMKeyState(DIMK eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMKeyState(eMouseKeyID);
}

_long CGameInstance::Get_DIMMoveState(DIMM eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMMoveState(eMouseMoveID);
}


_bool CGameInstance::Key_Down(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Key_Down(eKeyID);
}

_bool CGameInstance::Key_Up(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Key_Up(eKeyID);
}

_bool CGameInstance::Key_Pressing(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Key_Pressing(eKeyID);
}

_bool CGameInstance::Mouse_Down(DIMK eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Mouse_Down(eMouseKeyID);
}

_bool CGameInstance::Mouse_Up(DIMK eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Mouse_Up(eMouseKeyID);
}

_bool CGameInstance::Mouse_Pressing(DIMK eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Mouse_Pressing(eMouseKeyID);
}


HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(pTimerTag);	
}

_float CGameInstance::Get_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Get_TimeDelta(pTimerTag);
}

void CGameInstance::Update_TimeDelta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	m_pTimer_Manager->Update(pTimerTag);
}

HRESULT CGameInstance::Open_Level(unsigned int iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

CLevel * CGameInstance::Get_CurrentLevel()
{
	if (nullptr == m_pLevel_Manager)
		return nullptr;

	return m_pLevel_Manager->Get_CurrentLevel();
}

_uint CGameInstance::Get_CurrentLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return _uint();

	return m_pLevel_Manager->Get_CurrentLevelIndex();
}

_uint CGameInstance::Get_PastLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return _uint();

	return m_pLevel_Manager->Get_PastLevelIndex();
}

_uint CGameInstance::Get_DestinationLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return _uint();

	return m_pLevel_Manager->Get_DestinationLevelIndex();
}

void CGameInstance::Set_DestinationLevel(_uint LevelIndex)
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pLevel_Manager->Set_DestinationLevel(LevelIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObject(const _tchar * pPrototypeTag, _uint iLevelIndex, const _tchar * pLayerTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject(pPrototypeTag, iLevelIndex, pLayerTag, pArg);
}

CGameObject * CGameInstance::Clone_GameObject(const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Clone_GameObject(pPrototypeTag, pArg);
}

void CGameInstance::Clear_Layer(_uint iLevelIndex, const _tchar * LayerTag)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Clear_Layer(iLevelIndex, LayerTag);
}

CComponent * CGameInstance::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr; 

	return m_pObject_Manager->Get_Component(iLevelIndex, pLayerTag, pComponentTag, iIndex);
}

CGameObject * CGameInstance::Get_Object(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Objects(iLevelIndex, pLayerTag, iIndex);
}

list<CGameObject*>* CGameInstance::Get_ObjectList(_uint iSceneID, const _tchar * pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_ObjectList(iSceneID, pLayerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);	
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4(eState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4_TP(eState);
}

_float4 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return _float4();

	return m_pPipeLine->Get_CamPosition();
}

const LIGHTDESC * CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);	
}

HRESULT CGameInstance::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc);	
}

void CGameInstance::Set_LightDesc(_uint iIndex, LIGHTDESC * pLightDesc)
{
	if (nullptr == m_pLight_Manager)
		return;

	return m_pLight_Manager->Set_LightDesc(iIndex, pLightDesc);
}


void CGameInstance::Clear_AllLight()
{
	if (nullptr == m_pLight_Manager)
		return;

	return m_pLight_Manager->Clear_AllLight();
}

void CGameInstance::Clear_Light(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return;

	return m_pLight_Manager->Clear_Light(iIndex);
}

_uint CGameInstance::Get_LightSize()
{
	return m_pLight_Manager->Get_LightSize();
}

HRESULT CGameInstance::Set_ShadowLightView(_float4 vEye, _float4 vAt)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Set_ShadowLightView(vEye, vAt);
}

_float4x4 CGameInstance::Get_ShadowLightView()
{
	if (nullptr == m_pLight_Manager)
		return _float4x4();

	return m_pLight_Manager->Get_ShadowLightView();
}

HRESULT CGameInstance::Add_Fonts(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Fonts(pDevice, pContext, pFontTag, pFontFilePath);	
}

HRESULT CGameInstance::Render_Font(const _tchar * pFontTag, const _tchar * pText, _fvector vPos, _fvector vColor, _float fScale)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render_Font(pFontTag, pText, vPos, vColor, fScale);
}


void CGameInstance::PlaySounds(TCHAR * pSoundKey, const _uint & eID, const float & fVolume)
{
	if (nullptr == m_pSound_Manager)
		return;

	m_pSound_Manager->PlaySoundW(pSoundKey, eID, fVolume);
}

void CGameInstance::PlayBGM(TCHAR * pSoundKey, const float & fVolume)
{
	if (nullptr == m_pSound_Manager)
		return;

	m_pSound_Manager->PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::StopSound(const _uint & eID)
{
	if (nullptr == m_pSound_Manager)
		return;

	m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	if (nullptr == m_pSound_Manager)
		return;

	m_pSound_Manager->StopAll();
}

void CGameInstance::SetChannelVolume(const _uint & eID, const float & fVolume)
{
	if (nullptr == m_pSound_Manager)
		return;

	m_pSound_Manager->SetChannelVolume(eID, fVolume);
}

int CGameInstance::VolumeUp(const _uint & eID, const _float & _vol)
{
	if (nullptr == m_pSound_Manager)
		return 0;

	return m_pSound_Manager->VolumeUp(eID, _vol);
}

int CGameInstance::VolumeDown(const _uint & eID, const _float & _vol)
{
	if (nullptr == m_pSound_Manager)
		return 0;

	return m_pSound_Manager->VolumeDown(eID, _vol);
}

int CGameInstance::Pause(const _uint & eID)
{
	if (nullptr == m_pSound_Manager)
		return 0;

	return m_pSound_Manager->Pause(eID);
}

_vector CGameInstance::Get_RayPos()
{
	if (nullptr == m_pPicking)
		return _vector();

	return m_pPicking->Get_RayPos();
}

_vector CGameInstance::Get_RayDir()
{
	if (nullptr == m_pPicking)
		return _vector();

	return m_pPicking->Get_RayDir();
}


_bool CGameInstance::isIn_WorldFrustum(_fvector vPosition, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_WorldFrustum(vPosition, fRange);
}

void CGameInstance::Release_Engine()
{
	CGameInstance::Get_Instance()->Destroy_Instance();

	CLevel_Manager::Get_Instance()->Destroy_Instance();

	CObject_Manager::Get_Instance()->Destroy_Instance();

	CComponent_Manager::Get_Instance()->Destroy_Instance();
	
	CPipeLine::Get_Instance()->Destroy_Instance();

	CLight_Manager::Get_Instance()->Destroy_Instance();

	CTimer_Manager::Get_Instance()->Destroy_Instance();

	CInput_Device::Get_Instance()->Destroy_Instance();

	CFont_Manager::Get_Instance()->Destroy_Instance();

	CFrustum::Get_Instance()->Destroy_Instance();

	CTarget_Manager::Get_Instance()->Destroy_Instance();

	CSound_Manager::Get_Instance()->Destroy_Instance();

	CPicking::Get_Instance()->Destroy_Instance();

	CGraphic_Device::Get_Instance()->Destroy_Instance();
}

void CGameInstance::Free()
{	
	Safe_Release(m_pPicking);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
}
