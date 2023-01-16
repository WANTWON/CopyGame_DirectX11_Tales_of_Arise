#include "stdafx.h"
#include "..\Public\UI_Loading.h"

#include "GameInstance.h"

CUI_Loading::CUI_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Loading::CUI_Loading(const CUI_Loading & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Loading::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Loading::Initialize(void * pArg)
{
	m_fSize.x = g_iWinSizeX;
	m_fSize.y = g_iWinSizeY;
	m_fPosition.x = g_iWinSizeX >> 1;
	m_fPosition.y = g_iWinSizeY >> 1;
	m_fAlpha = 0.f;

	m_eShaderID = UI_EFFECTFADEOUT;

	m_bisinLoading = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Loading::Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() != LEVEL_LOADING)
		m_bDead = true;

	m_ftimer += fTimeDelta;

	if (m_ftimer > 0.048)
	{
		++m_itexnum;
		m_ftimer = 0.f;
	}


	

	if (m_bDead)
	{
		m_falphatimer += fTimeDelta;

		if (m_falphatimer > 0.1f)
		{
			m_fAlpha += 0.1f;
			m_falphatimer = 0.f;

		}
			

	}


	if (m_fAlpha >= 1.f)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CUI_Loading::Late_Tick(_float fTimeDelta)
{

	if (m_itexnum == 36)
		m_itexnum = 17;


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	//__super::Late_Tick(fTimeDelta);

}

HRESULT CUI_Loading::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CUI_Loading::Ready_Components(void * pArg)
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loadingsprite"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
		return E_FAIL;

	

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CUI_Loading * CUI_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Loading*	pInstance = new CUI_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_Loading::Clone(void * pArg)
{
	CUI_Loading*	pInstance = new CUI_Loading(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading::Free()
{
	__super::Free();
}
