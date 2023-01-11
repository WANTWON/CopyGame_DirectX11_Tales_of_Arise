#include "stdafx.h"
#include "..\Public\FireEffect.h"
#include "Weapon.h"
#include "Player.h"
#include "GameInstance.h"
#include "TreasureBox.h"
#include "FightEffect.h"
#include "CameraManager.h"

CFireEffect::CFireEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CFireEffect::CFireEffect(const CFireEffect & rhs)
	: CEffect(rhs)
{
}

HRESULT CFireEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFireEffect::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Set_Scale(m_EffectDesc.vInitScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.vInitPositon);
	m_pTransformCom->LookDir(m_EffectDesc.vLook);
	m_vScale = m_EffectDesc.vInitScale;


	
	
	m_eShaderID = FIRE;
	m_vColorFront = XMVectorSet(255, 153, 51, 255.f);
	m_vColorBack = XMVectorSet(255, 51, 0, 255.f);
	
	


	return S_OK;
}

int CFireEffect::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	__super::Tick(fTimeDelta);


	Tick_Fire(fTimeDelta);
	

	return OBJ_NOEVENT;
}

void CFireEffect::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CFireEffect::Render()
{

	__super::Render();


	return S_OK;
}

HRESULT CFireEffect::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_Fire"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Fire"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_TextureNoise"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireNoise"), (CComponent**)&m_pNoiseTexture)))
		return E_FAIL;

	switch (m_EffectDesc.eEffectID)
	{
	case FIRE_SMALL:
		if (FAILED(__super::Add_Components(TEXT("Com_TextureAlpha"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireAlphaSmall"), (CComponent**)&m_pAlphaTexture)))
			return E_FAIL;
		break;
	case FIRE_FORME:
		if (FAILED(__super::Add_Components(TEXT("Com_TextureAlpha"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireAlphaForme"), (CComponent**)&m_pAlphaTexture)))
			return E_FAIL;
		break;
	default:
		break;
	}
	

	return S_OK;
}


HRESULT CFireEffect::SetUp_ShaderID()
{
	return S_OK;
}

HRESULT CFireEffect::SetUp_ShaderResources()
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


	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_NoiseTexture", m_pNoiseTexture->Get_SRV())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_AlphaTexture", m_pAlphaTexture->Get_SRV(m_EffectDesc.iTextureNum))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ColorBack", &m_vColorBack, sizeof(_vector))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ColorFront", &m_vColorFront, sizeof(_vector))))
		return E_FAIL;

	m_fTime += 0.01f;
	if (m_fTime >= 1000.f)
		m_fTime = 0.f;

	if (FAILED(m_pShaderCom->Set_RawValue("frameTime", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("distortion1", &distortion1, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("distortion2", &distortion2, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("distortion3", &distortion3, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("distortionScale", &distortionScale, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("distortionScale", &distortionScale, sizeof(_float))))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CFireEffect::Change_Animation(_float fTimeDelta)
{
}

void CFireEffect::Tick_Fire(_float fTimeDelta)
{
	SetUp_BillBoard();
}



CFireEffect * CFireEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFireEffect*	pInstance = new CFireEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CFireEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFireEffect::Clone(void * pArg)
{
	CFireEffect*	pInstance = new CFireEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CFireEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFireEffect::Free()
{
	__super::Free();

	Safe_Release(m_pNoiseTexture);
	Safe_Release(m_pAlphaTexture);
}
