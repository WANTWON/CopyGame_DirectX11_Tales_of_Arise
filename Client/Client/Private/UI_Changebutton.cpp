#include "stdafx.h"
#include "..\Public\UI_Changebutton.h"

#include "GameInstance.h"
#include "BattleManager.h"



CUI_Changebutton::CUI_Changebutton(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Changebutton::CUI_Changebutton(const CUI_Changebutton & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Changebutton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Changebutton::Initialize(void * pArg)
{

	

	m_fSize.x = 32;
	m_fSize.y = 28.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;

	m_fAlpha = 1;

	m_bfadein = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Changebutton::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return OBJ_NOEVENT;

	//std::mbstowcs;
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return OBJ_NOEVENT;
	if (m_bfadein)
		m_fAlpha -= 0.04f; //»ý±æ¶§
	else if (m_bfadeout)
		m_fAlpha += 0.0483f;


	




	m_fSize.x = 32.f;
	m_fSize.y = 28.f;
	m_fPosition.x = 85.f;
	m_fPosition.y = 660.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	return OBJ_NOEVENT;
}

void CUI_Changebutton::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return ;
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return;


	if (m_fAlpha <= 0)
	{
		m_fAlpha = 0;
		m_bfadein = false;
	}

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CUI_Changebutton::Render()
{



	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


//	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), testvec[0], XMVectorSet(10.f, 0.f, 0.f, 1.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.6f);

//	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), testvec[1], XMVectorSet(20.f, 0.f, 0.f, 1.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.6f);

//	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), testvec[2], XMVectorSet(30.f, 0.f, 0.f, 1.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.6f);



	return S_OK;
}

HRESULT CUI_Changebutton::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_changebutton"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Changebutton::SetUp_ShaderResources()
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

CUI_Changebutton * CUI_Changebutton::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Changebutton*	pInstance = new CUI_Changebutton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Changebutton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_Changebutton::Clone(void * pArg)
{
	CUI_Changebutton*	pInstance = new CUI_Changebutton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Changebutton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Changebutton::Free()
{

	Safe_Release(m_pTextureCom1);
	__super::Free();
}


