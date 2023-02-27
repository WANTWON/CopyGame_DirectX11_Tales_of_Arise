#include "stdafx.h"

#include "EffectTexture.h"
#include "GameInstance.h"

void CEffectTexture::Set_WorldPosition(_matrix mWorldMatrix)
{
	_vector vPosition = (_vector)mWorldMatrix.r[3];
	m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
}

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
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;
	else if (m_bPreDead)
		m_bDead = true;

	if (!m_bCanStart)
	{
		if (m_tTextureEffectDesc.fStartAfter == 0)
			m_bCanStart = true;
		else
		{
			if (m_fTimer < m_tTextureEffectDesc.fStartAfter)
				m_fTimer += fTimeDelta;
			else
			{
				m_bCanStart = true;
				m_fTimer = 0.f;
			}
		}
	}

	if (m_bCanStart)
	{
		if (m_fTimer >= m_tTextureEffectDesc.fLifetime)
			m_bPreDead = true;
		else
		{
			ColorLerp();
			SizeLerp();
			AlphaLerp();

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

	return OBJ_NOEVENT;
}

void CEffectTexture::Late_Tick(_float fTimeDelta)
{
	if (!Check_IsinFrustum())
		return;

	if (!m_bCanStart)
		return;

	if (m_pRendererCom)
	{
		Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

		if (m_tTextureEffectDesc.m_bGlow)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);
	}
}

HRESULT CEffectTexture::Render()
{
	if (!m_bCanStart)
		return S_OK;

	if (!m_pShaderCom || !m_pVIBufferCom)
		return E_FAIL;

	__super::Render();

	m_pShaderCom->Begin(SHADER_RECT_EFFECT);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffectTexture::Render_Glow()
{
	if (!m_bCanStart)
		return S_OK;

	if (!m_pShaderCom || !m_pVIBufferCom)
		return E_FAIL;

	__super::Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_vGlowColor", &m_tTextureEffectDesc.vGlowColor, sizeof(_float3))))
		return E_FAIL;

	m_pShaderCom->Begin(SHADER_RECT_GLOW);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CEffectTexture::ColorLerp()
{
	if (m_ColorCurves.empty())
		return;

	/* 0 ~ 1 */
	_float fCurrentLifeNormalized = m_fTimer / m_tTextureEffectDesc.fLifetime;

	for (array<_float, 5>& fColorCurve : m_ColorCurves)
	{
		/* Break cause Curve should not start yet ('Y' is the Curve Start Time). */
		if (fCurrentLifeNormalized < fColorCurve[3])
			break;

		/* Skip cause Curve has already been lerped through ('Z' is the Curve End Time). */
		if (fCurrentLifeNormalized > fColorCurve[4])
		{
			m_tTextureEffectDesc.vColorInitial = _float3(fColorCurve[0], fColorCurve[1], fColorCurve[2]);
			continue;
		}

		_float fFactorDividend = (m_fTimer - (m_tTextureEffectDesc.fLifetime * fColorCurve[3]));
		_float fFactorDivisor = ((m_tTextureEffectDesc.fLifetime * fColorCurve[4]) - (m_tTextureEffectDesc.fLifetime * fColorCurve[3]));

		_float fInterpFactor = fFactorDividend / fFactorDivisor;
		_vector fLerpColor = XMLoadFloat3(&m_tTextureEffectDesc.vColorInitial) + fInterpFactor * (XMLoadFloat3(&_float3(fColorCurve[0], fColorCurve[1], fColorCurve[2])) - XMLoadFloat3(&m_tTextureEffectDesc.vColorInitial));
		XMStoreFloat3(&m_tTextureEffectDesc.vColor, fLerpColor);

		break;
	}
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

	return S_OK;
}

HRESULT CEffectTexture::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV())))
		return E_FAIL;
	if (FAILED(CGameInstance::Get_Instance()->Bind_RenderTarget_SRV(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;


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