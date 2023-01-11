#pragma once

#include "Base.h"

BEGIN(Engine)

class CQuadTree final : public CBase
{
public:
	enum CHILD { CHILD_LT, CHILD_RT, CHILD_RB, CHILD_LB, CHILD_END };
	enum CORNER { CORNER_LT, CORNER_RT, CORNER_RB, CORNER_LB, CORNER_END };
private:
	CQuadTree();
	virtual ~CQuadTree() = default;

public:
	HRESULT Initialize(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	void Culling(const class CFrustum*	pFrustum, _float3* pVerticesPos, FACEINDICES32* pIndices, _uint* pNumFaces);

private:
	_uint					m_iCorners[CORNER_END] = { 0 };
	CQuadTree*				m_pChilds[CHILD_END] = { nullptr };

	_uint					m_iCenter;
	_float					m_fRadius;


public:
	static CQuadTree* Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB);
	virtual void Free();
};

END