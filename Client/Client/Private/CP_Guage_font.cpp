#include "stdafx.h"
#include "..\Public\CP_Guage_font.h"

#include "GameInstance.h"

CCP_Guage_font::CCP_Guage_font(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CCP_Guage_font::CCP_Guage_font(const CCP_Guage_font & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CCP_Guage_font::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCP_Guage_font::Initialize(void * pArg)
{
	if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;

	m_eShaderID = 0;
	m_fSize.x = 14.f;
	m_fSize.y = 19.f;
	m_fPosition.x = 930.f + (m_iIndex * 15);
	m_fPosition.y = 135;

	if (m_iIndex < 3)
	{
		m_fSize.x = 20.f;
		m_fSize.y = 26.f;
		m_fPosition.x = 930.f + (m_iIndex * 16);
		m_fPosition.y = 135;
	}

	

	m_fAlpha = 0;


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

int CCP_Guage_font::Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Up(DIK_3))
	{
		m_iCurrentcp++;
		/*m_fSize.x = 55.f;
		m_fSize.y = 75.f;
		m_bsizedown = true;*/
		/*m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);*/
		//	m_bfadein = true;
		//	m_fAlpha = 1;
	}


	//if (m_bsizedown)
	//	sizedown();


	switch (m_iIndex)
	{
	case 0:
		if (m_iCurrentcp < 100)
		{
			m_bRender = false;
			return OBJ_NOEVENT;
		}
		else
		{
		
			m_itexnum = m_iCurrentcp / 100;
			m_bRender = true;
			break;

		}

	case 1:
		if (m_iCurrentcp < 10)
		{
			m_bRender = false;
			return OBJ_NOEVENT;
		}
		else
		{
		
			m_itexnum = ((m_iCurrentcp % 100) / 10);
			m_bRender = true;
			break;
		}
	case 2:
		if (m_iCurrentcp <= 0)
		{
			m_bRender = false;
			return OBJ_NOEVENT;
		}
		else
		{
		
			m_itexnum = m_iCurrentcp % 10;
			m_bRender = true;
			break;
		}
	case 3:
		m_bRender = true;
		m_itexnum = 10;
		break;

	case 4:
		if (m_iMaxcp <= 0)
		{
			m_bRender = false;
			return OBJ_NOEVENT;
		}
		else
		{
			m_itexnum = m_iMaxcp / 100;
			m_bRender = true;
			break;
		}
	case 5:
		if (m_iMaxcp <= 0)
		{
			m_bRender = false;
			return OBJ_NOEVENT;
		}
		else
		{
			m_itexnum = ((m_iMaxcp % 100) / 10);
			m_bRender = true;
			break;
		}
	case 6:
		if (m_iMaxcp <= 0)
		{
			m_bRender = false;
			return OBJ_NOEVENT;
		}
		else
		{
			m_itexnum = m_iMaxcp % 10;
			m_bRender = true;
			break;
		}
	}


	/*if (m_fSize.x <= 40.f || m_fSize.y <= 60.f)
		m_bsizedown = false;*/
	/*m_fSize.x = 28.f;
	m_fSize.y = 32.f;*/
	if (m_iIndex < 3)
	{
		m_fPosition.x = 1065.f + (m_iIndex * 16);
		m_fPosition.y = 300.f;
	}
	else if (m_iIndex == 3)
	{
		m_fPosition.x = 1074.f + (m_iIndex * 13);
		m_fPosition.y = 300.f;
	}
	else
	{
		m_fPosition.x = 1070.f + (m_iIndex * 13);
		m_fPosition.y = 300.f;
	}
	
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	/*if (m_iCurrenthit == 0)
		m_itexnum = 0;*/


	return OBJ_NOEVENT;
}

void CCP_Guage_font::Late_Tick(_float fTimeDelta)
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

HRESULT CCP_Guage_font::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CCP_Guage_font::Ready_Components(void * pArg)
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

HRESULT CCP_Guage_font::SetUp_ShaderResources()
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

CCP_Guage_font * CCP_Guage_font::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCP_Guage_font*	pInstance = new CCP_Guage_font(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCP_Guage_font"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCP_Guage_font::Clone(void * pArg)
{
	CCP_Guage_font*	pInstance = new CCP_Guage_font(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCP_Guage_font"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCP_Guage_font::Free()
{
	Safe_Release(m_pTextureCom1);
	__super::Free();
}
