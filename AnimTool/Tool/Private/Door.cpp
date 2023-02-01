#include "stdafx.h"
#include "..\Public\Door.h"

#include "GameInstance.h"

CDoor::CDoor(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	ZeroMemory(&m_tMove, sizeof(DOORDESC));
}

CDoor::CDoor(const CDoor& rhs)
	: CGameObject(rhs)
	, m_tMove(rhs.m_tMove)
	, m_iDoorType(rhs.m_iDoorType)
{
}

HRESULT CDoor::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_tMove, pArg, sizeof(DOORDESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(m_tMove.tPivot.vScale);

	if (m_tMove.tPivot.vRotation.x != 0)
		m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), m_tMove.tPivot.vRotation.x);
	if (m_tMove.tPivot.vRotation.y != 0)
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_tMove.tPivot.vRotation.y);
	if (m_tMove.tPivot.vRotation.z != 0)
		m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), m_tMove.tPivot.vRotation.z);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_tMove.tPivot.vPosition);

	return S_OK;
}

void CDoor::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_tMove.tPivot.vScale = m_pTransformCom->Get_Scale();
	XMStoreFloat3(&m_tMove.tPivot.vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	m_pAABBCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CDoor::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CDoor::Render(void)
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

#ifdef _DEBUG
	m_pAABBCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CDoor::Change_Collider(_float3 vScale)
{
	if (nullptr == m_pAABBCom)
		return;

	m_pAABBCom->Change_Scale(vScale);

	m_tMove.ColliderSize = vScale;
}

HRESULT CDoor::Ready_Components(void)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = m_tMove.ColliderSize;
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model*/
	switch (m_tMove.eType)
	{
	case NON_CABINET:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cabinet"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 2;
		break;
	case NON_CDOOR_LEFT:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Classroom_Door_Left"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 0;
		break;
	case NON_CDOOR_RIGHT:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Classroom_Door_Right"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 0;
		break;
	case NON_TOILET_DOOR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Toilet_Door"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 1;
		break;
	case NON_CABINET_BIG_LEFT:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cabinet_Big_Left"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 2;
		break;
	case NON_CABINET_BIG_RIGHT:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Cabinet_Big_Right"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 2;
		break;
	case NON_EXIT_DOOR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Exit_Door"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 1;
		break;
	case NON_IRON_DOOR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Iron_Door"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 1;
		break;
	case NON_KEYBOX_DOOR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Keybox_Door"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 2;
		break;
	case NON_NEW_DOOR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_New_Door"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 1;
		break;
	case NON_SECOND_DOOR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Second_Door"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 1;
		break;
	case NON_SHOWCASE_DOOR_LEFT:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Showcase_Door_Left"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 2;
		break;
	case NON_SHOWCASE_DOOR_RIGHT:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Showcase_Door_Right"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 2;
		break;
	case NON_TOILET_IN_DOOR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Toilet_In_Door"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 1;
		break;
	case NON_REGISTER_DOOR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Register_Door"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		m_tMove.iDoorType = m_iDoorType = 1;
		break;
	}

	return S_OK;
}

HRESULT CDoor::SetUp_ShaderResources(void)
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CDoor * CDoor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDoor* pInstnace = new CDoor(pDevice, pContext);

	if (FAILED(pInstnace->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CNonMovableObject"));
		Safe_Release(pInstnace);
	}

	return pInstnace;
}

CGameObject * CDoor::Clone(void * pAvg)
{
	CDoor* pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pAvg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CNonMovableObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free(void)
{
	__super::Free();

	Safe_Release(m_pAABBCom);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
