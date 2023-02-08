#include "stdafx.h"
#include "..\Public\ScreenFadeEffect.h"
#include "GameInstance.h"

CScreenFadeEffect::CScreenFadeEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CScreenFadeEffect::CScreenFadeEffect(const CScreenFadeEffect & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CScreenFadeEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScreenFadeEffect::Initialize(void * pArg)
{
	
	m_eShaderID = UI_BATTLESTART;
	m_fSize.x = g_iWinSizeX;
	m_fSize.y = g_iWinSizeY;
	m_fPosition.x = g_iWinSizeX >> 1;
	m_fPosition.y = g_iWinSizeY >> 1;

	if (pArg != nullptr)
	memcpy(&m_ScreenEffectdesc, pArg, sizeof(SCREENFADE_DESC));

	m_bin = m_ScreenEffectdesc.bin;

	if (m_bin)
		m_fAlpha = 0.f;
	else
		m_fAlpha = 1.f;

	scaler = 0.1f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CScreenFadeEffect::Tick(_float fTimeDelta)
{
	m_ftimer += fTimeDelta;


	



	
		scaler += 0.0529411764705882f;

		speed = 0.032f;
		//time = 0.f;
		time += fTimeDelta;


		if (time > 0.49f)
		{
			scaler = 0.1f;
			time = 0.f;
			return OBJ_DEAD;
		}


		


		m_fSize.x = g_iWinSizeX;
		m_fSize.y = g_iWinSizeY;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	
	
	/*if (m_ftimer > 0.05f)
	{
		if(m_bin)
			m_fAlpha += 0.035f;
		else
			m_fAlpha -= 0.035f;


		m_ftimer = 0.f;
	}*/

	return OBJ_NOEVENT;

	
}

void CScreenFadeEffect::Late_Tick(_float fTimeDelta)
{
	

		__super::Late_Tick(fTimeDelta);
	
}

HRESULT CScreenFadeEffect::Render()
{
	/*if (m_ftimer < 1.f)
		return S_OK;*/
	


	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_UVSpriteValue", &Sprite, sizeof(_float2))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_SpriteCurTime", &time, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_SpriteSpeed", &speed, sizeof(_float))))
		return E_FAIL;


	m_pShaderCom->Begin(UI_BATTLESTART);

	m_pVIBufferCom->Render();

	

	m_fSize.x *= scaler;
	m_fSize.y *= scaler;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	_float al = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &al, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHATESTSET);

	m_pVIBufferCom->Render();
	


	return S_OK;
}

HRESULT CScreenFadeEffect::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Battlestart"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScreenFadeEffect::SetUp_ShaderResources()
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CScreenFadeEffect * CScreenFadeEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CScreenFadeEffect*	pInstance = new CScreenFadeEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CScreenFadeEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CScreenFadeEffect::Clone(void * pArg)
{
	CScreenFadeEffect*	pInstance = new CScreenFadeEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CScreenFadeEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScreenFadeEffect::Free()
{
	__super::Free();
}
