#include "stdafx.h"
#include "..\Public\UI_Comboline.h"

#include "GameInstance.h"

CUI_Comboline::CUI_Comboline(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Comboline::CUI_Comboline(const CUI_Comboline & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Comboline::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Comboline::Initialize(void * pArg)
{
	m_eShaderID = UI_COMBOLINE;
	
	m_fSize.x = 300.f;
	m_fSize.y = 70.f;
	m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f + 50;
	m_fPosition.y = 200.f;

	m_fAlpha = 1.f;

	m_bfadein = false;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Comboline::Tick(_float fTimeDelta)
{

	//if(CGameInstance::Get_Instance()->Key_Up(DIK_3))
	m_fDietimer += fTimeDelta;

	if (m_fDietimer > 5.f)
		m_bfadeout = true;
	

	if (m_bfadein)
	{
		m_fAlpha -= 0.04; //생길때
		m_fOffsetX -= 10.f;

		if (m_fOffsetX <= -160.f)
		{
			m_fOffsetX = -160.f;
			m_fAlpha = 0.275f;
			m_bfadein = false;
		}
			

	}



	if (m_bfadeout)
	{
		m_fAlpha += 0.04; //생길때
		m_fOffsetX += 10.f;

		if (m_fOffsetX >= 0.f)
		{
			m_fOffsetX = 0.f;
			m_fAlpha = 1.f;
		//	m_bfadeout = false;
		}
	}
	


	//else if (m_bfadeout)
	//{
	////	m_fdietimer += fTimeDelta;

	//	//if(m_fdietimer > 0.8f)
	//		m_fAlpha += 0.0483f;
	//}
	
	

	if (CGameInstance::Get_Instance()->Key_Up(DIK_4)) // 사라질때
	{
		m_bfadeout = true;
    }

	  
		

	m_fSize.x = 350.f;
	m_fSize.y = 70.f;
	m_fPosition.x = 1345.f + m_fOffsetX;
	m_fPosition.y = 150.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	return OBJ_NOEVENT;
}

void CUI_Comboline::Late_Tick(_float fTimeDelta)
{



	/*if (m_fAlpha < 0.275f)
	{
		m_fAlpha = 0.275f;
		m_bfadein = false;
	}*/

	

		
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CUI_Comboline::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CUI_Comboline::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_line"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Comboline::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CUI_Comboline * CUI_Comboline::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Comboline*	pInstance = new CUI_Comboline(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Comboline"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_Comboline::Clone(void * pArg)
{
	CUI_Comboline*	pInstance = new CUI_Comboline(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Comboline"));
		Safe_Release(pInstance);
	}

	

	CUI_Manager::Get_Instance()->Set_comboline(pInstance);

	return pInstance;
}

void CUI_Comboline::Free()
{
	__super::Free();
}

void CUI_Comboline::setline()
{

	m_fDietimer = 0.f;
	if (m_bfadeout)
	{
		m_bfadein = true;
		m_bfadeout = false;
		m_fOffsetX = 0.f;
	}

}
