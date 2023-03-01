#include "stdafx.h"
#include "..\Public\NonAnim_Instance.h"
#include "GameInstance.h"


CNonAnim_Instance::CNonAnim_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CNonAnim_Instance::CNonAnim_Instance(const CNonAnim_Instance & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CNonAnim_Instance::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNonAnim_Instance::Initialize(void * pArg)
{
	if(pArg != nullptr)
		memcpy(&m_ModelDesc, pArg, sizeof(NONANIMDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	return S_OK;
}

int CNonAnim_Instance::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CNonAnim_Instance::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	//if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION
	//	&& CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_BATTLE)
	//{
	//	return;
	//}
		

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

		if(IsRenderShadow())
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);

		if (IsRenderGlow())
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

#ifdef _DEBUG
		if (m_pAABBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pAABBCom);
		if (m_pOBBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pOBBCom);
		if (m_pSPHERECom != nullptr)
			m_pRendererCom->Add_Debug(m_pSPHERECom);
#endif
	}

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	
	RELEASE_INSTANCE(CGameInstance);
	return;
}

HRESULT CNonAnim_Instance::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
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
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CNonAnim_Instance::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_ShadowLightView(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->RenderShadow(m_pShaderCom, i, iLevel, SHADOW)))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CNonAnim_Instance::Render_Glow()
{
	if (!m_pShaderCom || !m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_bool bUseDiffuseColor = true;
	if (FAILED(m_pShaderCom->Set_RawValue("g_bUseDiffuseColor", &bUseDiffuseColor, sizeof(_bool))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GlowTexture", nullptr)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, GLOW)))
			return E_FAIL;
	}

	return S_OK;
}

_bool CNonAnim_Instance::IsRenderShadow()
{
	LEVEL eLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();


	switch (eLevel)
	{
	case Client::LEVEL_SNOWFIELD:
		if (!strcmp(m_ModelDesc.pModeltag, "Dead_Grass_SnowField") ||
			!strcmp(m_ModelDesc.pModeltag, "SmallRock2_SnowField") ||
			!strcmp(m_ModelDesc.pModeltag, "Bush_SnowField") ||
			!strcmp(m_ModelDesc.pModeltag, "Snow2_SnowField") ||
			!strcmp(m_ModelDesc.pModeltag, "Birch2_SnowField") ||
			!strcmp(m_ModelDesc.pModeltag, "Broken_Tree_SnowField") ||
			!strcmp(m_ModelDesc.pModeltag, "BushWood_SnowField") ||
			!strcmp(m_ModelDesc.pModeltag, "Snow1_SnowField") ||
			!strcmp(m_ModelDesc.pModeltag, "Stalagmite5_SnowField") ||
			!strcmp(m_ModelDesc.pModeltag, "Stalagmite4_SnowField"))
			return false;
		break;
	case Client::LEVEL_BATTLE:
		if (!strcmp(m_ModelDesc.pModeltag, "Dead_Grass_BattleZone") ||
			!strcmp(m_ModelDesc.pModeltag, "SmallRock2_BattleZone") ||
			!strcmp(m_ModelDesc.pModeltag, "Bush_BattleZone") ||
			!strcmp(m_ModelDesc.pModeltag, "Snow2_BattleZone") ||
			!strcmp(m_ModelDesc.pModeltag, "Birch2_BattleZone") ||
			!strcmp(m_ModelDesc.pModeltag, "Broken_Tree_BattleZone"))
			return false;
		break;
	case Client::LEVEL_CITY:
		if (!strcmp(m_ModelDesc.pModeltag, "Bld_WallB") ||
			!strcmp(m_ModelDesc.pModeltag, "Bld_Wall01") ||
			!strcmp(m_ModelDesc.pModeltag, "Bld_Wall01_B02_Lod1") ||
			!strcmp(m_ModelDesc.pModeltag, "CIty_BigWall") ||
			!strcmp(m_ModelDesc.pModeltag, "City_Grass03") ||
			!strcmp(m_ModelDesc.pModeltag, "CIty_Grass04") ||
			!strcmp(m_ModelDesc.pModeltag, "City_Planter") ||
			!strcmp(m_ModelDesc.pModeltag, "City_Ivy1"))
			return false;
		break;
	case Client::LEVEL_RESTAURANT:
		break;
	case Client::LEVEL_WORKTOOL:
		break;
	case Client::LEVEL_LAWBATTLE:
		break;
	case Client::LEVEL_BOSS:
		break;
	}

	

	return true;
}

_bool CNonAnim_Instance::IsRenderGlow()
{
	if (!strcmp(m_ModelDesc.pModeltag, "Bld_WallB") ||
		!strcmp(m_ModelDesc.pModeltag, "Bld_Wall01"))
		return true;

	return false;
}

_float CNonAnim_Instance::Check_CullingRadius()
{
	if (!strcmp(m_ModelDesc.pModeltag, "Bld_WallB") ||
		!strcmp(m_ModelDesc.pModeltag, "Bld_Wall01") ||
		!strcmp(m_ModelDesc.pModeltag, "Bld_Wall01_B02_Lod1"))
		return 25.f;
	else if (!strcmp(m_ModelDesc.pModeltag, "SAT_Wall02"))
		return 25.f;
	else if (!strcmp(m_ModelDesc.pModeltag, "City_Ivy1"))
		return 35.f;
	else if (!strcmp(m_ModelDesc.pModeltag, "City_Palm"))
		return 10.f;

	return 5.f;
}


HRESULT CNonAnim_Instance::Ready_Components(void* pArg)
{

	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelInstance"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar			szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_ModelDesc.pModeltag, (int)strlen(m_ModelDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CNonAnim_Instance::SetUp_ShaderResources()
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

	if (m_pFogtexture != nullptr)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_FogTexture", m_pFogtexture->Get_SRV())))
			return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CNonAnim_Instance::SetUp_ShaderID()
{
	if (!strcmp(m_ModelDesc.pModeltag, "Snow1") ||
		!strcmp(m_ModelDesc.pModeltag, "Snow2"))
		m_eShaderID =  SNOW;
	else
		m_eShaderID = DEFAULT;
	return S_OK;
}

CNonAnim_Instance * CNonAnim_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CNonAnim_Instance*	pInstance = new CNonAnim_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CNonAnim_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CNonAnim_Instance::Clone(void * pArg)
{
	CNonAnim_Instance*	pInstance = new CNonAnim_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CNonAnim_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNonAnim_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pFogtexture);
	Safe_Release(m_pModelCom);
}
