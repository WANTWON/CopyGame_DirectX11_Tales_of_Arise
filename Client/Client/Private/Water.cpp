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

	m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, XMVectorSet(0.f, 1.f, 0.f, 1.f));

	return S_OK;
}

int CWater::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return OBJ_NOEVENT;
	m_fScrollingTimer += fTimeDelta;

	return OBJ_NOEVENT;
}

void CWater::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return;

	if (m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLENDLIGHTS, this);
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
	/* For.Com_Navigation */
	/*if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_SNOWFIELD, TEXT("Prototype_Component_Navigation"), (CComponent**)&m_pNavigationCom)))
		return E_FAIL;*/
	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Water_Plane"), (CComponent**)&m_pModelCom)))
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

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_fScrollingTimer", &m_fScrollingTimer, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CWater::Compute_ReflectionView()
{
	_float4 vPosition;
	XMStoreFloat4(&vPosition, m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION));

	_float fWaterHeight = vPosition.y; 
	
	_float4x4 CameraWorldMatrix = CGameInstance::Get_Instance()->Get_CamWorldMatrix();
	_float4 CameraLook = (_float4)CameraWorldMatrix.m[2];
	_float4 CameraPosition = (_float4)CameraWorldMatrix.m[3];

	/* Inverse */
	_vector InvCameraLook = XMVectorSetW(XMLoadFloat3(&_float3(CameraLook.x, -CameraLook.y, CameraLook.z)), 0.f);		
	_vector InvCameraPosition = XMVectorSetW(XMLoadFloat3(&_float3(CameraPosition.x, CameraPosition.y - fWaterHeight, CameraPosition.z)), 0.f);

	_vector InvCameraUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector vRight = XMVector3Cross(InvCameraLook, InvCameraUp);
	vRight = XMVectorSetW(vRight, 0.f);

	InvCameraUp = XMVector3Cross(InvCameraLook, vRight);
	InvCameraUp = XMVectorSetW(InvCameraUp, 0.f);

	/* Inversed Reflection View Matrix */
	m_ReflectionViewMatrix = XMMatrixLookAtLH(InvCameraPosition, InvCameraLook, InvCameraUp);
}

CWater * CWater::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CWater* pInstance = new CWater(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CWater"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CWater::Clone(void * pArg)
{
	CWater* pInstance = new CWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CWater"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWater::Free()
{
	__super::Free();

	//Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pWaveHeightMapCom);
}