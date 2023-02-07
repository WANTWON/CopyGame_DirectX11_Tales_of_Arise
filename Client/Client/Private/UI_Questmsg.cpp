#include "stdafx.h"
#include "..\Public\UI_Questmsg.h"
#include "GameInstance.h"




CUI_Questmsg::CUI_Questmsg(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Questmsg::CUI_Questmsg(const CUI_Questmsg & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Questmsg::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Questmsg::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_questdesc, pArg, sizeof(QUESTMSGDESC));


	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fAlpha = 0;

	if (CUI_Manager::Get_Instance()->Get_Quest1_Herb() == 3 && CUI_Manager::Get_Instance()->Get_Quest1_Lettuce() == 3 &&
		CUI_Manager::Get_Instance()->Get_QuestIndex() == 1)
	{
		CUI_Manager::Get_Instance()->Set_QuestComplete(0, true);
	}



	if (CUI_Manager::Get_Instance()->Get_QuestIndex() == 1 && CUI_Manager::Get_Instance()->Get_QuestComplete(0))
	{
		CUI_Manager::Get_Instance()->Set_Dialogue_section(1);
	}

	


	//if(m_popupdesc.iIndex == 0)
	/*switch (m_popupdesc.iIndex)
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
	}*/


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Questmsg::Tick(_float fTimeDelta)
{


		


		if (m_bfadein)
		{
			m_fAlpha += 0.02f; //»ý±æ¶§
			
			//m_bfirstarrived = true;
		}
		else if (m_bfadeout)
		{
			m_fAlpha -= 0.02f;
		

		}

		if (m_fAlpha >= 1.f)
		{
			timer += fTimeDelta;

		}

		if (timer > 1.f)
		{
			m_bfadeout = true;
		}

		/*if (m_bfadein1)
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
*/

	


	if (m_fAlpha >= 1.f && m_bfadein)
	{
		m_fAlpha = 1.f;
		m_bfadein = false;
	}

	

	if (m_fAlpha <= 0.f && m_bfadeout)
	{
		//m_fAlpha = 0.f;
		//m_bfadeout = false;
		//m_fAlpha1 = 0;
		//m_btick = false;

		return OBJ_DEAD;
	}


	return OBJ_NOEVENT;
}

void CUI_Questmsg::Late_Tick(_float fTimeDelta)
{


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	



}

HRESULT CUI_Questmsg::Render()
{


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;


	_float boxalpha = m_fAlpha * 0.2f; 
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &boxalpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 216.f;
	m_fSize.y = 36.f;
	m_fPosition.x = 620.f;
	m_fPosition.y = 105.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_DIALOGUEBOX);

	m_pVIBufferCom->Render();

	
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 216.f;
	m_fSize.y = 26.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 105.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_questdesc.eName))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 20.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 620.f;
	m_fPosition.y = 105.f ;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_questdesc.currentcount))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();



	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	
	m_fPosition.x += 20.f ;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(10))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();

	m_fPosition.x += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_questdesc.maxcount))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();









	return S_OK;
}

HRESULT CUI_Questmsg::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTMSG"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dialoguebox"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUI_Questmsg::SetUp_ShaderResources()
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




void CUI_Questmsg::Open_Dialogue(_uint index)//, _uint index1)
{
	//	m_iVectorIndex = index;
	m_btick = true;
	//	m_iVectorIndex1 = index1;

	m_bfadein = true;
	/*_uint test = 0;

	for (auto vec : (m_vCurrentDialogue[index])) test++;

	vectorsize = test;*/
}




CUI_Questmsg * CUI_Questmsg::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Questmsg*	pInstance = new CUI_Questmsg(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Questmsg"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_Questmsg::Clone(void * pArg)
{
	CUI_Questmsg*	pInstance = new CUI_Questmsg(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Questmsg"));
		Safe_Release(pInstance);
	}

	//CUI_Manager::Get_Instance()->Get_Itempopup_list()->push_back(pInstance);

	return pInstance;
}

void CUI_Questmsg::Free()
{




	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	__super::Free();
}


