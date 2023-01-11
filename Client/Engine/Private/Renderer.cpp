#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "Component.h"
#include "PipeLine.h"
#include "GameInstance.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::Initialize_Prototype()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	D3D11_VIEWPORT		ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);


	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	/* For.Target_Reflection */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Reflection"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	/* For.Target_Blur */  //블러하기 위한 화면을 찍어낸다.
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur"), ViewportDesc.Width, ViewportDesc.Height, 
		DXGI_FORMAT_B8G8R8A8_UNORM, &_float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Blend */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blend"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_B8G8R8A8_UNORM, &_float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_BlurDownSample */  //찍어낸 화면을 다운 샘플링 하고
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BlurDownSample"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_B8G8R8A8_UNORM, &_float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_BlurHor */ //수평으로 블러 후 찍어내고
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BlurHor"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_B8G8R8A8_UNORM, &_float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_BlurVer */ //수직으로 블러 후 찍어내고
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BlurVer"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_B8G8R8A8_UNORM, &_float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_BlurUpSample */ //그걸 다시 업 샘플링 한다.
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BlurUpSample"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_B8G8R8A8_UNORM, &_float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Grow */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Grow"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, &_float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	

	/* For.Target_ShadowDepth */
	_uint		iShadowMapCX = 1280.f *12.5f;
	_uint		iShadowMapCY = 720.f *12.5f;

	if (FAILED(m_pTarget_Manager->Ready_ShadowDepthStencilRenderTargetView(m_pDevice, iShadowMapCX, iShadowMapCY)))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth"), iShadowMapCX, iShadowMapCY,
		DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(1.0f, 1.0f, 1.0f, 1.0f))))
		return E_FAIL;


	/* For.MRT_Deferred */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Specular"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Blur"))))
		return E_FAIL;


	/* For.MRT_LightAcc */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Reflection"))))
		return E_FAIL;


	/* For.MRT_LightDepth*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth"), TEXT("Target_ShadowDepth"))))
		return E_FAIL;

	/* For.MRT_Blend*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blend"), TEXT("Target_Blend"))))
		return E_FAIL;

	/* For.MRT_NonLight*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_NonLight"), TEXT("Target_Blend"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_NonLight"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_NonLight"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_NonLight"), TEXT("Target_Blur"))))
		return E_FAIL;


	/* For.MRT_BlurDownSample*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurDownSample"), TEXT("Target_BlurDownSample"))))
		return E_FAIL;
	/* For.MRT_BlurHor*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurHor"), TEXT("Target_BlurHor"))))
		return E_FAIL;
	/* For.MRT_BlurVer*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurVer"), TEXT("Target_BlurVer"))))
		return E_FAIL;
	/* For.MRT_BlurUpSample*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlurUpSample"), TEXT("Target_BlurUpSample"))))
		return E_FAIL;
	/* For.MRT_Grow*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Grow"), TEXT("Target_Grow"))))
		return E_FAIL;




	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f)));

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = ViewportDesc.Width;
	m_WorldMatrix._22 = ViewportDesc.Height;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));


#ifdef _DEBUG

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 100.f, 250.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 100.f, 400.f, 150.f, 150.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 250, 100.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 250, 250.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Reflection"), 250, 400.f, 150.f, 150.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth"), 400, 100.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Blur"), 400, 250.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Blend"), 400, 400.f, 150.f, 150.f)))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BlurDownSample"), 100.f, 650.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BlurHor"), 250, 650.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BlurVer"), 400, 650.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BlurUpSample"), 550.f, 650.f, 150.f, 150.f)))
		return E_FAIL;
#endif


	//for Noise Texture
	m_pNoiseTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Noise/distortion_ending_00.png"));


	return S_OK;
}

HRESULT CRenderer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Render_GameObjects()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

	// 광원 깊이를 그린다.
	if (FAILED(Render_ShadowDepth()))
		return E_FAIL;

	if (FAILED(Render_NonAlphaBlend()))
		return E_FAIL;

	if (FAILED(Render_Lights()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;
	if (FAILED(Render_AlphaBlend()))
		return E_FAIL;

	// 블러
	if (FAILED(Render_BlurDownSample()))
		return E_FAIL;
	if (FAILED(Render_BlurHorizontal()))
		return E_FAIL;
	if (FAILED(Render_BlurVertical()))
		return E_FAIL;
	if (FAILED(Render_BlurUpSample()))
		return E_FAIL;
	if (FAILED(Render_Grow()))
		return E_FAIL;

#ifdef _DEBUG	
	Render_Debug();
#endif // _DEBUG

	if (FAILED(Render_UI()))
		return E_FAIL;


	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Add_Debug(CComponent* pDebugCom)
{
	m_DebugComponents.push_back(pDebugCom);

	Safe_AddRef(pDebugCom);

	return S_OK;
}

void CRenderer::Debug_Clear()
{
	for (auto& pComponent : m_DebugComponents)
	{

		Safe_Release(pComponent);
	}

	m_DebugComponents.clear();
}


#endif // _DEBUG


HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_GameObjects[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonAlphaBlend()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* 현재까지는 백버퍼가 셋팅되어있었지만.
	빛연산ㅇ에 필요한 정보를 받아오기위해 MRT_Deferred타겟들을 바인딩한다. */
	/* Target_Diffuse, Target_Normal에 그린다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Deferred"))))
		return E_FAIL;

	for (auto& pGameObject : m_GameObjects[RENDER_NONALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_NONALPHABLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_ShadowDepth()
{
	if (FAILED(m_pTarget_Manager->Begin_ShadowMRT(m_pContext, TEXT("MRT_LightDepth"))))
		return E_FAIL; 
		
	for (auto& pGameObject : m_GameObjects[RENDER_SHADOWDEPTH])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_ShadowDepth();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_SHADOWDEPTH].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}
HRESULT CRenderer::Render_BlurDownSample()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Target_Shader타겟에 빛 연산한 결과를 그린다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BlurDownSample"))))
		return E_FAIL;


	_float4x4			WorldMatrix;

	_uint				iNumViewport = 1;
	D3D11_VIEWPORT		ViewportDesc;

	m_pContext->RSGetViewports(&iNumViewport, &ViewportDesc);

	XMStoreFloat4x4(&WorldMatrix,
		XMMatrixTranspose(XMMatrixScaling(ViewportDesc.Width * 0.5f, ViewportDesc.Height * 0.5f, 0.f) * XMMatrixTranslation(0.0f, 0.0f, 0.f)));

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Blend"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;


	m_pShader->Begin(0);


	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BlurHorizontal()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Target_Shader타겟에 빛 연산한 결과를 그린다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BlurHor"))))
		return E_FAIL;


	_float4x4			WorldMatrix;

	_uint				iNumViewport = 1;
	D3D11_VIEWPORT		ViewportDesc;

	m_pContext->RSGetViewports(&iNumViewport, &ViewportDesc);

	XMStoreFloat4x4(&WorldMatrix,
		XMMatrixTranspose(XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 0.f) * XMMatrixTranslation(0.0f, 0.0f, 0.f)));

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_BlurDownSample"), m_pShader, "g_BlurTexture")))
		return E_FAIL;


	m_pShader->Begin(0);


	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BlurVertical()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Target_Shader타겟에 빛 연산한 결과를 그린다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BlurVer"))))
		return E_FAIL;


	_float4x4			WorldMatrix;

	_uint				iNumViewport = 1;
	D3D11_VIEWPORT		ViewportDesc;

	m_pContext->RSGetViewports(&iNumViewport, &ViewportDesc);

	XMStoreFloat4x4(&WorldMatrix,
		XMMatrixTranspose(XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 0.f) * XMMatrixTranslation(0.0f, 0.0f, 0.f)));

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_BlurHor"), m_pShader, "g_BlurTexture")))
		return E_FAIL;


	m_pShader->Begin(0);


	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BlurUpSample()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	/* Target_Shader타겟에 빛 연산한 결과를 그린다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BlurUpSample"))))
		return E_FAIL;


	_float4x4			WorldMatrix;

	_uint				iNumViewport = 1;
	D3D11_VIEWPORT		ViewportDesc;

	m_pContext->RSGetViewports(&iNumViewport, &ViewportDesc);

	XMStoreFloat4x4(&WorldMatrix,
		XMMatrixTranspose(XMMatrixScaling(ViewportDesc.Width * 2.f, ViewportDesc.Height * 2.f, 0.f) * XMMatrixTranslation(0.0f, 0.0f, 0.f)));

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_BlurVer"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;


	m_pShader->Begin(0);


	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Grow()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;


	_float4x4			WorldMatrix;

	_uint				iNumViewport = 1;
	D3D11_VIEWPORT		ViewportDesc;

	m_pContext->RSGetViewports(&iNumViewport, &ViewportDesc);

	XMStoreFloat4x4(&WorldMatrix,
		XMMatrixTranspose(XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 0.f) * XMMatrixTranslation(0.0f, 0.0f, 0.f)));

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Blend"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_BlurUpSample"), m_pShader, "g_BlurTexture")))
		return E_FAIL;
	

	m_pShader->Begin(4);


	m_pVIBuffer->Render();

	return S_OK;
}


HRESULT CRenderer::Render_AlphaBlend()
{

	m_GameObjects[RENDER_ALPHABLEND].sort([](CGameObject* pSour, CGameObject* pDest)
	{
		return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	});

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_NonLight"), false)))
		return E_FAIL;

	for (auto& pGameObject : m_GameObjects[RENDER_ALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_ALPHABLEND].clear();


	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_GameObjects[RENDER_UI_BACK])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_UI_BACK].clear();

	for (auto& pGameObject : m_GameObjects[RENDER_UI_FRONT])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_UI_FRONT].clear();

	for (auto& pGameObject : m_GameObjects[RENDER_UI_TOP])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_UI_TOP].clear();
	return S_OK;
}

HRESULT CRenderer::Render_Lights()
{
	if (nullptr == m_pTarget_Manager ||
		nullptr == m_pLight_Manager)
		return E_FAIL;

	/* Target_Shader에 그린다. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_LightAcc"))))
		return E_FAIL;

	/* 노말 렌더타겟의 SRV를 셰이더에 바인딩 한다. */
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	CPipeLine*				pPipeLine = GET_INSTANCE(CPipeLine);

	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CPipeLine);

	if (FAILED(m_pLight_Manager->Render_Lights(m_pShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	/* 백버퍼에 그린다. */
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"))))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Reflection"), m_pShader, "g_ReflectionTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_ShadowDepth"), m_pShader, "g_ShadowDepthTexture")))
		return E_FAIL;

	CPipeLine*				pPipeLine = GET_INSTANCE(CPipeLine);


	_float4x4	LightView = CLight_Manager::Get_Instance()->Get_ShadowLightView();

	if (FAILED(m_pShader->Set_RawValue("g_LightViewMatrix", &LightView, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_LightProjMatrix", &CPipeLine::Get_Instance()->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrixInv", &CPipeLine::Get_Instance()->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrixInv", &CPipeLine::Get_Instance()->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CPipeLine);

	m_pShader->Begin(3);

	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_NonLight"), false)))
		return E_FAIL;


	for (auto& pGameObject : m_GameObjects[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_NONLIGHT].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Render_Debug()
{
	if (CLevel_Manager::Get_Instance()->Get_CurrentLevelIndex() == 1)
	{
		Debug_Clear();
		return E_FAIL;
	}


	if (nullptr == m_pShader ||
		nullptr == m_pVIBuffer)
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	for (auto& pComponent : m_DebugComponents)
	{
		if (m_bRenderComponentDebug)
		{
			if (nullptr != pComponent)
				pComponent->Render();
		}

		Safe_Release(pComponent);
	}

	m_DebugComponents.clear();


	if (CGameInstance::Get_Instance()->Key_Up(DIK_TAB))
		m_bRenderDebug = !m_bRenderDebug;
	if (CGameInstance::Get_Instance()->Key_Up(DIK_CAPSLOCK))
		m_bRenderComponentDebug = !m_bRenderComponentDebug;

	if (m_bRenderDebug)
	{
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_LightDepth"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Blend"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_NonLight"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Blend"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_BlurDownSample"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_BlurHor"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_BlurVer"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_BlurUpSample"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
	}

	return S_OK;

	
}

#endif

CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);
	Safe_Release(m_pNoiseTexture);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);

}
