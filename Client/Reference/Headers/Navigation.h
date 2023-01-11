#pragma once

#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int			iCurrentCellIndex = -1;
	}NAVIDESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationData);
	virtual HRESULT Initialize(void* pArg);
	_float	Compute_Height(_vector vPosition, _float foffset);
	void	Compute_CurrentIndex_byDistance(_vector vPosition);
	_bool	Compute_CurrentIndex_byHeight(_vector vPosition);


public:
	_bool	isMove(_fvector vPosition);
	_bool	isMove3D(_fvector vPosition);
	_bool	isMove2D(_fvector vPosition);
	_int	Get_CurrentCellIndex() {return m_NaviDesc.iCurrentCellIndex;}
	_uint	Get_CurrentCelltype();
	_vector Get_CurrentCellCenter();
	_vector Get_LastNormal() { return XMLoadFloat3(&m_vLastNormal); }
	void	Set_2DNaviGation(_bool type) { m_bIs2D = true; }


#ifdef _DEBUG
public:
	virtual HRESULT Render() override;
#endif // _DEBUG

private:
	NAVIDESC				m_NaviDesc;
	vector<class CCell*>	m_Cells;
	_float3					m_vLastNormal = _float3(0.f, 0.f, 0.f);
	_bool					m_bIs2D = false;

	/* x z가 같고 높이가 다른 셀이 존재할 수도 있어서 다른 컨테이너에 담아두고*/
	/* 해당 벡터를 돌아서 높이 비교로 현재 셀을 확정 시킬 것*/
	vector<class CCell*>	m_ForComputeCells;

#ifdef _DEBUG
private:
	class CShader*			m_pShader = nullptr;
#endif // _DEBUG

private:
	HRESULT SetUp_Neighbor();

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationData);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END