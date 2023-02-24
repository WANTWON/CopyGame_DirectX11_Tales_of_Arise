#include "stdafx.h"
#include "..\Public\NonAnim.h"
#include "Imgui_Manager.h"
#include "PickingMgr.h"
#include "GameInstance.h"
#include "Camera_Manager.h"

CNonAnim::CNonAnim(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CNonAnim::CNonAnim(const CNonAnim & rhs)
	: CBaseObj(rhs)
{
	memcpy(&m_ModelDesc, &rhs.m_ModelDesc , sizeof(NONANIMDESC));
}

HRESULT CNonAnim::Initialize_Prototype()
{
	//ZeroMemory(&m_ModelDesc, sizeof(NONANIMDESC));
	XMStoreFloat4x4(&m_ModelDesc.WorldMatrix, XMMatrixIdentity());
	return S_OK;
}

HRESULT CNonAnim::Initialize(void * pArg)
{
	

	if(pArg != nullptr)
		memcpy(&m_ModelDesc, pArg, sizeof(NONANIMDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if(CImgui_Manager::Get_Instance()->Get_PickingType() != CImgui_Manager::PICKING_TERRAIN_SHAPE &&
		strcmp(m_ModelDesc.pModeltag, "Picking_Symbol"))
		CPickingMgr::Get_Instance()->Add_PickingGroup(this);

	m_eObjectID = OBJ_BLOCK;

	if (pArg != nullptr)
	{
		
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_ModelDesc.WorldMatrix));
		_vector vPosition = XMLoadFloat3(&m_ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), m_ModelDesc.vRotation.x);
		if(m_ModelDesc.vRotation.y != 1.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_ModelDesc.vRotation.y);
		m_pTransformCom->Turn(XMVectorSet(0.f,0.f,1.f,0.f), m_ModelDesc.vRotation.z);
		Set_Scale(m_ModelDesc.vScale);

		if (m_ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMLoadFloat3(&m_ModelDesc.vRotation), XMConvertToRadians(m_ModelDesc.m_fAngle));

	}
	

	if (CImgui_Manager::Get_Instance()->Get_PickingType() == CImgui_Manager::PICKING_CAMERA)
	{
		CCamera_Action::TOOLDESC CameraDesc =  CImgui_Manager::Get_Instance()->Get_CameraToolDesc();
		m_iSymbolType =  CCamera_Manager::Get_Instance()->Get_SymBolType();

		if (m_iSymbolType == 0)
		{
			XMStoreFloat4(&CameraDesc.vEyePosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
			CCamera_Manager::Get_Instance()->Add_CameraSymbol(CCamera_Manager::CAM_EYE, this);
		}			
		else if (m_iSymbolType == 1)
		{
			XMStoreFloat4(&CameraDesc.vAtPosition, m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
			CCamera_Manager::Get_Instance()->Add_CameraSymbol(CCamera_Manager::CAM_AT, this);
		}
			

		CImgui_Manager::Get_Instance()->Set_CameraToolDesc(CameraDesc);
	}	
	else
		CModelManager::Get_Instance()->Add_CreatedModel(this);



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
	//if (CGameInstance::Get_Instance()->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), Get_Scale().y*2) == false)
	//	return;

	if (nullptr != m_pRendererCom && CImgui_Manager::Get_Instance()->Get_ShowOnlyNavi() == false)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

	//Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));

	XMStoreFloat3(&m_ModelDesc.vPosition, Get_Position());
	m_ModelDesc.vScale = Get_Scale();
	m_ModelDesc.WorldMatrix = m_pTransformCom->Get_World4x4();
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
		if (m_iSymbolType != -1)
		{
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, m_iSymbolType)))
				return E_FAIL;

			if (m_iSymbolType == 0)
			{
				m_eShaderID = SHADER_SYMBOL;
				if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &XMVectorSet(1.f,1.f,0.f,1.f), sizeof(_float4))))
					return E_FAIL;
			}
			else if (m_iSymbolType == 1)
			{
				m_eShaderID = SHADER_SYMBOL;
				if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &XMVectorSet(1.f, 0.f, 1.f, 1.f), sizeof(_float4))))
					return E_FAIL;
			}
				
		}
		else
		{
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
				return E_FAIL;
		}
		

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}

	return S_OK;
}

_bool CNonAnim::Picking(_float3 * PickingPoint)
{
	/*if (CGameInstance::Get_Instance()->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), Get_Scale().y * 2) == false)
		return false;*/

	CImgui_Manager::PICKING_TYPE PickingType = CImgui_Manager::Get_Instance()->Get_PickingType();
	if (PickingType == CImgui_Manager::PICKING_TERRAIN_BRUSH ||
		PickingType == CImgui_Manager::PICKING_TERRAIN_TRANSFORM ||
		PickingType == CImgui_Manager::PICKING_TERRAIN_SHAPE)
		return false;
	
//	if (true == m_pModelCom->Picking(m_pTransformCom, PickingPoint))
//		return true; 

	return false;
}

void CNonAnim::PickingTrue()
{
	CImgui_Manager::PICKING_TYPE ePickingtype = CImgui_Manager::Get_Instance()->Get_PickingType();

	switch (ePickingtype)
	{
	case Client::CImgui_Manager::PICKING_MODEL:
		Set_Picked();
		break;
	case Client::CImgui_Manager::PICKING_TERRAIN_SHAPE:
		break;
	default:
		break;
	}
}

void CNonAnim::Set_Picked()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Mouse_Up(DIMK::DIMK_LBUTTON))
	{
		if (m_bPicked)
		{
			CModelManager::Get_Instance()->Set_AllPickedFalse();
			CPickingMgr::Get_Instance()->Set_PickedObj(nullptr);
		}	
		else
		{
			CModelManager::Get_Instance()->Set_AllPickedFalse();
			CPickingMgr::Get_Instance()->Set_PickedObj(this);
		}
			
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CNonAnim::Turn(_float3 vAxis, _float fAngle)
{

	_float fInputAngle = fAngle;
	if (fInputAngle > 180)
		fInputAngle = 360 - fInputAngle;

	if(vAxis.x != 0)
		m_ModelDesc.vRotation.x = fInputAngle;
	if (vAxis.y != 0)
		m_ModelDesc.vRotation.y = fInputAngle;
	if (vAxis.z != 0)
		m_ModelDesc.vRotation.z = fInputAngle;

	m_pTransformCom->Set_Rotation(m_ModelDesc.vRotation);
	
}

HRESULT CNonAnim::Ready_Components(void* pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	TransformDesc.fRotationPerSec = 1.f;
	TransformDesc.fSpeedPerSec = 1.f;

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar			szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_ModelDesc.pModeltag, strlen(m_ModelDesc.pModeltag), szModeltag, MAX_PATH);
	
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, szModeltag, (CComponent**)&m_pModelCom)))
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

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CNonAnim::SetUp_ShaderID()
{
	//_bool m_bWireFrame = CTerrain_Manager::Get_Instance()->Get_TerrainDesc().m_bShowWireFrame;

	//if (m_bWireFrame)
	//	m_eShaderID = SHADER_WIREFRAME;
	 if (m_bPicked)
		m_eShaderID = SHADER_PICKED;
	else
		m_eShaderID = SHADER_DEFAULT;

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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);
}
