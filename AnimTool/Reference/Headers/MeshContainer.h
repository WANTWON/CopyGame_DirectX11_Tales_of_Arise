#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMeshContainer final : public CVIBuffer
{
private:
	CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;

public:
	_uint Get_MaterialIndex(void) const { return m_iMaterialIndex; }
	_uint Get_NumBones(void) const { return m_iNumBones; }
	vector<class CHierarchyNode*> Get_Bones(void) const { return m_Bones; }

	void Get_BoneMatricese(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);

public:
	HRESULT Initialize_Prototype_Binary(CModel::TYPE eModelType, HANDLE hFile, _ulong* pdwByte, CModel* pModel, _fmatrix PivotMatrix);

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	virtual _bool Picking(_matrix WorldMatrix, vector<_float3>* vPickPoint);

public:
	HRESULT SetUp_Bones(class CModel* pModel);
	HRESULT SetUp_Bones_Binary(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);

private:
	char m_szName[MAX_PATH] = "";
	const aiMesh* m_pAIMesh = nullptr;
	_uint m_iMaterialIndex = 0;
	 
	_uint m_iNumBones = 0;
	vector<class CHierarchyNode*> m_Bones;

	VTXMODEL* m_pNonAnimVertices = nullptr;
	VTXANIMMODEL* m_pAnimVertices = nullptr;
	FACEINDICES32* m_pIndices = nullptr;

public:
	HRESULT Create_VertexBuffer_NonAnimModel(const aiMesh* pAIMesh, _fmatrix PivotMatrix);
	HRESULT Create_VertexBuffer_AnimModel(const aiMesh* pAIMesh, class CModel* pModel);
	HRESULT Create_VertexBuffer_NonAnim(HANDLE hFile, _ulong* pdwByte, _fmatrix PivotMatrix);
	HRESULT Create_VertexBuffer_Anim(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);

public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, const aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix);
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, HANDLE hFile, _ulong* pdwByte, CModel* pModel, _fmatrix PivotMatrix);
	
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};

END