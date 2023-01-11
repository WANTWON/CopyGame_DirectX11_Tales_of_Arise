#include "..\Public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "PipeLine.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif // _DEBUG
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif // _DEBUG
}

HRESULT CNavigation::Initialize_Prototype(const _tchar * pNavigationData)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pNavigationData, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;
	_uint iNum = 0;
	CCell::CELLTYPE eCelltype;
	_float3		vPoints[3];

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
		ReadFile(hFile, &eCelltype, sizeof(CCell::CELLTYPE), &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell*			pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (int)m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
		m_Cells.back()->Set_CellType(eCelltype);
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;


#ifdef _DEBUG

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_Navigation.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

#endif // _DEBUG
	return S_OK;
}

HRESULT CNavigation::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_NaviDesc, pArg, sizeof m_NaviDesc);



	return S_OK;
}

_float CNavigation::Compute_Height(_vector vPosition, _float foffset)
{
	if (m_NaviDesc.iCurrentCellIndex == -1)
		return XMVectorGetY(vPosition);

	_vector PointA = XMLoadFloat3(&m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_PointValue(CCell::POINT_A));
	PointA = XMVectorSetW(PointA, 1.f);
	_vector PointB = XMLoadFloat3(&m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_PointValue(CCell::POINT_B));
	PointB = XMVectorSetW(PointB, 1.f);
	_vector PointC = XMLoadFloat3(&m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_PointValue(CCell::POINT_C));
	PointC = XMVectorSetW(PointC, 1.f);

	_vector vPlane = XMPlaneFromPoints(PointA, PointB, PointC);

	// _float		fHeight = (-ax - cz - d) / b;
	_float		fHeight = (-XMVectorGetX(vPlane) * XMVectorGetX(vPosition) - XMVectorGetZ(vPlane) *XMVectorGetZ(vPosition) - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane) + foffset;

	return fHeight;
}

void CNavigation::Compute_CurrentIndex_byDistance(_vector vPosition)
{
	int	iIndexNum = 0;
	_float fMinDistance = 9999;
	_float fDistance;

	for (int i = 0; i < m_Cells.size(); ++i)
	{
		fDistance = XMVectorGetX(XMVector3Length(vPosition - m_Cells[i]->Get_Center()));

		if (fMinDistance > fDistance)
		{
			fMinDistance = fDistance;
			iIndexNum = i;
		}
	}

	m_NaviDesc.iCurrentCellIndex = iIndexNum;
}

_bool CNavigation::Compute_CurrentIndex_byHeight(_vector vPosition)
{
	_int iNeighborIndex = -1;

	//셀을 순회하여 내 위치에 xz로 셀이 존재하는 지 확인한다.
	for (int i = 0; i < m_Cells.size(); ++i)
	{


		if (m_Cells[i]->isIn(vPosition, &iNeighborIndex, &m_vLastNormal))
			m_ForComputeCells.push_back(m_Cells[i]);


	}

	//존재하지 않다면 false
	if (m_ForComputeCells.size() == 0)
		return false;
	else
	{
		_float fMinDistance = 9999;
		_float fDistance;

		for (int i = 0; i < m_ForComputeCells.size(); ++i)
		{
			if (XMVectorGetY(vPosition) < XMVectorGetY(m_ForComputeCells[i]->Get_Center()))
				continue;

			fDistance = XMVectorGetX(XMVector3Length(vPosition - m_ForComputeCells[i]->Get_Center()));

			if (fMinDistance > fDistance)
			{
				fMinDistance = fDistance;
				iNeighborIndex = m_ForComputeCells[i]->Get_Index();
			}
		}
		m_NaviDesc.iCurrentCellIndex = iNeighborIndex;
		m_ForComputeCells.clear();
		return true;
	}
}

_bool CNavigation::isMove(_fvector vPosition)
{
	if (m_bIs2D)
		return isMove2D(vPosition);
	else
		return isMove3D(vPosition);
}

_bool CNavigation::isMove3D(_fvector vPosition)
{

	if (m_NaviDesc.iCurrentCellIndex == -1)
	{
		Compute_CurrentIndex_byDistance(vPosition);
	}

	_int		iNeighborIndex = -1;

	/* 현재 존재하는 쎌안에서 움직였다. */
	if (true == m_Cells[m_NaviDesc.iCurrentCellIndex]->isIn(vPosition, &iNeighborIndex, &m_vLastNormal))
		return true;

	/* 현재 존재하는 쎌을 벗어난다.  */
	else
	{
		/* 나간 방향에 이웃셀이 존재한다. */
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iNeighborIndex, &m_vLastNormal))
					break;
			}

			m_NaviDesc.iCurrentCellIndex = iNeighborIndex;
			return true;
		}

		/* 나간 방향에 이웃셀이 존재하지않는다. . */
		else
		{
			/*슬라이딩을 위한 리턴을 정의해도 된다. */
			return false;
		}
	}
	return false;

}

_bool CNavigation::isMove2D(_fvector vPosition)
{
	if (m_NaviDesc.iCurrentCellIndex == -1)
	{
		return Compute_CurrentIndex_byHeight(vPosition);
	}

	_int		iNeighborIndex = -1;

	/* 현재 존재하는 쎌안에서 움직였다. */
	if (true == m_Cells[m_NaviDesc.iCurrentCellIndex]->isIn(vPosition, &iNeighborIndex, &m_vLastNormal))
		return true;

	/* 현재 존재하는 쎌을 벗어난다.  */
	else
	{
		/* 나간 방향에 이웃셀이 존재한다. */
		if (0 <= iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
				{
					return Compute_CurrentIndex_byHeight(vPosition);
				}

				if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iNeighborIndex, &m_vLastNormal))
					break;
			}

			m_NaviDesc.iCurrentCellIndex = iNeighborIndex;
			m_ForComputeCells.clear();
			return true;
		}
		else
			return Compute_CurrentIndex_byHeight(vPosition);
	}
}

_uint CNavigation::Get_CurrentCelltype()
{
	if (m_NaviDesc.iCurrentCellIndex == -1)
		return false;

	return (_uint)m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_CellType();
}

_vector CNavigation::Get_CurrentCellCenter()
{
	return m_Cells[m_NaviDesc.iCurrentCellIndex]->Get_Center();
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	_float4x4			WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());


	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	m_pShader->Set_RawValue("g_ViewMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4));
	m_pShader->Set_RawValue("g_ProjMatrix", &pPipeLine->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4));

	RELEASE_INSTANCE(CPipeLine);

	if (-1 == m_NaviDesc.iCurrentCellIndex)
	{
		m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));


		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
			{
				switch (pCell->Get_CellType())
				{
				case Engine::CCell::ACCESSIBLE:
					m_pShader->Set_RawValue("g_vColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4));
					break;
				case Engine::CCell::ONLYJUMP:
					m_pShader->Set_RawValue("g_vColor", &_float4(0.f, 0.f, 1.f, 1.f), sizeof(_float4));
					break;
				case Engine::CCell::DROP:
					m_pShader->Set_RawValue("g_vColor", &_float4(0.7f, 0.f, 1.f, 1.f), sizeof(_float4));
					break;
				case Engine::CCell::UPDOWN:
					m_pShader->Set_RawValue("g_vColor", &_float4(0.0f, 1.f, 1.f, 1.f), sizeof(_float4));
					break;
				default:
					break;
				}
				m_pShader->Begin(0);
				pCell->Render();
			}
		}
	}

	else
	{
		WorldMatrix._24 = 0.1f;
		m_pShader->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4));
		m_pShader->Set_RawValue("g_vColor", &_float4(1.f, 1.f, 1.f, 1.f), sizeof(_float4));

		m_pShader->Begin(0);
		m_Cells[m_NaviDesc.iCurrentCellIndex]->Render();
	}

	return S_OK;
}
#endif // _DEBUG

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_AB, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_BC, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pNavigationData)
{
	CNavigation*	pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationData)))
	{
		ERR_MSG(TEXT("Failed to Created : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*	pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif // _DEBUG

}
