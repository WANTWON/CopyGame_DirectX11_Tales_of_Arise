#include "stdafx.h"
#include "..\Public\MeshMap.h"

#include "GameInstance.h"
#include "Imgui_Manager.h"
#include "NonMovableObject.h"
#include "NaviPoint.h"
#include "Room.h"
#include "Door.h"

CMeshMap::CMeshMap(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	ZeroMemory(&m_tMapDesc, sizeof(MAPDESC));
}

CMeshMap::CMeshMap(const CMeshMap & rhs)
	: CGameObject(rhs)
	, m_tMapDesc(rhs.m_tMapDesc)
{
}

HRESULT CMeshMap::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMeshMap::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_tMapDesc, pArg, sizeof(MAPDESC));
	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&m_tMapDesc.matWorld.m[0]);
	m_pTransformCom->Set_State(CTransform::STATE_UP, *(_float3*)&m_tMapDesc.matWorld.m[1]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&m_tMapDesc.matWorld.m[2]);

	return S_OK;
}

void CMeshMap::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CImgui_Manager* pImgui = GET_INSTANCE(CImgui_Manager);

	if (GetKeyState(VK_LBUTTON) < 0 && !m_bPick && !pImgui->Get_WantCaptureMouse())
	{
		m_bPick = true;

		_float3 vPickPoint;

		_vector vCamPos = ((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform")))->Get_State(CTransform::STATE_TRANSLATION);

		if (true == m_pModelCom->Picking(m_pTransformCom->Get_WorldMatrix(), vCamPos, &vPickPoint))
		{
			if (pImgui->m_iNonChoice >= 0)
			{
				CNonMovableObject::NONMOVE NonMove;
				ZeroMemory(&NonMove, sizeof(CNonMovableObject::NONMOVE));

				NonMove.eType = NONOBJECT(pImgui->m_iNonChoice);
				NonMove.tPivot.vPosition = vPickPoint;
				NonMove.tPivot.vScale = _float3(pImgui->m_fPlaceScale[0], pImgui->m_fPlaceScale[1], pImgui->m_fPlaceScale[2]);
				NonMove.tPivot.vRotation = _float3(pImgui->m_fPlaceRotation[0], pImgui->m_fPlaceRotation[1], pImgui->m_fPlaceRotation[2]);

				if (NON_CHAIR == NonMove.eType)
				{
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonMovableObject"), LEVEL_GAMEPLAY, TEXT("Layer_Chair"), &NonMove)))
						return;
				}
				else if (NON_DESK == NonMove.eType)
				{
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonMovableObject"), LEVEL_GAMEPLAY, TEXT("Layer_Desk"), &NonMove)))
						return;
				}
				else if (NON_CABINET == NonMove.eType)
				{
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonMovableObject"), LEVEL_GAMEPLAY, TEXT("Layer_Cabinet_Instance"), &NonMove)))
						return;
				}
				
				else if (NON_CDOOR_LEFT == NonMove.eType || NON_CDOOR_RIGHT == NonMove.eType || NON_TOILET_DOOR == NonMove.eType ||	NON_CABINET_BIG_LEFT == NonMove.eType ||
					NON_CABINET_BIG_RIGHT == NonMove.eType || NON_EXIT_DOOR == NonMove.eType || NON_IRON_DOOR == NonMove.eType || NON_KEYBOX_DOOR == NonMove.eType ||
					NON_NEW_DOOR == NonMove.eType || NON_SECOND_DOOR == NonMove.eType || NON_SHOWCASE_DOOR_LEFT == NonMove.eType || NON_SHOWCASE_DOOR_RIGHT == NonMove.eType ||
					NON_TOILET_IN_DOOR == NonMove.eType || NON_REGISTER_DOOR == NonMove.eType)
				{
					CDoor::DOORDESC DoorDesc;
					ZeroMemory(&DoorDesc, sizeof(CDoor::DOORDESC));

					DoorDesc.eType = NonMove.eType;
					DoorDesc.tPivot = NonMove.tPivot;
					DoorDesc.ColliderSize = _float3(1.f, 1.f, 1.f);


					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonMovableObject"), LEVEL_GAMEPLAY, TEXT("Layer_Door"), &DoorDesc)))
						return;
				}
				else
				{
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonMovableObject"), LEVEL_GAMEPLAY, TEXT("Layer_NonObject"), &NonMove)))
						return;
				}

				pImgui->m_iNonChoice = -1;
			}
			else if (pImgui->m_iChoice >= 0)
			{
				if (0 == pImgui->m_iType)
				{
					PIVOT Pivot;
					ZeroMemory(&Pivot, sizeof(PIVOT));

					Pivot.vPosition = vPickPoint;
					Pivot.vScale = _float3(pImgui->m_fPlaceScale[0], pImgui->m_fPlaceScale[1], pImgui->m_fPlaceScale[2]);
					Pivot.vRotation = _float3(pImgui->m_fPlaceRotation[0], pImgui->m_fPlaceRotation[1], pImgui->m_fPlaceRotation[2]);

					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, TEXT("Layer_Player"), &Pivot)))
						return;
				}
				else if (1 == pImgui->m_iType)
				{
					PIVOT Pivot;
					ZeroMemory(&Pivot, sizeof(PIVOT));

					Pivot.vPosition = vPickPoint;
					Pivot.vScale = _float3(pImgui->m_fPlaceScale[0], pImgui->m_fPlaceScale[1], pImgui->m_fPlaceScale[2]);
					Pivot.vRotation = _float3(pImgui->m_fPlaceRotation[0], pImgui->m_fPlaceRotation[1], pImgui->m_fPlaceRotation[2]);

					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Monster"), LEVEL_GAMEPLAY, TEXT("Layer_Monster"), &Pivot)))
						return;
				}

				pImgui->m_iChoice = -1;
			}
			else if (pImgui->m_isNavigation)
			{
				for (auto& iter : pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_NaviPoint")))
				{
					for (_int i = 0; i < 3; ++i)
					{
						_vector vPick = XMLoadFloat3(&vPickPoint);
						_vector vInList = XMLoadFloat3(&((CNaviPoint*)iter)->Get_Originals()[i]);
						_float fDist = XMVectorGetX(XMVector3Length(vPick - vInList));
						if (0.15f > fDist)
							vPickPoint = ((CNaviPoint*)iter)->Get_Originals()[i];
					}	
				}

				pImgui->m_CellPoints.push_back(vPickPoint);
			}
			else if (pImgui->m_iRoomChoice >= 0)
			{
				CRoom::ROOM Room;
				ZeroMemory(&Room, sizeof(CRoom::ROOM));

				Room.eType = ROOMTYPE(pImgui->m_iRoomChoice);
				Room.tPivot.vPosition = vPickPoint;
				Room.tPivot.vScale = _float3(pImgui->m_fPlaceScale[0], pImgui->m_fPlaceScale[1], pImgui->m_fPlaceScale[2]);
				Room.tPivot.vRotation = _float3(pImgui->m_fPlaceRotation[0], pImgui->m_fPlaceRotation[1], pImgui->m_fPlaceRotation[2]);
				Room.ColliderSize = _float3(1.f, 1.f, 1.f);

				if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Room"), LEVEL_GAMEPLAY, TEXT("Layer_Room"), &Room)))
					return;

				pImgui->m_iRoomChoice = -1;
			}
		}
	}
	else if (GetKeyState(VK_LBUTTON) >= 0 && m_bPick)
		m_bPick = false;

	if (pImgui->m_isCreateNavi)
	{
		/* For.Com_Navigation */
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), (CComponent**)&m_pNavigationCom)))
			return;

		pImgui->m_isCreateNavi = false;
	}
	

	RELEASE_INSTANCE(CImgui_Manager);

	RELEASE_INSTANCE(CGameInstance);
}

void CMeshMap::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMeshMap::Render(void)
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
	if (nullptr != m_pNavigationCom)
		m_pNavigationCom->Render_Navigation();
#endif // _DEBUG

	return S_OK;
}

HRESULT CMeshMap::Ready_Components(void)
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

	/* For.Com_Model */
	switch (m_tMapDesc.eType)
	{
	case MAP_MAIN01:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CMeshMap::SetUp_ShaderResources(void)
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

CMeshMap * CMeshMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext * pContext)
{
	CMeshMap* pInstance = new CMeshMap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshMap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMeshMap::Clone(void * pArg)
{
	CMeshMap* pInstance = new CMeshMap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMeshMap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshMap::Free(void)
{
	__super::Free();

	if (nullptr != m_pNavigationCom)
		Safe_Release(m_pNavigationCom);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
}
