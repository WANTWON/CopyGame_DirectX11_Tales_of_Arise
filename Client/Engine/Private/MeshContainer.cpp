#include "..\Public\MeshContainer.h"
#include "HierarchyNode.h"
#include "Picking.h"
#include "Transform.h"

CMeshContainer::CMeshContainer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMeshContainer::CMeshContainer(const CMeshContainer & rhs)
	: CVIBuffer(rhs)
	, m_pAIMesh(rhs.m_pAIMesh)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumBones(rhs.m_iNumBones)
	, m_pAnimVertices(rhs.m_pAnimVertices)	// 추가
	, m_pNonAnimVertices(rhs.m_pNonAnimVertices) // 추가
	, m_pIndices(rhs.m_pIndices) // 추가
	, m_bIsProto(false)	// 추가
	, m_pPickingVertices(rhs.m_pPickingVertices) 
	, m_pPickingIndices(rhs.m_pPickingIndices)
{
	strcpy_s(m_szName, rhs.m_szName);
}

void CMeshContainer::Get_BoneMatrices(_float4x4 * pBoneMatrices, _fmatrix PivotMatrix)
{
	if (0 == m_iNumBones)
	{
		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());
		return;
	}

	_uint		iNumBones = 0;

	for (auto& pBoneNode : m_Bones)
	{
		/* Offset * Combined * Pivot */
		XMStoreFloat4x4(&pBoneMatrices[iNumBones++], /*XMMatrixTranspose*/(pBoneNode->Get_OffsetMatrix() * pBoneNode->Get_CombinedTransformationMatrix() * PivotMatrix));
	}
}

HRESULT CMeshContainer::Initialize_Prototype(CModel::TYPE eModelType, HANDLE hFile, _ulong* pdwByte, class CModel* pModel, _fmatrix PivotMatrix)
{
	/* 메시의 이름을 보관한다. */
	/* 정해진 뼈가 없는 경우에는 최초에는 항등으로 셋팅했었고 이후 메시가 배치되어야할 위치에 셋팅하기위해.
	이 메시의 이름과 같은 노드를 찾아서 저장해놓는다. + 뼈의 갯수로 강제로 1로 늘려놨었다. */
	ReadFile(hFile, m_szName, sizeof(char) * MAX_PATH, pdwByte, nullptr);

	/* 이 메시가 그려질때 맵핑되어야할 재질정보(텍스쳐)를 찾아내기위해. */
	/* 모델이 보관하고 있는 머테리얼에게서 찾아낸다. */
	ReadFile(hFile, &m_iMaterialIndex, sizeof(_uint), pdwByte, nullptr);

#pragma region VERTICES
	HRESULT hr = 0;

	if (CModel::TYPE_NONANIM == eModelType)
		hr = Create_VertexBuffer_NonAnimModel(hFile, pdwByte, PivotMatrix);
	else
		hr = Create_VertexBuffer_AnimModel(hFile, pdwByte, pModel);

	if (FAILED(hr))
		return E_FAIL;
#pragma endregion
#pragma region INDICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	ReadFile(hFile, &m_iNumPrimitive, sizeof(_uint), pdwByte, nullptr);
	m_iNumIndicesPerPrimitive = 3;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	/* 정적버퍼를 생성한다. */
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ulong);

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	m_pPickingIndices = new FACEINDICES32[m_iNumPrimitive];

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		ReadFile(hFile, &pIndices[i]._0, sizeof(_uint), pdwByte, nullptr);
		ReadFile(hFile, &pIndices[i]._1, sizeof(_uint), pdwByte, nullptr);
		ReadFile(hFile, &pIndices[i]._2, sizeof(_uint), pdwByte, nullptr);

		m_pPickingIndices[i]._0 = pIndices[i]._0;
		m_pPickingIndices[i]._1 = pIndices[i]._1;
		m_pPickingIndices[i]._2 = pIndices[i]._2;
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;
	
	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMeshContainer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CMeshContainer::Bin_Initialize_Prototype(CModel::TYPE eModelType, DATA_BINMESH * pAIMesh, CModel * pModel, _fmatrix PivotMatrix)
{
	strcpy_s(m_szName, pAIMesh->cName);
	m_iMaterialIndex = pAIMesh->iMaterialIndex;

#pragma region VERTEXBUFFER

	HRESULT		hr = 0;

	if (CModel::TYPE_NONANIM == eModelType)
		hr = Bin_Create_VertexBuffer_NonAnimModel(pAIMesh, PivotMatrix);
	else
		hr = Bin_Create_VertexBuffer_AnimModel(pAIMesh, pModel);

	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEXBUFFER
	m_iNumPrimitive = pAIMesh->iNumPrimitives;
	m_iIndicesByte = sizeof(FACEINDICES32);
	m_iNumIndicesPerPrimitive = 3;

	/*m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;*/

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);


	FACEINDICES32*		pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._0 = pAIMesh->pIndices[i]._0;
		pIndices[i]._1 = pAIMesh->pIndices[i]._1;
		pIndices[i]._2 = pAIMesh->pIndices[i]._2;
	}


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	m_pIndices = pIndices;

#pragma endregion

	return S_OK;
}

HRESULT CMeshContainer::Bin_Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CMeshContainer::SetUp_Bones(HANDLE hFile, _ulong* pdwByte, class CModel* pModel)
{
	if (nullptr == hFile)
		return E_FAIL;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		char BoneName[MAX_PATH] = "";
		ReadFile(hFile, BoneName, sizeof(char) * MAX_PATH, pdwByte, nullptr);

		CHierarchyNode* pBoneNode = pModel->Get_BonePtr(BoneName);
		if (nullptr == pBoneNode)
			return E_FAIL;

		m_Bones.push_back(pBoneNode);

		Safe_AddRef(pBoneNode);

		_float4x4 OffsetMatrix;
		ReadFile(hFile, &OffsetMatrix, sizeof(_float4x4), pdwByte, nullptr);

		pBoneNode->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
	}

	if (0 == m_iNumBones)
	{
		CHierarchyNode* pNode = pModel->Get_BonePtr(m_szName);
		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;

		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}

HRESULT CMeshContainer::Bin_SetUp_Bones(CModel * pModel, DATA_BINMESH * pAIMesh)
{
	m_iNumBones = pAIMesh->iNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		DATA_BINBONE*		pAIBone = &pAIMesh->pBones[i];

		CHierarchyNode*		pHierarchyNode = pModel->Get_BonePtr(pAIBone->cNames);

		_float4x4			OffsetMatrix;

		memcpy(&OffsetMatrix, &pAIBone->mOffsetTransform, sizeof(_float4x4));

		pHierarchyNode->Set_OffsetMatrix(XMLoadFloat4x4(&OffsetMatrix));

		m_Bones.push_back(pHierarchyNode);

		Safe_AddRef(pHierarchyNode);
	}

	if (0 == m_iNumBones)
	{

		CHierarchyNode*		pNode = pModel->Get_BonePtr(m_szName);

		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;

		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}

_bool CMeshContainer::Picking(CTransform * pTransform, _float3 * pOut)
{
	CPicking*		pPicking = GET_INSTANCE(CPicking);

	_matrix		WorldMatrixInv = pTransform->Get_WorldMatrixInverse();
	_float3			vTempRayDir, vTempRayPos;
	XMVECTOR		vRayDir, vRayPos;
	pPicking->Compute_LocalRayInfo(&vTempRayDir, &vTempRayPos, pTransform);

	vRayPos = XMLoadFloat3(&vTempRayPos);
	vRayPos = XMVectorSetW(vRayPos, 1.f);
	vRayDir = XMLoadFloat3(&vTempRayDir);
	vRayDir = XMVector3Normalize(vRayDir);

	_matrix	WorldMatrix = pTransform->Get_WorldMatrix();
	FACEINDICES32*		pIndices = new FACEINDICES32[m_iNumPrimitive];

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._0 = m_pPickingIndices[i]._0;
		pIndices[i]._1 = m_pPickingIndices[i]._1;
		pIndices[i]._2 = m_pPickingIndices[i]._2;
	}

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{

		_float3 vPosition0 = _float3(0.f, 0.f, 0.f);
		_float3 vPosition1 = _float3(0.f, 0.f, 0.f);
		_float3 vPosition2 = _float3(0.f, 0.f, 0.f);

		memcpy(&vPosition0, &m_pPickingVertices[pIndices[i]._0], sizeof(_float3));
		_vector vTemp_1 = XMLoadFloat3(&vPosition0);
		vTemp_1 = XMVectorSetW(vTemp_1, 1.f);
		memcpy(&vPosition1, &m_pPickingVertices[pIndices[i]._1], sizeof(_float3));
		_vector vTemp_2 = XMLoadFloat3(&vPosition1);
		vTemp_2 = XMVectorSetW(vTemp_2, 1.f);
		memcpy(&vPosition2, &m_pPickingVertices[pIndices[i]._2], sizeof(_float3));
		_vector vTemp_3 = XMLoadFloat3(&vPosition2);
		vTemp_3 = XMVectorSetW(vTemp_3, 1.f);

		_float fDist = 0;

		if (true == TriangleTests::Intersects((FXMVECTOR)vRayPos, (FXMVECTOR)vRayDir, (FXMVECTOR)vTemp_1, (GXMVECTOR)vTemp_2, (HXMVECTOR)vTemp_3, fDist))
		{
			_vector	vPickPos = vRayPos + vRayDir * fDist;

			XMStoreFloat3(pOut, XMVector3TransformCoord(vPickPos, WorldMatrix));

			Safe_Delete_Array(pIndices);
			RELEASE_INSTANCE(CPicking);
			return true;
		}

	}

	Safe_Delete_Array(pIndices);
	RELEASE_INSTANCE(CPicking);
	return false;
}




HRESULT CMeshContainer::Create_VertexBuffer_NonAnimModel(const aiMesh* pAIMesh, _fmatrix PivotMatrix)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXMODEL*			pVertices = new VTXMODEL[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &m_pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &m_pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));


		memcpy(&pVertices[i].vTexUV, &m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &m_pAIMesh->mTangents[i], sizeof(_float3));

	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;
	m_pNonAnimVertices = pVertices;	// 추가
									//Safe_Delete_Array(pVertices);	// 추가

	return S_OK;
}

HRESULT CMeshContainer::Create_VertexBuffer_AnimModel(const aiMesh* pAIMesh, CModel* pModel)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));


	m_iStride = sizeof(VTXANIMMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXANIMMODEL*			pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &m_pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &m_pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &m_pAIMesh->mTangents[i], sizeof(_float3));
	}

	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone*		pAIBone = pAIMesh->mBones[i];

		// pAIBone->mNumWeights : 몇개의 정점에 영향을 주고 있는지? 
		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			/* pAIBone->mWeights[j].mVertexId : 이 뼈가 j번째로 영향을 주는 정점의 인덱스다. */
			_uint		iVertexIndex = pAIBone->mWeights[j].mVertexId;

			if (0 == pVertices[iVertexIndex].vBlendWeight.x)
			{
				pVertices[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.x = i;
			}

			else if (0 == pVertices[iVertexIndex].vBlendWeight.y)
			{
				pVertices[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.y = i;
			}

			else if (0 == pVertices[iVertexIndex].vBlendWeight.z)
			{
				pVertices[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.z = i;
			}

			else if (0 == pVertices[iVertexIndex].vBlendWeight.w)
			{
				pVertices[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.w = i;
			}
		}
	}


	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_pAnimVertices = pVertices;	// 추가

	//Safe_Delete_Array(pVertices);	// 추가

	return S_OK;
}

HRESULT CMeshContainer::Create_VertexBuffer_NonAnimModel(HANDLE hFile, _ulong * pdwByte, _fmatrix PivotMatrix)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXMODEL);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), pdwByte, nullptr);
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
	m_pPickingVertices = new _float3[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		ReadFile(hFile, &pVertices[i].vPosition, sizeof(_float3), pdwByte, nullptr);
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));
		XMStoreFloat3(&m_pPickingVertices[i], XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		ReadFile(hFile, &pVertices[i].vNormal, sizeof(_float3), pdwByte, nullptr);
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		ReadFile(hFile, &pVertices[i].vTexUV, sizeof(_float2), pdwByte, nullptr);
		ReadFile(hFile, &pVertices[i].vTangent, sizeof(_float3), pdwByte, nullptr);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Create_VertexBuffer_AnimModel(HANDLE hFile, _ulong * pdwByte, CModel * pModel)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXANIMMODEL);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), pdwByte, nullptr);
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		ReadFile(hFile, &pVertices[i].vPosition, sizeof(_float3), pdwByte, nullptr);
		ReadFile(hFile, &pVertices[i].vNormal, sizeof(_float3), pdwByte, nullptr);
		ReadFile(hFile, &pVertices[i].vTexUV, sizeof(_float2), pdwByte, nullptr);
		ReadFile(hFile, &pVertices[i].vTangent, sizeof(_float3), pdwByte, nullptr);
	}

	/* 이 메시에 영향을 주는 뼈의 갯수. */
	ReadFile(hFile, &m_iNumBones, sizeof(_uint), pdwByte, nullptr);

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		_uint iNumWeights;
		ReadFile(hFile, &iNumWeights, sizeof(_uint), pdwByte, nullptr);

		// i번째 뼈가 pAIBone->mNumWeights : 몇개의 정점에 영향을 주고 있는지? 
		for (_uint j = 0; j < iNumWeights; ++j)
		{
			/* pAIBone->mWeights[j].mVertexId : 이 뼈가 j번째로 영향을 주는 정점의 인덱스다. */
			_uint iVertexIndex;
			ReadFile(hFile, &iVertexIndex, sizeof(_uint), pdwByte, nullptr);

			if (0 == pVertices[iVertexIndex].vBlendWeight.x)
			{
				ReadFile(hFile, &pVertices[iVertexIndex].vBlendWeight.x, sizeof(_float), pdwByte, nullptr);
				pVertices[iVertexIndex].vBlendIndex.x = i;
			}
			else if (0 == pVertices[iVertexIndex].vBlendWeight.y)
			{
				ReadFile(hFile, &pVertices[iVertexIndex].vBlendWeight.y, sizeof(_float), pdwByte, nullptr);
				pVertices[iVertexIndex].vBlendIndex.y = i;
			}
			else if (0 == pVertices[iVertexIndex].vBlendWeight.z)
			{
				ReadFile(hFile, &pVertices[iVertexIndex].vBlendWeight.z, sizeof(_float), pdwByte, nullptr);
				pVertices[iVertexIndex].vBlendIndex.z = i;
			}
			else if (0 == pVertices[iVertexIndex].vBlendWeight.w)
			{
				ReadFile(hFile, &pVertices[iVertexIndex].vBlendWeight.w, sizeof(_float), pdwByte, nullptr);
				pVertices[iVertexIndex].vBlendIndex.w = i;
			}
		}
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Bin_Create_VertexBuffer_NonAnimModel(DATA_BINMESH * pAIMesh, _fmatrix PivotMatrix)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->NumVertices;
	m_iStride = sizeof(VTXMODEL);
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXMODEL*		pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pVertices[i] = pAIMesh->pNonAnimVertices[i];
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;


	m_pNonAnimVertices = pVertices;

	return S_OK;
}

HRESULT CMeshContainer::Bin_Create_VertexBuffer_AnimModel(DATA_BINMESH * pAIMesh, CModel * pModel)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->NumVertices;
	m_iStride = sizeof(VTXANIMMODEL);
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXANIMMODEL*		pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i], &pAIMesh->pAnimVertices[i], sizeof(VTXANIMMODEL));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_pAnimVertices = pVertices;

	return S_OK;
}

void CMeshContainer::Get_MeshData(DATA_BINMESH * pMeshData)
{
	memcpy(&pMeshData->cName, &m_szName, sizeof(char) * MAX_PATH);
	pMeshData->iMaterialIndex = m_iMaterialIndex;
	pMeshData->NumVertices = m_iNumVertices;
	pMeshData->pNonAnimVertices = m_pNonAnimVertices;
	pMeshData->pAnimVertices = m_pAnimVertices;
	pMeshData->iNumPrimitives = m_iNumPrimitive;
	pMeshData->pIndices = m_pIndices;

	pMeshData->iNumBones = m_iNumBones;
	pMeshData->pBones = new DATA_BINBONE[m_iNumBones];
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		memcpy(&pMeshData->pBones[i].cNames, m_Bones[i]->Get_Name(), sizeof(char) * MAX_PATH);
		pMeshData->pBones[i].mOffsetTransform = m_Bones[i]->Get_OffSetMatrixXM();
		int a = 0;
	}
}


//CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, const aiMesh * pAIMesh, CModel * pModel, _fmatrix PivotMatrix)
//{
//	CMeshContainer*	pInstance = new CMeshContainer(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype(eModelType, pAIMesh, pModel, PivotMatrix)))
//	{
//		ERR_MSG(TEXT("Failed to Created : CMeshContainer"));
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, HANDLE hFile, _ulong * pdwByte, CModel * pModel, _fmatrix PivotMatrix)
{
	CMeshContainer* pInstance = new CMeshContainer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, hFile, pdwByte, pModel, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshContainer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMeshContainer * CMeshContainer::Bin_Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, DATA_BINMESH * pAIMesh, CModel * pModel, _fmatrix PivotMatrix)
{
	CMeshContainer*			pInstance = new CMeshContainer(pDevice, pContext);

	if (FAILED(pInstance->Bin_Initialize_Prototype(eModelType, pAIMesh, pModel, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed To Created : CMeshContainer_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMeshContainer::Clone(void * pArg)
{
	CMeshContainer*	pInstance = new CMeshContainer(*this);

	//if (!pArg)	// 추가
	//{
	//	if (FAILED(pInstance->Bin_Initialize(pArg)))
	//	{
	//		ERR_MSG(TEXT("Failed To Cloned : CMeshContainer_Bin"));
	//		Safe_Release(pInstance);
	//	}
	//}
	//else
	//{
		if (FAILED(pInstance->Initialize(pArg)))
		{
			ERR_MSG(TEXT("Failed To Cloned : CMeshContainer"));
			Safe_Release(pInstance);
		}
	//}

	return pInstance;
}

void CMeshContainer::Free()
{
	__super::Free();

	for (auto& pBoneNode : m_Bones)
		Safe_Release(pBoneNode);

	m_Bones.clear();


	if (m_bIsProto)
	{
		Safe_Delete_Array(m_pPickingVertices);
		Safe_Delete_Array(m_pPickingIndices);
	}
}
