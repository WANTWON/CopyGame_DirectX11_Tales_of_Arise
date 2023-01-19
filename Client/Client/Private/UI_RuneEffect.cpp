#include "stdafx.h"
#include "..\Public\UI_RuneEffect.h"


#include "GameInstance.h"

CUI_RuneEffect::CUI_RuneEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_RuneEffect::CUI_RuneEffect(const CUI_RuneEffect & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_RuneEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_RuneEffect::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&runedesc, pArg, sizeof(RUNEDESC));

	m_fPosition.x = runedesc.position.x;
	m_fPosition.y = runedesc.position.y;


	m_eShaderID = UI_RUNECOLOR;

	m_fSize.x = 24.f;
	m_fSize.y = 24.f;
	

	m_fAlpha = 1;

	m_bfadein = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_RuneEffect::Tick(_float fTimeDelta)
{
	

 	m_fPosition.x = runedesc.position.x;
	if (m_fAlpha <= 0.7f)
		m_bfadein1 = true;

	if (m_fAlpha1 <= 0.7f)
		m_bfadein2 = true;

    
	m_fDeadtimer += fTimeDelta;

	if (m_fDeadtimer > 5.f)
	{
		return OBJ_DEAD;
	}


	/*if (m_bfadeout && m_fAlpha >= 1)
	{
		m_fAlpha = 1.f;
		m_bfadein = true;
		++m_iRunecount;
		m_bfadeout = false;
	}*/
	
	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_3))
	{
		m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f - 45 + 160;
		m_bmoveleft = true;
		m_bfadein = true;
		m_fAlpha = 1; 
	}*/

	if (m_bfadein)
		m_fAlpha -= 0.03f; //생길때
	else if (m_bfadeout)
		m_fAlpha += 0.03f;

	if (m_bfadein1)
		m_fAlpha1 -= 0.03f; //생길때
	else if (m_bfadeout1)
		m_fAlpha1 += 0.03f;

	if (m_bfadein2)
		m_fAlpha2 -= 0.03f; //생길때
	else if (m_bfadeout2)
		m_fAlpha2 += 0.03f;


	//if (CGameInstance::Get_Instance()->Key_Up(DIK_4)) // 사라질때
	//{
	//	m_bfadeout = true;
	//}


	
	

	/*m_fSize.x = 70.f;
	m_fSize.y = 20.f;
	m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f - 45;
	m_fPosition.y = 135.f;*/

	


	/*switch (m_iRunecount)
	{
	case 0:m_fPosition.x = 300.f;
		break;

	case 1:
		m_fPosition.x = 300.f + 20.f;

		break;

	case 2:
		m_fPosition.x = 300.f + 40.f;
		break;

	}*/


	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (m_fAlpha <= 0.20f)
	{
		m_fAlpha = 0.20f;
		m_bfadein = false;
		m_bfadeout = true;

	}

	if (m_fAlpha1 <= 0.20f)
	{
		m_fAlpha1 = 0.20f;
		m_bfadein1 = false;
		m_bfadeout1 = true;

	}

	if (m_fAlpha2 <= 0.20f)
	{
		m_fAlpha2 = 0.20f;
		m_bfadein2 = false;
		m_bfadeout2 = true;

	}



	return OBJ_NOEVENT;




}

void CUI_RuneEffect::Late_Tick(_float fTimeDelta)
{
	/*if (m_fPosition.x <= 1200.f)
		m_bmoveleft = false;*/

	/*if (m_fAlpha <= 0)
	{
		m_fAlpha = 0;
		m_bfadein = false;
		m_bfadeout = true;
		
	}

	if (m_fAlpha1 <= 0)
	{
		m_fAlpha1 = 0;
		m_bfadein1 = false;
		m_bfadeout1 = true;

	}

	if (m_fAlpha2 <= 0)
	{
		m_fAlpha2 = 0;
		m_bfadein2 = false;
		m_bfadeout2 = true;
		
	}*/

	//if (m_fAlpha1 <= 0.f)



	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CUI_RuneEffect::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;


	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();



	m_fPosition.x +=  15.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha1, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


	m_fPosition.x += 15.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha2, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();



	return S_OK;
}

HRESULT CUI_RuneEffect::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_RuneEffect"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_RuneEffect::SetUp_ShaderResources()
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

CUI_RuneEffect * CUI_RuneEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_RuneEffect*	pInstance = new CUI_RuneEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_RuneEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_RuneEffect::Clone(void * pArg)
{
	CUI_RuneEffect*	pInstance = new CUI_RuneEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_RuneEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_RuneEffect::Free()
{

	Safe_Release(m_pTextureCom1);
	__super::Free();
}
