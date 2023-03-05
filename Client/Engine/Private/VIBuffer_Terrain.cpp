#include "..\Public\VIBuffer_Terrain.h"
#include "Frustum.h"
#include "QuadTree.h"
#include "Transform.h"
#include "Picking.h"
#include "Frustum.h"
#include "PhysX.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_pQuadTree(rhs.m_pQuadTree)
{
	Safe_AddRef(m_pQuadTree);
	memcpy(&m_pTerrainDesc, &rhs.m_pTerrainDesc, sizeof(TERRAINDESC));
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticeX, _uint iNumVerticeZ, _float fHeight, _bool bCreateQuadTree)
{
	m_bCreateQuadTree = bCreateQuadTree;
	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath, _bool bCreateQuadTree)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;
	_ulong*					pPixel = nullptr;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_pTerrainDesc.m_iVerticeNumX = m_iNumVerticesX;
	m_pTerrainDesc.m_iVerticeNumZ = m_iNumVerticesZ;

	pPixel = new _ulong[m_iNumVerticesX *m_iNumVerticesZ];
	ReadFile(hFile, pPixel, sizeof(_ulong) * m_iNumVerticesX *m_iNumVerticesZ, &dwByte, nullptr);

#pragma region VERTICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VTXNORTEX*			pVertices = new VTXNORTEX[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];
	
	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_uint	iIndex = i * m_iNumVerticesX + j;

			m_pVerticesPos[iIndex] = pVertices[iIndex].vPosition = _float3( -300.f + _float(j*10), (pPixel[iIndex] & 0x000000ff)/2.f - 10.f, -300.f + _float(i*10));
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTangent = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexture = _float2(j / (m_iNumVerticesX - 1.f), i / (m_iNumVerticesZ - 1.f));
		}
	}
#pragma endregion


#pragma region Indices
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesPerPrimitive = 3;

	FACEINDICES32*			pIndices = new FACEINDICES32[m_iNumPrimitive];

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[1];
			pIndices[iNumFaces]._2 = iIndices[2];

			_vector		vSourDir, vDestDir, vNormal;

			vSourDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal, ( XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal, (XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal, (XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal));
			++iNumFaces;

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[2];
			pIndices[iNumFaces]._2 = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal, (XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal, (XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal, (XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal));
			++iNumFaces;
		}
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
		
		XMVECTOR arbitraryVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		_vector normal = XMLoadFloat3(&pVertices[i].vNormal);	
		_vector tangent = XMVector3Normalize(arbitraryVector - XMVector3Dot(arbitraryVector, normal) * normal);
		
		XMStoreFloat3(&pVertices[i].vTangent, tangent);
	}
		


	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;
	
	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);
#pragma endregion

	Safe_Delete_Array(pPixel);
	CloseHandle(hFile);

	if (bCreateQuadTree)
	{
		_uint		iLT = m_iNumVerticesX * m_iNumVerticesZ - m_iNumVerticesX;
		_uint		iRT = m_iNumVerticesX * m_iNumVerticesZ - 1;
		_uint		iRB = m_iNumVerticesX - 1;
		_uint		iLB = 0;

		m_pQuadTree = CQuadTree::Create(iLT, iRT, iRB, iLB);
		if (nullptr == m_pQuadTree)
			return E_FAIL;

		m_pQuadTree->SetUp_Neighbors();
	}
	m_pTerrainDesc.m_isHeightMap = true;
	return S_OK;
}


HRESULT CVIBuffer_Terrain::Initialize(void * pArg)
{
	if (pArg == nullptr)
		return S_OK;

	memcpy(&m_pTerrainDesc, pArg, sizeof(TERRAINDESC));
	
#pragma region VERTEXBUFFER

	m_iNumVertexBuffers = 1;
	m_iNumVerticesX = (m_pTerrainDesc).m_iVerticeNumX;
	m_iNumVerticesZ = (m_pTerrainDesc).m_iVerticeNumZ;


	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iStride = sizeof(VTXNORTEX);

	VTXNORTEX*		pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	m_pVerticesPos = new _float3[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_int		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3( (_float)j, -0.f,  (_float)i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexture = _float2(j / _float(m_iNumVerticesX - 1), i / _float(m_iNumVerticesZ - 1));
		}
	}


#pragma endregion


#pragma region INDEXBUFFER
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iIndicesByte = sizeof(FACEINDICES32);
	m_iNumIndicesPerPrimitive = 3;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	FACEINDICES32*		pIndices = new FACEINDICES32[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitive);


	_uint			iNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vSourDir, vDestDir, vNormal;

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[1];
			pIndices[iNumFaces]._2 = iIndices[2];

			vSourDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);

			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal);

			++iNumFaces;


			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[2];
			pIndices[iNumFaces]._2 = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);

			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal,
				XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal);

			++iNumFaces;

		}
	}


#pragma endregion

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitive * m_iIndicesByte;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;


	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

	if (m_bCreateQuadTree)
	{
		_uint		iLT = m_iNumVerticesX * m_iNumVerticesZ - m_iNumVerticesX;
		_uint		iRT = m_iNumVerticesX * m_iNumVerticesZ - 1;
		_uint		iRB = m_iNumVerticesX - 1;
		_uint		iLB = 0;

		m_pQuadTree = CQuadTree::Create(iLT, iRT, iRB, iLB);
		if (nullptr == m_pQuadTree)
			return E_FAIL;

		m_pQuadTree->SetUp_Neighbors();
	}


	return S_OK;
}

_bool CVIBuffer_Terrain::Picking(CTransform* pTransform, _float3* pOut)
{
	CPicking*		pPicking = CPicking::Get_Instance();
	Safe_AddRef(pPicking);

	_matrix		WorldMatrixInv = pTransform->Get_WorldMatrixInverse();

	_float3			vTempRayDir, vTempRayPos;
	XMVECTOR		vRayDir, vRayPos;
	pPicking->Compute_LocalRayInfo(&vTempRayDir, &vTempRayPos, pTransform);

	vRayPos = XMLoadFloat3(&vTempRayPos);
	vRayPos = XMVectorSetW(vRayPos, 1.f);
	vRayDir = XMLoadFloat3(&vTempRayDir);
	vRayDir = XMVector3Normalize(vRayDir);

	CFrustum* pFrustum = GET_INSTANCE(CFrustum);

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

		//	if (false == pFrustum->isIn_WorldFrustum(XMVectorSetW(XMLoadFloat3(&m_pVerticesPos[iIndex]), 1.f), 1))
		//		continue;

			_uint		iIndices[] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};



			//iIndices[0]이랑 iIndices[2]사이에 직선이 있거나
			//iIndices[1]이랑 iIndices[3]사이에 직선이 있으면 픽킹 체크 
			_float discriminant0 = (vTempRayDir.z*(m_pVerticesPos[iIndices[0]].x - vTempRayPos.x) + vTempRayPos.z ) / vTempRayDir.x -m_pVerticesPos[iIndices[0]].z;
			_float discriminant1 = (vTempRayDir.z*(m_pVerticesPos[iIndices[1]].x - vTempRayPos.x) + vTempRayPos.z ) / vTempRayDir.x -m_pVerticesPos[iIndices[1]].z;
			_float discriminant2 = (vTempRayDir.z*(m_pVerticesPos[iIndices[2]].x - vTempRayPos.x) + vTempRayPos.z ) / vTempRayDir.x -m_pVerticesPos[iIndices[2]].z;
			_float discriminant3 = (vTempRayDir.z*(m_pVerticesPos[iIndices[3]].x - vTempRayPos.x) + vTempRayPos.z ) / vTempRayDir.x -m_pVerticesPos[iIndices[3]].z;


			if (discriminant0*discriminant2 < 0 && discriminant1*discriminant3 < 0)
				continue;


			_float		fDist;
			_matrix	WorldMatrix = pTransform->Get_WorldMatrix();


			_vector vTemp_1 = XMLoadFloat3(&m_pVerticesPos[iIndices[0]]);
			vTemp_1 = XMVectorSetW(vTemp_1, 1.f);
			_vector vTemp_2 = XMLoadFloat3(&m_pVerticesPos[iIndices[1]]);
			vTemp_2 = XMVectorSetW(vTemp_2, 1.f);
			_vector vTemp_3 = XMLoadFloat3(&m_pVerticesPos[iIndices[2]]);
			vTemp_3 = XMVectorSetW(vTemp_3, 1.f);
			if (true == TriangleTests::Intersects((FXMVECTOR)vRayPos, (FXMVECTOR)vRayDir, (FXMVECTOR)vTemp_1, (GXMVECTOR)vTemp_2, (HXMVECTOR)vTemp_3, fDist))
			{
				_vector	vPickPos = vRayPos + vRayDir * fDist;
				XMStoreFloat3(&m_bPickingLocalPos ,vPickPos);
				XMStoreFloat3(pOut, XMVector3TransformCoord(vPickPos, WorldMatrix));

				Safe_Release(pPicking);
				RELEASE_INSTANCE(CFrustum);
				return true;
			}

			vTemp_1 = XMLoadFloat3(&m_pVerticesPos[iIndices[0]]);
			vTemp_1 = XMVectorSetW(vTemp_1, 1.f);
			vTemp_2 = XMLoadFloat3(&m_pVerticesPos[iIndices[2]]);
			vTemp_2 = XMVectorSetW(vTemp_2, 1.f);
			vTemp_3 = XMLoadFloat3(&m_pVerticesPos[iIndices[3]]);
			vTemp_3 = XMVectorSetW(vTemp_3, 1.f);
			if (true == TriangleTests::Intersects((FXMVECTOR)vRayPos, (FXMVECTOR)vRayDir, (FXMVECTOR)vTemp_1, (GXMVECTOR)vTemp_2, (HXMVECTOR)vTemp_3, fDist))
			{
				_vector	vPickPos = vRayPos + vRayDir * fDist;

				XMStoreFloat3(pOut, XMVector3TransformCoord(vPickPos, WorldMatrix));

				Safe_Release(pPicking);
				RELEASE_INSTANCE(CFrustum);
				return true;
			}
		}
	}

	RELEASE_INSTANCE(CFrustum);
	Safe_Release(pPicking);
	return false;
}

void CVIBuffer_Terrain::Set_Terrain_Shape(_float fHeight, _float fRad, _float fSharp, _float3 vPoint, _float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXNORTEX* pVertices = (VTXNORTEX*)SubResource.pData;
	if (pVertices == nullptr)
	{
		m_pContext->Unmap(m_pVB, 0);
		return;
	}

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{

		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_float3 vPos = pVertices[iIndex].vPosition;

			_float fH = fHeight;
			_float fRange = fRad;

			_float3 vTemp1 = vPoint;
			_float3 vTemp2 = pVertices[iIndex].vPosition;
			vTemp1.y = vTemp2.y = 0.f;

			_vector fDis = XMLoadFloat3(&vTemp1) - XMLoadFloat3(&vTemp2);
			_float fLen = XMVectorGetX(XMVector3Length(fDis));

			if (fRange > fLen)
			{

				_float fAcc = (fRange - fLen) / fRange;

				_float fTempY = fH * fAcc;
				if (fH > 0.01f)
				{
					if (fTempY > vPos.y)
						vPos.y = fTempY;
				}
				else if (fH < -0.01f)
					vPos.y += fTempY * fTimeDelta;
				else
					vPos.y = 0.f;

			}

			pVertices[iIndex].vPosition = vPos;
		}

	}

	m_pContext->Unmap(m_pVB, 0);

}

void CVIBuffer_Terrain::Set_Terrain_UpDown(_float fVec, _float fRad, _float fSharp, _float3 vPoint, _float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXNORTEX* pVertices = (VTXNORTEX*)SubResource.pData;
	if (pVertices == nullptr)
	{
		m_pContext->Unmap(m_pVB, 0);
		return;
	}

	for (_uint i = 0; i < m_iNumVerticesZ - 1; ++i)
	{

		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_float3 vPos = pVertices[iIndex].vPosition;

			_float fH =  fVec;
			_float fRange = fRad;

			_float3 vTemp1 = vPoint;
			_float3 vTemp2 = pVertices[iIndex].vPosition;
			vTemp1.y = vTemp2.y = 0.f;

			_vector fDis = XMLoadFloat3(&vTemp1) - XMLoadFloat3(&vTemp2);
			_float fLen = XMVectorGetX(XMVector3Length(fDis));

			if (fRange > fLen)
			{

				_float fAcc = (fRange - fLen) / fRange;

				fAcc = pow(fAcc, (1.f / fSharp));

				_float fTempY = fH * fAcc;
				if (fH > pVertices[iIndex].vPosition.y)
				{
					vPos.y += fTempY * fTimeDelta;
				}
				else if (fH < pVertices[iIndex].vPosition.y)
				{
					vPos.y -= fTempY * fTimeDelta;
				}
				else
					vPos.y = vPos.y;

			}

			pVertices[iIndex].vPosition = vPos;
		}

	}

	m_pContext->Unmap(m_pVB, 0);
}

void CVIBuffer_Terrain::Set_Terrain_Buffer(TERRAINDESC TerrainDesc)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXNORTEX* pVertices = (VTXNORTEX*)SubResource.pData;

	for (_int i = 0; i < TerrainDesc.m_iVerticeNumZ; ++i)
	{
		for (_int j = 0; j < TerrainDesc.m_iVerticeNumX; ++j)
		{
			_int		iIndex = i * m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = m_pVerticesPos[iIndex] = _float3((_float)TerrainDesc.m_iPositionX + j, TerrainDesc.m_fHeight, (_float)TerrainDesc.m_iPositionZ + i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexture = _float2(j / _float(m_iNumVerticesX - 1), i / _float(m_iNumVerticesZ - 1));
		}
	}
	m_pContext->Unmap(m_pVB, 0);
}

HRESULT CVIBuffer_Terrain::Save_VertexPosition(HANDLE hFile, _ulong* dwByte)
{
	/* 한번 TerrainDesc 정보 읽기 */
	WriteFile(hFile, &m_pTerrainDesc, sizeof(TERRAINDESC), dwByte, nullptr);


	D3D11_MAPPED_SUBRESOURCE		SubResource;

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);
	VTXNORTEX* pVertices = (VTXNORTEX*)SubResource.pData;

	for (_uint i = 0; i < m_pTerrainDesc.m_iVerticeNumX; ++i)
	{
		for (_uint j = 0; j < m_pTerrainDesc.m_iVerticeNumZ ; ++j)
		{
			_uint	iIndex = i * m_pTerrainDesc.m_iVerticeNumX + j;

			WriteFile(hFile, &pVertices[iIndex], sizeof(VTXNORTEX), dwByte, nullptr);
		}
	}

	m_pContext->Unmap(m_pVB, 0);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Load_Prototype(HANDLE hFile, _ulong & dwByte, _bool bCreateQuadTree)
{

	VTXNORTEX VertexTag;

	/* 한번 TerrainDesc 정보 읽기 */
	ReadFile(hFile, &m_pTerrainDesc, sizeof(TERRAINDESC), &dwByte, nullptr);

	m_pTerrainDesc.m_isHeightMap = true;
#pragma region VERTEXBUFFER
	
	m_iNumVerticesX = (m_pTerrainDesc).m_iVerticeNumX;
	m_iNumVerticesZ = (m_pTerrainDesc).m_iVerticeNumZ;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VTXNORTEX*		pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);
	m_pVerticesPos = new _float3[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX; ++j)
		{
			_int		iIndex = i * m_iNumVerticesX + j;

			ReadFile(hFile, &pVertices[iIndex], sizeof(VTXNORTEX), &dwByte, nullptr);
			m_pVerticesPos[iIndex] = pVertices[iIndex].vPosition;
		}
	}


#pragma endregion

#pragma region INDEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	m_iNumPrimitive = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2;
	m_iNumIndicesPerPrimitive = 3;

	FACEINDICES32*			pIndices = new FACEINDICES32[m_iNumPrimitive];

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint		iIndex = i * m_iNumVerticesX + j;

			_uint		iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[1];
			pIndices[iNumFaces]._2 = iIndices[2];

			_vector		vSourDir, vDestDir, vNormal;

			vSourDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal);
			++iNumFaces;

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[2];
			pIndices[iNumFaces]._2 = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vPosition) - XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal, XMLoadFloat3(&pVertices[pIndices[iNumFaces]._2].vNormal) + vNormal);
			++iNumFaces;
		}
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));


	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);
#pragma endregion

	if (bCreateQuadTree)
	{
		_uint		iLT = m_iNumVerticesX * m_iNumVerticesZ - m_iNumVerticesX;
		_uint		iRT = m_iNumVerticesX * m_iNumVerticesZ - 1;
		_uint		iRB = m_iNumVerticesX - 1;
		_uint		iLB = 0;

		m_pQuadTree = CQuadTree::Create(iLT, iRT, iRB, iLB);
		if (nullptr == m_pQuadTree)
			return E_FAIL;

		m_pQuadTree->SetUp_Neighbors();
	}


	return S_OK;
}





void CVIBuffer_Terrain::Culling(const CTransform * pTransform)
{
	CFrustum*		pFrustum = GET_INSTANCE(CFrustum);

	_matrix			WorldMatrix = pTransform->Get_WorldMatrix();
	
	pFrustum->Transform_ToLocalSpace(WorldMatrix);

	D3D11_MAPPED_SUBRESOURCE		SubResource;
	ZeroMemory(&SubResource, sizeof(SubResource));

	m_pContext->Map(m_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	FACEINDICES32*		pIndices = (FACEINDICES32*)SubResource.pData;


	_uint				iNumFaces = 0;

	m_pQuadTree->Culling(pFrustum, m_pVerticesPos, XMMatrixInverse(nullptr, WorldMatrix), pIndices, &iNumFaces);

	m_pContext->Unmap(m_pIB, 0);

	m_iNumPrimitive = iNumFaces;

	RELEASE_INSTANCE(CFrustum);

}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar* pHeightMapFilePath, _bool bCreateQuadTree)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath, bCreateQuadTree)))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumVerticeX, _uint iNumVerticeZ, _float fHeight, _bool bCreateQuadTree)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticeX, iNumVerticeZ, fHeight, bCreateQuadTree)))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, HANDLE hFile, _ulong& dwByte, _bool bCreateQuadTree)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Load_Prototype(hFile, dwByte, bCreateQuadTree)))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
	
	if(m_pTerrainDesc.m_isHeightMap == false)
		Safe_Delete_Array(m_pVerticesPos);
	
	if (m_pQuadTree != nullptr)
		Safe_Release(m_pQuadTree);

}
