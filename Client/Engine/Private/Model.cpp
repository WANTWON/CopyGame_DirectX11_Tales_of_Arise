#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "Shader.h"
#include "HierarchyNode.h"
#include "Animation.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext), m_bIsProto(true) // 추가
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)	
	, m_Materials(rhs.m_Materials)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)	
	, m_iNumBones(rhs.m_iNumBones)
	, m_PivotMatrix(rhs.m_PivotMatrix)		
	, m_eModelType(rhs.m_eModelType)
	, m_pBin_AIScene(rhs.m_pBin_AIScene)	// 추가
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)	// 추가
	, m_iNumAnimations(rhs.m_iNumAnimations)	// 추가
	, m_DataMaterials(rhs.m_DataMaterials)	// 추가
	, m_bIsBin(rhs.m_bIsBin)	// 추가
{
	for (auto& pMeshContainer : rhs.m_Meshes)
	{
		m_Meshes.push_back((CMeshContainer*)pMeshContainer->Clone());
	}	
	
	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pMaterials[i]);
	}	
}

CHierarchyNode * CModel::Get_BonePtr(const char * pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CHierarchyNode* pNode) 
	{
		return !strcmp(pNode->Get_Name(), pBoneName);
	});

	return *iter;
}

HRESULT CModel::Initialize_Prototype(TYPE eModelType, const char * pModelFilePath, _fmatrix PivotMatrix)
{
	m_eModelType = eModelType;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	_uint			iFlag = 0;

	if(TYPE_NONANIM == eModelType)	
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
	
	/* 파일로부터 모든 데이터르 ㄹ읽어서 assimp만의 구조로 보관한다.(aiScene) */
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	/* aiScene안에 있는 aiMesh로부터 정점의 위치, 노멀, 텍스쳐uv, 탄젠트, 뼈의 인덱스, 뼈의 영향력를 읽어와서. 
	이후 렌더링에 사용하기 편한 상태로 만든다. */
	/* 정점 버퍼 생성, 인덱스 버퍼 생성. */
	if (FAILED(Create_MeshContainer()))
		return E_FAIL;

	/* 이 모델을 구성하는 메시들에게 각각 맵핑되어야할 텍스쳐 재질정볼르 읽어온다.  */
	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;

	/* 뼈를 로드하지않는이유 : 뼈는 이 모델이 복제될때마다 하나씩 생성해주기위해서. */

	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	if (TYPE_NONANIM == m_eModelType)
		return S_OK;


	/* 뼈의 정보 : 
	aiNode : 뼈의 부모자식관계를 보여준다(전체뼈의 상태, aiScene) 
	aiBone : 뼈가 어떤 정점에게 얼마만큼의 가중치로 영향을 끼치는지를 표현하기위해(특정 메시(정점들)에게 사용되는 뼈들을 모아서 보여준다. aiMesh)
	aiNodeAnim(채널) : 애니메이션이 사용하고 있는 뼈. 이친구가 가지고 있는 키프레임들을 표현하기 위해. aiAnimation */
	/* 뼈의 정보aiNode 를 로드한다. */
	if (FAILED(Create_HierarchyNodes(m_pAIScene->mRootNode)))
		return E_FAIL;

	/* 메시들은 자기정점들에게 영향ㅇ르 주는 뼈들의 정보를 모아놔야했어.
	why? 정점그릴때 뼈의 행렬을 가져와야하니께. 모델의 뼈들을 가져오면안돼? 안돼. 정점이 가지고 있는 뼈의 인덱스는 전체뼈의 인덱스를 ㄱ의미하는게 아니라. 
	메시당 영햐을 주는 뼈들의 인덱스를 의미하고 있으니까 .*/
	for (auto& pMeshContainer : m_Meshes)	
		pMeshContainer->SetUp_Bones(this);

	/* 애니메이션 객체를 생성했지. 애니메이션 객체 : 한 동작 전체.(대기, 공격, 달리기) */
	/* 애니메니션 하나는 뭘가진다 : 뼈들(애니메이션 구동에 필요한 뼈들) , 이름*/
	/* 뼈들 : 이 애니메이션 하나르 ㄹ구동할때 시간별로 표현되었어야할 상태(KEYFRAME) */
	if (FAILED(Create_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Bin_Initialize_Prototype(DATA_BINSCENE * pScene, TYPE eType, const char * pModelFilePath, _fmatrix PivotMatrix)
{
	m_bIsBin = true;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);
	m_eModelType = eType;

	m_pBin_AIScene = pScene;

	if (nullptr == m_pBin_AIScene)
		return E_FAIL;

	/* 모델을 구성하는 메시들을 만든다. */
	if (FAILED(Bin_Ready_MeshContainers(PivotMatrix)))
		return E_FAIL;


	if (FAILED(Bin_Ready_Materials(pModelFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Bin_Initialize(void * pArg)
{
	Bin_Ready_HierarchyNodes();

	for (int i = 0; i < m_pBin_AIScene->iNodeCount; ++i)
	{
		for (int j = 0; j < m_pBin_AIScene->iNodeCount; ++j)
		{
			if (nullptr != m_Bones[i]->Get_Parent())
				break;

			m_Bones[i]->Set_FindParent(m_Bones[j]);
		}
	}

	int iBin = 1;
	if (TYPE_ANIM == m_eModelType)
	{
		_uint		iNumMeshes = 0;

		for (auto& pMeshContainer : m_Meshes)
		{
			if (nullptr != pMeshContainer)
				pMeshContainer->Bin_SetUp_Bones(this, &m_pBin_AIScene->pBinMesh[iNumMeshes++]);
		}
	}

	if (FAILED(Bin_Ready_Animations(this)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::SetUp_Material(CShader * pShader, const char * pConstantName, _uint iMeshIndex, aiTextureType eType, _uint TextureNum)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	return pShader->Set_ShaderResourceView(pConstantName, m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pMaterials[eType]->Get_SRV(TextureNum));
}

_bool CModel::Play_Animation(_float fTimeDelta, _bool isLoop)
{
	if (m_iCurrentAnimIndex != m_iNextAnimIndex)
	{	//TODO: 현재애님과 다음 애님프레임간의 선형보간 함수 호출 할 것.
		if (m_bInterupted)
		{
			m_Animations[m_iCurrentAnimIndex]->Set_TimeReset();
			m_bInterupted = false;
		}
		m_bLinearFinished = m_Animations[m_iCurrentAnimIndex]->Animation_Linear_Interpolation(fTimeDelta, m_Animations[m_iNextAnimIndex]);

		if (m_bLinearFinished == true)
		{
			m_Animations[m_iCurrentAnimIndex]->Set_TimeReset();

			m_iCurrentAnimIndex = m_iNextAnimIndex;

		}
	}
	else
	{
		/* 뼈의 m_TransformationMatrix행렬을 갱신한다. */
		if (m_Animations[m_iCurrentAnimIndex]->Invalidate_TransformationMatrix(fTimeDelta, isLoop))
		{
			for (auto& pBoneNode : m_Bones)
			{
				/* 뼈의 m_CombinedTransformationMatrix행렬을 갱신한다. */
				pBoneNode->Invalidate_CombinedTransformationmatrix();
			}
			return true;
		}
	}

	for (auto& pBoneNode : m_Bones)
	{
		/* 뼈의 m_CombinedTransformationMatrix행렬을 갱신한다. */
		pBoneNode->Invalidate_CombinedTransformationmatrix();
	}

	return false;
}

HRESULT CModel::Render(CShader * pShader, _uint iMeshIndex, _uint iPassIndex)
{		
	/* 메시별로 그린다. */
	/* 메시 당 영향ㅇ르 주는 뼈들의 행렬을 가져온다. */
	if (TYPE_ANIM == m_eModelType)
	{
		_float4x4		BoneMatrix[264];


		/* 메시에게 접근해서 니가 들고있는 뼈들을 배열에 담아와.
		뼈 = 뼈의 오프셋 * 뼈의 컴바인드매트릭스 * 최초상태제어행렬 */
		m_Meshes[iMeshIndex]->Get_BoneMatrices(BoneMatrix, XMLoadFloat4x4(&m_PivotMatrix));

		pShader->Set_MatrixArray("g_BoneMatrices", BoneMatrix, 256);

	}
	
	pShader->Begin(iPassIndex);

	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

_bool CModel::Picking(CTransform * pTransform, _float3 * pOut)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		if (m_Meshes[i]->Picking(pTransform, pOut))
			return true;
	}

	return false;
}

HRESULT CModel::Set_AnimationReset()
{
	m_Animations[m_iCurrentAnimIndex]->Set_TimeReset();

	return S_OK;
}



HRESULT CModel::Create_MeshContainer()
{
	if (nullptr == m_pAIScene)
		return E_FAIL;

	/* 하나의 모델은 다수의 메시로 구성되어있다. */
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	m_Meshes.reserve(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		aiMesh*		pAIMesh = m_pAIScene->mMeshes[i];

		/* 메시를 생성한다. */
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, pAIMesh, this, XMLoadFloat4x4(&m_PivotMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_Meshes.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_Materials(const char* pModelFilePath)
{	
	if (nullptr == m_pAIScene)
		return E_FAIL;

	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial*		pAIMaterial = m_pAIScene->mMaterials[i];

		MODELMATERIAL		ModelMaterial;
		ZeroMemory(&ModelMaterial, sizeof(MODELMATERIAL));


		DATA_BINMATERIAL	DataMaterialDesc;	// 추가
		ZeroMemory(&DataMaterialDesc, sizeof(DATA_BINMATERIAL)); // 추가

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString		strPath;			

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;

			char			szName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";
			char			szTextureFileName[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szTextureFileName, szName);
			strcat_s(szTextureFileName, szExt);

			memcpy(&DataMaterialDesc.cNames[j], &szTextureFileName, sizeof(char) * MAX_PATH); // 추가


			char			szDirectory[MAX_PATH] = "";
			char			szFullPath[MAX_PATH] = "";
			_splitpath_s(pModelFilePath, nullptr, 0, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			strcpy_s(szFullPath, szDirectory);
			strcat_s(szFullPath, szTextureFileName);

			_tchar			szRealPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szRealPath, MAX_PATH);

			ModelMaterial.pMaterials[j] = CTexture::Create(m_pDevice, m_pContext, szRealPath);
			if (nullptr == ModelMaterial.pMaterials[j])
				return E_FAIL;
		}

		m_Materials.push_back(ModelMaterial);
		m_DataMaterials.push_back(DataMaterialDesc); // 추가
	}

	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes(const aiNode* pNode, CHierarchyNode* pParent)
{
	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode, pParent);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_Bones.push_back(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
		Create_HierarchyNodes(pNode->mChildren[i], pHierarchyNode);

	return S_OK;
}

HRESULT CModel::Create_Animations()
{
	if (nullptr == m_pAIScene)
		return E_FAIL;

	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation*	pAIAnimation = m_pAIScene->mAnimations[i];

		CAnimation*		pAnimation = CAnimation::Create(this, pAIAnimation);
		if (nullptr == pAIAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eModelType, const char * pModelFilePath, _fmatrix PivotMatrix)
{
	CModel*	pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CModel * CModel::Bin_Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, DATA_BINSCENE * pScene, TYPE eType, const char * pModelFilePath, _fmatrix PivotMatrix)
{
	CModel*			pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Bin_Initialize_Prototype(pScene, eType, pModelFilePath, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed To Created : CModel_Bin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CComponent * CModel::Clone(void * pArg)
{
	CModel*	pInstance = new CModel(*this);

	if (!m_pBin_AIScene)	// 추가
	{
		if (FAILED(pInstance->Initialize(pArg)))
		{
			ERR_MSG(TEXT("Failed to Cloned : CModel"));
			Safe_Release(pInstance);
		}
	}
	else
	{
		if (FAILED(pInstance->Bin_Initialize(pArg)))
		{
			ERR_MSG(TEXT("Failed To Cloned : CModel_Bin"));
			Safe_Release(pInstance);
		}
	}

	return pInstance;
}
void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	for (auto& pBoneNode : m_Bones)
		Safe_Release(pBoneNode);
	m_Bones.clear();

	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);
	m_Meshes.clear();

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)		
			Safe_Release(Material.pMaterials[i]);
	}
	m_Materials.clear();
		



	m_Importer.FreeScene();

	if (m_pBin_AIScene && m_bIsProto)	// 추가
	{
		Safe_Release_Scene();
	}
}

HRESULT CModel::Get_HierarchyNodeData(DATA_BINSCENE * pBinScene)
{
	if (0 == m_Bones.size())
	{
		pBinScene->pBinNodes = nullptr;
		return S_OK;
	}

	pBinScene->pBinNodes = new DATA_BINNODE[m_Bones.size()];
	pBinScene->iNodeCount = m_Bones.size();

	for (_int i = 0; i < m_Bones.size(); ++i)
	{
		DATA_BINNODE Bin_Node;
		ZeroMemory(&Bin_Node, sizeof(DATA_BINNODE));

		const char* pMyName = m_Bones[i]->Get_Name();
		const char* pParentName = m_Bones[i]->Get_ParentName();
		memcpy(&Bin_Node.cName, pMyName, sizeof(char) * MAX_PATH);
		memcpy(&Bin_Node.cParent, pParentName, sizeof(char) * MAX_PATH);

		Bin_Node.mTransform = m_Bones[i]->Get_OriTransformationMatrix();

		pBinScene->pBinNodes[i] = Bin_Node;
	}

	return S_OK;
}

HRESULT CModel::Get_MaterialData(DATA_BINSCENE * pBinScene)
{
	pBinScene->iMaterialCount = m_iNumMaterials;
	pBinScene->pBinMaterial = new DATA_BINMATERIAL[m_iNumMaterials];

	for (_int i = 0; i < m_DataMaterials.size(); ++i)
	{
		memcpy(&pBinScene->pBinMaterial[i], &m_DataMaterials[i], sizeof(DATA_BINMATERIAL));
	}

	return S_OK;
}

HRESULT CModel::Get_MeshData(DATA_BINSCENE * pBinScene)
{
	pBinScene->iMeshCount = m_iNumMeshes;
	pBinScene->pBinMesh = new DATA_BINMESH[m_iNumMeshes];
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		DATA_BINMESH BinMesh;
		m_Meshes[i]->Get_MeshData(&BinMesh);
		memcpy(&pBinScene->pBinMesh[i], &BinMesh, sizeof(DATA_BINMESH));
	}

	return S_OK;
}

HRESULT CModel::Get_AnimData(DATA_BINSCENE * pBinScene)
{
	if (0 == m_iNumAnimations)
	{
		pBinScene->pBinAnim = nullptr;
		pBinScene->iNumAnimations = 0;
		return S_OK;
	}

	pBinScene->iNumAnimations = m_iNumAnimations;
	pBinScene->pBinAnim = new DATA_BINANIM[m_iNumAnimations];

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		m_Animations[i]->Get_AnimData(&pBinScene->pBinAnim[i]);
	}

	return S_OK;
}


HRESULT CModel::Bin_Ready_MeshContainers(_fmatrix PivotMatrix)
{
	m_iNumMeshes = m_pBin_AIScene->iMeshCount;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Bin_Create(m_pDevice, m_pContext, m_eModelType, &m_pBin_AIScene->pBinMesh[i], this, PivotMatrix);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_Meshes.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Bin_Ready_Materials(const char * pModelFilePath)
{
	if (nullptr == m_pBin_AIScene)
		return E_FAIL;

	m_iNumMaterials = m_pBin_AIScene->iMaterialCount;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODELMATERIAL		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MODELMATERIAL));

		DATA_BINMATERIAL		pAIMaterial = m_pBin_AIScene->pBinMaterial[i];

		DATA_BINMATERIAL		DataMaterialDesc;
		ZeroMemory(&DataMaterialDesc, sizeof(DATA_BINMATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			if (!strcmp(pAIMaterial.cNames[j], ""))
				continue;


			char			szFullPath[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(pAIMaterial.cNames[j], nullptr, 0, nullptr, 0, szFullPath, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, pAIMaterial.cNames[j]);

			_tchar			szWideFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szWideFullPath, MAX_PATH);

			memcpy(&DataMaterialDesc.cNames[j], &pAIMaterial.cNames[j], sizeof(char) * MAX_PATH);

			MaterialDesc.pMaterials[j] = CTexture::Create(m_pDevice, m_pContext, szWideFullPath);
			if (nullptr == MaterialDesc.pMaterials[j])
				continue; //return E_FAIL;
		}

		m_Materials.push_back(MaterialDesc);
		m_DataMaterials.push_back(DataMaterialDesc);
	}

	return S_OK;
}

HRESULT CModel::Bin_Ready_HierarchyNodes()
{
	for (int i = 0; i < m_pBin_AIScene->iNodeCount; ++i)
	{
		DATA_BINNODE Node = m_pBin_AIScene->pBinNodes[i];

		CHierarchyNode*		pHierarchyNode = CHierarchyNode::Bin_Create(&Node);
		if (nullptr == pHierarchyNode)
			return E_FAIL;

		m_Bones.push_back(pHierarchyNode);
	}

	return S_OK;
}

HRESULT CModel::Bin_Ready_Animations(CModel* pModel)
{
	m_iNumAnimations = m_pBin_AIScene->iNumAnimations;

	for (_int i = 0; i < m_pBin_AIScene->iNumAnimations; ++i)
	{
		DATA_BINANIM*		pAIAnimation = &m_pBin_AIScene->pBinAnim[i];

		CAnimation*			pAnimation = CAnimation::Bin_Create(pAIAnimation, this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

HRESULT CModel::Safe_Release_Scene()
{
	Safe_Delete_Array(m_pBin_AIScene->pBinNodes);
	Safe_Delete_Array(m_pBin_AIScene->pBinMaterial);

	for (_int i = 0; i < m_pBin_AIScene->iMeshCount; ++i)
	{
		Safe_Delete_Array(m_pBin_AIScene->pBinMesh[i].pAnimVertices);
		Safe_Delete_Array(m_pBin_AIScene->pBinMesh[i].pNonAnimVertices);
		Safe_Delete_Array(m_pBin_AIScene->pBinMesh[i].pIndices);
		Safe_Delete_Array(m_pBin_AIScene->pBinMesh[i].pBones);
	}
	Safe_Delete_Array(m_pBin_AIScene->pBinMesh);


	for (_int i = 0; i < m_pBin_AIScene->iNumAnimations; ++i)
	{
		for (_int j = 0; j < m_pBin_AIScene->pBinAnim[i].iNumChannels; ++j)
		{
			Safe_Delete_Array(m_pBin_AIScene->pBinAnim[i].pBinChannel[j].pKeyFrames);
		}
		Safe_Delete_Array(m_pBin_AIScene->pBinAnim[i].pBinChannel);
	}
	Safe_Delete_Array(m_pBin_AIScene->pBinAnim);

	Safe_Delete(m_pBin_AIScene);

	return S_OK;
}
