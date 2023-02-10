#include "stdafx.h"
#include "..\Public\UI_Skillmessage.h"
#include "GameInstance.h"



CUI_Skillmessage::CUI_Skillmessage(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Skillmessage::CUI_Skillmessage(const CUI_Skillmessage & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Skillmessage::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Skillmessage::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_popupdesc, pArg, sizeof(POPUPDESC));

	CUI_Manager::Get_Instance()->Set_SkillMsg(this);
	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fAlpha = 0;

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

int CUI_Skillmessage::Tick(_float fTimeDelta)
{
	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_3))
	{
		m_bfadein = true;
		m_fAlpha = 0.f;
		m_fFadeX = 200.f;
		m_iNum = rand() % 6;
		timer = 0.f;


	}*/

	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_4))
	{
		m_bfadeout = true;
	}*/

	if (!m_btick)
		return OBJ_NOEVENT;
	
		//if (m_bfirstarrived)
			timer += fTimeDelta;

		if (timer > 4.f)
		{
			m_bfadeout = true;
			

		}


		if (m_bfadein)
		{
			m_fAlpha += 0.1f; //»ý±æ¶§
			m_fFadeX -= 18.f;
			if (m_fFadeX <= -20.f)
			{
				m_bfadein = false;
				m_fAlpha = 1.f;
				m_fFadeX = -20.f;
			}
			
			//m_bfirstarrived = true;
		}


		else if (m_bfadeout)
		{
			m_fAlpha -= 0.1f;
			m_fAlpha1 -= 0.1f;
		//	m_fFade1X -= 2.f;
			m_fFadeX -= 18.f;

			if (m_fAlpha <= 0.f)
			{
				m_bfadeout = false;
				/*m_fAlpha = 0.f;
				m_fFadeX = 180.f;*/
				m_btick = false;
			}

		}

		




		/*if (m_fAlpha >= 1.f && m_bfadein)
		{
			m_fAlpha = 1.f;
			m_bfadein = false;

		}*/



		/*if (m_fAlpha <= 0.f && m_bfadeout)
		{
			m_fAlpha = 0.f;
			m_bfadeout = false;
			m_fAlpha1 = 0;
			m_btick = false;

			return OBJ_DEAD;
		}*/
		return OBJ_NOEVENT;
}
	


	


void CUI_Skillmessage::Late_Tick(_float fTimeDelta)
{

	if (m_btick)
	{


		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	}



}

HRESULT CUI_Skillmessage::Render()
{
	if (!m_btick)
		return S_OK;


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	_float alpha = m_fAlpha * 0.5f;

	m_fSize.x = 430.f;
	m_fSize.y = 40.f;
	m_fPosition.x = 640.f - m_fFadeX;
	m_fPosition.y = 120.f;// +m_fFadeY + m_fIndexOffsetY;
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 400.f;
	m_fSize.y = 4.f;
	m_fPosition.x = 640.f -m_fFadeX;
	m_fPosition.y = 135.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();

	m_fPosition.y = 104.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();

	//m_fSize.x = 28.f;
	//m_fSize.y = 28.f;
	//m_fPosition.x = 600.f - m_fFadeX;
	//m_fPosition.y = 120.f;// + m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(5))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_BRIGHT);

	//m_pVIBufferCom->Render();

	m_fSize.x = 240.f;
	m_fSize.y = 26.f;
	m_fPosition.x = 700.f - m_fFadeX;
	m_fPosition.y = 120.f;// + m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_iNum))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();



	









	return S_OK;
}

HRESULT CUI_Skillmessage::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillmsg"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillname"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	///* For.Com_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_name"), (CComponent**)&m_pTextureCom2)))
	//	return E_FAIL;



	return S_OK;
}

HRESULT CUI_Skillmessage::SetUp_ShaderResources()
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




void CUI_Skillmessage::Skillmsg_on(_uint index)//, _uint index1)
{

	m_btick = true;
	
	m_bfadein = true;
	m_fAlpha = 0.f;
	m_fFadeX = 200.f;
	m_iNum = index;
	timer = 0.f;
}




CUI_Skillmessage * CUI_Skillmessage::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Skillmessage*	pInstance = new CUI_Skillmessage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Skillmessage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_Skillmessage::Clone(void * pArg)
{
	CUI_Skillmessage*	pInstance = new CUI_Skillmessage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Skillmessage"));
		Safe_Release(pInstance);
	}

	CUI_Manager::Get_Instance()->Get_Itempopup_list()->push_back(pInstance);

	return pInstance;
}

void CUI_Skillmessage::Free()
{

	//CUI_Manager::Get_Instance()->Erase_Itempopup_list(this);

	Safe_Release(m_pTextureCom1);
	//Safe_Release(m_pTextureCom2);
	__super::Free();
}


