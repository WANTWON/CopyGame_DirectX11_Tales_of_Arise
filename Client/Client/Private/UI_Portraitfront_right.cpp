#include "stdafx.h"
#include "..\Public\UI_Portraitfront_right.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Portraitfront_right::CUI_Portraitfront_right(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Portraitfront(pDevice, pContext)
{
}

CUI_Portraitfront_right::CUI_Portraitfront_right(const CUI_Portraitfront_right & rhs)
	: CUI_Portraitfront(rhs)
{
}

HRESULT CUI_Portraitfront_right::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Portraitfront_right::Initialize(void * pArg)
{
	m_eShaderID = UI_POTRAIT_READY;


	m_fPosition.x = -300.f;
	m_fPosition.y = 570.f;
	m_fSize.x = 95.f;
	m_fSize.y = 95.f;
	m_fAlpha = 1;
	m_itexnum = 2;
//	m_bfadein = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Portraitfront_right::Tick(_float fTimeDelta)
{
	if (m_bfirst == false)
	{
		m_fAlpha_p -= 0.05;
		if (m_fAlpha_p <= 0.f)
			m_bbackoff = true;

	}


	if (m_bArrived)
	{
		
		if (m_bfirst1 && CUI_Manager::Get_Instance()->Get_Arrived_Count() == 4)
		{
			UpdateShaderID();
			m_bfirst1 = false;
			m_fAlpha = 0;
		}
	}

	if (m_bmoveright)
	{
		if (m_fPosition.x > -20.f)//&& m_fAlpha > 0.3f)
		{
			m_bfadein = true;
		}
		if (m_fAlpha < 0.3f)
			m_bfadein = false;
		moveright();
	}




	if (m_fPosition.x >= 195.f && m_bfirst)
	{
		m_bmoveright = false;
		m_fPosition.x = 195.f;
		m_bArrived = true;
	//	m_itexnum = 2;
		CUI_Manager::Get_Instance()->plus_Arrived_Count();
		m_bfirst = false;
		m_bportraiton = true;
	}

	/*if (m_bmoveleft)
	moveleft();*/

	if (CGameInstance::Get_Instance()->Key_Up(DIK_3))
	{
		//m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f - 45 + 160;
		//m_bmoveleft = true;
		//m_bfadein = true;
		m_fAlpha -= 0.01;
	}
	if (CGameInstance::Get_Instance()->Key_Up(DIK_4))
	{
		//m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f - 45 + 160;
		//m_bmoveleft = true;
		//m_bfadein = true;
		m_fAlpha += 0.01;
	}




	if (m_bfadein)
		m_fAlpha -= 0.03f; //생길때
	else if (m_bfadeout)
		m_fAlpha += 0.0483f;
	/*m_fPosition.x = 140.f;
	m_fPosition.y = 505.f;
	m_fSize.x = 95.f;
	m_fSize.y = 95.f;*/

	if (CGameInstance::Get_Instance()->Key_Up(DIK_5)) // 사라질때
	{
		m_bfadeout = true;
	}




	/*m_fSize.x = 100.f;
	m_fSize.y = 24.f;
	m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f - 40;
	m_fPosition.y = 170.f;*/

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	return OBJ_NOEVENT;
}

void CUI_Portraitfront_right::Late_Tick(_float fTimeDelta)
{

	/*if (m_fPosition.x <= 1200.f)
	m_bmoveleft = false;*/

	if (m_fAlpha <= 0)
	{
		m_fAlpha = 0;
		m_bfadein = false;
	}

	if (m_fAlpha_p <= 0)
	{
		m_fAlpha_p = 0;
	}
	//m_fAlpha_p

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CUI_Portraitfront_right::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (!m_bbackoff)
	{
		if (FAILED(SetUp_ShaderResources()))
			return E_FAIL;
		m_pShaderCom->Begin(UI_POTRAIT_READY);

		m_pVIBufferCom->Render();
	}
	



	

	if (m_bportraiton)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha_p, sizeof(_float))))
			return E_FAIL;

		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();

	}



	return S_OK;
}

HRESULT CUI_Portraitfront_right::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portrait_front"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Portraitfront_right::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
			return E_FAIL;


		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
			return E_FAIL;
	

	


	return S_OK;
}

CUI_Portraitfront_right * CUI_Portraitfront_right::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Portraitfront_right*	pInstance = new CUI_Portraitfront_right(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Portraitfront_right"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_Portraitfront_right::Clone(void * pArg)
{
	CUI_Portraitfront_right*	pInstance = new CUI_Portraitfront_right(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Portraitfront_right"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Portraitfront_right::Free()
{

	Safe_Release(m_pTextureCom1);
	__super::Free();
}
