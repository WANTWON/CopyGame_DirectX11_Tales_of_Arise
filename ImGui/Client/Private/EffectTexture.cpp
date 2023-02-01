#include "stdafx.h"

#include "EffectTexture.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"

CEffectTexture::CEffectTexture(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CEffectTexture::CEffectTexture(const CEffectTexture & rhs)
	: CEffect(rhs)
{
}

HRESULT CEffectTexture::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectTexture::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

int CEffectTexture::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bPlay)
	{
		if (m_fTimer >= m_tTextureEffectDesc.fLifetime)
		{
			m_bPlay = false;
			CImgui_Manager::Get_Instance()->Set_Play(false);

			m_fTimer = 0.f;
		}
		else
		{
			VelocityLerp();
			SizeLerp();
			AlphaLerp();
			NoisePowerLerp();

			m_pTransformCom->Set_Scale(CTransform::STATE::STATE_RIGHT, m_tTextureEffectDesc.fSize);
			m_pTransformCom->Set_Scale(CTransform::STATE::STATE_UP, m_tTextureEffectDesc.fSize);
			m_pTransformCom->Set_Scale(CTransform::STATE::STATE_LOOK, m_tTextureEffectDesc.fSize);

			// Billboard
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			m_pTransformCom->LookAt(XMLoadFloat4(&pGameInstance->Get_CamPosition()));
			RELEASE_INSTANCE(CGameInstance);

			m_fTimer += fTimeDelta;
		}
	}

	return S_OK;
}

void CEffectTexture::Late_Tick(_float fTimeDelta)
{
	if (m_pRendererCom)
	{
		//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);		/* Non-Alphablend */
		//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLENDLIGHTS, this);	/* Alphablend with Normals */
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);			/* Alphablend */

		Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	}
}

HRESULT CEffectTexture::Render()
{
	if (!m_bPlay)
		return S_OK;

	if (!m_pShaderCom || !m_pVIBufferCom)
		return E_FAIL;

	__super::Render();

	m_pShaderCom->Begin(m_eShaderID);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CEffectTexture::Add_MaskTexture()
{
	if (wcscmp(m_tTextureEffectDesc.wcMaskTexture, TEXT("")))
	{
		Safe_Release(m_pMaskTexture);
		Remove_Components(TEXT("Com_TextureMask"));

		/* For.Com_TextureMask */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureMask"), LEVEL_STATIC, m_tTextureEffectDesc.wcMaskTexture, (CComponent**)&m_pMaskTexture)))
			return;
	}
}

void CEffectTexture::Add_NoiseTexture()
{
	if (wcscmp(m_tTextureEffectDesc.wcNoiseTexture, TEXT("")))
	{
		Safe_Release(m_pNoiseTexture);
		Remove_Components(TEXT("Com_TextureNoise"));

		/* For.Com_TextureNoise */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureNoise"), LEVEL_STATIC, m_tTextureEffectDesc.wcNoiseTexture, (CComponent**)&m_pNoiseTexture)))
			return;
	}
}

void CEffectTexture::Add_DissolveTexture()
{
	if (wcscmp(m_tTextureEffectDesc.wcDissolveTexture, TEXT("")))
	{
		Safe_Release(m_pDissolveTexture);
		Remove_Components(TEXT("Com_TextureDissolve"));

		/* For.Com_TextureDissolve */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureDissolve"), LEVEL_STATIC, m_tTextureEffectDesc.wcDissolveTexture, (CComponent**)&m_pDissolveTexture)))
			return;
	}
}

void CEffectTexture::VelocityLerp()
{
}

void CEffectTexture::SizeLerp()
{
	if (m_SizeCurves.empty())
		return;

	/* 0 ~ 1 */
	_float fCurrentLifeNormalized = m_fTimer / m_tTextureEffectDesc.fLifetime;

	for (_float3& fSizeCurve : m_SizeCurves)
	{
		/* Break cause Curve should not start yet ('Y' is the Curve Start Time). */
		if (fCurrentLifeNormalized < fSizeCurve.y)
			break;

		/* Skip cause Curve has already been lerped through ('Z' is the Curve End Time). */
		if (fCurrentLifeNormalized > fSizeCurve.z)
		{
			m_tTextureEffectDesc.fInitialSize = fSizeCurve.x;
			continue;
		}

		_float fFactorDividend = (m_fTimer - (m_tTextureEffectDesc.fLifetime * fSizeCurve.y));
		_float fFactorDivisor = ((m_tTextureEffectDesc.fLifetime * fSizeCurve.z) - (m_tTextureEffectDesc.fLifetime * fSizeCurve.y));

		_float fInterpFactor = fFactorDividend / fFactorDivisor;
		_float fLerpSize = m_tTextureEffectDesc.fInitialSize + fInterpFactor * (fSizeCurve.x - m_tTextureEffectDesc.fInitialSize);
		m_tTextureEffectDesc.fSize = fLerpSize;

		break;
	}
}

void CEffectTexture::AlphaLerp()
{
	if (m_AlphaCurves.empty())
		return;

	/* 0 ~ 1 */
	_float fCurrentLifeNormalized = m_fTimer / m_tTextureEffectDesc.fLifetime;

	for (_float3& fAlphaCurve : m_AlphaCurves)
	{
		/* Break cause Curve should not start yet ('Y' is the Curve Start Time). */
		if (fCurrentLifeNormalized < fAlphaCurve.y)
			break;

		/* Skip cause Curve has already been lerped through ('Z' is the Curve End Time). */
		if (fCurrentLifeNormalized > fAlphaCurve.z)
		{
			m_tTextureEffectDesc.fAlphaInitial = fAlphaCurve.x;
			continue;
		}

		_float fFactorDividend = (m_fTimer - (m_tTextureEffectDesc.fLifetime * fAlphaCurve.y));
		_float fFactorDivisor = ((m_tTextureEffectDesc.fLifetime * fAlphaCurve.z) - (m_tTextureEffectDesc.fLifetime * fAlphaCurve.y));

		_float fInterpFactor = fFactorDividend / fFactorDivisor;
		_float fLerpAlpha = m_tTextureEffectDesc.fAlphaInitial + fInterpFactor * (fAlphaCurve.x - m_tTextureEffectDesc.fAlphaInitial);
		m_tTextureEffectDesc.fAlpha = fLerpAlpha;

		break;
	}
}

void CEffectTexture::NoisePowerLerp()
{
	if (m_NoisePowerCurves.empty())
		return;

	/* 0 ~ 1 */
	_float fCurrentLifeNormalized = m_fTimer / m_tTextureEffectDesc.fLifetime;

	for (_float3& fNoisePowerCurve : m_NoisePowerCurves)
	{
		/* Break cause Curve should not start yet ('Y' is the Curve Start Time). */
		if (fCurrentLifeNormalized < fNoisePowerCurve.y)
			break;

		/* Skip cause Curve has already been lerped through ('Z' is the Curve End Time). */
		if (fCurrentLifeNormalized > fNoisePowerCurve.z)
		{
			m_tTextureEffectDesc.fNoisePowerInitial = fNoisePowerCurve.x;
			continue;
		}

		_float fFactorDividend = (m_fTimer - (m_tTextureEffectDesc.fLifetime * fNoisePowerCurve.y));
		_float fFactorDivisor = ((m_tTextureEffectDesc.fLifetime * fNoisePowerCurve.z) - (m_tTextureEffectDesc.fLifetime * fNoisePowerCurve.y));

		_float fInterpFactor = fFactorDividend / fFactorDivisor;
		_float fLerpNoisePower = m_tTextureEffectDesc.fNoisePowerInitial + fInterpFactor * (fNoisePowerCurve.x - m_tTextureEffectDesc.fNoisePowerInitial);
		m_tTextureEffectDesc.fNoisePower = fLerpNoisePower;

		break;
	}
}

HRESULT CEffectTexture::Ready_Components(void * pArg)
{
	memcpy(&m_tTextureEffectDesc, (TEXTUREEFFECTDESC*)pArg, sizeof(TEXTUREEFFECTDESC));

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
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, m_tTextureEffectDesc.wcPrototypeId, (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (wcscmp(m_tTextureEffectDesc.wcMaskTexture, TEXT("")))
	{
		/* For.Com_MaskTexture */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureMask"), LEVEL_STATIC, m_tTextureEffectDesc.wcMaskTexture, (CComponent**)&m_pMaskTexture)))
			return E_FAIL;
	}
	if (wcscmp(m_tTextureEffectDesc.wcNoiseTexture, TEXT("")))
	{
		/* For.Com_NoiseTexture */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureNoise"), LEVEL_STATIC, m_tTextureEffectDesc.wcNoiseTexture, (CComponent**)&m_pNoiseTexture)))
			return E_FAIL;
	}
	if (wcscmp(m_tTextureEffectDesc.wcDissolveTexture, TEXT("")))
	{
		/* For.Com_DissolveTexture */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureDissolve"), LEVEL_STATIC, m_tTextureEffectDesc.wcDissolveTexture, (CComponent**)&m_pDissolveTexture)))
			return E_FAIL;
	}
}

HRESULT CEffectTexture::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV())))
		return E_FAIL;

	if (m_pMaskTexture)
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_MaskTexture", m_pMaskTexture->Get_SRV())))
			return E_FAIL;
	if (m_pNoiseTexture)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_NoiseTexture", m_pNoiseTexture->Get_SRV())))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fNoiseSpeed", &m_tTextureEffectDesc.fNoiseSpeed, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fNoisePower", &m_tTextureEffectDesc.fNoisePower, sizeof(_float))))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_tTextureEffectDesc.vColor, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_tTextureEffectDesc.fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlphaDiscard", &m_tTextureEffectDesc.m_fAlphaDiscard, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fTimer", &m_fTimer, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CEffectTexture * CEffectTexture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffectTexture* pInstance = new CEffectTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffectTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffectTexture::Clone(void * pArg)
{
	CEffectTexture* pInstance = new CEffectTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffectTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectTexture::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}