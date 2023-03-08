#include "stdafx.h"
#include "..\Public\UI_Portraitback.h"
#include "GameInstance.h"
#include "BattleManager.h"

CPortraitback::CPortraitback(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CPortraitback::CPortraitback(const CPortraitback & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CPortraitback::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortraitback::Initialize(void * pArg)
{
	m_eShaderID = 5;

	m_fSize.x = 100.f;
	m_fSize.y = 24.f;
	m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f - 45 + 160;
	m_fPosition.y = 170.f;

	m_fAlpha = 1;

	m_bfadein = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CPortraitback::Tick(_float fTimeDelta)
{

	/*if (m_bmoveleft)
		moveleft();*/

	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return OBJ_NOEVENT;



	
	if (m_bfadein)
		m_fAlpha -= 0.04f; //���涧
	else if (m_bfadeout)
		m_fAlpha += 0.0483f;
	m_fPosition.x = 140.f;
	m_fPosition.y = 570.f;
	m_fSize.x = 50.f;
	m_fSize.y = 50.f;

	if (CGameInstance::Get_Instance()->Key_Up(DIK_5)) // �������
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

void CPortraitback::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return ;

	/*if (m_fPosition.x <= 1200.f)
		m_bmoveleft = false;*/

	if (m_fAlpha <= 0)
	{
		m_fAlpha = 0;
		m_bfadein = false;
	}

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CPortraitback::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	/*m_pShaderCom->Begin(5); // ����

	m_pVIBufferCom->Render();*/

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;


	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;

	return S_OK;
}

HRESULT CPortraitback::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Portrait_back"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPortraitback::SetUp_ShaderResources()
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

CPortraitback * CPortraitback::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPortraitback*	pInstance = new CPortraitback(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPortraitback"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CPortraitback::Clone(void * pArg)
{
	CPortraitback*	pInstance = new CPortraitback(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPortraitback"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortraitback::Free()
{

	Safe_Release(m_pTextureCom1);
	__super::Free();
}
