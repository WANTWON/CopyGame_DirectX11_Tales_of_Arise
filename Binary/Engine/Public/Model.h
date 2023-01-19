#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_float4x4 Get_PivotFloat4x4(void) const { return m_PivotMatrix; }

public:
	virtual HRESULT Initialize_Prototype(TYPE eModelType, const char* pModelFilePath, const char* pMaterialPath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

private:
	const aiScene* m_pAIScene = nullptr;
	Assimp::Importer m_Importer;

private:
	TYPE m_eModelType = TYPE_END;

	_uint m_iNumMeshes = 0;
	
private:
	_uint m_iNumMaterials = 0;

private:
	_uint m_iNumBones = 0;

private:
	_uint m_iNumAnimations = 0;

private:
	_float4x4 m_PivotMatrix;

	_float m_fCurrentTime = 0.f;

private:
	void Save_Hierarchy(HANDLE hFile, _ulong* pdwByte, const aiNode* pNode);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eModelType, const char* pModelFilePath, const char* pMaterialPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

END