#include "stdafx.h"

#include "ScreenDistortion.h"
#include "GameInstance.h"

CScreenDistortion::CScreenDistortion(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CScreenDistortion::CScreenDistortion(const CScreenDistortion & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CScreenDistortion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScreenDistortion::Initialize(void * pArg)
{
	m_fSize.x = g_iWinSizeX;
	m_fSize.y = g_iWinSizeY;
	m_fPosition.x = g_iWinSizeX >> 1;
	m_fPosition.y = g_iWinSizeY >> 1;

	m_bisinLoading = true;

	Copy_BackBuffer();
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CScreenDistortion::Tick(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CScreenDistortion::Late_Tick(_float fTimeDelta)
{
	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_FRONT, this);
}

HRESULT CScreenDistortion::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CScreenDistortion::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScreenDistortion::SetUp_ShaderResources()
{
	if (!m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pBackBufferSRV)))
		return E_FAIL;

	return S_OK;
}

void CScreenDistortion::Copy_BackBuffer()
{
	if (!m_pDevice)
		return;

	/* Get Current Render Target (BackBuffer). */
	ID3D11RenderTargetView* pBackBufferRTV = nullptr;
	m_pContext->OMGetRenderTargets(1, &pBackBufferRTV, nullptr);

	/* Get BackBuffer Render Target Description. */
	D3D11_RENDER_TARGET_VIEW_DESC pBackBufferDesc;
	pBackBufferRTV->GetDesc(&pBackBufferDesc);

	/* Create the Texture where the copy of the BackBuffer will be copied to. */
	D3D11_TEXTURE2D_DESC tBackBufferCopyDesc;
	ZeroMemory(&tBackBufferCopyDesc, sizeof(D3D11_TEXTURE2D_DESC));

	tBackBufferCopyDesc.Width = g_iWinSizeX;
	tBackBufferCopyDesc.Height = g_iWinSizeY;
	tBackBufferCopyDesc.MipLevels = 1;
	tBackBufferCopyDesc.ArraySize = 1;
	tBackBufferCopyDesc.Format = pBackBufferDesc.Format;
	tBackBufferCopyDesc.SampleDesc.Quality = 0;
	tBackBufferCopyDesc.SampleDesc.Count = 1;
	tBackBufferCopyDesc.Usage = D3D11_USAGE_DEFAULT;
	tBackBufferCopyDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tBackBufferCopyDesc.CPUAccessFlags = 0;
	tBackBufferCopyDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&tBackBufferCopyDesc, nullptr, &m_pBackBufferTextureCopy)))
		return;

	Safe_Release(pBackBufferRTV);

	/* Get Current Render Target (BackBuffer). */
	pBackBufferRTV = nullptr;
	m_pContext->OMGetRenderTargets(1, &pBackBufferRTV, nullptr);

	/* Get BackBuffer RenderTarget Texture. */
	ID3D11Resource* pBackBufferResource = nullptr;
	pBackBufferRTV->GetResource(&pBackBufferResource);

	/* Copy the BackBuffer Texture into "m_pBackBufferTextureCopy". */
	m_pContext->CopyResource(m_pBackBufferTextureCopy, pBackBufferResource);

	Safe_Release(m_pBackBufferSRV);

	/* Make a Shader Resource View based on the copied "m_pBackBufferTextureCopy". */
	if (FAILED(m_pDevice->CreateShaderResourceView(m_pBackBufferTextureCopy, nullptr, &m_pBackBufferSRV)))
		return;

	Safe_Release(pBackBufferResource);
	Safe_Release(pBackBufferRTV);
}

CScreenDistortion * CScreenDistortion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CScreenDistortion* pInstance = new CScreenDistortion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CScreenDistortion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CScreenDistortion::Clone(void * pArg)
{
	CScreenDistortion* pInstance = new CScreenDistortion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CScreenDistortion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScreenDistortion::Free()
{
	__super::Free();
}