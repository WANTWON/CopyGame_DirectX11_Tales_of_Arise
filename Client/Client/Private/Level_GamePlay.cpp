#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;	

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	pCameraManager->Ready_Camera(LEVEL::LEVEL_GAMEPLAY);
	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	


}

void CLevel_GamePlay::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));


	CBaseObj* pPlayer = dynamic_cast<CBaseObj*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	_float4		vLightEye, vLightAt;

	XMStoreFloat4(&vLightEye, pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION));
	vLightEye.y = 0.f;
	vLightAt = vLightEye;
	vLightEye.y += 50.f;
	vLightEye.z += 50.f;

	CGameInstance::Get_Instance()->Set_ShadowLightView(vLightEye, vLightAt);

}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	LIGHTDESC			LightDesc;

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(0.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);	

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	_float4		vLightEye, vLightAt;

	XMStoreFloat4(&vLightEye, XMVectorSet(36, 50, 70, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(36, 0, 20, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_STATIC, pLayerTag, nullptr)))
		return E_FAIL;	

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(10.f, 0.f, 10.f, 1.f));
	pPlayer->Change_Navigation(LEVEL_GAMEPLAY);
	pPlayer->Compute_CurrentIndex(LEVEL_GAMEPLAY);
	pPlayer->Check_Navigation();

	Safe_Release(pGameInstance);
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	for (_uint i = 0; i < 5; ++i)
	{
		_vector vPosition = { rand() % 20, 0.f, rand() % 20 , 1.f };
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Ice_Wolf"), LEVEL_GAMEPLAY, pLayerTag, &vPosition)))
			return E_FAIL;
	}
	

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (FAILED(pGameInstance->Add_GameObjectLoad(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Prototype_Component_VIBuffer_Terrain_Load"))))
	//	return E_FAIL;

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;*/
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Water"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Particle_Rect"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Particle_Point"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Dynamic::CAMERADESC_DERIVED				CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.InitPostion = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.vDistance = _float4(0, 10, -10, 0.f);

	CameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -10.f, 1.f);
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = 500.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_GAMEPLAY, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;*/


	for (int i = 0; i < 4; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPbar"), LEVEL_GAMEPLAY, pLayerTag, &i)))
			return E_FAIL;

	}

	for (int i = 0; i < 4; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPfont"), LEVEL_GAMEPLAY, pLayerTag, &i)))
			return E_FAIL;

	}
	
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Comboline"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL; 

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Damagesfont"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_font"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	for (int i = 0; i < 6; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_DAMAGES_fontnum"), LEVEL_GAMEPLAY, pLayerTag, &i)))
			return E_FAIL;

	}
	

//	for (int i = 0; i < 3; ++i)
//	{
//		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_fontnum"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;

//	}

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_CPguage"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	
	for (int i = 0; i < 7; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_CPguage_font"), LEVEL_GAMEPLAY, pLayerTag, &i)))
			return E_FAIL;

	}
	
	
	
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MPguage"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPbar_World"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;*/

//	for (int i = 0; i < 4; ++i)
//	{
//		_uint number = i;

//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPfont_World"), LEVEL_GAMEPLAY, pLayerTag, &i)))
//			return E_FAIL;
		/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPfont_World"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;*/

//	}
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitback"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_top"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_right"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_bottom"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_left"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_button"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_change_button"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_LOCKON"), LEVEL_GAMEPLAY, pLayerTag)))
			return E_FAIL;

		

		//1130  360  1130 420   1130 4801180

		
		//CUI_RuneEffect::RUNEDESC desc;
		/*desc.position.x = 1130.f;
		desc.position.y = 375.f;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, pLayerTag , &desc)))
			return E_FAIL;
		desc.position.y = 435.f;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, pLayerTag, &desc)))
			return E_FAIL;
		desc.position.y = 495.f;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, pLayerTag, &desc)))
			return E_FAIL;*/
		
		
		
		
	

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();


}
