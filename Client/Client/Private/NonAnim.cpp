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

	//CPickingMgr::Get_Instance()->Add_PickingGroup(this);

	m_eObjectID = OBJ_BLOCK;

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&m_ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		Set_Scale(m_ModelDesc.vScale);

		if(m_ModelDesc.m_fAngle != 0)
			m_pTransformCom->Turn(XMLoadFloat3(&m_ModelDesc.vRotation), m_ModelDesc.m_fAngle);
	}


	

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//
	//	/* For.Com_Model*/
	//	_tchar			szModeltag[MAX_PATH] = TEXT("");
	//	MultiByteToWideChar(CP_ACP, 0, m_ModelDesc.pModeltag, (int)strlen(m_ModelDesc.pModeltag), szModeltag, MAX_PATH);


	//	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//	char cName[MAX_PATH];
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(szModeltag, cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_NONANIM);
	//	//ERR_MSG(TEXT("Save_Bin_Model"));
	//	RELEASE_INSTANCE(CData_Manager);
	//

	//RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

int CNonAnim::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	

	return OBJ_NOEVENT;
}

void CNonAnim::Late_Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	if (iLevel == LEVEL_GAMEPLAY && Check_IsinFrustum(10.f) == false)
		goto FAILD_CULLING;
	else if (iLevel == LEVEL_TAILCAVE && Check_IsinFrustum(4.f) == false)
		goto FAILD_CULLING;
	else if (iLevel == LEVEL_TOWER && Check_IsinFrustum(14.f) == false)
		goto FAILD_CULLING;



	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		
#ifdef _DEBUG
		if (m_pAABBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pAABBCom);
		if (m_pOBBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pOBBCom);
		if (m_pSPHERECom != nullptr)
			m_pRendererCom->Add_Debug(m_pSPHERECom);
#endif
	}

	CBaseObj* pPlayer = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	_float fDistanceX = fabsf(XMVectorGetX(pPlayer->Get_TransformState(CTransform::STATE_POSITION)) - XMVectorGetX(Get_TransformState(CTransform::STATE_POSITION)));
	_float fDistanceZ = fabsf(XMVectorGetZ(pPlayer->Get_TransformState(CTransform::STATE_POSITION)) - XMVectorGetZ(Get_TransformState(CTransform::STATE_POSITION)));


	if (iLevel == LEVEL_GAMEPLAY && fDistanceX < 30.f && fDistanceZ < 30.f)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	else if (iLevel == LEVEL_TAILCAVE)
	{
		_float4		vLightEye, vLightAt;

		XMStoreFloat4(&vLightEye, pPlayer->Get_TransformState(CTransform::STATE_POSITION));
		vLightEye.y = 0.f;
		vLightAt = vLightEye;
		vLightEye.y += 50.f;
		vLightEye.z += 50.f;

		pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	}	
	else if (iLevel == LEVEL_TOWER)
	{
	
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	}
		

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	
	RELEASE_INSTANCE(CGameInstance);
	return;

FAILD_CULLING:
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

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
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

	//if (FAILED(m_pModelCom->RenderShadow(m_pShaderCom, 0, SHADER_NONANIM_SHADOW)))
	//	return E_FAIL;

	//if (FAILED(m_pModelCom->RenderShadow(m_pShaderCom, 1, SHADER_NONANIM_SHADOW)))
	//	return E_FAIL;

	//if (FAILED(m_pModelCom->RenderShadow(m_pShaderCom, 2, SHADER_NONANIM_SHADOW)))
	//	return E_FAIL;

	//if (FAILED(m_pModelCom->RenderShadow(m_pShaderCom, 3, SHADER_NONANIM_SHADOW)))
	//	return E_FAIL;

	//if (FAILED(m_pModelCom->RenderShadow(m_pShaderCom, 4, SHADER_NONANIM_SHADOW)))
	//	return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CNonAnim::PickingTrue()
{

}

HRESULT CNonAnim::Ready_Components(void* pArg)
{

	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar			szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_ModelDesc.pModeltag, (int)strlen(m_ModelDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), iLevel, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fog"), (CComponent**)&m_pFogtexture)))
		return E_FAIL;
	
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	_vector vPlayerPos = dynamic_cast<CBaseObj*> (pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")))->Get_TransformState(CTransform::STATE_POSITION);

	if (FAILED(m_pShaderCom->Set_RawValue("g_PlayerPosition", &vPlayerPos, sizeof(_float4))))
		return E_FAIL;

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

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSPHERECom);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
}
