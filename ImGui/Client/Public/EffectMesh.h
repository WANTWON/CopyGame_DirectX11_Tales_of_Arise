#pragma once

#include "Effect.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CEffectMesh final : public CEffect
{
public:
	typedef struct tagMeshEffectDescription
	{
		_tchar wcPrototypeId[MAX_PATH] = TEXT("");	/* "Spark.dds > Spark" */
	} MESHEFFECTDESC;

public:
	virtual _tchar* Get_PrototypeId() { return m_tMeshEffectDesc.wcPrototypeId; }

	MESHEFFECTDESC Get_MeshEffectDesc() { return m_tMeshEffectDesc; }
	
public:
	CEffectMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffectMesh(const CEffectMesh& rhs);
	virtual ~CEffectMesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;

private:
	MESHEFFECTDESC  m_tMeshEffectDesc;

	/* Mesh Effect */
	CModel* m_pModelCom = nullptr;

public:
	static CEffectMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END