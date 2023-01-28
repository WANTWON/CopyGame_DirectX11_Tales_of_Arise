#include "stdafx.h"
#include "..\Public\UI_font_Hits_number.h"

#include "GameInstance.h"

CUI_font_Hits_number::CUI_font_Hits_number(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_font_Hits_number::CUI_font_Hits_number(const CUI_font_Hits_number & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_font_Hits_number::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_font_Hits_number::Initialize(void * pArg)
{
	if (pArg != nullptr)
	m_iIndex = *(_uint*)pArg;

	m_eShaderID = 0;
	m_fSize.x = 50.f;
	m_fSize.y = 65.f;
	m_fPosition.x = 1050.f;
	m_fPosition.y = 120.f;

	m_fAlpha = 1;


	/*if (m_iYIndex == 0)
	{

	}*/
	/*if (m_iYIndex == 1)
	{
	m_fPosition.x = 1180.f + (m_iIndex * 14);
	m_fPosition.y = 415;
	}
	if (m_iYIndex == 2)
	{
	m_fPosition.x = 1180.f + (m_iIndex * 14);
	m_fPosition.y = 475;
	}
	if (m_iYIndex == 3)
	{
	m_fPosition.x = 1180.f + (m_iIndex * 14);
	m_fPosition.y = 535;
	}
	*/
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_font_Hits_number::Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_3))
	{
		m_iCurrenthit++; 
		m_fSize.x = 75.f;
		m_fSize.y = 80.f;
		m_bsizedown = true;
		/*m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);*/
		//	m_bfadein = true;
		//	m_fAlpha = 0;
	}
	for (_uint i = 0; i < 3; ++i)
		m_fbrightpos_hitfont[i] += 0.015f;

	for (_uint i = 0; i < 3; ++i)
	{
		if (m_fbrightpos_hitfont[i] >= 3.f)
			m_fbrightpos_hitfont[i] = 0.f;
	}

	if (m_bsizedown)
		sizedown();


	
	if (m_fSize.x <= 60.f || m_fSize.y <= 65.f)
		m_bsizedown = false;
	/*m_fSize.x = 28.f;
	m_fSize.y = 32.f;*/
	m_fPosition.x = 1060.f;
	//m_fPosition.y = 120.f;
	
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));


	if (m_iCurrenthit == 0)
		m_itexnum = 0;


	return OBJ_NOEVENT;
}

void CUI_font_Hits_number::Late_Tick(_float fTimeDelta)
{

		
	if (!m_bRender)
		return;


		/*	switch (m_iIndex)
			{
			case 0:


			case 1:

				break;

			case 2:


			}*/

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CUI_font_Hits_number::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_itexnum = m_iCurrenthit / 100;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	if (m_iCurrenthit >= 100)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hitfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}

	if (m_iCurrenthit >= 10)
	{
		m_itexnum = ((m_iCurrenthit % 100) / 10);

		m_fPosition.x = 1100.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hitfont[1], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	

	if (m_iCurrenthit >= 2)
	{
		m_itexnum = m_iCurrenthit % 10;

		m_fPosition.x = 1140.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hitfont[2], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();


	}

	return S_OK;
}

HRESULT CUI_font_Hits_number::Ready_Components(void * pArg)
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_font_Hits_number::SetUp_ShaderResources()
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

CUI_font_Hits_number * CUI_font_Hits_number::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_font_Hits_number*	pInstance = new CUI_font_Hits_number(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_font_Hits_number"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_font_Hits_number::Clone(void * pArg)
{
	CUI_font_Hits_number*	pInstance = new CUI_font_Hits_number(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_font_Hits_number"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_font_Hits_number::Free()
{
	Safe_Release(m_pTextureCom1);
	__super::Free();
}
