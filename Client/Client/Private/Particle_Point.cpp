#include "stdafx.h"
#include "..\Public\Particle_Point.h"

#include "GameInstance.h"

CParticle_Point::CParticle_Point(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CParticle_Point::CParticle_Point(const CParticle_Point & rhs)
	: CGameObject(rhs)
{
}

HRESULT CParticle_Point::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Point::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1.f, 0.f, 0.f, 1.f));
	
	return S_OK;
}

void CParticle_Point::Tick(_float fTimeDelta)
{
	m_pVIBufferCom->Update(fTimeDelta);

}

void CParticle_Point::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CParticle_Point::Render()
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

HRESULT CParticle_Point::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_PointInstance"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Point::SetUp_ShaderResources()
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	return S_OK;
}

CParticle_Point * CParticle_Point::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CParticle_Point*	pInstance = new CParticle_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CParticle_Point"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CParticle_Point::Clone(void * pArg)
{
	CParticle_Point*	pInstance = new CParticle_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CParticle_Point"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Point::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
