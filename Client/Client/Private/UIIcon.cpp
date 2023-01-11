#include "stdafx.h"
#include "..\Public\UIIcon.h"
#include "GameInstance.h"
#include "BaseObj.h"

CUIIcon::CUIIcon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CObj_UI(pDevice, pContext)
{
}

CUIIcon::CUIIcon(const CUIIcon & rhs)
	: CObj_UI(rhs)
{
}

HRESULT CUIIcon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIIcon::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_IconDesc, pArg, sizeof(ICONDESC));

	
	m_fSize.x = 58;
	m_fSize.y = 58;
	m_eShaderID = UI_ALPHADEFAULT;
	

	m_fPosition = m_IconDesc.pTarget->Get_ProjPosition();

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (m_IconDesc.iTexureNum == ICON_WARP)
	{
		CUI_Manager::Get_Instance()->Add_WarpGroup(this);
	}

	return S_OK;
}

int CUIIcon::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_UI_OpenType() != CUI_Manager::UI_MAP)
		return OBJ_NOEVENT;

	if (m_IconDesc.pTarget->Get_Dead() || m_IconDesc.pTarget == nullptr )
		m_bDead = true;

	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
	if (iLevel == LEVEL_GAMEPLAY)
	{
		m_eShaderID = UI_ALPHABLEND;
		m_fPosition.x = 325.f + XMVectorGetX(m_IconDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION))*9.6;
		m_fPosition.y = 575.f - XMVectorGetZ(m_IconDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION))*5.8f;
	}
	else if (iLevel == LEVEL_TAILCAVE)
	{
		m_fSize.x = 38.f;
		m_fSize.y = 38.f;
		m_eShaderID = UI_ALPHABLEND;
		m_fPosition.x = 340.f + XMVectorGetX(m_IconDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION))*5.65f;
		m_fPosition.y = 580.f - XMVectorGetZ(m_IconDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION))*5.3f;
	}

	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

void CUIIcon::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_UI_OpenType() != CUI_Manager::UI_MAP || !m_bShow)
		return;

	if (CUI_Manager::Get_Instance()->Get_NextLevel() == true)
		return;

	if (nullptr != m_pRendererCom)
	{
		if(m_IconDesc.iTexureNum == ICON_PLAYER)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_TOP, this);
		else
		{
			if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_TAILCAVE && CUI_Manager::Get_Instance()->Get_DgnMap() == true)
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_FRONT, this);
			else if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_GAMEPLAY)
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_FRONT, this);

		}
			

	}
		

}

HRESULT CUIIcon::Render()
{
	

	__super::Render();

	return S_OK;
}


HRESULT CUIIcon::Ready_Components(void * pArg)
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Icon"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUIIcon::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_IconDesc.iTexureNum))))
		return E_FAIL;

	return S_OK;
}

CUIIcon * CUIIcon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUIIcon*	pInstance = new CUIIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUIIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUIIcon::Clone(void * pArg)
{
	CUIIcon*	pInstance = new CUIIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUIIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIIcon::Free()
{
	__super::Free();
}
