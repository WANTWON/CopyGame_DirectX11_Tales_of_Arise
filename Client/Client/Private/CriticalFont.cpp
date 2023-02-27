#include "stdafx.h"
#include "..\Public\CriticalFont.h"

#include "GameInstance.h"
#include "UI_RuneEffect.h"
#include "Player.h"
#include "PlayerManager.h"
CCriticalFont::CCriticalFont(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CCriticalFont::CCriticalFont(const CCriticalFont & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CCriticalFont::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CCriticalFont::Initialize(void * pArg)
{


	if (pArg != nullptr)
		memcpy(&m_damagedesc, pArg, sizeof(DMGDESC));

	//m_fTargetPos = m_damagedesc.fposition;
	m_fPosition = m_damagedesc.position;
	if (m_damagedesc.itype == 1)
	{
		m_fPosition.x += 40.f;
		m_fPosition.y += 40.f;
    }
	else if (m_damagedesc.itype == 2)
	{
		m_fPosition.x += 60.f;
		m_fPosition.y += 30.f;
	}
	else if (m_damagedesc.itype == 3)
	{
		m_fPosition.x += 35.f;
		m_fPosition.y += 30.f;
	}
	
	
	


	m_eShaderID = UI_BRIGHT;
	m_fAlpha = 1.f;


	m_fSize.x = 100.f * m_fScaler;
	m_fSize.y = 25.f * m_fScaler;
	
	






	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	return S_OK;
}

int CCriticalFont::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	//CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID();

	/*if (m_damagedesc.pPointer == nullptr)
		return OBJ_DEAD;*/

	for (_uint i = 0; i < 4; ++i)
		m_fbrightpos_hp[i] += 0.015f;

	for (_uint i = 0; i < 4; ++i)
	{
		if (m_fbrightpos_hp[i] >= 2.f)
			m_fbrightpos_hp[i] = 0.f;
	}




	m_fStart_timer += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));;




	if (m_bfadein)
	{

		m_fScaler -= 0.34f;
		m_fAlpha -= 0.2f;
		if (m_fScaler <= 0.1f)
		{
			m_fScaler = 0.1f;
			m_fAlpha = 0.f;
			m_balphaup = true;
			m_bfadein = false;
		}

	}

	if (m_balphaup)
	{
		m_fScaler += 0.2f;
		m_fAlpha += 0.2f;

		if (m_fScaler >= 1.f)
		{
			m_fScaler = 1.f;
			m_fAlpha = 1.f;
			m_balphaup = false;
		}
	}

	if (m_fStart_timer > 0.873f)
	{
		m_fAlpha = 1.f;
		m_fStart_timer = 0.f;
		m_balphaup = false;
		m_bfadeout = true;
	}


	if (m_bfadeout)
	{
		m_fAlpha -= 0.25f;
		m_fYFadeout += 2.f;

		if (m_fAlpha <= 0.f)
			return OBJ_DEAD;

	}



	m_fSize.x = 125.f * m_fScaler;
	m_fSize.y = 25.f * m_fScaler;
	//m_fPosition.x -
	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -(m_fPosition.y- m_fYFadeout) + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	

	return OBJ_NOEVENT;
}


void CCriticalFont::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return ;
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);


}

HRESULT CCriticalFont::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;

		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();




	


	return S_OK;
}

HRESULT CCriticalFont::Ready_Components(void * pArg)
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SpecialDamagefont"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCriticalFont::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_damagedesc.itype))))
		return E_FAIL;
	

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CCriticalFont * CCriticalFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCriticalFont*	pInstance = new CCriticalFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCriticalFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CCriticalFont::Clone(void * pArg)
{
	CCriticalFont*	pInstance = new CCriticalFont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCriticalFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCriticalFont::Free()
{

	Safe_Release(m_pTextureCom1);
	__super::Free();
}
