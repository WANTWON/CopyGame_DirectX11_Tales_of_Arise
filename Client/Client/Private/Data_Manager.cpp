#include "stdafx.h"
#include "..\Public\Data_Manager.h"

#include "GameInstance.h"
#include "Model.h"
#include <fstream>
#include <string.h>
//This is needed for virtually everything in BrowseFolder.
#include <shlobj.h> 

IMPLEMENT_SINGLETON(CData_Manager)

typedef basic_string<TCHAR> tstring;

TCHAR* StringToTCHAR(string& s)
{
	tstring tstr;
	const char* all = s.c_str();
	int len = 1 + (int)strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

string TCHARToString(const TCHAR* ptsz)
{
	int len = (int)wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return s;
}

string ToString(wstring value)
{
	string temp;
	temp.assign(value.begin(), value.end());
	return temp;
}

vector<string> SplitPath(string path, char sep) {
	vector<string> out;
	stringstream stream(path);
	string temp;

	while (getline(stream, temp, sep)) {
		out.push_back(temp);
	}

	return out;
}


CData_Manager::CData_Manager()
{
}

HRESULT CData_Manager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

//HRESULT CData_Manager::Conv_Bin_Model(CModel * pModel, char* cModelName, DATA_TYPE eTYPE)
//{
//	DATA_BINSCENE Scene;
//	ZeroMemory(&Scene, sizeof(DATA_BINSCENE));
//
//	pModel->Get_HierarchyNodeData(&Scene);
//	pModel->Get_MaterialData(&Scene);
//	pModel->Get_MeshData(&Scene);
//	pModel->Get_AnimData(&Scene);
//
//	SaveSceneData(&Scene, cModelName, eTYPE);
//
//	Safe_Delete_Array(Scene.pBinNodes);
//	Safe_Delete_Array(Scene.pBinMaterial);
//
//	for (_int i = 0; i < Scene.iMeshCount; ++i)
//	{
//		Safe_Delete_Array(Scene.pBinMesh[i].pBones);
//	}
//	Safe_Delete_Array(Scene.pBinMesh);
//
//
//	for (_int i = 0; i < Scene.iNumAnimations; ++i)
//	{
//		for (_int j = 0; j < Scene.pBinAnim[i].iNumChannels; ++j)
//		{
//			Safe_Delete_Array(Scene.pBinAnim[i].pBinChannel[j].pKeyFrames);
//		}
//		Safe_Delete_Array(Scene.pBinAnim[i].pBinChannel);
//	}
//	Safe_Delete_Array(Scene.pBinAnim);
//
//
//	return S_OK;
//}

HRESULT CData_Manager::SaveSceneData(DATA_BINSCENE * pScene, char* cModelName, DATA_TYPE eTYPE)
{
	char cPullName[MAX_PATH];
	if (eTYPE == DATA_ANIM)
		strcpy_s(cPullName, "../../../Bin/Bin_Data/Anim/");
	else if (eTYPE == DATA_NONANIM)
		strcpy_s(cPullName, "../../../Bin/Bin_Data/NonAnim/");
	else if (eTYPE == DATA_PARTS)
		strcpy_s(cPullName, "../../../Bin/Bin_Data/Parts/");

	strcat_s(cPullName, cModelName);

	std::ofstream ofs(cPullName, ios::out | ios::binary);

	if (!ofs)
		return E_FAIL;

	// HierarchyNode
	ofs.write((char*)&pScene->iNodeCount, sizeof(int));
	for (int i = 0; i < pScene->iNodeCount; ++i)
	{
		DATA_BINNODE Node = pScene->pBinNodes[i];
		ofs.write((char*)&Node, sizeof(DATA_BINNODE));
	}
	// Material
	ofs.write((char*)&pScene->iMaterialCount, sizeof(int));
	for (int i = 0; i < pScene->iMaterialCount; ++i)
	{
		DATA_BINMATERIAL pMaterial = pScene->pBinMaterial[i];
		ofs.write((char*)&pMaterial, sizeof(DATA_BINMATERIAL));
	}

	// MashNode

	ofs.write((char*)&pScene->iMeshCount, sizeof(int));
	for (int i = 0; i < pScene->iMeshCount; ++i)
	{
		DATA_BINMESH pMash = pScene->pBinMesh[i];

		ofs.write((char*)&pMash.cName, sizeof(char)*MAX_PATH);
		ofs.write((char*)&pMash.iMaterialIndex, sizeof(int));


		ofs.write((char*)&pMash.NumVertices, sizeof(int));
		int iIsAnim = 0;
		if (nullptr != pMash.pAnimVertices)
		{
			iIsAnim = 1;
			ofs.write((char*)&iIsAnim, sizeof(int));

			for (int j = 0; j < pMash.NumVertices; ++j)
			{
				VTXANIMMODEL VtxAniModel = pMash.pAnimVertices[j];
				ofs.write((char*)&VtxAniModel, sizeof(VTXANIMMODEL));
			}
		}
		else if (nullptr != pMash.pNonAnimVertices)
		{
			iIsAnim = 0;
			ofs.write((char*)&iIsAnim, sizeof(int));

			for (int j = 0; j < pMash.NumVertices; ++j)
			{
				VTXMODEL VtxNoneAniModel = pMash.pNonAnimVertices[j];
				ofs.write((char*)&VtxNoneAniModel, sizeof(VTXMODEL));
			}
		}


		ofs.write((char*)&pMash.iNumPrimitives, sizeof(int));
		for (int j = 0; j < pMash.iNumPrimitives; ++j)
		{
			FACEINDICES32 Indices32 = pMash.pIndices[j];
			ofs.write((char*)&Indices32, sizeof(FACEINDICES32));
		}

		ofs.write((char*)&pMash.iNumBones, sizeof(int));
		for (int j = 0; j < pMash.iNumBones; ++j)
		{
			DATA_BINBONE bon = pMash.pBones[j];
			ofs.write((char*)&bon, sizeof(DATA_BINBONE));
		}
	}


	// Animation
	ofs.write((char*)&pScene->iNumAnimations, sizeof(int));
	for (int i = 0; i < pScene->iNumAnimations; ++i)
	{
		DATA_BINANIM Anim = pScene->pBinAnim[i];
		ofs.write((char*)&Anim.iNumChannels, sizeof(int));
		ofs.write((char*)&Anim.fDuration, sizeof(float));
		ofs.write((char*)&Anim.fTickPerSecond, sizeof(float));
		ofs.write((char*)&Anim.bLoop, sizeof(bool));
		ofs.write((char*)&Anim.szName, sizeof(char)*MAX_PATH);

		for (int j = 0; j < Anim.iNumChannels; ++j)
		{
			ofs.write((char*)&Anim.pBinChannel[j].szName, sizeof(char)*MAX_PATH);
			ofs.write((char*)&Anim.pBinChannel[j].iNumKeyFrames, sizeof(int));

			for (int k = 0; k < Anim.pBinChannel[j].iNumKeyFrames; ++k)
			{
				ofs.write((char*)&Anim.pBinChannel[j].pKeyFrames[k], sizeof(KEYFRAME));
			}
		}
	}

	ofs.close();


	return S_OK;
}
HRESULT CData_Manager::ReadSceneData(char * pFileName, DATA_BINSCENE* ReadScene, DATA_TYPE eTYPE)
{
	char cPullName[MAX_PATH];
	if (DATA_ANIM == eTYPE)
		strcpy_s(cPullName, "../../../Bin/Bin_Data/Anim/");
	else if (DATA_NONANIM == eTYPE)
		strcpy_s(cPullName, "../../../Bin/Bin_Data/NonAnim/");
	else if (DATA_PARTS == eTYPE)
		strcpy_s(cPullName, "../../../Bin/Bin_Data/Parts/");

	strcat_s(cPullName, pFileName);


	ZeroMemory(ReadScene, sizeof(DATA_BINSCENE));
	std::ifstream ifs(cPullName, ios::in | ios::binary);

	if (!ifs)
		return E_FAIL;

	// Node
	int iNodeCount = 0;
	ifs.read((char*)&iNodeCount, sizeof(int));
	ReadScene->iNodeCount = iNodeCount;
	ReadScene->pBinNodes = new DATA_BINNODE[iNodeCount];
	for (int i = 0; i < iNodeCount; ++i)
	{
		DATA_BINNODE* pBinNode = &ReadScene->pBinNodes[i];
		ifs.read((char*)pBinNode, sizeof(DATA_BINNODE));
	}

	// Material
	int iMaterialCount = 0;
	ifs.read((char*)&iMaterialCount, sizeof(int));
	ReadScene->iMaterialCount = iMaterialCount;
	ReadScene->pBinMaterial = new DATA_BINMATERIAL[iMaterialCount];
	for (int i = 0; i < iMaterialCount; ++i)
	{
		DATA_BINMATERIAL* pBinMarterial = &ReadScene->pBinMaterial[i];
		ifs.read((char*)pBinMarterial, sizeof(DATA_BINMATERIAL));
	}

	// MashNode
	int iMashCount = 0;
	ifs.read((char*)&iMashCount, sizeof(int));
	ReadScene->iMeshCount = iMashCount;
	ReadScene->pBinMesh = new DATA_BINMESH[iMashCount];
	for (int i = 0; i < iMashCount; ++i)
	{
		DATA_BINMESH* pBinMash = &ReadScene->pBinMesh[i];

		ifs.read((char*)&pBinMash->cName, sizeof(char)*MAX_PATH);
		ifs.read((char*)&pBinMash->iMaterialIndex, sizeof(int));

		ifs.read((char*)&pBinMash->NumVertices, sizeof(int));

		int iIsAnim = 0;
		ifs.read((char*)&iIsAnim, sizeof(int));
		if (iIsAnim)
		{
			pBinMash->pAnimVertices = new VTXANIMMODEL[pBinMash->NumVertices];
			pBinMash->pNonAnimVertices = nullptr;
			for (int j = 0; j < pBinMash->NumVertices; ++j)
			{
				VTXANIMMODEL* VtxAniModel = &pBinMash->pAnimVertices[j];
				ifs.read((char*)VtxAniModel, sizeof(VTXANIMMODEL));
			}
		}
		else
		{
			pBinMash->pNonAnimVertices = new VTXMODEL[pBinMash->NumVertices];
			pBinMash->pAnimVertices = nullptr;
			for (int j = 0; j < pBinMash->NumVertices; ++j)
			{
				VTXMODEL* VtxNonAniModel = &pBinMash->pNonAnimVertices[j];
				ifs.read((char*)VtxNonAniModel, sizeof(VTXMODEL));
			}
		}

		pBinMash->iNumPrimitives = 0;
		ifs.read((char*)&pBinMash->iNumPrimitives, sizeof(int));
		pBinMash->pIndices = new FACEINDICES32[pBinMash->iNumPrimitives];
		for (int j = 0; j < pBinMash->iNumPrimitives; ++j)
		{
			FACEINDICES32* Indices32 = &pBinMash->pIndices[j];
			ifs.read((char*)Indices32, sizeof(FACEINDICES32));
		}


		pBinMash->iNumBones = 0;
		ifs.read((char*)&pBinMash->iNumBones, sizeof(int));
		pBinMash->pBones = new DATA_BINBONE[pBinMash->iNumBones];
		for (int j = 0; j < pBinMash->iNumBones; ++j)
		{
			DATA_BINBONE* pBone = &pBinMash->pBones[j];
			ifs.read((char*)pBone, sizeof(DATA_BINBONE));
		}
	}


	// Animation
	int iNumAnimations = 0;
	ifs.read((char*)&iNumAnimations, sizeof(int));
	ReadScene->iNumAnimations = iNumAnimations;
	ReadScene->pBinAnim = new DATA_BINANIM[iNumAnimations];

	for (int i = 0; i < iNumAnimations; ++i)
	{
		ifs.read((char*)&ReadScene->pBinAnim[i].iNumChannels, sizeof(int));
		ifs.read((char*)&ReadScene->pBinAnim[i].fDuration, sizeof(float));
		ifs.read((char*)&ReadScene->pBinAnim[i].fTickPerSecond, sizeof(float));
		ifs.read((char*)&ReadScene->pBinAnim[i].bLoop, sizeof(bool));
		ifs.read((char*)&ReadScene->pBinAnim[i].szName, sizeof(char) * MAX_PATH);

		ReadScene->pBinAnim[i].pBinChannel = new DATA_BINCHANNEL[ReadScene->pBinAnim[i].iNumChannels];
		for (int j = 0; j < ReadScene->pBinAnim[i].iNumChannels; ++j)
		{
			DATA_BINCHANNEL* pChannel = &ReadScene->pBinAnim[i].pBinChannel[j];
			ifs.read((char*)&pChannel->szName, sizeof(char)*MAX_PATH);
			ifs.read((char*)&pChannel->iNumKeyFrames, sizeof(int));

			pChannel->pKeyFrames = new KEYFRAME[pChannel->iNumKeyFrames];
			for (int k = 0; k < pChannel->iNumKeyFrames; ++k)
			{
				KEYFRAME* pKeyFrame = &pChannel->pKeyFrames[k];
				ifs.read((char*)pKeyFrame, sizeof(KEYFRAME));
			}
		}
	}


	ifs.close();
	return S_OK;
}


//HRESULT CData_Manager::Create_Try_BinModel(const _tchar * pModelName, LEVEL eLEVEL, DATA_TYPE eTYPE)
//{
//
//	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
//	// 원본 체크
//	if (FAILED(pGameInstance->Check_Prototype(eLEVEL, pModelName)))
//	{
//		RELEASE_INSTANCE(CGameInstance);
//		return S_OK;
//	}
//	RELEASE_INSTANCE(CGameInstance);
//
//	string str = TCHARToString(pModelName);
//	char cTempName[MAX_PATH];
//	char cPathName[MAX_PATH];
//	TCtoC(pModelName, cTempName);
//
//	if (str.find("Field") != string::npos)
//	{
//		
//		TCtoC(TEXT("Field"), cPathName);
//	}
//	else if (str.find("TailCave") != string::npos)
//	{
//		TCtoC(TEXT("TailCave"), cPathName);
//	}
//	else
//	{
//		TCtoC(pModelName, cPathName);
//	}
//	
//
//	char tPath[MAX_PATH] = ""; //Get_ManagedChar();
//	if (DATA_ANIM == eTYPE)
//		strcpy_s(tPath, "../../../Bin/Resources/Meshes/Anim/");
//	else if (DATA_NONANIM == eTYPE)
//		strcpy_s(tPath, "../../../Bin/Resources/Meshes/NonAnim/");
//	else if (DATA_PARTS == eTYPE)
//		strcpy_s(tPath, "../../../Bin/Resources/Meshes/Parts/");
//
//	strcat_s(tPath, cPathName);
//	strcat_s(tPath, "/");
//
//	char tFileName[MAX_PATH] = ""; //Get_ManagedChar();
//	strcpy_s(tFileName, cTempName);
//	char TempName[MAX_PATH];
//	strcpy_s(TempName, tFileName);
//	strcat_s(tFileName, ".fbx");
//
//	// Bin 체크 And Load
//	DATA_BINSCENE* Scene = new DATA_BINSCENE;
//	ZeroMemory(Scene, sizeof(DATA_BINSCENE));
//	_bool bIsBin = true;
//	if (FAILED(CData_Manager::Get_Instance()->ReadSceneData(TempName, Scene, eTYPE)))
//	{
//		bIsBin = false;
//		Safe_Delete(Scene);
//	}
//
//
//
//	pGameInstance = GET_INSTANCE(CGameInstance);
//
//
//	_matrix PivotMatrix;
//	CModel::TYPE etype = CModel::TYPE_END;
//	if (DATA_ANIM == eTYPE)
//		etype = CModel::TYPE_ANIM;
//	else
//		etype = CModel::TYPE_NONANIM;
//	// 원본 생성
//	if (bIsBin)
//	{
//		if (DATA_ANIM == eTYPE)
//			PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
//		else
//			PivotMatrix = XMMatrixScaling(1.f, 1.f, 1.f);
//		if (FAILED(pGameInstance->Add_Prototype(eLEVEL, pModelName,
//			CModel::Bin_Create(m_pDevice, m_pContext, Scene, etype, tPath, PivotMatrix))))
//		{
//			// 뭔가 파일 경로가 잘 못 됨.
//			RELEASE_INSTANCE(CGameInstance);
//			return E_FAIL;
//		}
//	}
//	else
//	{
//		if (DATA_ANIM == eTYPE)
//			PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
//		else
//			PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
//		if (FAILED(pGameInstance->Add_Prototype(eLEVEL, pModelName,
//			CModel::Create(m_pDevice, m_pContext, etype, tPath, PivotMatrix))))
//		{
//			// 뭔가 파일 경로가 잘 못 됨.
//			RELEASE_INSTANCE(CGameInstance);
//			return E_FAIL;
//		}
//	}
//
//
//	RELEASE_INSTANCE(CGameInstance);
//
//	return S_OK;
//}

void CData_Manager::Free()
{
	for (auto& iter : m_pModelTags)
		Safe_Delete(iter);
	m_pModelTags.clear();


	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
