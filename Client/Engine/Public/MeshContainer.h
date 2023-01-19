#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

/* 모델을 구성하는 각각의 메시다. */
class CMeshContainer final : public CVIBuffer
{
private:
	CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

	void Get_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix);

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, HANDLE hFile, _ulong* pdwByte, class CModel* pModel, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Bin_Initialize_Prototype(CModel::TYPE eModelType, DATA_BINMESH* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix);	// 추가
	virtual HRESULT Bin_Initialize(void* pArg);	// 추가
	char* Get_Name() { return m_szName; }

public:
	HRESULT SetUp_Bones(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);
	HRESULT Bin_SetUp_Bones(class CModel* pModel, DATA_BINMESH* pAIMesh); // 추가
	_bool Picking(CTransform * pTransform, _float3 * pOut);

private:
	char						m_szName[MAX_PATH] = "";
	const aiMesh*				m_pAIMesh = nullptr;
	_uint						m_iMaterialIndex = 0;

	/* 이 메시에 영향을 주는 뼈들의 갯수 .*/
	_uint							m_iNumBones = 0;
	vector<class CHierarchyNode*>	m_Bones;

public:
	HRESULT Create_VertexBuffer_NonAnimModel(const aiMesh* pAIMesh, _fmatrix PivotMatrix);
	HRESULT Create_VertexBuffer_AnimModel(const aiMesh* pAIMesh, class CModel* pModel);

	HRESULT Create_VertexBuffer_NonAnimModel(HANDLE hFile, _ulong* pdwByte, _fmatrix PivotMatrix);
	HRESULT Create_VertexBuffer_AnimModel(HANDLE hFile, _ulong* pdwByte, class CModel* pModel);

private:	// 추가
	HRESULT Bin_Create_VertexBuffer_NonAnimModel(DATA_BINMESH* pAIMesh, _fmatrix PivotMatrix);
	HRESULT Bin_Create_VertexBuffer_AnimModel(DATA_BINMESH* pAIMesh, CModel* pModel);


	// For. Data 추가
public:
	void Get_MeshData(DATA_BINMESH* pMeshData);

private:	// 추가
	VTXMODEL*			m_pNonAnimVertices = nullptr;
	VTXANIMMODEL*		m_pAnimVertices = nullptr;
	FACEINDICES32*		m_pIndices = nullptr;
	_bool				m_bIsProto = false;

public:
	//static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix);
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, HANDLE hFile, _ulong* pdwByte, CModel* pModel, _fmatrix PivotMatrix);
	static CMeshContainer* Bin_Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, DATA_BINMESH* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix); // 추가
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END