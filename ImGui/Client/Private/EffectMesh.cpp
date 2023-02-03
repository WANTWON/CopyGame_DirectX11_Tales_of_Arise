#include "stdafx.h"

#include "EffectMesh.h"
#include "GameInstance.h"
#include "Imgui_Manager.h"

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

	if (m_bPlay)
	{
		if (m_fTimer >= m_tMeshEffectDesc.fLifetime)
		{
			m_bPlay = false;
			m_fTimer = 0.f;
			m_pTransformCom->Set_Rotation(_float3(0.f, 0.f, 0.f));
			CImgui_Manager::Get_Instance()->Set_Play(false);
		}
		else
		{
			m_pTransformCom->Change_RotationPerSec(m_tMeshEffectDesc.fTurnVelocity);

			_vector vTurnAxis = XMVectorSet(m_tMeshEffectDesc.vTurn.x, m_tMeshEffectDesc.vTurn.y, m_tMeshEffectDesc.vTurn.z, 0.f);
			if (!XMVector3Equal(vTurnAxis, XMVectorSet(0.f, 0.f, 0.f, 0.f)))
				m_pTransformCom->Turn(vTurnAxis, fTimeDelta);

			ColorLerp();
			SizeLerp();
			AlphaLerp();
			TurnVelocityLerp();
			NoisePowerLerp();

			m_fTimer += fTimeDelta;
		}
	}

	return OBJ_NOEVENT;
}

void CEffectMesh::Late_Tick(_float fTimeDelta)
{
	if (m_pRendererCom)
	{
		Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
		
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
		
		if (m_tMeshEffectDesc.bGlow)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);
	}
}

HRESULT CEffectMesh::Render()
{
	if (!m_bPlay)
		return S_OK;

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

		if (m_pModelCom->Get_Materials()[i].pMaterials[aiTextureType_DIFFUSE])
		{
			_bool bDiffuse = true;
			if (FAILED(m_pShaderCom->Set_RawValue("g_bDiffuse", &bDiffuse, sizeof(_bool))))
				return E_FAIL;
		}

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 4)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffectMesh::Render_Glow()
{
	if (!m_bPlay)
		return S_OK;

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

		if (FAILED(m_pShaderCom->Set_RawValue("g_vGlowColor", &m_tMeshEffectDesc.vGlowColor, sizeof(_float3))))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 5)))
			return E_FAIL;
	}

	return S_OK;
}

void CEffectMesh::Add_MaskTexture()
{
	if (wcscmp(m_tMeshEffectDesc.wcMaskTexture, TEXT("")))
	{
		Safe_Release(m_pMaskTexture);
		Remove_Components(TEXT("Com_TextureMask"));

		/* For.Com_TextureMask */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureMask"), LEVEL_STATIC, m_tMeshEffectDesc.wcMaskTexture, (CComponent**)&m_pMaskTexture)))
			return;
	}
}

void CEffectMesh::Add_NoiseTexture()
{
	if (wcscmp(m_tMeshEffectDesc.wcNoiseTexture, TEXT("")))
	{
		Safe_Release(m_pNoiseTexture);
		Remove_Components(TEXT("Com_TextureNoise"));

		/* For.Com_TextureNoise */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureNoise"), LEVEL_STATIC, m_tMeshEffectDesc.wcNoiseTexture, (CComponent**)&m_pNoiseTexture)))
			return;
	}
}

void CEffectMesh::ColorLerp()
{
	if (m_ColorCurves.empty())
		return;

	/* 0 ~ 1 */
	_float fCurrentLifeNormalized = m_fTimer / m_tMeshEffectDesc.fLifetime;

	for (array<_float, 5>& fColorCurve : m_ColorCurves)
	{
		/* Break cause Curve should not start yet ('Y' is the Curve Start Time). */
		if (fCurrentLifeNormalized < fColorCurve[3])
			break;

		/* Skip cause Curve has already been lerped through ('Z' is the Curve End Time). */
		if (fCurrentLifeNormalized > fColorCurve[4])
		{
			m_tMeshEffectDesc.vColorInitial = _float3(fColorCurve[0], fColorCurve[1], fColorCurve[2]);
			continue;
		}

		_float fFactorDividend = (m_fTimer - (m_tMeshEffectDesc.fLifetime * fColorCurve[3]));
		_float fFactorDivisor = ((m_tMeshEffectDesc.fLifetime * fColorCurve[4]) - (m_tMeshEffectDesc.fLifetime * fColorCurve[3]));

		_float fInterpFactor = fFactorDividend / fFactorDivisor;
		_vector fLerpColor = XMLoadFloat3(&m_tMeshEffectDesc.vColorInitial) + fInterpFactor * (XMLoadFloat3(&_float3(fColorCurve[0], fColorCurve[1], fColorCurve[2])) - XMLoadFloat3(&m_tMeshEffectDesc.vColorInitial));
		XMStoreFloat3(&m_tMeshEffectDesc.vColor, fLerpColor);

		break;
	}
}

void CEffectMesh::SizeLerp()
{
}

void CEffectMesh::AlphaLerp()
{
	if (m_AlphaCurves.empty())
		return;

	/* 0 ~ 1 */
	_float fCurrentLifeNormalized = m_fTimer / m_tMeshEffectDesc.fLifetime;

	for (_float3& fAlphaCurve : m_AlphaCurves)
	{
		/* Break cause Curve should not start yet ('Y' is the Curve Start Time). */
		if (fCurrentLifeNormalized < fAlphaCurve.y)
			break;

		/* Skip cause Curve has already been lerped through ('Z' is the Curve End Time). */
		if (fCurrentLifeNormalized > fAlphaCurve.z)
		{
			m_tMeshEffectDesc.fAlphaInitial = fAlphaCurve.x;
			continue;
		}

		_float fFactorDividend = (m_fTimer - (m_tMeshEffectDesc.fLifetime * fAlphaCurve.y));
		_float fFactorDivisor = ((m_tMeshEffectDesc.fLifetime * fAlphaCurve.z) - (m_tMeshEffectDesc.fLifetime * fAlphaCurve.y));

		_float fInterpFactor = fFactorDividend / fFactorDivisor;
		_float fLerpAlpha = m_tMeshEffectDesc.fAlphaInitial + fInterpFactor * (fAlphaCurve.x - m_tMeshEffectDesc.fAlphaInitial);
		m_tMeshEffectDesc.fAlpha = fLerpAlpha;

		break;
	}
}

void CEffectMesh::TurnVelocityLerp()
{
	if (m_TurnVelocityCurves.empty())
		return;

	/* 0 ~ 1 */
	_float fCurrentLifeNormalized = m_fTimer / m_tMeshEffectDesc.fLifetime;

	for (_float3& fTurnVelocityCurve : m_TurnVelocityCurves)
	{
		/* Break cause Curve should not start yet ('Y' is the Curve Start Time). */
		if (fCurrentLifeNormalized < fTurnVelocityCurve.y)
			break;

		/* Skip cause Curve has already been lerped through ('Z' is the Curve End Time). */
		if (fCurrentLifeNormalized > fTurnVelocityCurve.z)
		{
			m_tMeshEffectDesc.fTurnVelocityInitial = fTurnVelocityCurve.x;
			continue;
		}

		_float fFactorDividend = (m_fTimer - (m_tMeshEffectDesc.fLifetime * fTurnVelocityCurve.y));
		_float fFactorDivisor = ((m_tMeshEffectDesc.fLifetime * fTurnVelocityCurve.z) - (m_tMeshEffectDesc.fLifetime * fTurnVelocityCurve.y));

		_float fInterpFactor = fFactorDividend / fFactorDivisor;
		_float fLerpTurnVelocity = m_tMeshEffectDesc.fTurnVelocityInitial + fInterpFactor * (fTurnVelocityCurve.x - m_tMeshEffectDesc.fTurnVelocityInitial);
		m_tMeshEffectDesc.fTurnVelocity = fLerpTurnVelocity;

		break;
	}
}

void CEffectMesh::NoisePowerLerp()
{
	if (m_NoisePowerCurves.empty())
		return;

	/* 0 ~ 1 */
	_float fCurrentLifeNormalized = m_fTimer / m_tMeshEffectDesc.fLifetime;

	for (_float3& fNoisePowerCurve : m_NoisePowerCurves)
	{
		/* Break cause Curve should not start yet ('Y' is the Curve Start Time). */
		if (fCurrentLifeNormalized < fNoisePowerCurve.y)
			break;

		/* Skip cause Curve has already been lerped through ('Z' is the Curve End Time). */
		if (fCurrentLifeNormalized > fNoisePowerCurve.z)
		{
			m_tMeshEffectDesc.fNoisePowerInitial = fNoisePowerCurve.x;
			continue;
		}

		_float fFactorDividend = (m_fTimer - (m_tMeshEffectDesc.fLifetime * fNoisePowerCurve.y));
		_float fFactorDivisor = ((m_tMeshEffectDesc.fLifetime * fNoisePowerCurve.z) - (m_tMeshEffectDesc.fLifetime * fNoisePowerCurve.y));

		_float fInterpFactor = fFactorDividend / fFactorDivisor;
		_float fLerpNoisePower = m_tMeshEffectDesc.fNoisePowerInitial + fInterpFactor * (fNoisePowerCurve.x - m_tMeshEffectDesc.fNoisePowerInitial);
		m_tMeshEffectDesc.fNoisePower = fLerpNoisePower;

		break;
	}
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, m_tMeshEffectDesc.wcPrototypeId, (CComponent**)&m_pModelCom)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (wcscmp(m_tMeshEffectDesc.wcMaskTexture, TEXT("")))
	{
		/* For.Com_MaskTexture */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureMask"), LEVEL_STATIC, m_tMeshEffectDesc.wcMaskTexture, (CComponent**)&m_pMaskTexture)))
			return E_FAIL;
	}
	if (wcscmp(m_tMeshEffectDesc.wcNoiseTexture, TEXT("")))
	{
		/* For.Com_NoiseTexture */
		if (FAILED(__super::Add_Components(TEXT("Com_TextureNoise"), LEVEL_STATIC, m_tMeshEffectDesc.wcNoiseTexture, (CComponent**)&m_pNoiseTexture)))
			return E_FAIL;
	}
}

HRESULT CEffectMesh::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();

	if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_tMeshEffectDesc.vColor, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_tMeshEffectDesc.fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fTimer", &m_fTimer, sizeof(_float))))
		return E_FAIL;

	if (m_pMaskTexture)
	{
		_bool bMask = true;
		if (FAILED(m_pShaderCom->Set_RawValue("g_bMask", &bMask, sizeof(_bool))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fMaskSpeed", &m_tMeshEffectDesc.fMaskSpeed, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fMaskDirectionX", &m_tMeshEffectDesc.fMaskDirectionX, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fMaskDirectionY", &m_tMeshEffectDesc.fMaskDirectionY, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_MaskTexture", m_pMaskTexture->Get_SRV())))
			return E_FAIL;
	}
		
	if (m_pNoiseTexture)
	{
		_bool bNoise = true;
		if (FAILED(m_pShaderCom->Set_RawValue("g_bNoise", &bNoise, sizeof(_bool))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fNoiseSpeed", &m_tMeshEffectDesc.fNoiseSpeed, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fNoisePower", &m_tMeshEffectDesc.fNoisePower, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fNoiseDirectionX", &m_tMeshEffectDesc.fNoiseDirectionX, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fNoiseDirectionY", &m_tMeshEffectDesc.fNoiseDirectionY, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_NoiseTexture", m_pNoiseTexture->Get_SRV())))
			return E_FAIL;
	}

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