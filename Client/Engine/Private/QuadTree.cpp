#include "..\Public\QuadTree.h"
#include "Frustum.h"

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

void CQuadTree::Culling(const CFrustum*	pFrustum, _float3* pVerticesPos, FACEINDICES32* pIndices, _uint* pNumFaces )
{
	/* 더이상 분할 될 수 없는 쿼드트,리였다. */
	if (nullptr == m_pChilds[CHILD_LT])
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

		/* 오른쪽 위 삼각형이 그려져야해? */
		if (true == isIn[0] &&
			true == isIn[1] &&
			true == isIn[2])
		{
			pIndices[*pNumFaces]._0 = iIndices[0];
			pIndices[*pNumFaces]._1 = iIndices[1];
			pIndices[*pNumFaces]._2 = iIndices[2];
			(*pNumFaces)++;

		}

		/* 왼쪽 아래 삼각형이 그려져야해? */
		if (true == isIn[0] &&
			true == isIn[2] &&
			true == isIn[3])
		{
			pIndices[*pNumFaces]._0 = iIndices[0];
			pIndices[*pNumFaces]._1 = iIndices[2];
			pIndices[*pNumFaces]._2 = iIndices[3];
			(*pNumFaces)++;
		}

		return;
	}



	_float		fRadius = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVerticesPos[m_iCorners[CORNER_LT]]) - XMLoadFloat3(&pVerticesPos[m_iCenter])));

	if (true == pFrustum->isIn_LocalFrustum(XMLoadFloat3(&pVerticesPos[m_iCenter]), fRadius))
	{
		for (_uint i = 0; i < CHILD_END; ++i)
		{
			if (nullptr != m_pChilds[i])
				m_pChilds[i]->Culling(pFrustum, pVerticesPos, pIndices, pNumFaces);
		}
	}

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
}
