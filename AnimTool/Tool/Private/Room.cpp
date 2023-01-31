#include "stdafx.h"
#include "..\Public\Room.h"

#include "GameInstance.h"

CRoom::CRoom(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	ZeroMemory(&m_tRoom, sizeof(ROOM));
}

CRoom::CRoom(const CRoom & rhs)
	: CGameObject(rhs)
	, m_tRoom(rhs.m_tRoom)
	, m_isRender(rhs.m_isRender)
{
}

HRESULT CRoom::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRoom::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	if (nullptr != pArg)
		memcpy(&m_tRoom, pArg, sizeof(ROOM));
	
	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	m_pTransformCom->Set_Scale(m_tRoom.tPivot.vScale);

	if (m_tRoom.tPivot.vRotation.x != 0)
		m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), m_tRoom.tPivot.vRotation.x);
	if (m_tRoom.tPivot.vRotation.y != 0)
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_tRoom.tPivot.vRotation.y);
	if (m_tRoom.tPivot.vRotation.z != 0)
		m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), m_tRoom.tPivot.vRotation.z);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_tRoom.tPivot.vPosition);

	return S_OK;
}

void CRoom::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_tRoom.tPivot.vScale = m_pTransformCom->Get_Scale();
	XMStoreFloat3(&m_tRoom.tPivot.vPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	m_pAABBCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CRoom::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	/*if (nullptr != m_pAABBCom)
	{
		CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

		CCollider* pPlayerCollider = (CCollider*)pInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB"));

		RELEASE_INSTANCE(CGameInstance);

		if (nullptr == pPlayerCollider)
			m_isRender = false;			

		else if (m_pAABBCom->Collision(pPlayerCollider))
			m_isRender = true;
		else
			m_isRender = false;
	}*/

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CRoom::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (m_isRender)
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

			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
				return E_FAIL;
		}
	}

#ifdef _DEBUG
		m_pAABBCom->Render();
	
#endif // _DEBUG

	return S_OK;
}

void CRoom::Change_Collider(_float3 vScale)
{
	if (nullptr == m_pAABBCom)
		return;
	
	m_pAABBCom->Change_Scale(vScale);

	m_tRoom.ColliderSize = vScale;
}

HRESULT CRoom::Ready_Components(void)
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

	ColliderDesc.vScale = m_tRoom.ColliderSize;
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;
	
	/* For.Com_Model*/
	switch (m_tRoom.eType)
	{
	case ROOM_GYM:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Gym_Room"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_HOUSE:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_House_Room"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_MACHINE_HALLWAY:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Machine_Hallway"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_MACHINE:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Machine_Room"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_1F_HALL:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Main_1f_Hall"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_TEACHER:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Main_1f_Teacher_Room"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2F_HALL:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Main_2f_Hall"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_MOUNTAIN:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Main_2f_Mountain"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_STUDENT:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Main_2f_Student_Room"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_MAIN_HALLWAY:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Main_HallWay"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_MOUNTAIN_HALLWAY:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Mountain_HallWay"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_PHOTO:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Photo_Room"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_1:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_1"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_2:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_2"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_3:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_3"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_4:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_4"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_5:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_5"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_6:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_6"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_7:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_7"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_8:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_8"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_9:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_9"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_10:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_10"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_11:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_11"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2_12:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Room_2_12"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_TOILET_M_1F:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Toilet_M_1f"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_TOILET_M_2F:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Toilet_M_2f"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_TOILET_W_1F:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Toilet_W_1f"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_TOILET_W_2F:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Toilet_W_2f"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ROOM_2F_HALLWAY:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_2f_HallWay"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CRoom::SetUp_ShaderResources(void)
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

CRoom * CRoom::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRoom* pInstnace = new CRoom(pDevice, pContext);

	if (FAILED(pInstnace->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRoom"));
		Safe_Release(pInstnace);
	}

	return pInstnace;
}

CGameObject * CRoom::Clone(void * pAvg)
{
	CRoom* pInstance = new CRoom(*this);

	if (FAILED(pInstance->Initialize(pAvg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRoom"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRoom::Free(void)
{
	__super::Free();

	Safe_Release(m_pAABBCom);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
