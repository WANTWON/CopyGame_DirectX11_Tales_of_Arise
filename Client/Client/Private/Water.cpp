#include "stdafx.h"
#include "Water.h"

#include "GameInstance.h"

CWater::CWater(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CWater::CWater(const CWater & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CWater::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWater::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	NONANIMDESC ModelDesc;

	if (pArg != nullptr)
		memcpy(&ModelDesc, pArg, sizeof(NONANIMDESC));

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(ModelDesc.vScale);

		if (ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(ModelDesc.m_fAngle));
	}

	return S_OK;
}

int CWater::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	m_fScrollingTimer += fTimeDelta;

	return OBJ_NOEVENT;
}

void CWater::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	if (m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
#ifdef _DEBUG		
		m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif
	}

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
}

HRESULT CWater::Render()
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

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, SHADER_NONANIM_WATER)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CWater::Ready_Components(void *pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	/* For.Com_Model */
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_Plane"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	/* For.Com_WaterNoise */
	if (FAILED(__super::Add_Components(TEXT("Com_WaterNoise"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_WaterNoise"), (CComponent**)&m_pWaterNoiseTextureCom)))
		return E_FAIL;
	/* For.Com_WaterNormal */
	if (FAILED(__super::Add_Components(TEXT("Com_WaterNormal"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_WaterNormal"), (CComponent**)&m_pWaterNormalTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CWater::SetUp_ShaderResources()
{
	if (!m_pShaderCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Bind_RenderTarget_SRV(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	/* Water Shader Properties. */
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_WaterNoiseTexture", m_pWaterNoiseTextureCom->Get_SRV())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_WaterNormalTexture", m_pWaterNormalTextureCom->Get_SRV())))
		return E_FAIL;

	_float WaterMaxRange;
	_float3 WaterColorDeep;
	_float3 WaterColorShallow;
	_float WaterFoamDepth;
	_float WaterNoiseCutoff;
	_float WaterNormalPower;
	_float2 WaterNoiseWrap;
	_float2 WaterNoiseScroll;

	if (pGameInstance->Get_CurrentLevelIndex() == LEVEL_CITY)
	{
		WaterMaxRange = 7.5f;
		WaterColorDeep = _float3(0.6f, 0.8f, 0.8f);
		WaterColorShallow = _float3(0.56f, 0.72f, 0.62f);
		WaterFoamDepth = 0.6f;
		WaterNoiseCutoff = 0.777f;
		WaterNormalPower = 0.27f;
		WaterNoiseScroll = _float2(0.f, 0.02f);
		WaterNoiseWrap = _float2(20.f, 7.5);
	}
	else
	{
		WaterMaxRange = 5.f;
		WaterColorDeep = _float3(0.1f, 0.35f, .7f);
		WaterColorShallow = _float3(0.02f, 0.59f, 0.86f);
		WaterFoamDepth = 0.4f;
		WaterNoiseCutoff = 0.777f;
		WaterNormalPower = 0.27f;
		WaterNoiseScroll = _float2(0.02f, 0.f);
		WaterNoiseWrap = _float2(7.5f, 30.f);
	}

	if (FAILED(m_pShaderCom->Set_RawValue("g_fWaterMaxRange", &WaterMaxRange, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WaterColorDeep", &WaterColorDeep, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WaterColorShallow", &WaterColorShallow, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fFoamDepth", &WaterFoamDepth, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fNoiseCutoff", &WaterNoiseCutoff, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fNormalPower", &WaterNormalPower, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vNoiseScroll", &WaterNoiseScroll, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_vNoiseWrap", &WaterNoiseWrap, sizeof(_float2))))
		return E_FAIL;

	_float fWaterPlaneScale = m_pTransformCom->Get_Scale(CTransform::STATE::STATE_RIGHT);
	if (FAILED(m_pShaderCom->Set_RawValue("g_fWaterPlaneScale", &fWaterPlaneScale, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fScrollTimer", &m_fScrollingTimer, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CWater * CWater::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CWater* pInstance = new CWater(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Create: CWater"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWater::Clone(void * pArg)
{
	CWater* pInstance = new CWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Clone: CWater"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWater::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pWaterNoiseTextureCom);
	Safe_Release(m_pWaterNormalTextureCom);
	Safe_Release(m_pNavigationCom);
}