#include "stdafx.h"
#include "..\Public\NonMovableObject.h"

#include "GameInstance.h"
#include "Imgui_Manager.h"

CNonMovableObject::CNonMovableObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	ZeroMemory(&m_tMove, sizeof(NONMOVE));
}

CNonMovableObject::CNonMovableObject(const CNonMovableObject & rhs)
	: CGameObject(rhs)
	, m_tMove(rhs.m_tMove)
	, m_iDoorType(rhs.m_iDoorType)
{
}

HRESULT CNonMovableObject::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CNonMovableObject::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	memcpy(&m_tMove, pArg, sizeof(NONMOVE));
	
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

void CNonMovableObject::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CNonMovableObject::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CNonMovableObject::Render(void)
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

	return S_OK;
}

HRESULT CNonMovableObject::Ready_Components(void)
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
	
	/* For.Com_Model*/
	switch (m_tMove.eType)
	{
	case NON_CHAIR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Chair"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_DESK:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Desk"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_BLOODBOOK:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BloodBook"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_COIN:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Coin"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_DRIVER:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Driver"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_FIRE:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fire"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_FUSE1:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fuse1"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_FUSE2:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fuse2"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_FUSE3:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fuse3"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_MASTERKEY:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_MasterKey"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_MAGIC:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Magic"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_PAPER:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Paper"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_POISON:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Poison"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_MAP:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Map"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_SOYBEAN:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Soybean"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_SPRAY:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Spray"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_TABLET:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tablet"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_FRIDAY:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Friday"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_THURSDAY:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Thursday"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_OBJECTS:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_BackGround"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NON_VENTILATION_DOOR:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ventilation_Door"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CNonMovableObject::SetUp_ShaderResources(void)
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

CNonMovableObject * CNonMovableObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CNonMovableObject* pInstnace = new CNonMovableObject(pDevice, pContext);

	if (FAILED(pInstnace->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CNonMovableObject"));
		Safe_Release(pInstnace);
	}

	return pInstnace;
}

CGameObject * CNonMovableObject::Clone(void * pAvg)
{
	CNonMovableObject* pInstance = new CNonMovableObject(*this);

	if (FAILED(pInstance->Initialize(pAvg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CNonMovableObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNonMovableObject::Free(void)
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
