#include "stdafx.h"
#include "..\Public\PlayerHandle.h"
#include "GameInstance.h"

CPlayerHandle::CPlayerHandle(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	ZeroMemory(&m_tHandleDesc, sizeof(HANDLEDESC));
}

CPlayerHandle::CPlayerHandle(const CPlayerHandle & rhs)
	: CGameObject(rhs)
	, m_tHandleDesc(rhs.m_tHandleDesc)
{
}

void CPlayerHandle::Collision(void)
{
	if (nullptr != m_pOBBCom)
		m_pOBBCom->Tick(XMLoadFloat4x4(&m_CombinedWorldMatrix));
}

HRESULT CPlayerHandle::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CPlayerHandle::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	if (nullptr != pArg)
		memcpy(&m_tHandleDesc, pArg, sizeof(HANDLEDESC));
	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CPlayerHandle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_matrix SocketMatrix = /*m_tHandleDesc.pSocket->Get_OffsetMatrix() **/
		m_tHandleDesc.pSocket->Get_CombinedTransformationMatrix() *
		XMLoadFloat4x4(&m_tHandleDesc.SocketPivotMatrix) * XMLoadFloat4x4(m_tHandleDesc.pParentWorldMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_CombinedWorldMatrix, SocketMatrix);
}

void CPlayerHandle::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CPlayerHandle::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;
	
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	
	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	if (nullptr != m_pOBBCom)
		m_pOBBCom->Render();

	return S_OK;
}

HRESULT CPlayerHandle::Ready_Components(void)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	
	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	
	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, m_tHandleDesc.pModelTag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC ColliderDesc;

	/* For.Com_Sphere */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.25f, 0.25f, 3.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, -2.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerHandle::SetUp_ShaderResources(void)
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_float4x4 WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_CombinedWorldMatrix)));
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPlayerHandle * CPlayerHandle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayerHandle* pInstance = new CPlayerHandle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayerHandle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayerHandle::Clone(void * pArg)
{
	CPlayerHandle* pInstance = new CPlayerHandle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPlayerHandle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerHandle::Free(void)
{
	__super::Free();

	Safe_Release(m_tHandleDesc.pSocket);
	
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}
