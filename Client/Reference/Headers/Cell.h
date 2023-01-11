#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum CELLTYPE { ACCESSIBLE, ONLYJUMP, DROP, UPDOWN };

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	const _float3* Get_Point(POINT ePoint) { return &m_vPoints[ePoint]; }
	_float3 Get_PointValue(POINT ePoint) const { return m_vPoints[ePoint]; }

	void SetUp_Neighbor(LINE eLine, const CCell* pCell)
	{
		m_iNeighborIndices[eLine] = pCell->m_iIndex;
	}


public:
	HRESULT		Initialize(const _float3* pPoints, _int iIndex);
	_bool		Compare_Points(const _float3* pSourPoint, const _float3* pDestPoint);
	_bool		isIn(_fvector vPosition, _int* pNeighborIndex, _float3* vLastNormal = nullptr);

	void		Set_CellType(CELLTYPE eType) { m_eCellType = eType; }
	CELLTYPE	Get_CellType() { return m_eCellType;}
	_vector		Get_Center();
	_int		Get_Index() { return m_iIndex; }


#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;



private:
	_int					m_iIndex = 0;
	_float3					m_vPoints[POINT_END];
	_float3					m_vNormals[LINE_END];
	_int					m_iNeighborIndices[LINE_END] = { -1, -1, -1 };
	

	CELLTYPE				m_eCellType = ACCESSIBLE;
private:
#ifdef _DEBUG
	class CVIBuffer_Navigation*		m_pVIBuffer = nullptr;
#endif // _DEBUG
public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
	virtual void Free() override;
};

END