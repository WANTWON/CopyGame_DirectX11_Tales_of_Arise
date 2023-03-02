#include "stdafx.h"
#include "..\Public\EffectObject.h"
#include "GameInstance.h"


CEffectObject::CEffectObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CEffectObject::CEffectObject(const CEffectObject & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CEffectObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectObject::Initialize(void * pArg)
{
	if(pArg != nullptr)
		memcpy(&m_EffectDesc, pArg, sizeof(EFFECTDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	if (pArg != nullptr)
	{

		_vector vPosition = XMLoadFloat3(&m_EffectDesc.m_ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_pTransformCom->Set_Rotation(m_EffectDesc.m_ModelDesc.vRotation);
		Set_Scale(m_EffectDesc.m_ModelDesc.vScale);

		if (m_EffectDesc.m_ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_EffectDesc.m_ModelDesc.m_fAngle));

	}


	return S_OK;
}

int CEffectObject::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	m_fTime += fTimeDelta;
	Tick_Direction(fTimeDelta);

	return OBJ_NOEVENT;
}

void CEffectObject::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;
	

	if (m_EffectDesc.fDeadTime > m_fTime)
	{
		m_bDead = true;
	}

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


	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	return;

}

HRESULT CEffectObject::Render()
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

HRESULT CEffectObject::Render_ShadowDepth()
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

_float CEffectObject::Check_CullingRadius()
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	
	return 3.f;
}

void CEffectObject::Tick_Direction(_float fTimeDelta)
{
	m_pTransformCom->LookDir(m_EffectDesc.vTargetDir);
	m_pTransformCom->Go_PosDir(fTimeDelta, m_EffectDesc.vTargetDir);
}


HRESULT CEffectObject::Ready_Components(void* pArg)
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
	MultiByteToWideChar(CP_ACP, 0, m_EffectDesc.m_ModelDesc.pModeltag, (int)strlen(m_EffectDesc.m_ModelDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fog"), (CComponent**)&m_pFogtexture)))
	//	return E_FAIL;
	
	return S_OK;
}

HRESULT CEffectObject::SetUp_ShaderResources()
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

	return S_OK;
}

HRESULT CEffectObject::SetUp_ShaderID()
{
	

	return S_OK;
}

CEffectObject * CEffectObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CEffectObject*	pInstance = new CEffectObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CEffectObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CEffectObject::Clone(void * pArg)
{
	CEffectObject*	pInstance = new CEffectObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffectObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectObject::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
}
