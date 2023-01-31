#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CModel;
class CNavigation;
END

BEGIN(Client)

class CMeshMap final : public CGameObject
{
public:
	typedef struct tagMapDesc
	{
		MAP eType;
		_float4x4 matWorld;
	}MAPDESC;
private:
	CMeshMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshMap(const CMeshMap& rhs);
	virtual ~CMeshMap() = default;

public:
	MAP Get_MapType(void) const { return m_tMapDesc.eType; }

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
	CModel* m_pModelCom = nullptr;
	CNavigation* m_pNavigationCom = nullptr;

private:
	_bool m_bPick;

	MAPDESC m_tMapDesc;

private:
	HRESULT Ready_Components(void);
	HRESULT SetUp_ShaderResources(void);

public:
	static CMeshMap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};

END