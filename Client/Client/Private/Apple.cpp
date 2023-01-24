#include "stdafx.h"
#include "..\Public\Apple.h"
#include "Player.h"
#include "Level_Manager.h"
#include "CameraManager.h"

CApple::CApple(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInteractObject(pDevice, pContext)
{
}

CApple::CApple(const CApple & rhs)
	: CInteractObject(rhs)
{

}

HRESULT CApple::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CApple::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);

	_matrix PivotMatrix = XMMatrixIdentity()*XMMatrixScaling(0.1f, 0.1f, 0.1f);

	

	_vector vPosition = *(_vector*)pArg;
	

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 3.5f);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, 3.5f);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, 3.5f);
	
	return S_OK;
}

int CApple::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);


	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CApple::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
	if (!pPlayer)
		return;


	m_bCollision = m_pSPHERECom->Collision(pPlayer->Get_Collider());
	
	if (m_bCollision)
	{
		m_bIsGain = true;
	}
		
}

HRESULT CApple::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();


	if (false == m_bIsGain)
	{
		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;

			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
				return E_FAIL;
		}
	}


	return S_OK;
}

HRESULT CApple::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_ShadowLightView(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	//for(_uint i = 0; i < iNumMeshes; ++i)
	//{
	//	if (FAILED(m_pModelCom->Render(m_pShaderCom, i, SHADER_ANIMSHADOW)))
	//		return S_OK;
	//}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CApple * CApple::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CApple* pInstance = new CApple(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CApple"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CApple::Clone(void * pArg)
{
	CApple* pInstance = new CApple(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CApple"));
		Safe_Release(pInstance);
	}



	return pInstance;
}

void CApple::Free()
{
	__super::Free();

}

HRESULT CApple::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Apple"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.40f, 0.40f, 0.40f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;
	

	return S_OK;
}

_bool CApple::Is_AnimationLoop(_uint eAnimId)
{
	return false;
}


