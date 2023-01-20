#include "..\Public\QuadTree.h"
#include "Frustum.h"
#include "PipeLine.h"

CQuadTree::CQuadTree()
{
}

HRESULT CQuadTree::Initialize(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	m_iCorners[CORNER_LT] = iLT;
	m_iCorners[CORNER_RT] = iRT;
	m_iCorners[CORNER_RB] = iRB;
	m_iCorners[CORNER_LB] = iLB;

	if (1 == m_iCorners[CORNER_RT] - m_iCorners[CORNER_LT])
		return S_OK;

	m_iCenter = (iLT + iRB) >> 1;

	_uint			iLC, iTC, iRC, iBC;

	iLC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_LB]) >> 1;
	iTC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RT]) >> 1;
	iRC = (m_iCorners[CORNER_RT] + m_iCorners[CORNER_RB]) >> 1;
	iBC = (m_iCorners[CORNER_LB] + m_iCorners[CORNER_RB]) >> 1;

	m_pChilds[CHILD_LT] = CQuadTree::Create(m_iCorners[CORNER_LT], iTC, m_iCenter, iLC);
	m_pChilds[CHILD_RT] = CQuadTree::Create(iTC, m_iCorners[CORNER_RT], iRC, m_iCenter);
	m_pChilds[CHILD_RB] = CQuadTree::Create(m_iCenter, iRC, m_iCorners[CORNER_RB], iBC);
	m_pChilds[CHILD_LB] = CQuadTree::Create(iLC, m_iCenter, iBC, m_iCorners[CORNER_LB]);
		
	return S_OK;
}

void CQuadTree::Culling(const CFrustum*	pFrustum, _float3* pVerticesPos, _fmatrix WorldMatrixInv, FACEINDICES32* pIndices, _uint* pNumFaces )
{
	/* 더이상 분할 될 수 없는 쿼드트,리였다. */
	if (nullptr == m_pChilds[CHILD_LT] || 
		true == isDraw_LOD(pVerticesPos, WorldMatrixInv))
	{
		_uint		iIndices[4] = {
			m_iCorners[CORNER_LT], 
			m_iCorners[CORNER_RT],
			m_iCorners[CORNER_RB],
			m_iCorners[CORNER_LB],
		};

		_bool		isIn[4] = {
			pFrustum->isIn_LocalFrustum(XMLoadFloat3(&pVerticesPos[iIndices[0]]), 0.f),
			pFrustum->isIn_LocalFrustum(XMLoadFloat3(&pVerticesPos[iIndices[1]]), 0.f),
			pFrustum->isIn_LocalFrustum(XMLoadFloat3(&pVerticesPos[iIndices[2]]), 0.f),
			pFrustum->isIn_LocalFrustum(XMLoadFloat3(&pVerticesPos[iIndices[3]]), 0.f)
		};

		_bool		isDraw[NEIGHBOR_END] = { true, true, true, true };

		if(nullptr != m_pNeighbors[NEIGHBOR_LEFT])
			isDraw[NEIGHBOR_LEFT] = m_pNeighbors[NEIGHBOR_LEFT]->isDraw_LOD(pVerticesPos, WorldMatrixInv);

		if (nullptr != m_pNeighbors[NEIGHBOR_TOP])
			isDraw[NEIGHBOR_TOP] = m_pNeighbors[NEIGHBOR_TOP]->isDraw_LOD(pVerticesPos, WorldMatrixInv);

		if (nullptr != m_pNeighbors[NEIGHBOR_RIGHT])
		isDraw[NEIGHBOR_RIGHT] = m_pNeighbors[NEIGHBOR_RIGHT]->isDraw_LOD(pVerticesPos, WorldMatrixInv);

		if (nullptr != m_pNeighbors[NEIGHBOR_BOTTOM])
			isDraw[NEIGHBOR_BOTTOM] = m_pNeighbors[NEIGHBOR_BOTTOM]->isDraw_LOD(pVerticesPos, WorldMatrixInv);

		if (true == isDraw[NEIGHBOR_LEFT] &&
			true == isDraw[NEIGHBOR_TOP] &&
			true == isDraw[NEIGHBOR_RIGHT] &&
			true == isDraw[NEIGHBOR_BOTTOM])
		{
			/* 오른쪽 위 삼각형이 그려져야해? */
			if (true == isIn[0] ||
				true == isIn[1] ||
				true == isIn[2])
			{
				pIndices[*pNumFaces]._0 = iIndices[0];
				pIndices[*pNumFaces]._1 = iIndices[1];
				pIndices[*pNumFaces]._2 = iIndices[2];
				(*pNumFaces)++;

			}

			/* 왼쪽 아래 삼각형이 그려져야해? */
			if (true == isIn[0] ||
				true == isIn[2] ||
				true == isIn[3])
			{
				pIndices[*pNumFaces]._0 = iIndices[0];
				pIndices[*pNumFaces]._1 = iIndices[2];
				pIndices[*pNumFaces]._2 = iIndices[3];
				(*pNumFaces)++;
			}
			return;
		}

		_uint		iLC, iTC, iRC, iBC;

		iLC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_LB]) >> 1;
		iTC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RT]) >> 1;
		iRC = (m_iCorners[CORNER_RT] + m_iCorners[CORNER_RB]) >> 1;
		iBC = (m_iCorners[CORNER_LB] + m_iCorners[CORNER_RB]) >> 1;

		if (true == isIn[0] ||
			true == isIn[2] ||
			true == isIn[3])
		{

			if (false == isDraw[NEIGHBOR_LEFT])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = iLC;
				(*pNumFaces)++;

				pIndices[*pNumFaces]._0 = iLC;
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_LB];
				(*pNumFaces)++;
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_LB];
				(*pNumFaces)++;
			}

			if (false == isDraw[NEIGHBOR_BOTTOM])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LB];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = iBC;
				(*pNumFaces)++;

				pIndices[*pNumFaces]._0 = iBC;
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_RB];
				(*pNumFaces)++;
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LB];
				pIndices[*pNumFaces]._1 = m_iCenter;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_RB];
				(*pNumFaces)++;
			}
		}

		if (true == isIn[0] ||
			true == isIn[1] ||
			true == isIn[2])
		{
		
			if (false == isDraw[NEIGHBOR_RIGHT])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_RT];
				pIndices[*pNumFaces]._1 = iRC;
				pIndices[*pNumFaces]._2 = m_iCenter;
				(*pNumFaces)++;

				pIndices[*pNumFaces]._0 = m_iCenter;
				pIndices[*pNumFaces]._1 = iRC;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_RB];
				(*pNumFaces)++;
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_RT];
				pIndices[*pNumFaces]._1 = m_iCorners[CORNER_RB];
				pIndices[*pNumFaces]._2 = m_iCenter;
				(*pNumFaces)++;
			}

			if (false == isDraw[NEIGHBOR_TOP])
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = iTC;
				pIndices[*pNumFaces]._2 = m_iCenter;
				(*pNumFaces)++;

				pIndices[*pNumFaces]._0 = m_iCenter;
				pIndices[*pNumFaces]._1 = iTC;
				pIndices[*pNumFaces]._2 = m_iCorners[CORNER_RT];
				(*pNumFaces)++;
			}
			else
			{
				pIndices[*pNumFaces]._0 = m_iCorners[CORNER_LT];
				pIndices[*pNumFaces]._1 = m_iCorners[CORNER_RT];
				pIndices[*pNumFaces]._2 = m_iCenter;
				(*pNumFaces)++;
			}
		}

		return;
	}

	_float		fRadius = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVerticesPos[m_iCorners[CORNER_LT]]) - XMLoadFloat3(&pVerticesPos[m_iCenter])));

	if (true == pFrustum->isIn_LocalFrustum(XMLoadFloat3(&pVerticesPos[m_iCenter]), fRadius))
	{
		for (_uint i = 0; i < CHILD_END; ++i)
		{
			if (nullptr != m_pChilds[i])
				m_pChilds[i]->Culling(pFrustum, pVerticesPos, WorldMatrixInv, pIndices, pNumFaces);
		}
	}

}

void CQuadTree::SetUp_Neighbors()
{
	if (nullptr == m_pChilds[CHILD_LT]->m_pChilds[CHILD_LT])
		return;

	m_pChilds[CHILD_LT]->m_pNeighbors[NEIGHBOR_RIGHT] = m_pChilds[CHILD_RT];
	m_pChilds[CHILD_LT]->m_pNeighbors[NEIGHBOR_BOTTOM] = m_pChilds[CHILD_LB];

	m_pChilds[CHILD_RT]->m_pNeighbors[NEIGHBOR_LEFT] = m_pChilds[CHILD_LT];
	m_pChilds[CHILD_RT]->m_pNeighbors[NEIGHBOR_BOTTOM] = m_pChilds[CHILD_RB];

	m_pChilds[CHILD_RB]->m_pNeighbors[NEIGHBOR_LEFT] = m_pChilds[CHILD_LB];
	m_pChilds[CHILD_RB]->m_pNeighbors[NEIGHBOR_TOP] = m_pChilds[CHILD_RT];

	m_pChilds[CHILD_LB]->m_pNeighbors[NEIGHBOR_RIGHT] = m_pChilds[CHILD_RB];
	m_pChilds[CHILD_LB]->m_pNeighbors[NEIGHBOR_TOP] = m_pChilds[CHILD_LT];

	if (nullptr != m_pNeighbors[NEIGHBOR_RIGHT])
	{
		m_pChilds[CHILD_RT]->m_pNeighbors[NEIGHBOR_RIGHT] = m_pNeighbors[NEIGHBOR_RIGHT]->m_pChilds[CHILD_LT];
		m_pChilds[CHILD_RB]->m_pNeighbors[NEIGHBOR_RIGHT] = m_pNeighbors[NEIGHBOR_RIGHT]->m_pChilds[CHILD_LB];
	}

	if (nullptr != m_pNeighbors[NEIGHBOR_BOTTOM])
	{
		m_pChilds[CHILD_LB]->m_pNeighbors[NEIGHBOR_BOTTOM] = m_pNeighbors[NEIGHBOR_BOTTOM]->m_pChilds[CHILD_LT];
		m_pChilds[CHILD_RB]->m_pNeighbors[NEIGHBOR_BOTTOM] = m_pNeighbors[NEIGHBOR_BOTTOM]->m_pChilds[CHILD_RT];
	}


	if (nullptr != m_pNeighbors[NEIGHBOR_LEFT])
	{
		m_pChilds[CHILD_LT]->m_pNeighbors[NEIGHBOR_LEFT] = m_pNeighbors[NEIGHBOR_LEFT]->m_pChilds[CHILD_RT];
		m_pChilds[CHILD_LB]->m_pNeighbors[NEIGHBOR_LEFT] = m_pNeighbors[NEIGHBOR_LEFT]->m_pChilds[CHILD_RB];
	}

	if (nullptr != m_pNeighbors[NEIGHBOR_TOP])
	{
		m_pChilds[CHILD_LT]->m_pNeighbors[NEIGHBOR_TOP] = m_pNeighbors[NEIGHBOR_TOP]->m_pChilds[CHILD_LB];
		m_pChilds[CHILD_RT]->m_pNeighbors[NEIGHBOR_TOP] = m_pNeighbors[NEIGHBOR_TOP]->m_pChilds[CHILD_RB];
	}

	for (_uint i = 0; i < CHILD_END; ++i)
	{
		if (nullptr != m_pChilds[i])
			m_pChilds[i]->SetUp_Neighbors();
	}

}

_bool CQuadTree::isDraw_LOD(const _float3 * pVerticesPos, _fmatrix WorldMatrixInv)
{
	CPipeLine*			pPipeLine = GET_INSTANCE(CPipeLine);

	_vector				vCamPosition = XMVector3TransformCoord(XMLoadFloat4(&pPipeLine->Get_CamPosition()), WorldMatrixInv);

	_vector			vDir = vCamPosition - XMLoadFloat3(&pVerticesPos[m_iCenter]);

	RELEASE_INSTANCE(CPipeLine);

	_float			fDistance = XMVectorGetX(XMVector3Length(vDir));

	_float			fWidth = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVerticesPos[m_iCorners[CORNER_RT]]) - XMLoadFloat3(&pVerticesPos[m_iCorners[CORNER_LT]])));

	if (fDistance * 0.2f > fWidth)
		return true;

	else
		return false;	
}

CQuadTree * CQuadTree::Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	CQuadTree*	pInstance = new CQuadTree();

	if (FAILED(pInstance->Initialize(iLT, iRT, iRB, iLB)))
	{
		ERR_MSG(TEXT("Failed to Created : CQuadTree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuadTree::Free()
{
	for (auto& pQuadTree : m_pChilds)
		Safe_Release(pQuadTree);
}
