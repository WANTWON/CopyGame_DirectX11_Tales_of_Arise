#include "stdafx.h"
#include "..\Public\MessageBox.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CMessageBox::CMessageBox(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CObj_UI(pDevice, pContext)
{
}

CMessageBox::CMessageBox(const CMessageBox & rhs)
	: CObj_UI(rhs)
{
}

HRESULT CMessageBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMessageBox::Initialize(void * pArg)
{

	if (pArg != nullptr)
		memcpy(&m_MsgDesc, pArg, sizeof(MSGDESC));

	switch (m_MsgDesc.m_eMsgType)
	{
	case Client::CMessageBox::GET_ITEM:
		m_fSize.x = 1166 / 1.5f;
		m_fSize.y = 365 / 1.5f;
		break;
	case Client::CMessageBox::SHOP_TALK:
	case Client::CMessageBox::MARIN_TALK:
	case Client::CMessageBox::CRANEGAME_TALK:
	case Client::CMessageBox::FIELDNPC_TALK:
	case Client::CMessageBox::ALBATOSS_TALK:
		m_fSize.x = 1048 / 1.5f;
		m_fSize.y = 240 / 1.5f;
		break;
	default:
		m_fSize.x = 1048 / 1.5f;
		m_fSize.y = 240 / 1.5f;
		break;
	}

	


	m_fPosition = m_MsgDesc.fPosition;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_eShaderID = UI_ALPHASET;

	return S_OK;
}

int CMessageBox::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);

	return OBJ_NOEVENT;
}

void CMessageBox::Late_Tick(_float fTimeDelta)
{
	//__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_FRONT, this);

	if (m_MsgDesc.m_eMsgType == MARIN_TALK && CUI_Manager::Get_Instance()->Get_NpcGet())
	{
		m_bDead = true;
		return;
	}
		

	if (CUI_Manager::Get_Instance()->Get_Talking() && !m_bClosing)
	{
		m_iTextureNum = CUI_Manager::Get_Instance()->Get_MessageType().iTextureNum;
		if (CUI_Manager::Get_Instance()->Get_OpenMessage())
		{
			m_fAlpha += 0.1f;
			if (m_fAlpha >= 1.f)
				m_fAlpha = 1.f;
			m_bRender = true;
		}
		else
		{
			m_fAlpha -= 0.1f;

			if (m_fAlpha <= 0.f)
			{
				m_fAlpha = 0.f;
				m_bRender = false;
				m_bDead = true;
			}

		}
	}
	else
	{
		m_bClosing = true;
		m_fAlpha -= 0.1f;

		if (m_fAlpha <= 0.f)
		{
			m_fAlpha = 0.f;
			m_bRender = false;
			m_bDead = true;
		}
	}
		

}

HRESULT CMessageBox::Render()
{
	if (!m_bRender)
		return E_FAIL;
	__super::Render();

	return S_OK;
}

HRESULT CMessageBox::Ready_Components(void * pArg)
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

	switch (m_MsgDesc.m_eMsgType)
	{
	case Client::CMessageBox::GET_ITEM:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_GetMessage"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CMessageBox::SHOP_TALK:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ShopTalk"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CMessageBox::MARIN_TALK:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MarinTalk"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CMessageBox::CRANEGAME_TALK:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CraneGameTalk"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CMessageBox::FIELDNPC_TALK:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_FieldNpcMessage"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CMessageBox::ALBATOSS_TALK:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_AlbatossMessage"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CMessageBox::TARIN_TALK:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_TarinTalk"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::CMessageBox::TELEPHONE_TALK:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_TelephoneTalk"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	default:
		break;
	}

	

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;

}


HRESULT CMessageBox::SetUp_ShaderResources()
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

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_iTextureNum))))
		return E_FAIL;

	return S_OK;
}


CMessageBox * CMessageBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMessageBox*	pInstance = new CMessageBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMessageBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMessageBox::Clone(void * pArg)
{
	CMessageBox*	pInstance = new CMessageBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMessageBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMessageBox::Free()
{
	__super::Free();
}
