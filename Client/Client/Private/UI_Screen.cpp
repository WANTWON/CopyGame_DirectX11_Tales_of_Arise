#include "stdafx.h"
#include "..\Public\UI_Screen.h"
#include "GameInstance.h"
#include "ScreenFadeEffect.h"

CUI_Screen::CUI_Screen(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Screen::CUI_Screen(const CUI_Screen & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Screen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Screen::Initialize(void * pArg)
{
	//m_eShaderID = UI_SCREEN;
	m_fSize.x = g_iWinSizeX;
	m_fSize.y = g_iWinSizeY;
	m_fPosition.x = g_iWinSizeX >> 1;
	m_fPosition.y = g_iWinSizeY >> 1;
	m_fAlpha = 1.f;

	m_bfadein = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Screen::Tick(_float fTimeDelta)
{
	if (!m_bspritemode)
	{
		m_ftimer += fTimeDelta;

		if (m_ftimer > 0.025f)
		{
			if (m_bfadein)
				m_fAlpha -= 0.04f;
			else
				m_fAlpha += 0.04f;
			//++m_itexnum;
			/*CScreenFadeEffect::SCREENFADE_DESC fadedesc;
			fadedesc.bin = true;
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_ScreenFadeEffect"), LEVEL_GAMEPLAY, TEXT("fadeeffect"), &fadedesc);*/
			m_ftimer = 0.f;
		}


		if (!m_bfadein && m_fAlpha >= 1)
		{
			++m_itexnum;
			m_bfadein = true;
		}


		if (m_bfadein && m_fAlpha <= 0)
		{
			m_fshowtimer += fTimeDelta;

		}

		if (m_fshowtimer > 2.5f)
		{
			m_fshowtimer = 0.f;
			m_bfadein = false;
		}


		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	}
	else
	{
		m_ftimer += fTimeDelta;

		if (m_ftimer > 0.032)
		{
			++m_isprite_texnum;
			m_ftimer = 0.f;
		}
	}



	return OBJ_NOEVENT;
}

void CUI_Screen::Late_Tick(_float fTimeDelta)
{
	if (!m_bspritemode)
	{
		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;


		else if (m_fAlpha <= 0.f)
			m_fAlpha = 0.f;

		if (m_itexnum >= 3)
			m_itexnum = 3;

		if (m_itexnum == 3 && !m_bfadein && m_fAlpha == 0)
		{
			m_bspritemode = true;
			m_ftimer = 0.f;
		}
	}


	//if()


	if (m_isprite_texnum == 237)
		m_isprite_texnum = 176;
	__super::Late_Tick(fTimeDelta);



}

HRESULT CUI_Screen::Render()
{


	if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_LOADING)
		return E_FAIL;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (!m_bspritemode)
	{
		if (FAILED(SetUp_ShaderID()))
			return E_FAIL;

		if (FAILED(SetUp_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(0))))
			return E_FAIL;


		m_pShaderCom->Begin(UI_SCREEN);
		m_pVIBufferCom->Render();

		return S_OK;

	}
	else
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_isprite_texnum))))
			return E_FAIL;


		m_pShaderCom->Begin(UI_ALPHABLEND);
		m_pVIBufferCom->Render();

		return S_OK;
	}


	return S_OK;
}

HRESULT CUI_Screen::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fade"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logosprite"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Screen::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Screen * CUI_Screen::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Screen*	pInstance = new CUI_Screen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Screen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_Screen::Clone(void * pArg)
{
	CUI_Screen*	pInstance = new CUI_Screen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Screen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Screen::Free()
{

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	__super::Free();
}
