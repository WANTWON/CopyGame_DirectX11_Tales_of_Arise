#pragma once

#include "VIBuffer_Instance.h"
#include "Model.h"

BEGIN(Engine)

class CMeshContainer_Instance final : public CVIBuffer_Instance
{
private:
	CMeshContainer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshContainer_Instance(const CMeshContainer_Instance& rhs);
	virtual ~CMeshContainer_Instance() = default;

public:
	_uint Get_MaterialIndex(void) const { return m_iMaterialIndex; }

public:
	void Get_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, HANDLE hFile, _ulong* pdwByte, const char* pDataFilePath, class CModel* pModel, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT SetUp_Bones(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);
	void Tick(_float fRaius = 3.f);

private:
	char m_szName[MAX_PATH] = "";
	_uint m_iMaterialIndex = 0;

	/* 이 메시에 영향을 주는 뼈들의 갯수 .*/
	_uint m_iNumBones = 0;
	vector<class CHierarchyNode*> m_Bones;

	VTXMATRIX* m_pInstance = nullptr;
	_uint m_iOriginalInstance = 0;

public:
	static CMeshContainer_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, HANDLE hFile, _ulong* pdwByte, const char* pDataFilePath, CModel* pModel, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};

END