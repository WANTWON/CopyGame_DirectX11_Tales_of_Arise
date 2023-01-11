#include "stdafx.h"
#include "..\Public\UIName.h"
#include "GameInstance.h"


CUIName::CUIName(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CObj_UI(pDevice, pContext)
{
}

CUIName::CUIName(const CUIName & rhs)
	: CObj_UI(rhs)
{
}

HRESULT CUIName::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIName::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_ButtonDesc, pArg, sizeof(NAMEDESC));

	m_fSize.x = 192;
	m_fSize.y = 48;
	m_eShaderID = UI_ALPHASET;
	m_bShow = false;

	m_fPosition.x = m_ButtonDesc.vPosition.x;
	m_fPosition.y = m_ButtonDesc.vPosition.y;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CUI_Manager::Get_Instance()->Add_NameBox(this);

	return S_OK;
}

int CUIName::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CUIName::Late_Tick(_float fTimeDelta)
{
	//__super::Late_Tick(fTimeDelta);

	if (CUI_Manager::Get_Instance()->Get_NextLevel() == true)
		return;


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_FRONT, this);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (m_bShow)
	{
		m_fAlpha += 0.02f;
		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;
	}
	else
	{
		m_fAlpha -= 0.02f;
		if (m_fAlpha <= 0.f)
		{
			m_fAlpha = 0.f;
		}
			
	}

}

HRESULT CUIName::Render()
{
	if (!m_bShow && m_fAlpha <= 0)
		return E_FAIL;

	__super::Render();

	return S_OK;
}


HRESULT CUIName::Ready_Components(void * pArg)
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
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Name"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIName::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_ButtonDesc.iTexNum))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIName::SetUp_ShaderID()
{
	
	

	return S_OK;
}


CUIName * CUIName::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUIName*	pInstance = new CUIName(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUIName"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUIName::Clone(void * pArg)
{
	CUIName*	pInstance = new CUIName(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUIName"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIName::Free()
{
	__super::Free();
}
