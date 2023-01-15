#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "NonAnim.h"
#include "Cell.h"

BEGIN(Engine)
class CCell;
class CShader;
END


BEGIN(Client)
class CNavigation_Manager final : public CBase
{
	DECLARE_SINGLETON(CNavigation_Manager)

public:
	enum SYMBOL { SYMBOL1,  SYMBOL2, SYMBOLEND};


public:
	CNavigation_Manager();
	virtual ~CNavigation_Manager() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Render();

public:
	_uint Get_CellsSize() { return (_uint)m_Cells.size(); }
	CCell* Get_Cell();
	CCell* Get_Cell(_uint iIndex);
	_float3 Get_ClickedPos() { return m_vClickedPos; }
	_int Get_CurrentCellIndex() { return m_iClickedCellIndex; }
	void Set_CilckedCellIndex(_uint iIndex) { m_iClickedCellIndex = iIndex; }
	void Set_CellType(CCell::CELLTYPE eType);
	CCell::CELLTYPE Get_SellType() { return m_Cells[m_iClickedCellIndex]->Get_CellType(); }

public:
	HRESULT Add_ClickedSymbol(_float3 vClickPos, SYMBOL Symboltype);
	HRESULT Add_Cell(_float3* vPoss, CCell::CELLTYPE eType, _bool bCheckOverlap = false);
	void Click_Position(_vector vPosition);
	void Clear_ClickedPosition();
	void Update_ClickedPosition(_float3 position);
	void Clear_Cells();
	void Cancle_Cell();
	void Erase_Cell();
	void Sort_CellByPosition(_float3 * vPoss);
	void Sort_CellByDot(_float3 * vPoss);
	CCell* Find_PickingCell(_vector vPickingPos);

private:
	_float3 Find_MinDistance(_vector vPosition);
	_bool Check_Sell(_float3 * vPoss);

private:
	_uint				m_iClickedCellIndex = 0;
	_float3				m_vClickedPos;
	_float				m_fMinDistance = MAX_NUM;
	CCell::CELLTYPE		m_eCellType = CCell::DEFAULT;

private:
	vector<class CCell*>			m_Cells;
	typedef vector<class CCell*>	CELLS;
	CNonAnim*						m_pClickedSymbol[SYMBOLEND] = {nullptr};
	vector<_float3>					m_vClickedPoints;

private:  /*For Render */
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	class CShader*			m_pShader = nullptr;

public:
	virtual void Free() override;
};

END