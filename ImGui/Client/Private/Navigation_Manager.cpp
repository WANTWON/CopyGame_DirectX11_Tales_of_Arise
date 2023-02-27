#include "stdafx.h"
#include "..\Public\Navigation_Manager.h"
#include "GameInstance.h"
#include "Cell.h"
#include "VIBuffer_Navigation.h"
#include "Picking.h"

IMPLEMENT_SINGLETON(CNavigation_Manager)


CNavigation_Manager::CNavigation_Manager()
{
}

HRESULT CNavigation_Manager::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	if (nullptr == pDevice || nullptr == pContext)
		return E_FAIL;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_Cells.reserve(0);

	return S_OK;
}


void CNavigation_Manager::Click_Position(_vector vPosition)
{
	_float3 vClickPosition;

	//셀이 1개 이상 있으면 점을 찍을 때 가장 가까운 점이 있는지 확인.
	//찍은 점의 위치가 기존 셀의 점의 위치와 비슷하다면 해당 점을 찍게한다.
	vClickPosition = Find_MinDistance(vPosition);

	//만약 순회해서 찾은 셀의 점이 거리가 멀다면 그냥 기존에 찍은 점을 pushback하게 한다.
	if (m_fMinDistance > 0.7f)
		XMStoreFloat3(&vClickPosition, vPosition);

	m_fMinDistance = MAX_NUM;
	if (2 < m_vClickedPoints.size())
		return;

	if (0 == m_vClickedPoints.size())
		Add_ClickedSymbol(vClickPosition, SYMBOL1);
	else if (1 == m_vClickedPoints.size())
		Add_ClickedSymbol(vClickPosition, SYMBOL2);

	m_vClickedPoints.push_back(vClickPosition);
	m_vClickedPos = vClickPosition;

	//찍은 점의 개수가 3개가 된다면. 삼각형이 그려지니 Cell에 추가한다.
	if (3 == m_vClickedPoints.size())
	{
		_float3 vPoss[3];
		for (_int i = 0; i < 3; ++i)
			vPoss[i] = m_vClickedPoints[i];

		Add_Cell(vPoss, m_eCellType, true);
		Clear_ClickedPosition();
	}
}

void CNavigation_Manager::Clear_ClickedPosition()
{
	
	CGameInstance::Get_Instance()->Clear_Layer(LEVEL_GAMEPLAY, TEXT("Layer_PickingSymbol"));

	m_vClickedPoints.clear();
	m_fMinDistance = MAX_NUM;
}

void CNavigation_Manager::Update_ClickedPosition(_float3 position)
{
	if (m_vClickedPoints.size() != 0)
	{
		m_vClickedPoints.back() = position;
		_vector vPosition = XMLoadFloat3(&position);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pClickedSymbol[m_vClickedPoints.size() - 1]->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
		
}

HRESULT CNavigation_Manager::Add_ClickedSymbol(_float3 vClickPos, SYMBOL Symboltype)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	NONANIMDESC NonAnimDesc;
	char cModelTag[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, 0, TEXT("Picking_Symbol"), MAX_PATH, cModelTag, MAX_PATH, NULL, NULL);
	strcpy(NonAnimDesc.pModeltag, cModelTag);
	NonAnimDesc.vPosition = vClickPos;
	NonAnimDesc.vScale = _float3(0.5, 0.5, 0.5);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_GAMEPLAY, TEXT("Layer_PickingSymbol"), &NonAnimDesc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
		

	m_pClickedSymbol[Symboltype] = dynamic_cast<CNonAnim*>(pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_PickingSymbol"), Symboltype));
	m_pClickedSymbol[Symboltype]->Setting_PickingSymbol(Symboltype);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CNavigation_Manager::Add_Cell(_float3 * vPoss, CCell::CELLTYPE eType, _bool bCheckOverlap)
{
	// 시계 방향으로 정리한다 0 1 2
	Sort_CellByPosition(vPoss);
	// 마우스 Lay와 Cell의 법선 벡터를 내적하여 2차 Sort를 한다.
	Sort_CellByDot(vPoss);

	if (Check_Sell(vPoss))
		return E_FAIL;

	// 추가한다.
	CCell*			pCell = CCell::Create(m_pDevice, m_pContext, vPoss, (_int)m_Cells.size());
	if (nullptr == pCell)
		return E_FAIL;
	m_Cells.push_back(pCell);
	m_Cells.back()->Set_CellType(eType);
	return S_OK;
}

void CNavigation_Manager::Clear_Cells()
{
	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();
}

void CNavigation_Manager::Cancle_Cell()
{
	if (m_Cells.size() == 0)
		return;

	Safe_Release(m_Cells.back());
	m_Cells.pop_back();
}

void CNavigation_Manager::Erase_Cell()
{
	auto& iter = m_Cells.begin();
	while (iter != m_Cells.end())
	{
		if (*iter == m_Cells[m_iClickedCellIndex])
		{
			iter = m_Cells.erase(iter);
			return;
		}
		else
			++iter;
	}
}

CCell * CNavigation_Manager::Find_PickingCell(_vector vPickingPos)
{
	_float fMinDistance = MAX_NUM;
	CCell* pMinCell = nullptr;

	for (_uint i = 0; i < m_Cells.size(); ++i)
	{

		CCell* pCell = m_Cells[i];
		if (nullptr == pCell)
			continue;

		_vector vCenter = pCell->Get_Center();
		_float fDistance = XMVectorGetX(XMVector3Length(vCenter - vPickingPos));

		if (fMinDistance > fDistance)
		{
			fMinDistance = fDistance;
			pMinCell = m_Cells[i];
			m_iClickedCellIndex = i;
		}
	}

	if (fMinDistance != MAX_NUM)
	{
 		return pMinCell;
	}
	return nullptr;
}

_float3 CNavigation_Manager::Find_MinDistance(_vector vPosition)
{
	// 셀이 없다면 무조건 그냥 찍은 좌표로 준다.
	_float3 vClickPosition;
	XMStoreFloat3(&vClickPosition, vPosition);

	if (m_Cells.size() == 0)
		return vClickPosition;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	XMVECTOR		vRayDir, vRayPos;

	vRayDir = pGameInstance->Get_RayDir();
	vRayDir = XMVector3Normalize(vRayDir);
	vRayPos = pGameInstance->Get_RayPos();
	vRayPos = XMVectorSetW(vRayPos, 1.f);

	//셀을 순회한다.
	for (_uint i = 0; i < m_Cells.size(); ++i)
	{
		CCell* pCell = m_Cells[i];
		if (nullptr == pCell)
			continue;

		_vector vPoint[CCell::POINT_END];

		//셀마다 각 포인트 A,B,C를 받아온다
		vPoint[CCell::POINT_A] = XMLoadFloat3(&pCell->Get_PointValue(CCell::POINT_A));
		vPoint[CCell::POINT_A] = XMVectorSetW(vPoint[CCell::POINT_A], 1.f);
		vPoint[CCell::POINT_B] = XMLoadFloat3(&pCell->Get_PointValue(CCell::POINT_B));
		vPoint[CCell::POINT_B] = XMVectorSetW(vPoint[CCell::POINT_B], 1.f);
		vPoint[CCell::POINT_C] = XMLoadFloat3(&pCell->Get_PointValue(CCell::POINT_C));
		vPoint[CCell::POINT_C] = XMVectorSetW(vPoint[CCell::POINT_C], 1.f);


		//내가 피킹한 마우스 Postion과 셀과의 거리를 구한다.
		//그 중에서 가장 작은 값을 체크
		_float fDis_A = XMVectorGetX(XMVector3Length(vPoint[CCell::POINT_A] - vPosition));
		_float fDis_B = XMVectorGetX(XMVector3Length(vPoint[CCell::POINT_B] - vPosition));
		_float fDis_C = XMVectorGetX(XMVector3Length(vPoint[CCell::POINT_C] - vPosition));
		_float fMinDistance = min(min(fDis_A, fDis_B), fDis_C);

		//만약 이전에 순회한 셀보다 거리가 크다면 continue,
		//더 작다면 해당 점을 click 한 점으로 Store해준다.
		if (m_fMinDistance < fMinDistance)
			continue;

		m_fMinDistance = fMinDistance;
		if (fDis_A == m_fMinDistance)
			XMStoreFloat3(&vClickPosition, vPoint[CCell::POINT_A]);
		else if (fDis_B == m_fMinDistance)
			XMStoreFloat3(&vClickPosition, vPoint[CCell::POINT_B]);
		else if (fDis_C == m_fMinDistance)
			XMStoreFloat3(&vClickPosition, vPoint[CCell::POINT_C]);

	}

	RELEASE_INSTANCE(CGameInstance);

	return vClickPosition;
}

void CNavigation_Manager::Sort_CellByPosition(_float3 * vPoss)
{
	//Position 기준으로 1단계로 시계방향으로 정리한다 
	_float3 vTempPoss[3];

	memcpy(vTempPoss, vPoss, sizeof(_float3) * 3);

	//3개의 점을 순회해서 x 좌표가 가장 적은 인덱스를 찾는다.
	for (_uint i = 0; i < 2; ++i)
	{
		_uint iMinIndex = i;
		for (_uint j = i + 1; j < 3; ++j)
		{
			
			if (vTempPoss[iMinIndex].x > vTempPoss[j].x)
			{
				iMinIndex = j;
			}
		}

		_float3 Temp;
		Temp = vTempPoss[i];
		vTempPoss[i] = vTempPoss[iMinIndex];
		vTempPoss[iMinIndex] = Temp;
	}

	// 중간 x값을 찾는다. [1]  
	// 작은 x값을 찾는다. [0]
	// 만약 작은 x값의 z값이 중간 x값의 z값보다 작다면 반대로
	if (vTempPoss[0].z < vTempPoss[1].z)
	{
		vPoss[0] = vTempPoss[1];
		vPoss[1] = vTempPoss[2];
		vPoss[2] = vTempPoss[0];
	}
	else
	{
		vPoss[0] = vTempPoss[1];
		vPoss[1] = vTempPoss[0];
		vPoss[2] = vTempPoss[2];
	}
}

void CNavigation_Manager::Sort_CellByDot(_float3 * vPoss)
{
	//각 선분을 외적해서 면이 바라보는 방향벡터를 구한다.
	_vector vABDir = XMLoadFloat3(&vPoss[1]) - XMLoadFloat3(&vPoss[0]);
	_vector vBCDir = XMLoadFloat3(&vPoss[2]) - XMLoadFloat3(&vPoss[1]);
	_vector vCloss = XMVector3Cross(vABDir, vBCDir);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vMouseDir = pGameInstance->Get_RayDir() * -1.f;
	vCloss = XMVector3Normalize(vCloss);
	vMouseDir = XMVector3Normalize(vMouseDir);

	// 면의 방향벡터와 마우스의 방향벡터를 뒤집은 벡터를 내적
	// 양수면 뒤집어준다.
	if (0.f > XMVectorGetX(XMVector3Dot(vCloss, vMouseDir)))
	{
		_float3 vTemp = vPoss[0];
		vPoss[0] = vPoss[2];
		vPoss[2] = vTemp;
	}

	RELEASE_INSTANCE(CGameInstance);
}

_bool CNavigation_Manager::Check_Sell(_float3 * vPoss)
{
	for (_uint i = 0; i < m_Cells.size(); ++i)
	{
		if (nullptr == m_Cells[i])
			continue;

		_float3 vPoint[CCell::POINT_END];

		vPoint[CCell::POINT_A] = m_Cells[i]->Get_PointValue(CCell::POINT_A);
		vPoint[CCell::POINT_B] = m_Cells[i]->Get_PointValue(CCell::POINT_B);
		vPoint[CCell::POINT_C] = m_Cells[i]->Get_PointValue(CCell::POINT_C);

		if (XMVector3Equal(XMLoadFloat3(&vPoss[0]), XMLoadFloat3(&vPoint[CCell::POINT_A]))
			&& XMVector3Equal(XMLoadFloat3(&vPoss[1]), XMLoadFloat3(&vPoint[CCell::POINT_B]))
			&& XMVector3Equal(XMLoadFloat3(&vPoss[2]), XMLoadFloat3(&vPoint[CCell::POINT_C])))
		{
			return true;
		}
	}

	return false;
}


HRESULT CNavigation_Manager::Render()
{
	if (m_Cells.size() == 0)
		return S_OK;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	_float4x4			WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	if (FAILED(m_pShader->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShader->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	_float3 vCamPos = *(_float3*)&pGameInstance->Get_CamPosition();

	RELEASE_INSTANCE(CGameInstance);


#ifdef _DEBUG
	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
		{

			m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));

			CCell::CELLTYPE eType = pCell->Get_CellType();
			
			switch (eType)
			{
			case Engine::CCell::DEFAULT:
				m_pShader->Set_RawValue("g_vColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4));
				break;
			case Engine::CCell::ONLYJUMP:
				m_pShader->Set_RawValue("g_vColor", &_float4(0.f, 0.f, 1.f, 1.f), sizeof(_float4));
				break;
			default:
				break;
			}

			
			m_pShader->Begin(0);
			pCell->Render();
		}

	}
#endif // _DEBUG
	return S_OK;
}

CCell * CNavigation_Manager::Get_Cell()
{
	if (m_Cells.size() == 0 || m_iClickedCellIndex >= m_Cells.size())
		return nullptr;

	return m_Cells[m_iClickedCellIndex];
}

CCell * CNavigation_Manager::Get_Cell(_uint iIndex)
{
	if (m_Cells.size() == 0 || iIndex >= m_Cells.size())
		return nullptr;

	return m_Cells[iIndex];
}

void CNavigation_Manager::Set_CellType(CCell::CELLTYPE eType)
{
	if(m_Cells.size() != 0)
		m_Cells[m_iClickedCellIndex]->Set_CellType(eType);  
	m_eCellType = eType; 
}

void CNavigation_Manager::Free()
{
	Clear_Cells();


	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pShader);
}
