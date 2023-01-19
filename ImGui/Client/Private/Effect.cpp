#include "stdafx.h"
#include "Effect.h"
#include "GameInstance.h"

CEffect::CEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CEffect::CEffect(const CEffect & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

int CEffect::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CEffect::Late_Tick(_float fTimeDelta)
{
	if (m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);			/* Non-Alphablend */
		/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLENDLIGHTS, this);*/	/* Alphablend with Normals */
		/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);*/		/* Alphablend */

		Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	}
}

HRESULT CEffect::Render()
{
	switch (m_tEffectDesc.eType)
	{
		case CEffect::EFFECT_TYPE::TYPE_MESH:
		{
			if (!m_pShaderCom || !m_pModelCom)
				return E_FAIL;

			if (FAILED(SetUp_ShaderResources()))
				return E_FAIL;
			
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
	
			break;
		}
		case CEffect::EFFECT_TYPE::TYPE_TEXTURE:
		{
			if (!m_pShaderCom || !m_pVIBufferCom)
				return E_FAIL;

			if (FAILED(SetUp_ShaderResources()))
				return E_FAIL;

			m_pShaderCom->Begin(0);
			m_pVIBufferCom->Render();

			break;
		}
	}

	return S_OK;
}

//void CEffect::Add_ScaleCurve(_float4 ScaleCurve)
//{
//	m_tEffectDesc.ScaleCurves.push_back(ScaleCurve);
//}
//
//void CEffect::Add_ColorCurve(_float4 ColorCurve)
//{
//	m_tEffectDesc.ColorCurves.push_back(ColorCurve);
//}

HRESULT CEffect::Ready_Components(void * pArg)
{
	ZeroMemory(&m_tEffectDesc, sizeof(EFFECTDESC));
	memcpy(&m_tEffectDesc, (EFFECTDESC*)pArg, sizeof(EFFECTDESC));

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 0.1f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	switch (m_tEffectDesc.eType)
	{
		case EFFECT_TYPE::TYPE_TEXTURE:
		{
			/* For.Com_VIBuffer */
			if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
				return E_FAIL;
			/* For.Com_Texture */
			if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, m_tEffectDesc.wcTexturePrototypeId, (CComponent**)&m_pTextureCom)))
				return E_FAIL;
			/* For.Com_Shader */
			if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
				return E_FAIL;

			break;
		}
		case EFFECT_TYPE::TYPE_MESH:
		{
			/* For.Com_Model */
			if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, m_tEffectDesc.wcModelPrototypeId, (CComponent**)&m_pModelCom)))
				return E_FAIL;
			/* For.Com_Shader */
			if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
				return E_FAIL;

			break;
		}
		case EFFECT_TYPE::TYPE_PARTICLE:
		{
			/* TODO: .. */

			break;
		}
	}

	return S_OK;
}

HRESULT CEffect::SetUp_ShaderResources()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (m_tEffectDesc.eType != EFFECT_TYPE::TYPE_MESH)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect * CEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffect* pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect::Clone(void * pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pVIBufferPointInstanceCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}