#include "stdafx.h"
#include "..\Public\Sky.h"

#include "GameInstance.h"
#include "UI_Manager.h"

CSky::CSky(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CSky::CSky(const CSky & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSky::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSky::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;	

	return S_OK;
}

int CSky::Tick(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

void CSky::Late_Tick(_float fTimeDelta)
{


	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	/*if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return;*/

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, 
		XMLoadFloat4(&pGameInstance->Get_CamPosition()));

	RELEASE_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CSky::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSky::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTexture"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	switch (iLevel)
	{
	case Client::LEVEL_SNOWFIELD:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::LEVEL_BATTLE:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_BATTLE, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::LEVEL_BOSS:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_BOSS, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::LEVEL_CITY:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_CITY, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case Client::LEVEL_LAWBATTLE:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_LAWBATTLE, TEXT("Prototype_Component_Texture_Sky"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CSky::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

CSky * CSky::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSky*	pInstance = new CSky(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSky"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CSky::Clone(void * pArg)
{
	CSky*	pInstance = new CSky(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSky"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSky::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
