#pragma once

#include "Effect.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CEffectMesh final : public CEffect
{
public:
	typedef struct tagEffectDescription
	{
		EFFECT_TYPE eType = TYPE_END;

		_tchar wcFileName[MAX_PATH] = TEXT("");				// "Effect.dds" ~ "Effect.png" ~ "Effect.fbx" 
		_tchar wcFilePath[MAX_PATH] = TEXT("");				// "../../../Bin/Resources/Textures/Effect/*" ~ "../../../Bin/Resources/Meshes/Effect/*"

		_tchar wcTexturePrototypeId[MAX_PATH] = TEXT("");
		_tchar wcModelPrototypeId[MAX_PATH] = TEXT("");
	} EFFECTDESC;

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
	/* Mesh Effect */
	CModel* m_pModelCom = nullptr;

public:
	static CEffectMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END