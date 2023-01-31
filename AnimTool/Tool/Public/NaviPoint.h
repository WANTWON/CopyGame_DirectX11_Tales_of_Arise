#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CVIBuffer_Navigation;
END

BEGIN(Client)

class CNaviPoint final : public CGameObject
{
private:
	CNaviPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNaviPoint(const CNaviPoint& rhs);
	virtual ~CNaviPoint() = default;

public:
	const _float3* Get_Points(void) const { return m_vPoints; }
	const _float3* Get_Originals(void) const { return m_vOriginal; }
	void Set_Choice(void) { m_isChoice = true; }
	void Reset_Choice(void) { m_isChoice = false; }

public:
	void Change_Cell(void); 

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render(void);

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Navigation* m_pVIBufferCom = nullptr;

private:
	_float3 m_vPoints[3];
	_float3 m_vOriginal[3];
	_float3 m_vNormal;

	_bool m_isChoice = false;

private:
	HRESULT Ready_Components(void);
	HRESULT SetUp_ShaderResources(void);
	void Sort_Points(void);

public:
	static CNaviPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};

END