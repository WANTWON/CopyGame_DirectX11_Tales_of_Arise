#include "stdafx.h"
#include "..\Public\\PlayerState.h"
#include "GameInstance.h"

CPlayerState::CPlayerState(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CObj_UI(pDevice, pContext)
{
}

CPlayerState::CPlayerState(const CPlayerState & rhs)
	: CObj_UI(rhs)
{
}

HRESULT CPlayerState::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerState::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_StateDesc, pArg, sizeof(STATEDESC));

	m_fSize.x = 30;
	m_fSize.y = 30;
	m_fPosition = m_StateDesc.fPosition;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_eShaderID = UI_ALPHABLEND;

	switch (m_StateDesc.m_eType)
	{
	case Client::CPlayerState::HP:
		CUI_Manager::Get_Instance()->Add_HpGroup(this);
		break;
	case Client::CPlayerState::KEY:
		CUI_Manager::Get_Instance()->Add_KeyGroup(this);
		break;
	case Client::CPlayerState::RUBY:
	case Client::CPlayerState::NUMBER:
		m_fSize.x = 25;
		m_fSize.y = 40;
		CUI_Manager::Get_Instance()->Add_RubyGroup(this);
		break;
	case Client::CPlayerState::TYPE_END:
		break;
	default:
		break;
	}

	

	return S_OK;
}

int CPlayerState::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CPlayerState::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_NextLevel() == true)
		return;

	__super::Late_Tick(fTimeDelta);
}

HRESULT CPlayerState::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CPlayerState::Ready_Components(void * pArg)
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


	switch (m_StateDesc.m_eType)
	{
	case Client::CPlayerState::HP:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CPlayerState::KEY:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmallKey"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CPlayerState::NUMBER:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Number"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CPlayerState::RUBY:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Rupee"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CPlayerState::TYPE_END:
		break;
	default:
		break;
	}


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerState::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iTextureNum))))
		return E_FAIL;

	return S_OK;
}

CPlayerState * CPlayerState::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayerState*	pInstance = new CPlayerState(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayerState::Clone(void * pArg)
{
	CPlayerState*	pInstance = new CPlayerState(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CInvenItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerState::Free()
{
	__super::Free();
}
