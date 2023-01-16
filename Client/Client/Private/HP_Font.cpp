#include "stdafx.h"
#include "..\Public\HP_Font.h"

#include "GameInstance.h"

CHP_Font::CHP_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CHP_Font::CHP_Font(const CHP_Font & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CHP_Font::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHP_Font::Initialize(void * pArg)
{
	if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;

	m_fSize.x = 16.f;
	m_fSize.y = 20.f;

	if (m_iYIndex == 0)
	{
		m_fPosition.x = 1180.f + (m_iIndex * 14);
		m_fPosition.y = 375;
	}
	if (m_iYIndex == 1)
	{
		m_fPosition.x = 1180.f + (m_iIndex * 14);
		m_fPosition.y = 435;
	}
	if (m_iYIndex == 2)
	{
		m_fPosition.x = 1180.f + (m_iIndex * 14);
		m_fPosition.y = 495;
	}
	if (m_iYIndex == 3)
	{
		m_fPosition.x = 1180.f + (m_iIndex * 14);
		m_fPosition.y = 555;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CHP_Font::Tick(_float fTimeDelta)
{
	if (m_iIndex == 0)
	{
		m_itexnum = m_iCurrenthp / 1000;
	}
	else if (m_iIndex == 1)
	{
		m_itexnum = ((m_iCurrenthp % 1000) / 100);
	}
	else if (m_iIndex == 2)
	{
		m_itexnum = ((m_iCurrenthp % 100) / 10);
	}
	else if (m_iIndex == 3)
	{
		m_itexnum = m_iCurrenthp % 10;
	}

	if (m_iCurrenthp == 0)
		m_itexnum = 0;
	return OBJ_NOEVENT;
}

void CHP_Font::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CHP_Font::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CHP_Font::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHP_Font::SetUp_ShaderResources()
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

CHP_Font * CHP_Font::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHP_Font*	pInstance = new CHP_Font(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHP_Font"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CHP_Font::Clone(void * pArg)
{
	CHP_Font*	pInstance = new CHP_Font(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHP_Font"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP_Font::Free()
{
	__super::Free();
}
