#include "stdafx.h"
#include "..\Public\Trail.h"
#include "GameInstance.h"

CTrail::CTrail(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CTrail::CTrail(const CTrail & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CTrail::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrail::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

int CTrail::Tick(_float fTimeDelta)
{
	/*CVIBuffer_Trail::TRAILPOINT TrailPointDesc;

	TrailPointDesc.vPointUp = m_vPoint_Up;
	TrailPointDesc.vPointDown = m_vPointDown;

	TrailPointDesc.vTrailMatrix = m_WeaponMatrix;

	m_pVIBufferCom->Update(TrailPointDesc);*/

	return OBJ_NOEVENT;
}

void CTrail::Late_Tick(_float fTimeDelta)
{
	//Color Setting

}

HRESULT CTrail::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	return S_OK;

}

HRESULT CTrail::Ready_Components(void * pArg)
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SwordTrail"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	///* For.Com_Shader */
	//if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"), (CComponent**)&m_pShaderCom)))
	//	return E_FAIL;

	return S_OK;
}


HRESULT CTrail::SetUp_ShaderID()
{
	return S_OK;
}

HRESULT CTrail::SetUp_ShaderResources()
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

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_EffectDesc.iTextureNum))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_ColorBack", &m_vColorBack, sizeof(_vector))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ColorFront", &m_vColorFront, sizeof(_vector))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;*/

	return S_OK;
}

void CTrail::Update_Trail_Point(_float3 vPointDown, _float3 vPointUp, _fmatrix vWeaponMatrix)
{
	m_vPointDown = vPointDown;
	m_vPoint_Up = vPointUp;

	XMStoreFloat4x4(&m_WeaponMatrix, vWeaponMatrix);

}


CTrail * CTrail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTrail*	pInstance = new CTrail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTrail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTrail::Clone(void * pArg)
{
	CTrail*	pInstance = new CTrail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTrail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
