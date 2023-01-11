#include "stdafx.h"
#include "..\Public\UIScreen.h"
#include "BaseObj.h"
#include "GameInstance.h"
#include "Player.h"
#include "Level_GamePlay.h"

CUIScreen::CUIScreen(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CObj_UI(pDevice, pContext)
{
}

CUIScreen::CUIScreen(const CUIScreen & rhs)
	: CObj_UI(rhs)
{
}

HRESULT CUIScreen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIScreen::Initialize(void * pArg)
{
	m_fSize.x = g_iWinSizeX;
	m_fSize.y = g_iWinSizeY;
	m_fPosition.x = g_iWinSizeX >> 1;
	m_fPosition.y = g_iWinSizeY >> 1;
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_eShaderID = UI_ALPHABLEND;

	if (m_BackgroundDesc.eVisibleScreen == VISIBLE_SCREEN)
	{
		m_fAlpha = 0.f;
		m_eShaderID = UI_SCREEN;
	}
	else if (m_BackgroundDesc.eVisibleScreen == VISIBLE_EFFECT)
	{
		m_fAlpha = 0.f;
		m_eShaderID = UI_EFFECTSCREEN;
	}

	return S_OK;
}

int CUIScreen::Tick(_float fTimeDelta)
{
	
	return S_OK;
}

void CUIScreen::Late_Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_LOADING && m_BackgroundDesc.eVisibleScreen != VISIBLE_LOADING)
		return;


	if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_LOADING
		|| CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_LOGO)
	{
		if (m_BackgroundDesc.eVisibleScreen != VISIBLE_LOGO)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_FRONT, this);
		else 
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	}
	else
		__super::Late_Tick(fTimeDelta);
		

	if (m_BackgroundDesc.eVisibleScreen == VISIBLE_SCREEN)
	{
		if (CUI_Manager::Get_Instance()->Get_NextLevel() == true)
		{
			m_fAlpha += 0.05f;

			if (m_fAlpha >= 1.f)
			{
				CUI_Manager::Get_Instance()->Set_NextLevelFinished(true);
				m_fAlpha = 1.f;
			}
			else
				CUI_Manager::Get_Instance()->Set_NextLevelFinished(false);
		}
		else
		{
			m_fAlpha -= 0.05f;
			if (m_fAlpha <= 0.f)
				m_fAlpha = 0.f;
		}
	}

	if (m_BackgroundDesc.eVisibleScreen == VISIBLE_EFFECT && CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_GAMEPLAY)
	{
		CLevel_GamePlay* pLevel = dynamic_cast<CLevel_GamePlay*>(CGameInstance::Get_Instance()->Get_CurrentLevel());
		if (pLevel->Get_FieldType() == CLevel_GamePlay::FIELD)
		{
			m_fAlpha -= 0.01f;
			if (m_fAlpha <= 0.f)
				m_fAlpha = 0.f;
		}
		else
		{
			m_fAlpha += 0.01f;
			if (m_fAlpha >= 0.7f)
				m_fAlpha = 0.7f;
		}
	}
	else
	{
		m_fAlpha -= 0.01f;
		if (m_fAlpha <= 0.f)
			m_fAlpha = 0.f;
	}

}

HRESULT CUIScreen::Render()
{
	if (CUI_Manager::Get_Instance()->Get_UI_OpenType() == CUI_Manager::UI_END && m_BackgroundDesc.eVisibleScreen == VISIBLE_PLAYGAME)
		return E_FAIL;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CUIScreen::Ready_Components(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_BackgroundDesc, pArg, sizeof(BACKGROUNDESC));


	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (m_BackgroundDesc.pTextureTag != nullptr)
	{
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, m_BackgroundDesc.pTextureTag, (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_TextureFog"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fog"), (CComponent**)&m_pFogtexture)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIScreen::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	

	if (m_BackgroundDesc.eVisibleScreen == VISIBLE_SCREEN || m_BackgroundDesc.eVisibleScreen == VISIBLE_EFFECT)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
			return E_FAIL;
	}

	if (m_BackgroundDesc.pTextureTag != nullptr)
	{

		if (m_BackgroundDesc.eVisibleScreen == VISIBLE_PLAYGAME &&  CUI_Manager::Get_Instance()->Get_UI_OpenType() != CUI_Manager::UI_END)
		{
			_uint iIndex = CUI_Manager::Get_Instance()->Get_UI_OpenType();

			if (iIndex == CUI_Manager::UI_MAP)
			{
				if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_TAILCAVE)
				{
					if (CUI_Manager::Get_Instance()->Get_DgnMap() == true)
						iIndex = DGN_MAP_OPEN;
					else
						iIndex = DGN_MAP_MASK;
				}
				else if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() != LEVEL_GAMEPLAY)
					iIndex = NO_MAP;
				else
					iIndex = FIELD_MAP;
			}
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(iIndex))))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
				return E_FAIL;
		}
		
	}

	if (m_pFogtexture != nullptr && m_BackgroundDesc.eVisibleScreen == VISIBLE_EFFECT)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*> (CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		_float2 fPlayerPos = pPlayer->Get_ProjPosition();
		_vector vPlayerPos = XMVectorSet(fPlayerPos.x, fPlayerPos.y, 0.f, 1.f);

		if (FAILED(m_pShaderCom->Set_RawValue("g_PlayerProjPos", &vPlayerPos, sizeof(_float4))))
			return E_FAIL;

		m_fTexUV = pPlayer->Get_PlayerTexUV();
		if (FAILED(m_pShaderCom->Set_RawValue("g_TexUV", &m_fTexUV, sizeof(_float2))))
			return E_FAIL;
	

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_FogTexture", m_pFogtexture->Get_SRV())))
			return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CUIScreen::SetUp_ShaderID()
{
	return S_OK;
}

CUIScreen * CUIScreen::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUIScreen*	pInstance = new CUIScreen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUIScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUIScreen::Clone(void * pArg)
{
	CUIScreen*	pInstance = new CUIScreen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUIScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIScreen::Free()
{
	__super::Free();

	Safe_Release(m_pFogtexture);

}
