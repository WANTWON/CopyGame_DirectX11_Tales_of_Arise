#include "stdafx.h"
#include "..\Public\UI_Get_item_Popup.h"
#include "GameInstance.h"



CUI_Get_item_Popup::CUI_Get_item_Popup(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Get_item_Popup::CUI_Get_item_Popup(const CUI_Get_item_Popup & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Get_item_Popup::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Get_item_Popup::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_popupdesc, pArg, sizeof(POPUPDESC));


	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fAlpha = 0;
	
	//if(m_popupdesc.iIndex == 0)
		switch (m_popupdesc.iIndex)
		{
		case 0:
			m_fIndexOffsetY = 40.f;
			break;

		case 1:
			m_fIndexOffsetY = 80.f;
			break;

		case 2:
			m_fIndexOffsetY = 120.f;
			break;

		case 3:
			m_fIndexOffsetY = 160.f;
			break;

		case 4:
			m_fIndexOffsetY = 160.f;
			break;
		case 5:
			m_fIndexOffsetY = 200.f;
			break;
		}
			

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Get_item_Popup::Tick(_float fTimeDelta)
{
	if (m_btick)
	{
		if (m_bfirstarrived)
			timer += fTimeDelta;

		if (timer > 5.f)
		{


			m_bfadein1 = true;
			m_bgoup = true;     //++m_iDialogueindex;
			m_bseconton = true;
			m_fDietimer += fTimeDelta;
			timer = 0.f;
			m_bDeadtimeron = true;

		}

		if (m_bDeadtimeron)
		{
			m_fDietimer += fTimeDelta;

			if (m_fDietimer > 4.f)
			{
				m_bfadeout = true;
				m_bfirstarrived = false;

				m_fDietimer = 0.f;
				m_bDeadtimeron = false;
			}


		}



		if (m_bfadein)
		{
			m_fAlpha += 0.1f; //»ý±æ¶§
			m_fFadeX += 2.f;
			//m_bfirstarrived = true;
		}
		else if (m_bfadeout)
		{
			m_fAlpha -= 0.1f;
			m_fAlpha1 -= 0.1f;
			m_fFade1X -= 2.f;
			m_fFadeX -= 2.f;
			
		}

		if (m_bfadein1)
		{
			m_fAlpha1 += 0.1f;
			m_fFade1X += 2.f;
		}

		if (m_bgoup)
		{
			m_fFadeY -= 3.f;
		}

		if (m_fFadeY < -60.f)
			m_bgoup = false;


	}



	if (m_fAlpha >= 1.f && m_bfadein)
	{
		m_fAlpha = 1.f;
		m_bfadein = false;
		m_bfirstarrived = true;
	}

	if (m_fAlpha1 >= 1.f && m_bfadein1)
	{
		m_fAlpha1 = 1.f;
		m_bfadein1 = false;
	}

	if (m_fAlpha <= 0.f && m_bfadeout)
	{
		m_fAlpha = 0.f;
		m_bfadeout = false;
		m_fAlpha1 = 0;
		m_fFadeY = -20.f;
		m_fFade1Y = -20.f;
		m_btick = false;
		m_bIsdead = true;
		return OBJ_DEAD;
	}


	return OBJ_NOEVENT;
}

void CUI_Get_item_Popup::Late_Tick(_float fTimeDelta)
{

	if (m_btick)
	{
		

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	}



}

HRESULT CUI_Get_item_Popup::Render()
{
	if (!m_btick)
		return S_OK;


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	_float alpha = m_fAlpha * 0.1f;
	
	m_fSize.x = 250.f;
	m_fSize.y = 40.f;
	m_fPosition.x = 1150.f - m_fFadeX;
	m_fPosition.y = 400.f + m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_DIALOGUEBOX);

	m_pVIBufferCom->Render();

	m_fSize.x = 32.f;
	m_fSize.y = 32.f;
	m_fPosition.x = 1075.f - m_fFadeX;
	m_fPosition.y = 400.f + m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_popupdesc.eType))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();



	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 350.f;
	m_fSize.y = 5.f;
	m_fPosition.x = 1200.f - m_fFadeX;
	m_fPosition.y = 400.f + m_fFadeY+ m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;*/

	/*m_pShaderCom->Begin(UI_DIALOGUELINE);

	m_pVIBufferCom->Render();
*/


	





	return S_OK;
}

HRESULT CUI_Get_item_Popup::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dialoguebox"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMICON"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"), (CComponent**)&m_pTextureCom2)))
	return E_FAIL;



	return S_OK;
}

HRESULT CUI_Get_item_Popup::SetUp_ShaderResources()
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

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
	return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fFlowCurrent, sizeof(_float))))
	return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fFlowMAX, sizeof(_float))))
	return E_FAIL;*/


	return S_OK;
}




void CUI_Get_item_Popup::Open_Dialogue(_uint index)//, _uint index1)
{
//	m_iVectorIndex = index;
	m_btick = true;
	//	m_iVectorIndex1 = index1;

	m_bfadein = true;
	/*_uint test = 0;

	for (auto vec : (m_vCurrentDialogue[index])) test++;

	vectorsize = test;*/
}




CUI_Get_item_Popup * CUI_Get_item_Popup::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Get_item_Popup*	pInstance = new CUI_Get_item_Popup(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Get_item_Popup"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_Get_item_Popup::Clone(void * pArg)
{
	CUI_Get_item_Popup*	pInstance = new CUI_Get_item_Popup(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Get_item_Popup"));
		Safe_Release(pInstance);
	}

	CUI_Manager::Get_Instance()->Get_Itempopup_list()->push_back(pInstance);

	return pInstance;
}

void CUI_Get_item_Popup::Free()
{

	//CUI_Manager::Get_Instance()->Erase_Itempopup_list(this);



	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	__super::Free();
}


