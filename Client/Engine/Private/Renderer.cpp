#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "Component.h"
#include "PipeLine.h"
#include "GameInstance.h"
#include "Texture.h"

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
	if (!m_pTarget_Manager)
		return E_FAIL;

	D3D11_VIEWPORT ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof ViewportDesc);

	_uint iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	/** 
		Render Target Views
	**/
	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 1.f))))
		return E_FAIL;
	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	/* For.Target_ShadowDepth */
	_uint iShadowMapCX = 1280.f * 12.5f;
	_uint iShadowMapCY = 720.f * 12.5f;

	if (FAILED(m_pTarget_Manager->Ready_ShadowDepthStencilRenderTargetView(m_pDevice, iShadowMapCX, iShadowMapCY)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth"), iShadowMapCX, iShadowMapCY,
		DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(1.0f, 1.0f, 1.0f, 1.0f))))
		return E_FAIL;

	/* For.Target_ShadowDepth_Blur */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth_Blur"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R32G32B32A32_FLOAT, &_float4(1.0f, 1.0f, 1.0f, 1.0f))))
		return E_FAIL;

	/* Copy Back Buffer */
	if (FAILED(m_pTarget_Manager->Ready_BackBufferCopyTexture(m_pDevice, m_pContext, ViewportDesc.Width, ViewportDesc.Height)))
		return E_FAIL;

	/* For.Target_Glow */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Glow"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;
	/* For.Target_Distortion */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Distortion"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	/* For.Target_Glow_UI */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Glow_UI"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	/* For.Target_Blur_Horizontal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_Horizontal"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;
	/* For.Target_Blur_Vertical */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_Vertical"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	/* For.Target_Fog */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Fog"), ViewportDesc.Width, ViewportDesc.Height,
		DXGI_FORMAT_R8G8B8A8_UNORM, &_float4(0.0f, 0.0f, 0.0f, 0.0f))))
		return E_FAIL;

	/**
		Multiple Render Targets
	**/
	/* For.MRT_Deferred */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;

	/* For.MRT_LightAcc */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	/* For.MRT_LightDepth */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth"), TEXT("Target_ShadowDepth"))))
		return E_FAIL;

	/* For.MRT_LightDepth_Blur */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightDepth_Blur"), TEXT("Target_ShadowDepth_Blur"))))
		return E_FAIL;

	/* For.MRT_Blur_Horizontal */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_Horizontal"), TEXT("Target_Blur_Horizontal"))))
		return E_FAIL;
	/* For.MRT_Blur_Vertical */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_Vertical"), TEXT("Target_Blur_Vertical"))))
		return E_FAIL;

	/* For.MRT_Glow */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Glow"), TEXT("Target_Glow"))))
		return E_FAIL;
	/* For.MRT_Distortion */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		return E_FAIL;

	/* For.MRT_Glow_UI */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Glow_UI"), TEXT("Target_Glow_UI"))))
		return E_FAIL;

	/* For.MRT_Fog */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Fog"), TEXT("Target_Fog"))))
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (!m_pVIBuffer)
		return E_FAIL;

	m_pShaderDeferred = CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (!m_pShaderDeferred)
		return E_FAIL;

	m_pShaderPostProcessing = CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_PostProcessing.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (!m_pShaderPostProcessing)
		return E_FAIL;	

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f)));

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = ViewportDesc.Width;
	m_WorldMatrix._22 = ViewportDesc.Height;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_WorldMatrix)));

#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 75.f, 75.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 75.f, 225.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 75.f, 375.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 225.f, 75.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), 225.f, 225.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth"), 225.f, 375.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth_Blur"), 225.f, 525.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Glow"), 375.f, 75.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Distortion"), 375.f, 225.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Fog"), 375.f, 375.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Glow_UI"), 525.f, 75.f, 150.f, 150.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Blur_Horizontal"), 1205.f, 75.f, 150.f, 150.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Blur_Vertical"), 1205.f, 225.f, 150.f, 150.f)))
		return E_FAIL;
#endif

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
	if (FAILED(Render_ShadowDepth()))
		return E_FAIL;

	/* Deferred Renders */
	if (FAILED(Render_NonAlphaBlend()))
		return E_FAIL;
	if (FAILED(Render_AlphaBlendLights()))
		return E_FAIL;
	if (FAILED(Render_Lights()))
		return E_FAIL;

	/* Deferred Blend */
	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;
	if (FAILED(Render_AlphaBlend()))
		return E_FAIL;
	if (FAILED(Render_Glow()))
		return E_FAIL;
	if (FAILED(Render_Distortion()))
		return E_FAIL;

	/* Post Processing */
	if (FAILED(Render_PostProcessing()))
		return E_FAIL;

#ifdef _DEBUG	
	Render_Debug();
#endif // _DEBUG

	if (FAILED(Render_UI()))
		return E_FAIL;

	/* UI Post Processing Renders */
	if (FAILED(Render_UI_Glow()))
		return E_FAIL;

	/* UI Post Processing Blend */
	if (FAILED(Render_UI_PostProcessing()))
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

HRESULT CRenderer::Render_ShadowDepth()
{
	if (m_GameObjects[RENDER_SHADOWDEPTH].empty())
		return S_OK;

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

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	m_GameObjects[RENDER_SHADOWDEPTH].clear();

	//if (FAILED(m_pTarget_Manager->Begin_ShadowMRT(m_pContext, TEXT("MRT_LightDepth_Blur"))))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_ShadowDepth"), m_pShaderPostProcessing, "g_BlurTexture")))
	//	return E_FAIL;

	//m_pShaderPostProcessing->Begin(8); /* Horizontal Blur Shadow */
	//m_pVIBuffer->Render();

	//if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_ShadowDepth_Blur"), m_pShaderPostProcessing, "g_BlurTexture")))
	//	return E_FAIL;

	//m_pShaderPostProcessing->Begin(9); /* Vertical Blur Shadow */
	//m_pVIBuffer->Render();

	//if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
	//	return E_FAIL;

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

	return S_OK;
}

HRESULT CRenderer::Render_AlphaBlendLights()
{
	m_GameObjects[RENDER_ALPHABLENDLIGHTS].sort([](CGameObject* pSour, CGameObject* pDest)
	{
		return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	});

	for (auto& pGameObject : m_GameObjects[RENDER_ALPHABLENDLIGHTS])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_ALPHABLENDLIGHTS].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

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

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Normal"), m_pShaderDeferred, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShaderDeferred, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Specular"), m_pShaderDeferred, "g_SpecularTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderDeferred->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ProjMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ViewMatrixInv", &pPipeLine->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_vCamPosition", &pPipeLine->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CPipeLine);

	if (FAILED(m_pLight_Manager->Render_Lights(m_pShaderDeferred, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (!m_pTarget_Manager)
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Diffuse"), m_pShaderDeferred, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShaderDeferred, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Shade"), m_pShaderDeferred, "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Specular"), m_pShaderDeferred, "g_SpecularTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_ShadowDepth"), m_pShaderDeferred, "g_ShadowDepthTexture")))
		return E_FAIL;
	/*if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_ShadowDepth_Blur"), m_pShaderDeferred, "g_ShadowDepthTexture")))
		return E_FAIL;*/

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	_float4x4 LightView = CLight_Manager::Get_Instance()->Get_ShadowLightView();

	if (FAILED(m_pShaderDeferred->Set_RawValue("g_LightViewMatrix", &LightView, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_LightProjMatrix", &CPipeLine::Get_Instance()->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ViewMatrixInv", &CPipeLine::Get_Instance()->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ProjMatrixInv", &CPipeLine::Get_Instance()->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderDeferred->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CPipeLine);

	m_pShaderDeferred->Begin(3);
	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_GameObjects[RENDER_NONLIGHT])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_AlphaBlend()
{
	m_GameObjects[RENDER_ALPHABLEND].sort([](CGameObject* pSour, CGameObject* pDest)
	{
		return pSour->Get_CamDistance() > pDest->Get_CamDistance();
	});

	for (auto& pGameObject : m_GameObjects[RENDER_ALPHABLEND])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render();
			Safe_Release(pGameObject);
		}
	}

	m_GameObjects[RENDER_ALPHABLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Glow()
{
	/* If there are not Glow Object return. */
	if (m_GameObjects[RENDER_GLOW].empty())
		return S_OK;
	else
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Glow"))))
			return E_FAIL;

		m_GameObjects[RENDER_GLOW].sort([](CGameObject* pSour, CGameObject* pDest)
		{
			return pSour->Get_CamDistance() > pDest->Get_CamDistance();
		});

		for (auto& pGameObject : m_GameObjects[RENDER_GLOW])
		{
			if (pGameObject)
			{
				pGameObject->Render_Glow();
				Safe_Release(pGameObject);
			}
		}

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;
	}

	/* Glow */
	if (!m_GameObjects[RENDER_GLOW].empty())
	{
		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
			return E_FAIL;

		m_pTarget_Manager->Copy_BackBufferTexture(m_pDevice, m_pContext);
		if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_BackBufferTexture", m_pTarget_Manager->Get_BackBufferCopySRV())))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Glow"), m_pShaderPostProcessing, "g_BlurTexture")))
			return E_FAIL;

		/* Horizontal Blur */
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blur_Horizontal"))))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(1);
		m_pVIBuffer->Render();

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Blur_Horizontal"), m_pShaderPostProcessing, "g_BlurTexture")))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blur_Vertical"))))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(2); /* Vertical Blur */
		m_pVIBuffer->Render();

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		/* Bind the Horizontally and Vertically Blurred Image to the Glow Texture. */
		if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Blur_Vertical"), m_pShaderPostProcessing, "g_BlendTexture")))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(0);
		m_pVIBuffer->Render();

		m_GameObjects[RENDER_GLOW].clear();

		return S_OK;
	}
}

HRESULT CRenderer::Render_Distortion()
{
	/* If there are not Distortion Object return. */
	if (m_GameObjects[RENDER_DISTORTION].empty())
		return S_OK;
	else
	{
		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
			return E_FAIL;

		m_fDistortionTimer += CTimer_Manager::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_fDistortionTimer", &m_fDistortionTimer, sizeof(_float))))
			return E_FAIL;

		for (auto& pGameObject : m_GameObjects[RENDER_DISTORTION])
		{
			if (pGameObject)
			{
				if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Distortion"))))
					return E_FAIL;

				pGameObject->Render_Distort();
				Safe_Release(pGameObject);

				if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
					return E_FAIL;

				m_pTarget_Manager->Copy_BackBufferTexture(m_pDevice, m_pContext);
				if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_BackBufferTexture", m_pTarget_Manager->Get_BackBufferCopySRV())))
					return E_FAIL;

				if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Distortion"), m_pShaderPostProcessing, "g_DistortionTexture")))
					return E_FAIL;

				if (!m_pDistortionNoiseTexture)
					m_pDistortionNoiseTexture = (CTexture*)CComponent_Manager::Get_Instance()->Clone_Component(0, TEXT("Distortion_Noise"));

				if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_DistortionNoiseTexture", m_pDistortionNoiseTexture->Get_SRV())))
					return E_FAIL;

				m_pShaderPostProcessing->Begin(6);
				m_pVIBuffer->Render();
			}
		}
	}

	m_GameObjects[RENDER_DISTORTION].clear();
		
	return S_OK;
}

HRESULT CRenderer::Render_PostProcessing()
{
	if (!m_pTarget_Manager)
		return E_FAIL;

	/* Depth of Field */
	m_pTarget_Manager->Copy_BackBufferTexture(m_pDevice, m_pContext);
	if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_BackBufferTexture", m_pTarget_Manager->Get_BackBufferCopySRV())))
		return E_FAIL;
	if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_BlurTexture", m_pTarget_Manager->Get_BackBufferCopySRV())))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blur_Horizontal"))))
		return E_FAIL;

	m_pShaderPostProcessing->Begin(1); /* Horizontal Blur */
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Blur_Horizontal"), m_pShaderPostProcessing, "g_BlurTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blur_Vertical"))))
		return E_FAIL;

	m_pShaderPostProcessing->Begin(2); /* Vertical Blur */
	m_pVIBuffer->Render();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Blur_Vertical"), m_pShaderPostProcessing, "g_BlurredBackBufferTexture")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShaderPostProcessing, "g_DepthTexture")))
		return E_FAIL;

	m_pShaderPostProcessing->Begin(7); /* Final Pass */
	m_pVIBuffer->Render();

	/* Fog */
	if (m_bFog)
	{
		if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Depth"), m_pShaderPostProcessing, "g_DepthTexture")))
			return E_FAIL;

		/* For.MRT_Fog */
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Fog"))))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(3);
		m_pVIBuffer->Render();

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		m_pTarget_Manager->Copy_BackBufferTexture(m_pDevice, m_pContext);
		if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_BackBufferTexture", m_pTarget_Manager->Get_BackBufferCopySRV())))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Fog"), m_pShaderPostProcessing, "g_FogTexture")))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(4);
		m_pVIBuffer->Render();
	}

	/* Distortion */
	/* This Distortion is a Post Processing Effect applied to the entire Screen. 
	Unlike the Distortion Effect computed in the Render_Distortion() function which renders just Distorted Objects. */
	if (m_bDistort)
	{
		m_pTarget_Manager->Copy_BackBufferTexture(m_pDevice, m_pContext);
		if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_BackBufferTexture", m_pTarget_Manager->Get_BackBufferCopySRV())))
			return E_FAIL;

		if (!m_pScreenDistortionTexture)
			m_pScreenDistortionTexture = (CTexture*)CComponent_Manager::Get_Instance()->Clone_Component(0, TEXT("Screen_Distortion"));

		if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_ScreenDistortionTexture", m_pScreenDistortionTexture->Get_SRV())))
			return E_FAIL;

		m_fScreenDistortionTimer += CTimer_Manager::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_fScreenDistortionTimer", &m_fScreenDistortionTimer, sizeof(_float))))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(5);
		m_pVIBuffer->Render();
	}

	/* Zoom Blur */
	if (m_bZoomBlur)
	{
		m_pTarget_Manager->Copy_BackBufferTexture(m_pDevice, m_pContext);
		if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_BlurTexture", m_pTarget_Manager->Get_BackBufferCopySRV())))
			return E_FAIL;

		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_fFocusPower", &m_fFocusPower, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderPostProcessing->Set_RawValue("g_iFocusDetail", &m_iFocusDetail, sizeof(_int))))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(10); /* Zoom Blur */
		m_pVIBuffer->Render();
	}

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == 1)
	{
		Debug_Clear();
		return E_FAIL;
	}

	if (!m_pShaderDeferred || !m_pVIBuffer)
		return E_FAIL;

	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderDeferred->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
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

	if (CGameInstance::Get_Instance()->Key_Up(DIK_BACKSPACE))
		m_bRenderDebug = !m_bRenderDebug;
	if (CGameInstance::Get_Instance()->Key_Up(DIK_EQUALS))
		m_bRenderComponentDebug = !m_bRenderComponentDebug;

	if (m_bRenderDebug)
	{
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_LightDepth"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_LightDepth_Blur"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Glow"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Distortion"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Fog"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Glow_UI"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Blur_Horizontal"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Render_Debug(TEXT("MRT_Blur_Vertical"), m_pShaderDeferred, m_pVIBuffer)))
			return E_FAIL;
	}

	return S_OK;
}
#endif // _DEBUG

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
	return S_OK;
}

HRESULT CRenderer::Render_UI_Glow()
{
	/* If there are not Distortion Object return. */
	if (m_GameObjects[RENDER_UI_GLOW].empty())
		return S_OK;
	else
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Glow_UI"))))
			return E_FAIL;

		for (auto& pGameObject : m_GameObjects[RENDER_UI_GLOW])
		{
			if (pGameObject)
			{
				pGameObject->Render_Glow();
				Safe_Release(pGameObject);
			}
		}

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		return S_OK;
	}
}

HRESULT CRenderer::Render_UI_PostProcessing()
{
	if (!m_GameObjects[RENDER_UI_GLOW].empty())
	{
		m_pTarget_Manager->Copy_BackBufferTexture(m_pDevice, m_pContext);
		if (FAILED(m_pShaderPostProcessing->Set_ShaderResourceView("g_BackBufferTexture", m_pTarget_Manager->Get_BackBufferCopySRV())))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Glow_UI"), m_pShaderPostProcessing, "g_BlurTexture")))
			return E_FAIL;

		/* Horizontal Blur */
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blur_Horizontal"))))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(1);
		m_pVIBuffer->Render();

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		/* Bind the Horizontally Blurred Image to the Glow Texture. */
		if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Blur_Horizontal"), m_pShaderPostProcessing, "g_BlurTexture")))
			return E_FAIL;

		/* Vertical Blur */
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blur_Vertical"))))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(2);
		m_pVIBuffer->Render();

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		/* Bind the Horizontally and Vertically Blurred Image to the Glow Texture. */
		if (FAILED(m_pTarget_Manager->Bind_ShaderResource(TEXT("Target_Blur_Vertical"), m_pShaderPostProcessing, "g_BlendTexture")))
			return E_FAIL;

		m_pShaderPostProcessing->Begin(0);
		m_pVIBuffer->Render();

		m_GameObjects[RENDER_UI_GLOW].clear();
	}
}

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

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShaderDeferred);
	Safe_Release(m_pShaderPostProcessing);

	Safe_Release(m_pDistortionTexture);
	Safe_Release(m_pDistortionNoiseTexture);
	Safe_Release(m_pScreenDistortionTexture);
}