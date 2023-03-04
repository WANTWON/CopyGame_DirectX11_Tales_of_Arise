#include "stdafx.h"
#include "..\Public\NonAnim.h"
#include "GameInstance.h"


CNonAnim::CNonAnim(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CNonAnim::CNonAnim(const CNonAnim & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CNonAnim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNonAnim::Initialize(void * pArg)
{
	if(pArg != nullptr)
		memcpy(&m_ModelDesc, pArg, sizeof(NONANIMDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	if (pArg != nullptr)
	{

		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_ModelDesc.WorldMatrix));
		_vector vPosition = XMLoadFloat3(&m_ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_pTransformCom->Set_Rotation(m_ModelDesc.vRotation);
		Set_Scale(m_ModelDesc.vScale);

		if (m_ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_ModelDesc.m_fAngle));


	}


	return S_OK;
}

int CNonAnim::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	

	return OBJ_NOEVENT;
}

void CNonAnim::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL eLevel =  (LEVEL)pGameInstance->Get_CurrentLevelIndex();
	
		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
		{

			switch (eLevel)
			{
			case Client::LEVEL_SNOWFIELD:
				break;
			case Client::LEVEL_BATTLE:
				if (strcmp(m_ModelDesc.pModeltag, "Snow_Mountain"))
				{
					RELEASE_INSTANCE(CGameInstance);
					return;
				}
				break;
			case Client::LEVEL_CITY:
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
		}
	


	_float fRadius = Check_CullingRadius();

	if (Check_IsinFrustum(fRadius) == false)
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();


	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		
		if (!strcmp(m_ModelDesc.pModeltag, "House01_2F_1200x800") ||
			!strcmp(m_ModelDesc.pModeltag, "House01_2F") ||
			!strcmp(m_ModelDesc.pModeltag, "house01_3F_1200x1200"))
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EDGE_DETECTION, this);

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

HRESULT CNonAnim::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CNonAnim::Render_ShadowDepth()
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
		if (FAILED(m_pModelCom->RenderShadow(m_pShaderCom, i, iLevel, SHADER_NONANIM_SHADOW)))
			return E_FAIL;

	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CNonAnim::Render_EdgeDetection()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 7)))
			return E_FAIL;
	}
	return S_OK;
}

_float CNonAnim::Check_CullingRadius()
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	switch (iLevel)
	{
	case Client::LEVEL_SNOWFIELD:
	case Client::LEVEL_BATTLE:
		if (!strcmp(m_ModelDesc.pModeltag, "CliffRock") ||
			!strcmp(m_ModelDesc.pModeltag, "CliffRock2") ||
			!strcmp(m_ModelDesc.pModeltag, "CliffRock3") ||
			!strcmp(m_ModelDesc.pModeltag, "CliffRock4") )
			return 25.f;
		else if (!strcmp(m_ModelDesc.pModeltag, "Hut"))
			return 20.f;
		else if (!strcmp(m_ModelDesc.pModeltag, "CommonBridge") ||
			!strcmp(m_ModelDesc.pModeltag, "Snow_Mountain"))
			return 50.f;
		else if (!strcmp(m_ModelDesc.pModeltag, "Vehicle_Boat"))
			return 10.f;
		break;
	case Client::LEVEL_CITY:
		if (!strcmp(m_ModelDesc.pModeltag, "Vehicle_Boat"))
			return 10.f;
		else if (!strcmp(m_ModelDesc.pModeltag, "Ground03") ||
			!strcmp(m_ModelDesc.pModeltag, "Ground02") ||
			!strcmp(m_ModelDesc.pModeltag, "Ground04") ||
			!strcmp(m_ModelDesc.pModeltag, "CliffWall01") ||
			!strcmp(m_ModelDesc.pModeltag, "CliffWall02") ||
			!strcmp(m_ModelDesc.pModeltag, "Castle01_Lod1"))
			return 1000.f;
		else if (!strcmp(m_ModelDesc.pModeltag, "Fountain") ||
			!strcmp(m_ModelDesc.pModeltag, "House01_2F_1200x800") || 
			!strcmp(m_ModelDesc.pModeltag, "House01_2F") || 
			!strcmp(m_ModelDesc.pModeltag, "house01_3F_1200x1200"))
			return 20.f;
		else if(!strcmp(m_ModelDesc.pModeltag, "LineNew01") ||
			!strcmp(m_ModelDesc.pModeltag, "LineNew02") ||
			!strcmp(m_ModelDesc.pModeltag, "LineNew03") ||
			!strcmp(m_ModelDesc.pModeltag, "BigGateArch01") ||
			!strcmp(m_ModelDesc.pModeltag, "Stair01") ||
			!strcmp(m_ModelDesc.pModeltag, "castleHouse") ||
			!strcmp(m_ModelDesc.pModeltag, "Bridge_base"))
			return 30.f;
		break;
	case Client::LEVEL_RESTAURANT:
		if (!strcmp(m_ModelDesc.pModeltag, "CookingTable"))
			return 25.f;
		if (!strcmp(m_ModelDesc.pModeltag, "Carpet"))
			return 10.f;
		else if (!strcmp(m_ModelDesc.pModeltag, "InteriorFloor"))
			return 1000.f;
		break;
	case Client::LEVEL_WORKTOOL:
		if (!strcmp(m_ModelDesc.pModeltag, "CookingTable"))
			return 25.f;
		if (!strcmp(m_ModelDesc.pModeltag, "Carpet3") ||
			!strcmp(m_ModelDesc.pModeltag, "Banner") || 
			!strcmp(m_ModelDesc.pModeltag, "Road"))
			return 10.f;
		else if (!strcmp(m_ModelDesc.pModeltag, "InteriorFloor"))
			return 1000.f;
		else if (!strcmp(m_ModelDesc.pModeltag, "Ceiling"))
			return 1000.f;
		break;
	case Client::LEVEL_BOSS:
		if (!strcmp(m_ModelDesc.pModeltag, "Bld_Floor01_") ||
			!strcmp(m_ModelDesc.pModeltag, "Ceiling") ||
			!strcmp(m_ModelDesc.pModeltag, "Bld_Floor01_outside_Lod1") ||
			!strcmp(m_ModelDesc.pModeltag, "Bld_D04_Door01_Wall_Lod1"))
			return 50.f;
		break;
	case Client::LEVEL_LAWBATTLE:
		if (!strcmp(m_ModelDesc.pModeltag, "FountainDel") ||
			!strcmp(m_ModelDesc.pModeltag, "StoneBlock") ||
			!strcmp(m_ModelDesc.pModeltag, "StoneBlockA") ||
			!strcmp(m_ModelDesc.pModeltag, "Bld_D04_Door01_Wall_Lod1"))
			return 1000.f;
		break;
	default:
		break;
	}

	
	return 3.f;
}


HRESULT CNonAnim::Ready_Components(void* pArg)
{

	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationPerSec = 1.f;
	TransformDesc.fSpeedPerSec = 1.f;
	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar			szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_ModelDesc.pModeltag, (int)strlen(m_ModelDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fog"), (CComponent**)&m_pFogtexture)))
	//	return E_FAIL;
	
	return S_OK;
}

HRESULT CNonAnim::SetUp_ShaderResources()
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

	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
	//	return E_FAIL;


	//_vector vPlayerPos = dynamic_cast<CBaseObj*> (pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")))->Get_TransformState(CTransform::STATE_TRANSLATION);

	//if (FAILED(m_pShaderCom->Set_RawValue("g_PlayerPosition", &vPlayerPos, sizeof(_float4))))
	//	return E_FAIL;

	if (m_pFogtexture != nullptr)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_FogTexture", m_pFogtexture->Get_SRV())))
			return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CNonAnim::SetUp_ShaderID()
{
	

	return S_OK;
}

CNonAnim * CNonAnim::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CNonAnim*	pInstance = new CNonAnim(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CNonAnim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CNonAnim::Clone(void * pArg)
{
	CNonAnim*	pInstance = new CNonAnim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CNonAnim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNonAnim::Free()
{
	__super::Free();

	Safe_Release(m_pFogtexture);
	Safe_Release(m_pModelCom);
}
