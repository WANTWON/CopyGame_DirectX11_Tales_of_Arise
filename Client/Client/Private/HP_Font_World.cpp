#include "stdafx.h"
#include "..\Public\HP_Font_World.h"

#include "GameInstance.h"

CHP_Font_World::CHP_Font_World(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CHP_Font_World::CHP_Font_World(const CHP_Font_World & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CHP_Font_World::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHP_Font_World::Initialize(void * pArg)
{
	if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;

	

	/*if (m_iYIndex == 0)
	{
		m_fPosition.x = 1180.f + (m_iIndex * 14);
		m_fPosition.y = 375;
	}
	if (m_iYIndex == 1)
	{
		m_fPosition.x = 1180.f + (m_iIndex * 14);
		m_fPosition.y = 435;
	}
	if (m_iYIndex == 2)
	{
		m_fPosition.x = 1180.f + (m_iIndex * 14);
		m_fPosition.y = 495;
	}
	if (m_iYIndex == 3)
	{
		m_fPosition.x = 1180.f + (m_iIndex * 14);
		m_fPosition.y = 555;
	}*/

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;
	m_pTransformCom->Change_Speed(10.f);
	Set_Scale({ 0.35f, 0.35f, 1.f });
	return S_OK;
}

int CHP_Font_World::Tick(_float fTimeDelta)
{
	/*if (m_iIndex == 0)
	{
		m_itexnum = m_iCurrenthp / 1000;
	}
	else if (m_iIndex == 1)
	{
		m_itexnum = ((m_iCurrenthp % 1000) / 100);
	}
	else if (m_iIndex == 2)
	{
		m_itexnum = ((m_iCurrenthp % 100) / 10);
	}
	else if (m_iIndex == 3)
	{
		m_itexnum = m_iCurrenthp % 10;
	}*/
	SetUp_BillBoard();
	CGameObject* pGameObject = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CTransform*	pPlayerTransform = (CTransform*)CGameInstance::Get_Instance()->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION));
//	SetUp_BillBoard();
	

	_float3 pos = { 0.f,0.f,0.f };
	_vector vpos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	XMStoreFloat3(&pos, vpos);
	pos.y -= 1.15f;
	//pos.x += m_iIndex * 0.24f + 1.f;
	vpos = XMLoadFloat3(&pos);
	vpos = XMVectorSetW(vpos, 1.f);
	//SetUp_BillBoard();
	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vpos);
	m_pTransformCom->Go_Right(0.11f);
	

	

	if (m_iCurrenthp == 0)
		m_itexnum = 0;
	return OBJ_NOEVENT;
}

void CHP_Font_World::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CHP_Font_World::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_itexnum = m_iCurrenthp / 1000;
	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (m_iCurrenthp >= 1000)
	{
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	
	
	
	//////////////////////////////////////////////
	if (m_iCurrenthp >= 100)
	{
		m_pTransformCom->Go_Right(0.025f);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		m_itexnum = ((m_iCurrenthp % 1000) / 100);
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}
	
//////////////////////////////////////////////////
	if (m_iCurrenthp >= 10)
	{
		m_pTransformCom->Go_Right(0.025f);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		m_itexnum = ((m_iCurrenthp % 100) / 10);
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}
  
   
///////////////////////////////////////////////////
   m_pTransformCom->Go_Right(0.025f);
   if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	   return E_FAIL;
   m_itexnum = m_iCurrenthp % 10;
   if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
	   return E_FAIL;
   m_pShaderCom->Begin(m_eShaderID);

   m_pVIBufferCom->Render();

	

	
	

	return S_OK;
}

HRESULT CHP_Font_World::Ready_Components(void * pArg)
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

	return S_OK;
}

HRESULT CHP_Font_World::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &CGameInstance::Get_Instance()->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CGameInstance::Get_Instance()->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CHP_Font_World * CHP_Font_World::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHP_Font_World*	pInstance = new CHP_Font_World(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHP_Font_World"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CHP_Font_World::Clone(void * pArg)
{
	CHP_Font_World*	pInstance = new CHP_Font_World(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHP_Font_World"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP_Font_World::Free()
{
	__super::Free();
}
