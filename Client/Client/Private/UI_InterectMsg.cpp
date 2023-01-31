#include "stdafx.h"
#include "..\Public\UI_InterectMsg.h"
#include "GameInstance.h"



CUI_InterectMsg::CUI_InterectMsg(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_InterectMsg::CUI_InterectMsg(const CUI_InterectMsg & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_InterectMsg::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_InterectMsg::Initialize(void * pArg)
{


	m_FontR = 0.9019607843137255f;
	m_FontG = 0.8235294117647059f;
	m_FontB = 0.6588235294117647f;
	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fFontPos.x = 500.f;
	m_fAlpha = 0;

	


	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_InterectMsg::Tick(_float fTimeDelta)
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
			m_fAlpha += 0.1f; //생길때
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
			m_fFadeY -= 5.f;
		}

		if (m_fFadeY < -110.f)
			m_bgoup = false;



		//	m_fAlpha = 0.5f;

		if (CGameInstance::Get_Instance()->Key_Up(DIK_4)) // 사라질때
		{
			m_bfadeout = true;
		}
		if (CGameInstance::Get_Instance()->Key_Up(DIK_5)) // 생겨질때
		{
			m_bfadein = true;
		}
		if (CGameInstance::Get_Instance()->Key_Up(DIK_7)) // 생겨질때
		{
			m_bfadein = true;

		}
	}






	return OBJ_NOEVENT;
}

void CUI_InterectMsg::Late_Tick(_float fTimeDelta)
{

	if (m_btick)
	{
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
		}

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	}



}

HRESULT CUI_InterectMsg::Render()
{
	if (!m_btick)
		return S_OK;


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	_float alpha = m_fAlpha * 0.1f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	m_fSize.x = 400.f;
	m_fSize.y = 40.f;
	m_fPosition.x = 190.f + m_fFadeX;
	m_fPosition.y = 680.f + m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_DIALOGUEBOX);

	m_pVIBufferCom->Render();


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 350.f;
	m_fSize.y = 5.f;
	m_fPosition.x = 200.f + m_fFadeX;
	m_fPosition.y = 700.f + m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_DIALOGUELINE);

	m_pVIBufferCom->Render();




	m_fSize.x = 72.f;
	m_fSize.y = 72.f;
	m_fPosition.x = 80.f + m_fFadeX;
	m_fPosition.y = 670.f + m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();

	if (m_bseconton)
	{
		alpha = m_fAlpha1 * 0.1f;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
			return E_FAIL;
		m_fSize.x = 400.f;
		m_fSize.y = 40.f;
		m_fPosition.x = 190.f + m_fFade1X;
		m_fPosition.y = 680.f + m_fFade1Y;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_DIALOGUEBOX);

		m_pVIBufferCom->Render();


		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha1, sizeof(_float))))
			return E_FAIL;

		m_fSize.x = 350.f;
		m_fSize.y = 5.f;
		m_fPosition.x = 200.f + m_fFade1X;
		m_fPosition.y = 700.f + m_fFade1Y;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_DIALOGUELINE);

		m_pVIBufferCom->Render();


		m_fSize.x = 72.f;
		m_fSize.y = 72.f;
		m_fPosition.x = 80.f + m_fFade1X;
		m_fPosition.y = 670.f + m_fFade1Y;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(0))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();
	}



	



	return S_OK;
}

HRESULT CUI_InterectMsg::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sideportrait"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	/*if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"), (CComponent**)&m_pTextureCom2)))
	return E_FAIL;*/



	return S_OK;
}

HRESULT CUI_InterectMsg::SetUp_ShaderResources()
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




void CUI_InterectMsg::Open_Dialogue(_uint index)//, _uint index1)
{
	m_iVectorIndex = index;
	m_btick = true;
	//	m_iVectorIndex1 = index1;

	m_bfadein = true;
	/*_uint test = 0;

	for (auto vec : (m_vCurrentDialogue[index])) test++;

	vectorsize = test;*/
}

	/*CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vDialogue[index][0], XMVectorSet(m_fFontPos.x, m_fFontPos.y, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);
	if (m_vDialogue[index].size() == 1)
	return;
	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vDialogue[index][1], XMVectorSet(m_fFontPos.x, m_fFontPos.y + 35.f, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);
	if (m_vDialogue[index].size() == 2)
	return;
	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vDialogue[index][2], XMVectorSet(m_fFontPos.x, m_fFontPos.y + 70.f, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);*/





CUI_InterectMsg * CUI_InterectMsg::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_InterectMsg*	pInstance = new CUI_InterectMsg(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_InterectMsg"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_InterectMsg::Clone(void * pArg)
{
	CUI_InterectMsg*	pInstance = new CUI_InterectMsg(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_InterectMsg"));
		Safe_Release(pInstance);
	}

	//CUI_Manager::Get_Instance()->Set_Dialoguepopup(pInstance);

	return pInstance;
}

void CUI_InterectMsg::Free()
{

	
	


	Safe_Release(m_pTextureCom1);
	__super::Free();
}


