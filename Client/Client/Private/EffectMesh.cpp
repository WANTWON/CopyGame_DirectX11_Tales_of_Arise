#include "stdafx.h"

#include "EffectMesh.h"
#include "GameInstance.h"

CEffectMesh::CEffectMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CEffectMesh::CEffectMesh(const CEffectMesh & rhs)
	: CEffect(rhs)
{
}

HRESULT CEffectMesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectMesh::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

int CEffectMesh::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CEffectMesh::Late_Tick(_float fTimeDelta)
{
	if (m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);			/* Non-Alphablend */
		/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLENDLIGHTS, this);*/	/* Alphablend with Normals */
		/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);*/		/* Alphablend */

		Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	}
}

HRESULT CEffectMesh::Render()
{
	if (!m_pShaderCom || !m_pModelCom)
		return E_FAIL;

	__super::Render();

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffectMesh::Ready_Components(void * pArg)
{
	memcpy(&m_tMeshEffectDesc, (MESHEFFECTDESC*)pArg, sizeof(MESHEFFECTDESC));

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 0.1f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	/* For.Com_Model */
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, m_tMeshEffectDesc.wcPrototypeId, (CComponent**)&m_pModelCom)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
}

HRESULT CEffectMesh::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();

	return S_OK;
}

CEffectMesh * CEffectMesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffectMesh* pInstance = new CEffectMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffectMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffectMesh::Clone(void * pArg)
{
	CEffectMesh* pInstance = new CEffectMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffectMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectMesh::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}