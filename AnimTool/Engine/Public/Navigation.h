#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	enum MOVE { MOVE_IN, MOVE_OUT, MOVE_SLIDE, MOVE_END };
public:
	typedef struct tagNavigationDesc
	{
		_int iCurrentCellIndex = -1;
	}NAVIDESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationData);
	virtual HRESULT Initialize(void* pArg);

public:
	_int isMove(_float3* vAfterPosition, _float3* vPrePosition);

#ifdef _DEBUG
public:
	HRESULT Render_Navigation(void);
#endif // _DEBUG

private:
	NAVIDESC m_tNaviDesc;
	vector<class CCell*> m_Cells;

#ifdef _DEBUG
private:
	class CShader* m_pShader = nullptr;
#endif // _DEBUG

private:
	HRESULT SetUp_Neighbor(void);

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationData);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};

END