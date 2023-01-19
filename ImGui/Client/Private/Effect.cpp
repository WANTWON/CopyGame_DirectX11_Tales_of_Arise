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
	return 0;
}

void CEffect::Late_Tick(_float fTimeDelta)
{
	if (m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);		/* Non-Alphablend */
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLENDLIGHTS, this);	/* Alphablend with Normals */
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);		/* Alphablend */

		Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	}
}

HRESULT CEffect::Render()
{
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
			if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, /*m_tEffectDesc.wcTexturePrototypeId*/TEXT("Prototype_Component_Texture_Test"), (CComponent**)&m_pTextureCom)))
				return E_FAIL;
			/* For.Com_Shader */
			if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
				return E_FAIL;

			break;
		}
		case EFFECT_TYPE::TYPE_MESH:
		{
			/* For.Com_Model */
			if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, m_tEffectDesc.wcPrototypeId, (CComponent**)&m_pModelCom)))
				return E_FAIL;
			/* For.Com_Shader */
			if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
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
	Safe_Release(m_pModelCom);
	Safe_Release(m_pVIBufferPointInstanceCom);
}