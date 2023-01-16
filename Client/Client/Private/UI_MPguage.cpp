#include "stdafx.h"
#include "..\Public\UI_MPguage.h"

#include "GameInstance.h"

CMP_Guage::CMP_Guage(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CMP_Guage::CMP_Guage(const CMP_Guage & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CMP_Guage::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMP_Guage::Initialize(void * pArg)
{
	m_eShaderID = UI_MP_GUAGE;
	//m_fSize.x = 1000.f;
	//m_fSize.y = 1000.f;
	//m_fPosition.x = 600.f;
	//m_fPosition.y = 600.f;
	///*m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f;
	//m_fPosition.y = g_iWinSizeY - m_fSize.y * 0.5f;
//*/
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	/*if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;*/
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1.f, 0.f, 0.f, 1.f));
	return S_OK;
}

int CMP_Guage::Tick(_float fTimeDelta)
{
	/*m_fPosition.x = 600.f;
	m_fPosition.y = 600.f;
	m_fSize.x = 1000.f;
	m_fSize.y = 1000.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));*/

	return OBJ_NOEVENT;
}

void CMP_Guage::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CMP_Guage::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom1)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom1->Render();

	/*m_fPosition.x += 13.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_MP_GUAGE);

	m_pVIBufferCom->Render();

	m_fPosition.x += 13.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_MP_GUAGE);

	m_pVIBufferCom->Render();*/
		
	

	return S_OK;
}

HRESULT CMP_Guage::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MPguage"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_PointInstance"), (CComponent**)&m_pVIBufferCom1)))
		return E_FAIL;

	//m_pVIBufferCom1
	return S_OK;
}

HRESULT CMP_Guage::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &CGameInstance::Get_Instance()->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;*/

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;*/


	return S_OK;
}

CMP_Guage * CMP_Guage::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMP_Guage*	pInstance = new CMP_Guage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMP_Guage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMP_Guage::Clone(void * pArg)
{
	CMP_Guage*	pInstance = new CMP_Guage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMP_Guage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMP_Guage::Free()
{
	__super::Free();
}
