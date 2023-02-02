#include "stdafx.h"
#include "..\Public\Imgui_Manager.h"
#include "imgui.h"
#include "GameInstance.h"
#include "PickingMgr.h"
#include "Cell.h"
#include "BaseObj.h"
#include <windows.h>
#include <string.h>

//This is needed for virtually everything in BrowseFolder.
#include <shlobj.h> 


IMPLEMENT_SINGLETON(CImgui_Manager)

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

CImgui_Manager::CImgui_Manager()
	: m_pTerrain_Manager(CTerrain_Manager::Get_Instance())
	, m_pModel_Manager(CModelManager::Get_Instance())
	, m_pNavigation_Manager(CNavigation_Manager::Get_Instance())
	, m_pCamera_Manager(CCamera_Manager::Get_Instance())
	, m_pEffectManager(CEffect_Manager::Get_Instance())
{
	Safe_AddRef(m_pModel_Manager);
	Safe_AddRef(m_pTerrain_Manager);
	Safe_AddRef(m_pNavigation_Manager);
	Safe_AddRef(m_pCamera_Manager);
	Safe_AddRef(m_pEffectManager);

	char* LayerTag = "Layer_Map";
	char* LayerTag2 = "Layer_Terrain";
	char* LayerTag3 = "Layer_CameraModel";

	m_stLayerTags.push_back(LayerTag);
	m_stLayerTags.push_back(LayerTag2);
	m_stLayerTags.push_back(LayerTag3);

	ZeroMemory(&m_BoxDesc, sizeof(CTreasureBox::BOXTAG));
}


HRESULT CImgui_Manager::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	m_TempLayerTags.reserve(0);

	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	io.FontDefault = io.Fonts->AddFontFromFileTTF("../../../Bin/Resources/Fonts/Quicksand-Medium.ttf", 16.0f);
	ImGui::StyleColorsBlue();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	
	/* Effect Tool */
	Read_EffectsData();

	_tchar wcTexturesPath[MAX_PATH] = TEXT("../../../Bin/Resources/Textures/Effect/");
	Read_Textures(wcTexturesPath);

	_tchar wcMeshesPath[MAX_PATH] = TEXT("../../../Bin/Bin_Data/Effect/");
	Read_Meshes(wcMeshesPath);
	/**/

	return S_OK;
}


void CImgui_Manager::Tick(_float fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_iCurrentLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();
	CTerrain_Manager::TERRAINDESC TerrainDesc = m_pTerrain_Manager->Get_TerrainDesc();

	if (pGameInstance->Key_Up(DIK_LEFT))
		TerrainDesc.TerrainDesc.m_iPositionX -= m_pTerrain_Manager->Get_MoveOffset();

	if (pGameInstance->Key_Up(DIK_RIGHT))
		TerrainDesc.TerrainDesc.m_iPositionX += m_pTerrain_Manager->Get_MoveOffset();

	if (pGameInstance->Key_Up(DIK_UP))
		TerrainDesc.TerrainDesc.m_iPositionZ += m_pTerrain_Manager->Get_MoveOffset();

	if (pGameInstance->Key_Up(DIK_DOWN))
		TerrainDesc.TerrainDesc.m_iPositionZ -= m_pTerrain_Manager->Get_MoveOffset();

	m_pTerrain_Manager->Set_TerrainDesc(&TerrainDesc);
	RELEASE_INSTANCE(CGameInstance);

	Tick_Imgui();
	ImGui::EndFrame();

}

void CImgui_Manager::Render()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CImgui_Manager::Tick_Imgui()
{
	ImGui::Begin(u8"Editor", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar
		| ImGuiWindowFlags_AlwaysVerticalScrollbar);

	//메뉴바
	if (ImGui::BeginMenuBar())
	{
		// 메뉴
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("save"))
				m_bSave = true;
			ImGui::Separator();
			if (ImGui::MenuItem("open"))
				m_bLoad = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("Show Mouse Pos", NULL, &m_bShowSimpleMousePos);
			ImGui::Separator();
			ImGui::MenuItem("Show Picked Object", NULL, &m_bShowPickedObject);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Style Editor", NULL, &m_bShow_app_style_editor);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	const char* Level[] = { "Level_GamePlay", "Level_Boss" };
	static int iCurrentLevel = (int)m_iCurrentLevel;
	ImGui::Combo("##0", &iCurrentLevel, Level, IM_ARRAYSIZE(Level));
	m_iCurrentLevel = (LEVEL)iCurrentLevel;
	ImGui::NewLine();

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Terrain Tool"))
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Move"); ImGui::SameLine();
			ImGui::RadioButton("##Picking for Moving Terrain", &m_PickingType, PICKING_TERRAIN_TRANSFORM); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Shape"); ImGui::SameLine();
			ImGui::RadioButton("##Picking for Shaping Terrain", &m_PickingType, PICKING_TERRAIN_SHAPE); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Brush"); ImGui::SameLine();
			ImGui::RadioButton("##Picking for Brush Terrain", &m_PickingType, PICKING_TERRAIN_BRUSH);


			Set_Terrain_Map();
			Set_Terrain_Shape();
			Set_HeightMap();
			Set_Brush();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Model Tool"))
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Picking for Object"); ImGui::SameLine();
			ImGui::RadioButton("##Picking for Object", &m_PickingType, PICKING_MODEL);


			if (ImGui::BeginTabBar("ModelsTabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Show Model List"))
				{
					Show_ModelList();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Show Current Model List"))
				{
					Show_CurrentModelList();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("TreasureBox Setting"))
				{
					Set_TrasureBox();
					ImGui::EndTabItem();
				}


				ImGui::EndTabBar();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Navigation Tool"))
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Picking for Navigation"); ImGui::SameLine();
			ImGui::Checkbox("##Picking for Navigation", &m_bNaviPicking);

			ImGui::SameLine();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Show Only Navigation"); ImGui::SameLine();
			ImGui::Checkbox("##Show Only Navigation", &m_bShowOnlyNavi);

			ImGui::Text("This is the navigation tool ");
			Set_Navigation();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Camera Tool"))
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Add Camera Mode on"); ImGui::SameLine();
			ImGui::Checkbox("##Add Camera Mode on", &m_bMakeCamera);
			Set_Camera();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Light Tool"))
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Add Light Mode on"); ImGui::SameLine();
			ImGui::Checkbox("##Add Light Mode on", &m_bMakeLight);
			Set_Light();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Effect Tool"))
		{
			Set_Effect();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}


	if (m_bSave || m_bLoad)			BrowseForFolder();
	if (m_bShowSimpleMousePos)      ShowSimpleMousePos(&m_bShowSimpleMousePos);
	if (m_bShow_app_style_editor) { ImGui::Begin("Dear ImGui Style Editor", &m_bShow_app_style_editor); ImGui::ShowStyleEditor(); ImGui::End(); }
	ImGui::End();

}

void CImgui_Manager::BrowseForFolder()
{

	if (m_bSave)
	{
		OPENFILENAME OFN;
		TCHAR filePathName[300] = L"";
		TCHAR lpstrFile[300] = L"";
		static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = g_hWnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 300;
		OFN.lpstrInitialDir = L".";
		OFN.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

		if (GetSaveFileName(&OFN))
		{
			//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
			//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

			HANDLE hFile = 0;
			_ulong dwByte = 0;
			NONANIMDESC  ModelDesc;
			_int iNum = 0;
			_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
			m_TempLayerTags.push_back(LayerTag);
			m_iCurrentLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
			list<CGameObject*>* plistClone = CGameInstance::Get_Instance()->Get_ObjectList(m_iCurrentLevel, LayerTag);
			if (nullptr == plistClone)
			{
				m_bSave = false;
				return;
			}


			iNum = (_int)plistClone->size();

			hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);;
			if (0 == hFile)
				return;

			/* 첫줄은 object 리스트의 size 받아서 갯수만큼 for문 돌리게 하려고 저장해놓음*/
			WriteFile(hFile, &(iNum), sizeof(_int), &dwByte, nullptr);

			for (auto& iter : *plistClone)
			{
				if (iter == nullptr)
					continue;

				ModelDesc = dynamic_cast<CNonAnim*>(iter)->Get_ModelDesc();
				WriteFile(hFile, &ModelDesc, sizeof(NONANIMDESC), &dwByte, nullptr);

			}

			CloseHandle(hFile);

		}

		m_bSave = false;
	}
	else if (m_bLoad)
	{
		OPENFILENAME OFN;
		TCHAR filePathName[300] = L"";
		TCHAR lpstrFile[300] = L"";
		static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = g_hWnd;
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 300;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
			MessageBox(g_hWnd, filePathName, L"열기 선택", MB_OK);

			//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
			//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

			HANDLE hFile = 0;
			_ulong dwByte = 0;
			NONANIMDESC  ModelDesc;
			_int iNum = 0;
			LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
			_matrix			PivotMatrix = XMMatrixIdentity();

			_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
			m_TempLayerTags.push_back(LayerTag);

			hFile = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
				return;

			/* 타일의 개수 받아오기 */
			ReadFile(hFile, &(iNum), sizeof(_int), &dwByte, nullptr);

			for (_uint i = 0; i < iNum; ++i)
			{
				ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
				m_pModel_Manager->Set_InitModelDesc(ModelDesc);
				_tchar			szModeltag[MAX_PATH] = TEXT("");
				MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, (_int)strlen(ModelDesc.pModeltag), szModeltag, MAX_PATH);
				m_pModel_Manager->Create_Model(iLevel, szModeltag, LayerTag, m_pDevice, m_pContext, CModel::TYPE_NONANIM, PivotMatrix);
			}
			CloseHandle(hFile);
		}

		m_bLoad = false;
	}
}

void CImgui_Manager::Set_TrasureBox()
{
	static int selected = 0;
	{
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);

		if ((int)m_pModel_Manager->Get_TreasureBoxSize() != 0)
		{
			for (int i = 0; i < m_pModel_Manager->Get_TreasureBoxSize();)
			{
				//char label[MAX_PATH] = "";
				char szLayertag[MAX_PATH] = "TreasureBox";

				char label[MAX_PATH] = "TreasureBox";
				char buffer[MAX_PATH];
				sprintf(buffer, "%d", i);
				strcat(label, buffer);
				if (ImGui::Selectable(label, m_iTreasureIndex == i))
				{
					m_iTreasureIndex = i;
					m_pModel_Manager->Get_TreasureBox(m_iTreasureIndex)->Set_Picked();
				}
				i++;
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();
	// ------------------------ Right -----------------------------------
	{

		ImGui::BeginGroup();
		ImGui::BeginChild("TreasureBox view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us


		Set_LayerTag();
		ImGui::NewLine();

		ImGui::CollapsingHeader("Show Current TreasureBox");

		ImGui::Text("Selected Index : "); ImGui::SameLine();  ImGui::Text("%d", m_iTreasureIndex);
		CTreasureBox* pTreasureBox = m_pModel_Manager->Get_TreasureBox(m_iTreasureIndex);
		

		if (pTreasureBox != nullptr)
		{
			XMStoreFloat3(&m_vPickedObjPos, pTreasureBox->Get_Position());
			CTreasureBox::BOXTAG  BoxTag = pTreasureBox->Get_BoxDesc();
			m_ItemType = BoxTag.eItemType;
		}
		else
		{
			m_vPickedObjPos = _float3(0.f, 0.f, 0.f);
			m_vPickedObjScale = _float3(1.f, 1.f, 1.f);
			m_ItemType = 0;
		}

		ImGui::NewLine();
		ImGui::BulletText("Position");
		ImGui::Text("Position X");
		ImGui::SameLine();
		ImGui::DragFloat("##PositionX", &m_vPickedObjPos.x, 0.01f);

		ImGui::Text("Position Z");
		ImGui::SameLine();
		ImGui::DragFloat("##PositionZ", &m_vPickedObjPos.z, 0.01f);

		ImGui::Text("Position Y");
		ImGui::SameLine();
		ImGui::DragFloat("##PositionY", &m_vPickedObjPos.y, 0.01f, -10, 10);

		ImGui::NewLine();
	

		const char* TypeList[] = { "Item1", "Item2","Item3","Item4", "Item5", "Item6", "Item7" };
		ImGui::Combo("ItemType", &m_ItemType, TypeList, IM_ARRAYSIZE(TypeList));

		static _float Scale[3] = { m_InitDesc.vScale.x , m_InitDesc.vScale.y, m_InitDesc.vScale.z };
		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", Scale);
		m_InitDesc.vScale = _float3(Scale[0], Scale[1], Scale[2]);

		static _float Rotation[3] = { m_InitDesc.vRotation.x , m_InitDesc.vRotation.y, m_InitDesc.vRotation.z };
		ImGui::Text("Rotation axis"); ImGui::SameLine();
		ImGui::InputFloat3("##2", Rotation);
		m_InitDesc.vRotation = _float3(Rotation[0], Rotation[1], Rotation[2]);

		static _float Offset[2] = { m_InitDesc.m_fAngle , m_fDist };
		ImGui::Text("Rotaion Angle / dist"); ImGui::SameLine();
		ImGui::InputFloat2("##3", Offset);
		m_InitDesc.m_fAngle = Offset[0];
		m_fDist = Offset[1];


		if (pTreasureBox != nullptr)
		{
			CTreasureBox::BOXTAG BoxDesc; 
			BoxDesc.vPosition = m_vPickedObjPos;
			BoxDesc.vScale = m_InitDesc.vScale; 
			BoxDesc.vRotation = m_InitDesc.vRotation;
			BoxDesc.m_fAngle = m_InitDesc.m_fAngle;
			m_vPickedObjPos = BoxDesc.vPosition;
			BoxDesc.eItemType = m_ItemType;

			pTreasureBox->Set_BoxDesc(BoxDesc);
		}


		if (ImGui::Button("Save TreasureBox"))
		{
			Save_TrasureBox();
		}
		if (ImGui::Button("Load TreasureBox"))
		{
			Load_TrasureBox();
		}

		ImGui::EndChild();
		ImGui::EndGroup();
	}


	if (m_PickingType == PICKING_MODEL && CGameInstance::Get_Instance()->Key_Up(DIK_X))
	{
		if (CPickingMgr::Get_Instance()->Picking())
		{
			m_BoxDesc.vPosition = CPickingMgr::Get_Instance()->Get_PickingPos();
			m_BoxDesc.eItemType = m_ItemType;
			m_BoxDesc.vScale = m_InitDesc.vScale;
			m_BoxDesc.vRotation = m_InitDesc.vRotation;
			m_BoxDesc.m_fAngle = m_InitDesc.m_fAngle;
			_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
			m_TempLayerTags.push_back(LayerTag);
			if (FAILED(m_pModel_Manager->Add_TreasureBox(LayerTag, &m_BoxDesc)))
				return;
			m_fTreasureBoxPos = m_pModel_Manager->Get_LastTreasureBox()->Get_BoxDesc().vPosition;

		}
	}
	if (m_PickingType == PICKING_MODEL && CGameInstance::Get_Instance()->Key_Up(DIK_Z))
	{
		_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
		m_iCurrentLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
		list<CGameObject*>* plistClone = CGameInstance::Get_Instance()->Get_ObjectList(m_iCurrentLevel, LayerTag);
		if (nullptr == plistClone || plistClone->size() == 0)
		{
			delete LayerTag;
			return;
		}

		auto iter = --plistClone->end();
		m_pModel_Manager->Out_TreasureBoxGroup(dynamic_cast<CTreasureBox*>(*iter));
		Safe_Release(*iter);
		plistClone->erase(iter);

		delete LayerTag;
	}

	
}

void CImgui_Manager::Save_TrasureBox()
{
	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";
	OFN.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&OFN))
	{
		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		CTreasureBox::BOXTAG	TreasureBoxDesc;

		_uint iNum = 0;
		_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
		m_TempLayerTags.push_back(LayerTag);
		m_iCurrentLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
	
		iNum = m_pModel_Manager->Get_TreasureBoxSize();

		hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);;
		if (0 == hFile)
			return;

		/* 첫줄은 object 리스트의 size 받아서 갯수만큼 for문 돌리게 하려고 저장해놓음*/
		WriteFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

		for (_int i =0; i< iNum; ++i)
		{
			TreasureBoxDesc = m_pModel_Manager->Get_TreasureBox(i)->Get_BoxDesc();
			WriteFile(hFile, &TreasureBoxDesc, sizeof(CTreasureBox::BOXTAG), &dwByte, nullptr);

		}
		CloseHandle(hFile);

	}
}

void CImgui_Manager::Load_TrasureBox()
{
	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";

	if (GetOpenFileName(&OFN) != 0) {
		wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
		MessageBox(g_hWnd, filePathName, L"열기 선택", MB_OK);

		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		CTreasureBox::BOXTAG  ModelDesc;
		_uint iNum = 0;
		LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
		_matrix			PivotMatrix = XMMatrixIdentity();

		_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
		m_TempLayerTags.push_back(LayerTag);

		hFile = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;

		/* 타일의 개수 받아오기 */
		ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

		for (_uint i = 0; i < iNum; ++i)
		{
			ReadFile(hFile, &(ModelDesc), sizeof(CTreasureBox::BOXTAG), &dwByte, nullptr);
			m_pModel_Manager->Add_TreasureBox(LayerTag, &ModelDesc);
			
		}
		CloseHandle(hFile);
	}

}


void CImgui_Manager::Set_LayerTag()
{
	//Layer tag를 만들거나, 삭제한다. 
	ImGui::BulletText("Layer Setting");

	if (ImGui::BeginCombo("Layer List", m_stLayerTags[m_iSeletecLayerNum].c_str()))
	{
		for (int i = 0; i < m_stLayerTags.size(); i++)
		{
			if (ImGui::Selectable(m_stLayerTags[i].c_str(), m_iSeletecLayerNum == i))
				m_iSeletecLayerNum = i;
		}
		ImGui::EndCombo();
	}
	if (ImGui::Button("Delete Layer Tag"))
		ImGui::OpenPopup("Delete Layer Tag?"); ImGui::SameLine();

	if (ImGui::Button("Add New Layer Tag"))
		ImGui::OpenPopup("Add Layer Tag");

	Show_PopupBox();
}

void CImgui_Manager::Set_Macro()
{
	vector<const _tchar*> ModelTags = m_pModel_Manager->Get_PrototypeTag();
	ImGui::NewLine();
	ImGui::BulletText("Macro Setting");
	ImGui::Text("fbx size : "); ImGui::SameLine(); ImGui::Text("%d", ModelTags.size());

	static int LoopOption[2]{ 0 };
	static float fDistance[3]{ 0.f };
	static float fPosition[3]{ 0.f };
	static float fScale[3]{ 1.f, 1.f, 1.f };

	const char* MacroTypeList[] = { "Col & Row", "Col & Aphabet" };
	static int MacroType = 0;
	ImGui::Combo("MacroType", &MacroType, MacroTypeList, IM_ARRAYSIZE(MacroTypeList));

	switch (MacroType)
	{
	case 0:
		ImGui::Text("Col & Row :"); ImGui::SameLine(); ImGui::InputInt2("##Col & Row", LoopOption);
		break;
	case 1:
		ImGui::Text("Col & Aphabet :"); ImGui::SameLine(); ImGui::InputInt2("##Col & Row", LoopOption);
		break;
	default:
		break;
	}


	ImGui::Text("DistanceXYZ :"); ImGui::SameLine(); ImGui::InputFloat3("##DistanceXYZ", fDistance);
	ImGui::Text("PositionXYZ :"); ImGui::SameLine(); ImGui::InputFloat3("##PositionXYZ", fPosition);
	ImGui::Text("ScaleXYZ :"); ImGui::SameLine(); ImGui::InputFloat3("##ScaleXYZ", fScale);

	if (ImGui::Button("Create Macro"))
	{
		if (MacroType == 0)
		{
			for (int i = 0; i < LoopOption[0]; ++i)
			{
				for (int j = 0; j < LoopOption[1]; ++j)
				{
					_int iIndex = i*LoopOption[1] + j;

					m_InitDesc.vPosition = _float3(fPosition[0] + j*fDistance[0], fPosition[1], fPosition[2] + i*fDistance[2]);
					m_InitDesc.vScale = _float3(fScale[0], fScale[1], fScale[2]);
					_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
					Create_Model(ModelTags[iIndex], LayerTag);
					m_TempLayerTags.push_back(LayerTag);
				}
			}
		}
		else if (MacroType == 1)
		{
			_int iIndex = 0;
			for (int i = 0; i < LoopOption[0]; ++i)
			{
				for (int j = 0; j < LoopOption[1]; ++j)
				{
					if (iIndex >= ModelTags.size())
						continue;

					_int LastAlphabet = j + 65;
					_int LastAlphabet2 = ModelTags[iIndex][_tcslen(ModelTags[iIndex]) - 5];

					if (LastAlphabet2 != LastAlphabet)
						continue;

					m_InitDesc.vPosition = _float3(fPosition[0] + j*fDistance[0], fPosition[1], fPosition[2] + i*fDistance[2]);
					m_InitDesc.vScale = _float3(fScale[0], fScale[1], fScale[2]);
					_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
					Create_Model(ModelTags[iIndex], LayerTag);
					m_TempLayerTags.push_back(LayerTag);
					iIndex++;

				}
			}
		}
	}

}

void CImgui_Manager::Set_Navigation()
{
	static int selected = 0;
	{
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);

		if ((int)m_pNavigation_Manager->Get_CellsSize() != 0)
		{
			for (int i = 0; i < m_pNavigation_Manager->Get_CellsSize();)
			{
				//char label[MAX_PATH] = "";
				char szLayertag[MAX_PATH] = "Cell";

				char label[MAX_PATH] = "Cell ";
				char buffer[MAX_PATH];
				sprintf(buffer, "%d", i);
				strcat(label, buffer);
				if (ImGui::Selectable(label, m_iCellIndex == i))
				{
					m_iCellIndex = i;

				}
				i++;
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();
	// ------------------------ Right -----------------------------------
	{

		ImGui::BeginGroup();
		ImGui::BeginChild("Cell view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us


		ImGui::CollapsingHeader("Show Current Cell");

		ImGui::Text("Selected Index : "); ImGui::SameLine();  ImGui::Text("%d", m_iCellIndex);
		m_pNavigation_Manager->Set_CilckedCellIndex(m_iCellIndex);
		CCell* pCurrentCell = m_pNavigation_Manager->Get_Cell();

		static float fPointA[3]{ 0,0,0 };
		static float fPointB[3]{ 0,0,0 };
		static float fPointC[3]{ 0,0,0 };
		static float fClickedPosition[3]{ m_fClickPoint.x, m_fClickPoint.y, m_fClickPoint.z };
		fClickedPosition[0] = m_fClickPoint.x;
		fClickedPosition[1] = m_fClickPoint.y;
		fClickedPosition[2] = m_fClickPoint.z;

		if (pCurrentCell != nullptr)
		{
			fPointA[0] = pCurrentCell->Get_PointValue(CCell::POINT_A).x;
			fPointA[1] = pCurrentCell->Get_PointValue(CCell::POINT_A).y;
			fPointA[2] = pCurrentCell->Get_PointValue(CCell::POINT_A).z;

			fPointB[0] = pCurrentCell->Get_PointValue(CCell::POINT_B).x;
			fPointB[1] = pCurrentCell->Get_PointValue(CCell::POINT_B).y;
			fPointB[2] = pCurrentCell->Get_PointValue(CCell::POINT_B).z;

			fPointC[0] = pCurrentCell->Get_PointValue(CCell::POINT_C).x;
			fPointC[1] = pCurrentCell->Get_PointValue(CCell::POINT_C).y;
			fPointC[2] = pCurrentCell->Get_PointValue(CCell::POINT_C).z;

			m_iCellType = pCurrentCell->Get_CellType();
		}


		ImGui::Text("PointA :"); ImGui::SameLine(); ImGui::InputFloat3("##PointA", fPointA);
		ImGui::Text("PointB :"); ImGui::SameLine(); ImGui::InputFloat3("##PointB", fPointB);
		ImGui::Text("PointC :"); ImGui::SameLine(); ImGui::InputFloat3("##PointC", fPointC);

		ImGui::Text("ClickPointXYZ :"); ImGui::SameLine(); ImGui::DragFloat3("##ClickPointXYZ", fClickedPosition, 0.01f);
		m_fClickPoint = _float3(fClickedPosition[0], fClickedPosition[1], fClickedPosition[2]);
		m_pNavigation_Manager->Update_ClickedPosition(m_fClickPoint);


		if (ImGui::Button("Cancle Click Point"))
			m_pNavigation_Manager->Clear_ClickedPosition();

		if (ImGui::Button("PopBack Cell"))
			m_pNavigation_Manager->Cancle_Cell();
		if (ImGui::Button("Erase Picked Cell"))
			m_pNavigation_Manager->Erase_Cell();
		if (ImGui::Button("All_Clear Cell"))
			m_pNavigation_Manager->Clear_Cells();
		


		ImGui::CollapsingHeader("Setting Cell Type");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "All Accessible"); ImGui::SameLine();
		if (ImGui::RadioButton("##All Accessible", &m_iCellType, 0))
			m_pNavigation_Manager->Set_CellType((CCell::CELLTYPE)m_iCellType);
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Only Jump"); ImGui::SameLine();
		if (ImGui::RadioButton("##Only Jump", &m_iCellType, 1))
			m_pNavigation_Manager->Set_CellType((CCell::CELLTYPE)m_iCellType);
		ImGui::TextColored(ImVec4(0.7f, 0.0f, 1.0f, 1.0f), "Drop"); ImGui::SameLine();
		if (ImGui::RadioButton("##Drop", &m_iCellType, 2))
			m_pNavigation_Manager->Set_CellType((CCell::CELLTYPE)m_iCellType);
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "UpDown"); ImGui::SameLine();
		if (ImGui::RadioButton("##UpDown", &m_iCellType, 3))
			m_pNavigation_Manager->Set_CellType((CCell::CELLTYPE)m_iCellType);


		
		if (ImGui::Button("Save Navigation"))
		{
			Save_Navigation();
		}
		if (ImGui::Button("Load Navigation"))
		{
			Load_Navigation();
		}





		ImGui::EndChild();
		ImGui::EndGroup();
	}





	if (CGameInstance::Get_Instance()->Key_Up(DIK_X))
	{
		if (CPickingMgr::Get_Instance()->Picking())
		{
			_float3 fPosition = CPickingMgr::Get_Instance()->Get_PickingPos();
			_vector vPosition = XMLoadFloat3(&fPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);
			m_pNavigation_Manager->Click_Position(vPosition);
			m_fClickPoint = m_pNavigation_Manager->Get_ClickedPos();
		}

	}
	else if (m_bNaviPicking && CGameInstance::Get_Instance()->Mouse_Up(DIMK_RBUTTON))
	{
		if (CPickingMgr::Get_Instance()->Picking())
		{
			_float3 fPosition = CPickingMgr::Get_Instance()->Get_PickingPos();
			_vector vPosition = XMLoadFloat3(&fPosition);
			vPosition = XMVectorSetW(vPosition, 1.f);
			m_pNavigation_Manager->Find_PickingCell(vPosition);

			m_iCellIndex = m_pNavigation_Manager->Get_CurrentCellIndex();
		}
		
	}

	m_pNavigation_Manager->Render();

}

void CImgui_Manager::Save_Navigation()
{
	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";
	OFN.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&OFN))
	{
		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		_uint iNum = 0;
		_float3	 vPoints[3];
		CCell::CELLTYPE eCelltype;
		m_iCurrentLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();


		iNum = (_int)m_pNavigation_Manager->Get_CellsSize();

		hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);;
		if (0 == hFile)
			return;

		/* 첫줄은 object 리스트의 size 받아서 갯수만큼 for문 돌리게 하려고 저장해놓음*/
		WriteFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iNum; ++i)
		{
			CCell* pCell = m_pNavigation_Manager->Get_Cell(i);
			vPoints[0] = pCell->Get_PointValue(CCell::POINT_A);
			vPoints[1] = pCell->Get_PointValue(CCell::POINT_B);
			vPoints[2] = pCell->Get_PointValue(CCell::POINT_C);
			eCelltype = pCell->Get_CellType();

			WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
			WriteFile(hFile, &eCelltype, sizeof(CCell::CELLTYPE), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}

void CImgui_Manager::Load_Navigation()
{

	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";

	if (GetOpenFileName(&OFN) != 0) {
		wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
		MessageBox(g_hWnd, filePathName, L"열기 선택", MB_OK);

		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		_uint iNum = 0;
		CCell::CELLTYPE eCelltype;
		_float3		vPoints[3];

		hFile = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;

		

		/* 타일의 개수 받아오기 */
		ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

		for(int i =0; i< iNum; ++i)
		{
			ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
			ReadFile(hFile, &eCelltype, sizeof(CCell::CELLTYPE), &dwByte, nullptr);
			if (0 == dwByte)
				break;

			m_pNavigation_Manager->Add_Cell(vPoints, eCelltype);
		}
		CloseHandle(hFile);

	}

}


void CImgui_Manager::Set_Terrain_Map()
{
	ImGui::GetIO().NavActive = false;
	ImGui::GetIO().WantCaptureMouse = true;
	ImGui::CollapsingHeader("Terrain_Map");

	CTerrain_Manager::TERRAINDESC TerrainDesc = m_pTerrain_Manager->Get_TerrainDesc();

	static _int iNumVertice[2] = { TerrainDesc.TerrainDesc.m_iVerticeNumX, TerrainDesc.TerrainDesc.m_iVerticeNumZ };
	ImGui::Text("NumX / NumZ"); ImGui::SameLine(0, 10);
	ImGui::InputInt2("##1", iNumVertice, 10);
	TerrainDesc.TerrainDesc.m_iVerticeNumX = iNumVertice[0];
	TerrainDesc.TerrainDesc.m_iVerticeNumZ = iNumVertice[1];

	ImGui::Text("Position X"); ImGui::SameLine();
	ImGui::DragInt("##PositionX", &TerrainDesc.TerrainDesc.m_iPositionX);

	ImGui::Text("Position Z"); ImGui::SameLine();
	ImGui::DragInt("##PositionZ", &TerrainDesc.TerrainDesc.m_iPositionZ);

	ImGui::Text("Position Y"); ImGui::SameLine();
	ImGui::DragFloat("##PositionY", &TerrainDesc.TerrainDesc.m_fHeight, 0.05f);

	static _int iOffset = m_pTerrain_Manager->Get_MoveOffset();
	ImGui::Text("Move Offset"); ImGui::SameLine();
	ImGui::InputInt("##MoveOffset", &iOffset);
	m_pTerrain_Manager->Set_MoveOffset(iOffset);

	if (ImGui::Button("Update Terrain"))
	{
		m_pTerrain_Manager->Out_DebugTerrain();

		CTerrain_Manager::TERRAINDESC TerrainDesc = CTerrain_Manager::Get_Instance()->Get_TerrainDesc();
		TerrainDesc.m_eDebugTerrain = CTerrain_Manager::DEBUG_SOILD;
		TerrainDesc.m_bShowWireFrame = true;
		CGameInstance::Get_Instance()->Clear_Layer(LEVEL_GAMEPLAY, TEXT("Layer_DebugTerrian"));
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, TEXT("Layer_DebugTerrian"), &TerrainDesc)))
			return;
	}

	ImGui::SameLine();
	if (ImGui::Button("Create Terrain"))
	{
		if (FAILED(m_pTerrain_Manager->Create_Terrain(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"))))
			return;
	}

	ImGui::SameLine();
	ImGui::Checkbox("WireFrame", &TerrainDesc.m_bShowWireFrame);
	m_pTerrain_Manager->Set_bWireFrame(TerrainDesc.m_bShowWireFrame);
	ImGui::SameLine();
	ImGui::Checkbox("Show", &m_bTerrain_Show);
	m_pTerrain_Manager->Set_TerrainShow(m_bTerrain_Show);
	ImGui::NewLine();

	m_pTerrain_Manager->Set_TerrainDesc(&TerrainDesc);

	if (m_PickingType == PICKING_TERRAIN_TRANSFORM )
	{
		if (CGameInstance::Get_Instance()->Mouse_Down(DIMK_LBUTTON))
			CPickingMgr::Get_Instance()->Picking();
	}


	if (CGameInstance::Get_Instance()->Key_Up(DIK_Z))
	{
		list<CGameObject*>* plistClone = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"));
		if (nullptr == plistClone || plistClone->size() == 0)
			return;

		auto iter = --plistClone->end();
		CPickingMgr::Get_Instance()->Out_PickingGroup(*iter);
		Safe_Release(*iter);
		plistClone->erase(iter);
	}

}


void CImgui_Manager::Set_Object_Map()
{
	ImGui::NewLine();
	ImGui::BulletText("Transform Setting");

	static _float Position[3] = { m_InitDesc.vPosition.x , m_InitDesc.vPosition.y, m_InitDesc.vPosition.z };
	ImGui::Text("Position"); ImGui::SameLine();
	ImGui::InputFloat3("##1", Position);

	static _float OffsetPos [3] = { m_vfOffset.x , m_vfOffset.y, m_vfOffset.z };
	ImGui::Text("Offset"); ImGui::SameLine();
	ImGui::InputFloat3("##1", Position);
	m_vfOffset = _float3(OffsetPos[0], OffsetPos[1], OffsetPos[2]);
	m_InitDesc.vPosition = _float3(Position[0], Position[1], Position[2]);

	static _float Scale[3] = { m_InitDesc.vScale.x , m_InitDesc.vScale.y, m_InitDesc.vScale.z };
	ImGui::Text("Scale"); ImGui::SameLine();
	ImGui::InputFloat3("##Scale", Scale);
	m_InitDesc.vScale = _float3(Scale[0], Scale[1], Scale[2]);

	static _float Rotation[3] = { m_InitDesc.vRotation.x , m_InitDesc.vRotation.y, m_InitDesc.vRotation.z };
	ImGui::Text("Rotation axis"); ImGui::SameLine();
	ImGui::InputFloat3("##2", Rotation);
	m_InitDesc.vRotation = _float3(Rotation[0], Rotation[1], Rotation[2]);

	static _float Offset[2] = { m_InitDesc.m_fAngle , m_fDist };
	ImGui::Text("Rotaion Angle / dist"); ImGui::SameLine();
	ImGui::InputFloat2("##3", Offset);
	m_InitDesc.m_fAngle = Offset[0];
	m_fDist = Offset[1];

}

void CImgui_Manager::Set_Terrain_Shape()
{
	ImGui::GetIO().NavActive = false;
	ImGui::GetIO().WantCaptureMouse = true;

	ImGui::CollapsingHeader("Shape Setting (Height, Sharp, Range)");

	ImGui::Text("Height/UpDonwValue");
	ImGui::SameLine();
	ImGui::DragFloat("##fHeight", &m_TerrainShapeDesc.fHeight, 0.1f);

	ImGui::Text("Radius(Shape&Brush)");
	ImGui::SameLine();
	ImGui::DragFloat("##fRadius", &m_TerrainShapeDesc.fRadius, 0.1f);

	ImGui::Text("Sharp(=BrushStrength)");
	ImGui::SameLine();
	ImGui::DragFloat("##fSharp", &m_TerrainShapeDesc.fSharp, 0.1f);


	m_pTerrain_Manager->Set_TerrainShapeDesc(&m_TerrainShapeDesc);

	if (m_PickingType == PICKING_TERRAIN_SHAPE)
	{
		CPickingMgr::Get_Instance()->Picking();
	}

	if (ImGui::Button("Save Terrian"))
	{
		Save_Terrain();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Terrian"))
	{
		Load_Terrain();
	}

}

void CImgui_Manager::Save_Terrain()
{
	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";
	OFN.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&OFN))
	{
		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		_uint iNum = 0;
		_float3	 vPoints[3];
		list<CGameObject*>* pTerrainList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"));


		iNum = (_int)pTerrainList->size();

		hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);;
		if (0 == hFile)
			return;

		/* 첫줄은 object 리스트의 size 받아서 갯수만큼 for문 돌리게 하려고 저장해놓음*/
		WriteFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

		for (auto& iter : *pTerrainList)
		{
			dynamic_cast<CTerrain*>(iter)->Save_Terrain(hFile, &dwByte);
		}

		CloseHandle(hFile);
	}
}

void CImgui_Manager::Load_Terrain()
{
	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";

	if (GetOpenFileName(&OFN) != 0) {
		wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
		MessageBox(g_hWnd, filePathName, L"열기 선택", MB_OK);

		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		_uint iNum = 0;
		_float3		vPoints[3];

		hFile = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		/* 타일의 개수 받아오기 */
		ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);
	
		for (int i = 0; i< iNum; ++i)
		{
			_tchar			szFullPath[MAX_PATH] = TEXT("Prototype_Component_VIBuffer_Terrain_Load");

			if(pGameInstance->Check_Prototype(LEVEL_STATIC, szFullPath) == S_OK)
				pGameInstance->Add_Prototype(LEVEL_STATIC, szFullPath, m_pTerrain_Manager->Create_Terrain(m_pDevice,m_pContext , hFile, dwByte));
			pGameInstance->Add_GameObjectLoad(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), szFullPath);
		}
		CloseHandle(hFile);

		RELEASE_INSTANCE(CGameInstance);
	}
}

void CImgui_Manager::Set_HeightMap()
{
	ImGui::GetIO().NavActive = false;
	ImGui::GetIO().WantCaptureMouse = true;
	ImGui::CollapsingHeader("Height_Map");

	vector<const _tchar*> TerrainTags = m_pTerrain_Manager->Get_PrototypeTagList();

	// ------------------------ Left-----------------------------------
	static int selected = 0;
	{
		ImGui::BeginChild("left pane", ImVec2(150, 50), true);

		int i = 0;

		if (TerrainTags.size() != 0)
		{
			for (auto& iter : TerrainTags)
			{
				if (iter == nullptr)
					continue;
				char label[MAX_PATH];
				char szLayertag[MAX_PATH] = "";
				WideCharToMultiByte(CP_ACP, 0, iter, MAX_PATH, szLayertag, MAX_PATH, NULL, NULL);
				sprintf(label, szLayertag);
				if (ImGui::Selectable(label, selected == i))
					selected = i;
				i++;
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();



	// ------------------------ Right -----------------------------------
	{

		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, 50)); // Leave room for 1 line below us
		char szLayertag[MAX_PATH] = "";
		if (TerrainTags.size() != 0)
			WideCharToMultiByte(CP_ACP, 0, TerrainTags[selected], MAX_PATH, szLayertag, MAX_PATH, NULL, NULL);
		ImGui::Text("Selected :"); ImGui::SameLine();  ImGui::Text(szLayertag);
		
			
			if (ImGui::Button("Load_Bmp"))
			{
				m_pTerrain_Manager->Create_Terrain(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TerrainTags[selected]);
			}
			

		ImGui::EndChild();
		ImGui::EndGroup();
	}

}

void CImgui_Manager::Set_Brush()
{
	ImGui::GetIO().NavActive = false;
	ImGui::GetIO().WantCaptureMouse = true;


	ImGui::CollapsingHeader("Setting Brush Type");

	static char FilePath[MAX_PATH] = "";
	WideCharToMultiByte(CP_ACP, 0, m_pBmpFilePath, MAX_PATH, FilePath, MAX_PATH, NULL, NULL);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "BmpPath"); ImGui::SameLine();
	if(ImGui::InputText("##BmpPath", FilePath, MAX_PATH))
	MultiByteToWideChar(CP_ACP, 0, FilePath, MAX_PATH, m_pBmpFilePath, MAX_PATH); ImGui::SameLine();

	if (ImGui::Button("Update"))
		m_bUpdateTerrain = true;

	ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.0f, 1.0f), "red"); ImGui::SameLine();
	ImGui::RadioButton("##RED", &m_eBrushType, 0);
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "green"); ImGui::SameLine();
	ImGui::RadioButton("##GREEN", &m_eBrushType, 1);
	ImGui::TextColored(ImVec4(0.5f, 0.7f, 1.0f, 1.0f), "blue"); ImGui::SameLine();
	ImGui::RadioButton("##BLUE", &m_eBrushType, 2);
		

	if (m_PickingType == PICKING_TERRAIN_BRUSH)
	{
		CPickingMgr::Get_Instance()->Picking();
	}
}

void CImgui_Manager::ShowSimpleMousePos(bool* p_open)
{
	static int location = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (location >= 0)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	else if (location == -2)
	{
		// Center window
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Mouse Debug tab", p_open, window_flags))
	{
		ImGui::Text("Mouse Debug");
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
		{
			_float3 vPickingPos = CPickingMgr::Get_Instance()->Get_PickingPos();
			ImGui::Text("Mouse UI: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			ImGui::Text("Mouse World: (%.1f,%.1f,%.1f)", vPickingPos.x, vPickingPos.y, vPickingPos.z);
		}
		else
			ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
			if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
			if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
			if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
			if (p_open && ImGui::MenuItem("Close")) *p_open = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}


void CImgui_Manager::ShowPickedObj()
{
	CPickingMgr* pPickingMgr = GET_INSTANCE(CPickingMgr);
	CGameObject* pPickedObj = pPickingMgr->Get_PickedObj();


	if (pPickedObj != nullptr)
	{
		DirectX::XMStoreFloat3(&m_vPickedObjPos, dynamic_cast<CBaseObj*>(pPickedObj)->Get_Position());
		m_vPickedObjScale = dynamic_cast<CBaseObj*>(pPickedObj)->Get_Scale();
		m_vPickedRotAxis = dynamic_cast<CNonAnim*>(pPickedObj)->Get_ModelDesc().vRotation;
		m_fRotAngle = dynamic_cast<CNonAnim*>(pPickedObj)->Get_ModelDesc().m_fAngle;
	}
	else
	{
		m_vPickedObjPos = _float3(0.f, 0.f, 0.f);
		m_vPickedObjScale = _float3(1.f, 1.f, 1.f);
		m_fRotAngle = 0.f;
 	}


	ImGui::NewLine();
	ImGui::BulletText("Position");
	ImGui::Text("Position X");
	ImGui::SameLine();
	ImGui::DragFloat("##PositionX", &m_vPickedObjPos.x, 0.01f);

	ImGui::Text("Position Z");
	ImGui::SameLine();
	ImGui::DragFloat("##PositionZ", &m_vPickedObjPos.z, 0.01f);

	ImGui::Text("Position Y");
	ImGui::SameLine();
	ImGui::DragFloat("##PositionY", &m_vPickedObjPos.y, 0.01f);

	ImGui::NewLine();

	ImGui::BulletText("Scale");

	static _float Pos[3] = { m_vPickedObjScale.x, m_vPickedObjScale.y,  m_vPickedObjScale.z };
	Pos[0] = m_vPickedObjScale.x;
	Pos[1] = m_vPickedObjScale.y;
	Pos[2] = m_vPickedObjScale.z;

	ImGui::Text("Scale");
	ImGui::SameLine();
	ImGui::InputFloat3("##SettingScale", Pos);
	m_vPickedObjScale = _float3(Pos[0], Pos[1], Pos[2]);

	ImGui::BulletText("Rotation");
	static _float Rotation[3] = { m_vPickedRotAxis.x , m_vPickedRotAxis.y, m_vPickedRotAxis.z };
	ImGui::Text("Rotation axis"); ImGui::SameLine();
	ImGui::InputFloat3("##Setting RotAxis", Rotation);
	m_vPickedRotAxis = _float3(Rotation[0], Rotation[1], Rotation[2]);

	ImGui::Text("Rotaion Angle"); ImGui::SameLine();
	ImGui::DragFloat("##Setting Rot", &m_fRotAngle, 0.1f);
	//m_fRotAngle = RotAngle;


	if (pPickedObj != nullptr)
	{
		_vector vSettingPosition = DirectX::XMLoadFloat3(&m_vPickedObjPos);
		vSettingPosition = XMVectorSetW(vSettingPosition, 1.f);
		dynamic_cast<CBaseObj*>(pPickedObj)->Set_State(CTransform::STATE_TRANSLATION, vSettingPosition);
		dynamic_cast<CBaseObj*>(pPickedObj)->Set_Scale(m_vPickedObjScale);
		dynamic_cast<CNonAnim*>(pPickedObj)->Turn(m_vPickedRotAxis, m_fRotAngle);
	}

	if (ImGui::Button("Delete Object"))
	{
		CPickingMgr* pPickingMgr = GET_INSTANCE(CPickingMgr);
		if (pPickingMgr->Get_PickedObj() != nullptr)
			ImGui::OpenPopup("Delete Object?");
		RELEASE_INSTANCE(CPickingMgr);

	}
	Show_PopupBox();
	RELEASE_INSTANCE(CPickingMgr);
}

void CImgui_Manager::Show_PopupBox()
{
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	bool unused_open = true;

	if (ImGui::BeginPopupModal("Delete Object?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		CPickingMgr* pPickingMgr = GET_INSTANCE(CPickingMgr);
		CGameObject* pPickedObj = pPickingMgr->Get_PickedObj();
		CNonAnim* pNonAnim = dynamic_cast<CNonAnim*>(pPickedObj);
		ImGui::Text("Are yoou Sure Delete This Object?\n\n");
		ImGui::Separator();

		ImGui::Text("Object Name : ");  ImGui::SameLine(); ImGui::Text(pNonAnim->Get_Modeltag());

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			m_iCreatedSelected = 0;
			pPickingMgr->Set_PickedObj(nullptr);
			pPickedObj->Set_Dead(true);
			m_pModel_Manager->Out_CreatedModel(pNonAnim);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();

		RELEASE_INSTANCE(CPickingMgr);
	}


	if (ImGui::BeginPopupModal("Delete Layer Tag?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("All those LayerTag files will be deleted.\nThis operation cannot be undone!\n\n");
		ImGui::Separator();

		static int unused_i = 0;
		ImGui::Combo("Combo", &unused_i, "Layer1\0Layer2\0");

		static bool dont_ask_me_next_time = false;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
		ImGui::PopStyleVar();

		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}


	if (ImGui::BeginPopupModal("Add Layer Tag", NULL, ImGuiWindowFlags_MenuBar))
	{
		static char LayerBuffer[MAX_PATH] = "";

		ImGui::Text("Enter Layer Tag");
		ImGui::InputText("Layer Tag", LayerBuffer, MAX_PATH);

		if (ImGui::Button("Add Layer Tag"))
		{
			bool bFailed = false;

			if (m_stLayerTags.size() != 0)
			{
				for (auto& iter : m_stLayerTags)
				{
					if (!strcmp("", LayerBuffer) || !strcmp(iter.c_str(), LayerBuffer))
					{
						bFailed = true;
						ImGui::OpenPopup("Failed");
					}

				}
			}
			if (!bFailed)
			{
				string NewLayer = LayerBuffer;
				m_stLayerTags.push_back(NewLayer);
				ImGui::OpenPopup("Succese");
			}

		}

		if (ImGui::BeginPopupModal("Failed", &unused_open))
		{
			ImGui::Text("Failed Add Layer Tag");
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (ImGui::BeginPopupModal("Succese", &unused_open))
		{
			ImGui::Text("Succese");
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}


		ImGui::SameLine();
		if (ImGui::Button("Close"))  ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}

}


void CImgui_Manager::Show_ModelList()
{

	vector<const _tchar*> ModelTags = m_pModel_Manager->Get_PrototypeTag();

	// ------------------------ Left-----------------------------------
	static int selected = 0;
	{
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);

		int i = 0;

		if (ModelTags.size() != 0)
		{
			for (auto& iter : ModelTags)
			{
				if (iter == nullptr)
					continue;
				char label[MAX_PATH];
				char szLayertag[MAX_PATH] = "";
				WideCharToMultiByte(CP_ACP, 0, iter, MAX_PATH, szLayertag, MAX_PATH, NULL, NULL);
				sprintf(label, szLayertag);
				if (ImGui::Selectable(label, selected == i))
					selected = i;
				i++;
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();

	// ------------------------ Right -----------------------------------
	{

		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		char szLayertag[MAX_PATH] = "";
		if (ModelTags.size() != 0)
			WideCharToMultiByte(CP_ACP, 0, ModelTags[selected], MAX_PATH, szLayertag, MAX_PATH, NULL, NULL);
		ImGui::Text("Selected :"); ImGui::SameLine();  ImGui::Text(szLayertag);
		ImGui::Separator();
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Setting"))
			{
				ImGui::NewLine();
				Set_LayerTag();
				Set_Object_Map();
				ImGui::EndTabItem();

				if (ImGui::Button("Add_Model"))
				{
					_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
					Create_Model(ModelTags[selected], LayerTag);
					m_TempLayerTags.push_back(LayerTag);
				}
				ImGui::SameLine();
				if (ImGui::Button("Revert")) 
				{
					_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
					m_iCurrentLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
					list<CGameObject*>* plistClone = CGameInstance::Get_Instance()->Get_ObjectList(m_iCurrentLevel, LayerTag);
					if (nullptr == plistClone || plistClone->size() == 0)
					{
						delete LayerTag;
						return;
					}


					auto iter = --plistClone->end();
					CPickingMgr::Get_Instance()->Out_PickingGroup(*iter);
					m_pModel_Manager->Out_CreatedModel(dynamic_cast<CNonAnim*>(*iter));
					Safe_Release(*iter);
					plistClone->erase(iter);

					delete LayerTag;
				}
			}
			if (ImGui::BeginTabItem("Macro"))
			{
				ImGui::NewLine();
				Set_LayerTag();
				Set_Macro();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::EndChild();
		ImGui::EndGroup();
	}


	if (m_PickingType == PICKING_MODEL && CGameInstance::Get_Instance()->Key_Up(DIK_X))
	{
		if (CPickingMgr::Get_Instance()->Picking())
		{
			m_InitDesc.vPosition = CPickingMgr::Get_Instance()->Get_PickingPos();
			m_InitDesc.vPosition.x += m_vfOffset.x;
			m_InitDesc.vPosition.y += m_vfOffset.y;
			m_InitDesc.vPosition.z += m_vfOffset.z;
			_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
			Create_Model(ModelTags[selected], LayerTag, true);
			m_TempLayerTags.push_back(LayerTag);

		}
	}

	if (m_PickingType == PICKING_MODEL && CGameInstance::Get_Instance()->Key_Up(DIK_Z))
	{
		_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
		m_iCurrentLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
		list<CGameObject*>* plistClone = CGameInstance::Get_Instance()->Get_ObjectList(m_iCurrentLevel, LayerTag);
		if (nullptr == plistClone || plistClone->size() == 0)
		{
			delete LayerTag;
			return;
		}


		auto iter = --plistClone->end();
		CPickingMgr::Get_Instance()->Out_PickingGroup(*iter);
		CPickingMgr::Get_Instance()->Set_PickedObj(nullptr);
		m_pModel_Manager->Out_CreatedModel(dynamic_cast<CNonAnim*>(*iter));
		Safe_Release(*iter);
		plistClone->erase(iter);

		delete LayerTag;
	}
}

void CImgui_Manager::Show_CurrentModelList()
{
	//vector<class CNonAnim*> vecCreatedModel = m_pModel_Manager->Get_CreatedModel();
	vector<class CNonAnim*> vecCreatedModel = m_pModel_Manager->Get_CreatedModel();


	// Left
	{
		ImGui::BeginChild("Object List", ImVec2(150, 0), true);

		int i = 0;

		if (vecCreatedModel.size() != 0)
		{
			for (auto& iter : vecCreatedModel)
			{
				if (iter == nullptr)
					continue;

				char label[MAX_PATH] = "";
				char szLayertag[MAX_PATH] = "";

				string  ModelTag = iter->Get_Modeltag();
				ModelTag = ModelTag + to_string(i);
				_tchar* RealModelTag = StringToTCHAR(ModelTag);
				WideCharToMultiByte(CP_ACP, 0, RealModelTag, MAX_PATH, szLayertag, MAX_PATH, NULL, NULL);
				sprintf(label, szLayertag);
				delete(RealModelTag);
				if (ImGui::Selectable(label, m_iCreatedSelected == i))
				{
					m_iCreatedSelected = i;
					vecCreatedModel[i]->Set_Picked();
				}

				i++;
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();

	// Right
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		char szLayertag[MAX_PATH] = "";
		if (vecCreatedModel.size() != 0)
		{
			if(vecCreatedModel[m_iCreatedSelected] != nullptr)
				strcpy_s(szLayertag, MAX_PATH, vecCreatedModel[m_iCreatedSelected]->Get_Modeltag());
		}
			
		ImGui::Text(szLayertag);
		ImGui::Separator();
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("LayerList"))
			{
				ShowPickedObj();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Details"))
			{
				ImGui::Text("ID: 0123456789");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndChild();
		if (ImGui::Button("Revert")) {}
		ImGui::SameLine();
		if (ImGui::Button("Save")) {}
		ImGui::EndGroup();
	}


	if (m_PickingType == PICKING_MODEL && CGameInstance::Get_Instance()->Key_Up(DIK_Z))
	{
		_tchar* LayerTag = StringToTCHAR(m_stLayerTags[m_iSeletecLayerNum]);
		m_iCurrentLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
		list<CGameObject*>* plistClone = CGameInstance::Get_Instance()->Get_ObjectList(m_iCurrentLevel, LayerTag);
		if (nullptr == plistClone || plistClone->size() == 0)
		{
			delete LayerTag;
			return;
		}

		auto iter = --plistClone->end();
		m_pModel_Manager->Out_CreatedModel(dynamic_cast<CNonAnim*>(*iter));
		Safe_Release(*iter);
		plistClone->erase(iter);

		delete LayerTag;
	}
}


void CImgui_Manager::Set_Camera()
{
	static int selected = 0;
	{
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);

		if ((int)m_pCamera_Manager->Get_CameraSize()!= 0)
		{
			for (int i = 0; i < m_pCamera_Manager->Get_CameraSize();)
			{
				//char label[MAX_PATH] = "";
				char szLayertag[MAX_PATH] = "Camera";

				char label[MAX_PATH] = "Camera";
				char buffer[MAX_PATH];
				sprintf(buffer, "%d", i);
				strcat(label, buffer);
				if (ImGui::Selectable(label, m_iCameraIndex == i))
				{
					m_iCameraIndex = i;

				}
				i++;
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();
	// ------------------------ Right -----------------------------------
	{

		ImGui::BeginGroup();
		ImGui::BeginChild("Cell view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us


		ImGui::CollapsingHeader("Show Current Camera");

		ImGui::Text("Selected Index : "); ImGui::SameLine();  ImGui::Text("%d", m_iCameraIndex);
		m_pCamera_Manager->Set_CilckedCamIndex(m_iCameraIndex);
		CNonAnim* pCurrentCam = m_pCamera_Manager->Get_CurrentCam();

	
		static float fCamPositionX  = m_fCamPosition.x ;
		static float fCamPositionY = m_fCamPosition.y ;
		static float fCamPositionZ = m_fCamPosition.z ;
		

		if (pCurrentCam != nullptr)
		{
			_vector vCamPos =  pCurrentCam->Get_Position();
			fCamPositionX = XMVectorGetX(vCamPos);
			fCamPositionY = XMVectorGetY(vCamPos);
			fCamPositionZ = XMVectorGetZ(vCamPos);
			m_fCamPosition = _float3(fCamPositionX, fCamPositionY, fCamPositionZ);
		}


		ImGui::Text("CamPositionXYZ");  
		ImGui::DragFloat("CamPosX", &fCamPositionX, 0.01f);
		ImGui::DragFloat("CamPosY", &fCamPositionY, 0.01f);
		ImGui::DragFloat("CamPosZ", &fCamPositionZ, 0.01f);

		m_fCamPosition = _float3(fCamPositionX, fCamPositionY, fCamPositionZ);
		//m_pCamera_Manager->Update_CamPosition(m_fCamPosition);


		if (ImGui::Button("Erase Picked Camera"))
		{
			m_pCamera_Manager->Out_CreatedCamera(pCurrentCam);
			pCurrentCam->Set_Dead(true);
		}
			
		if (ImGui::Button("All_Clear Camera"))
			m_pCamera_Manager->Clear_Camreras();


		if (ImGui::Button("Save Camera"))
		{
			Save_Camera();
		}
		if (ImGui::Button("Load Camera"))
		{
			Load_Camera();
		}

		ImGui::EndChild();
		ImGui::EndGroup();
	}


	if (m_bMakeCamera && CGameInstance::Get_Instance()->Key_Up(DIK_X))
	{
		
		if (FAILED(m_pCamera_Manager->Add_Camera()))
			return;
		XMStoreFloat3(&m_fCamPosition, m_pCamera_Manager->Get_LastCam()->Get_Position());
	}
	
}

void CImgui_Manager::Save_Camera()
{
	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";
	OFN.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&OFN))
	{
		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		_uint iNum = 0;
		_float3	 vPosition;
		m_iCurrentLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();

		list<CGameObject*>* plistClone = CGameInstance::Get_Instance()->Get_ObjectList(m_iCurrentLevel, TEXT("Layer_CameraModel"));
		iNum = (_int)m_pCamera_Manager->Get_CameraSize();


		hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);;
		if (0 == hFile)
			return;

		/* 첫줄은 object 리스트의 size 받아서 갯수만큼 for문 돌리게 하려고 저장해놓음*/
		WriteFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

		for (auto& iter : *plistClone)
		{
			
			XMStoreFloat3(&vPosition, dynamic_cast<CBaseObj*>(iter)->Get_Position());
			WriteFile(hFile, &vPosition, sizeof(_float3), &dwByte, nullptr);

		}

		CloseHandle(hFile);
	}
}

void CImgui_Manager::Load_Camera()
{
	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";

	if (GetOpenFileName(&OFN) != 0) {
		wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
		MessageBox(g_hWnd, filePathName, L"열기 선택", MB_OK);

		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		_uint iNum = 0;
		_float3		vPosition;

		hFile = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;



		/* 타일의 개수 받아오기 */
		ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i< iNum; ++i)
		{
			ReadFile(hFile, &vPosition, sizeof(_float3), &dwByte, nullptr);
			if (0 == dwByte)
				break;

			m_pCamera_Manager->Add_Camera(vPosition);
		}
		CloseHandle(hFile);

	}
}

void CImgui_Manager::Set_Light()
{
	static int selected = 0;
	{
		ImGui::BeginChild("left pane", ImVec2(150, 0), true);

		if ((int)CGameInstance::Get_Instance()->Get_LightSize() != 0)
		{
			for (int i = 0; i < CGameInstance::Get_Instance()->Get_LightSize();)
			{
				//char label[MAX_PATH] = "";
				char szLayertag[MAX_PATH] = "Light";

				char label[MAX_PATH] = "Light";
				char buffer[MAX_PATH];
				sprintf(buffer, "%d", i);
				strcat(label, buffer);
				if (ImGui::Selectable(label, m_iLightIndex == i))
				{
					m_iLightIndex = i;

				}
				i++;
			}
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();
	// ------------------------ Right -----------------------------------
	{

		ImGui::BeginGroup();
		ImGui::BeginChild("Cell view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us


		ImGui::CollapsingHeader("Show Current Light");

		ImGui::Text("Selected Index : "); ImGui::SameLine();  ImGui::Text("%d", m_iLightIndex);
		
		
		static float fPositionX = m_LightDesc.vPosition.x;
		static float fPositionY = m_LightDesc.vPosition.y;
		static float fPositionZ = m_LightDesc.vPosition.z;


		LIGHTDESC LightDesc;
		ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
		if (CGameInstance::Get_Instance()->Get_LightDesc(m_iLightIndex) != nullptr)
		{
			LightDesc = *CGameInstance::Get_Instance()->Get_LightDesc(m_iLightIndex);
			memcpy(&m_LightDesc, &m_LightDesc, sizeof(LIGHTDESC));
			m_iLightType = LightDesc.eType;
		}

		const char* TypeList[] = { "TYPE_DIRECTIONAL", "TYPE_POINT" };
		ImGui::Combo("LightType", &m_iLightType, TypeList, IM_ARRAYSIZE(TypeList));
		m_LightDesc.eType = (LIGHTDESC::TYPE)m_iLightType;


		ImGui::Text("LightPosXYZ");
		ImGui::DragFloat("LightPosX", &LightDesc.vPosition.x, 0.01f);
		ImGui::DragFloat("LightPosY", &LightDesc.vPosition.y, 0.01f);
		ImGui::DragFloat("LightPosZ", &LightDesc.vPosition.z, 0.01f);
		m_LightDesc.vPosition = LightDesc.vPosition;
		
		
		static float vDiffuse[4]{ LightDesc.vDiffuse.x,  LightDesc.vDiffuse.y, LightDesc.vDiffuse.z , LightDesc.vDiffuse.w };
		vDiffuse[0] = LightDesc.vDiffuse.x;
		vDiffuse[1] = LightDesc.vDiffuse.y;
		vDiffuse[2] = LightDesc.vDiffuse.z;
		vDiffuse[3] = LightDesc.vDiffuse.w;
		ImGui::DragFloat4("Diffuse", vDiffuse, 0.01f, 0.f, 1.f);
		LightDesc.vDiffuse = _float4(vDiffuse[0], vDiffuse[1], vDiffuse[2], vDiffuse[3]);
		m_LightDesc.vDiffuse = _float4(vDiffuse[0], vDiffuse[1], vDiffuse[2], vDiffuse[3]);
		

		
		static float vAmbient[4]{ LightDesc.vAmbient.x,  LightDesc.vAmbient.y, LightDesc.vAmbient.z , LightDesc.vAmbient.w };
		vAmbient[0] = LightDesc.vAmbient.x;
		vAmbient[1] = LightDesc.vAmbient.y;
		vAmbient[2] = LightDesc.vAmbient.z;
		vAmbient[3] = LightDesc.vAmbient.w;
		ImGui::DragFloat4("Ambient", vAmbient, 0.01f, 0.f, 1.f);
		LightDesc.vAmbient = _float4(vAmbient[0], vAmbient[1], vAmbient[2], vAmbient[3]);
		m_LightDesc.vAmbient = _float4(vAmbient[0], vAmbient[1], vAmbient[2], vAmbient[3]);

		
		static float vSpecular[4]{ LightDesc.vSpecular.x,  LightDesc.vSpecular.y, LightDesc.vSpecular.z , LightDesc.vSpecular.w };
		vSpecular[0] =LightDesc.vSpecular.x;
		vSpecular[1] =LightDesc.vSpecular.y;
		vSpecular[2] =LightDesc.vSpecular.z;
		vSpecular[3] =LightDesc.vSpecular.w;
		ImGui::DragFloat4("Specular", vSpecular, 0.01f, 0.f, 1.f);
		LightDesc.vSpecular = _float4(vSpecular[0], vSpecular[1], vSpecular[2], vSpecular[3]);
		m_LightDesc.vSpecular = _float4(vSpecular[0], vSpecular[1], vSpecular[2], vSpecular[3]);

		if (m_LightDesc.eType == LIGHTDESC::TYPE_DIRECTIONAL)
		{
			static float vDirection[4]{ LightDesc.vDirection.x,  LightDesc.vDirection.y, LightDesc.vDirection.z , LightDesc.vDirection.w};
			vDirection[0] = LightDesc.vDirection.x;
			vDirection[1] = LightDesc.vDirection.y;
			vDirection[2] = LightDesc.vDirection.z;
			vDirection[3] = LightDesc.vDirection.w;
			ImGui::DragFloat4("Direction", vDirection, 0.01f, 0.f, 1.f);
			m_LightDesc.vDirection = _float4(vDirection[0], vDirection[1], vDirection[2], vDirection[3]);
			LightDesc.vDirection = _float4(vDirection[0], vDirection[1], vDirection[2], vDirection[3]);

		}
		else
		{
			ImGui::DragFloat("Range", &LightDesc.fRange, 0.01f, 0.f);
			m_LightDesc.fRange = LightDesc.fRange;
		}


		if (CGameInstance::Get_Instance()->Get_LightDesc(m_iLightIndex) != nullptr)
		{
			CGameInstance::Get_Instance()->Set_LightDesc(m_iLightIndex, &m_LightDesc);
		}

		
		if (ImGui::Button("Add Light"))
		{
			if (FAILED(CGameInstance::Get_Instance()->Add_Light(m_pDevice, m_pContext, m_LightDesc)))
				return;
			m_iLightIndex = CGameInstance::Get_Instance()->Get_LightSize() - 1;
		}

		if (ImGui::Button("Erase Picked Light"))
		{
			CGameInstance::Get_Instance()->Clear_Light(m_iLightIndex);
		}
		ImGui::SameLine();
		if (ImGui::Button("All_Clear Light"))
			CGameInstance::Get_Instance()->Clear_AllLight();


		if (ImGui::Button("Save Light"))
		{
			Save_Light();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load Light"))
		{
			Load_Light();
		}

		ImGui::EndChild();
		ImGui::EndGroup();
	}
}

void CImgui_Manager::Save_Light()
{
	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";
	OFN.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&OFN))
	{
		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		_uint iNum = 0;
		LIGHTDESC LightDesc;
		m_iCurrentLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();


		iNum = (_int)CGameInstance::Get_Instance()->Get_LightSize();

		hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);;
		if (0 == hFile)
			return;

		/* 첫줄은 object 리스트의 size 받아서 갯수만큼 for문 돌리게 하려고 저장해놓음*/
		WriteFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iNum; ++i)
		{
			LightDesc = *CGameInstance::Get_Instance()->Get_LightDesc(i);
			WriteFile(hFile, &LightDesc, sizeof(LIGHTDESC), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
}

void CImgui_Manager::Load_Light()
{
	OPENFILENAME OFN;
	TCHAR filePathName[300] = L"";
	TCHAR lpstrFile[300] = L"";
	static TCHAR filter[] = L"데이터 파일\0*.dat\0텍스트 파일\0*.txt";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 300;
	OFN.lpstrInitialDir = L".";

	if (GetOpenFileName(&OFN) != 0) {
		wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
		MessageBox(g_hWnd, filePathName, L"열기 선택", MB_OK);

		//ERR_MSG(TEXT("save-as  '%s'\n"), ofn.lpstrFile); //경로// 파일이름.확장자
		//ERR_MSG(TEXT("filename '%s'\n"), ofn.lpstrFile + ofn.nFileOffset); 

		HANDLE hFile = 0;
		_ulong dwByte = 0;
		_uint iNum = 0;
		LIGHTDESC LightDesc;

		hFile = CreateFile(OFN.lpstrFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return;

		/* 타일의 개수 받아오기 */
		ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i< iNum; ++i)
		{
			ReadFile(hFile, &LightDesc, sizeof(LIGHTDESC), &dwByte, nullptr);
			if (0 == dwByte)
				break;

			CGameInstance::Get_Instance()->Add_Light(m_pDevice, m_pContext, LightDesc);
		}
		CloseHandle(hFile);

	}
}

void CImgui_Manager::Draw_EffectModals()
{
	/* Create Effect Modal */
	ImVec2 pCenter = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(pCenter, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f)); // Center Window when appearing
	if (ImGui::BeginPopupModal("Create Effect", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Effect Created!");
		ImGui::NewLine();
		ImGui::Text("You can customize you effect by adding");
		ImGui::Text("Textures, Meshes and/or Particles.");
		ImGui::Separator();

		_float fButtonSize = 100.f;
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2) - (fButtonSize / 2));
		
		if (ImGui::Button("Got it!", ImVec2(fButtonSize, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::SetItemDefaultFocus();
		ImGui::EndPopup();
	}

	/* Create Curve Editor Modal */
	ImGui::SetNextWindowPos(pCenter, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f)); // Center Window when appearing
	if (ImGui::BeginPopupModal("Curve Editor", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("CurveEditorTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Velocity Curves"))
			{
				if (ImGui::BeginTable("VelocityCurvesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchSame, ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 6)))
				{
					ImGui::TableSetupScrollFreeze(0, 1);
					ImGui::TableSetupColumn("Velocity");
					ImGui::TableSetupColumn("Start");
					ImGui::TableSetupColumn("End");
					ImGui::TableHeadersRow();

					vector<_float3> VelocityCurves;

					if (m_pSelectedEffect)
						VelocityCurves = m_pSelectedEffect->Get_VelocityCurves();

					for (_uint i = 0; i < VelocityCurves.size(); i++)
					{
						ImGui::TableNextColumn();
						if (ImGui::Selectable(to_string(VelocityCurves[i].x).c_str(), i == m_iSelectedVelocityCurve, ImGuiSelectableFlags_SpanAllColumns)) /* Velocity */
							m_iSelectedVelocityCurve = i;

						ImGui::TableNextColumn();
						ImGui::Text(to_string(VelocityCurves[i].y).c_str()); /* Start */

						ImGui::TableNextColumn();
						ImGui::Text(to_string(VelocityCurves[i].z).c_str()); /* End */
					}

					ImGui::EndTable();
				}
				if (ImGui::Button("Delete"))
					if (m_pSelectedEffect->Get_VelocityCurves().size() > m_iSelectedVelocityCurve)
						m_pSelectedEffect->Remove_VelocityCurve(m_iSelectedVelocityCurve);

				ImGui::NewLine();

				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##VelocityCurve", &m_fCurveValue, 0.05f, 0.f, 100.f, "Velocity: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##VelocityStart", &m_fCurveStart, 0.05f, 0.f, 1.f, "Start: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##VelocityEnd", &m_fCurveEnd, 0.05f, 0.f, 1.f, "End: %.02f"))
				{
				}
				ImGui::SameLine();

				if (ImGui::Button("Add"))
				{
					if (m_pSelectedEffect)
						m_pSelectedEffect->Add_VelocityCurve(_float3(m_fCurveValue, m_fCurveStart, m_fCurveEnd));
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Size Curves"))
			{
				if (ImGui::BeginTable("SizeCurvesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchSame, ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 6)))
				{
					ImGui::TableSetupScrollFreeze(0, 1);
					ImGui::TableSetupColumn("Size");
					ImGui::TableSetupColumn("Start");
					ImGui::TableSetupColumn("End");
					ImGui::TableHeadersRow();

					vector<_float3> SizeCurves;

					if (m_pSelectedEffect)
						SizeCurves = m_pSelectedEffect->Get_SizeCurves();

					for (_uint i = 0; i < SizeCurves.size(); i++)
					{
						ImGui::TableNextColumn();
						if (ImGui::Selectable(to_string(SizeCurves[i].x).c_str(), i == m_iSelectedSizeCurve, ImGuiSelectableFlags_SpanAllColumns)) /* Size */
							m_iSelectedSizeCurve = i;

						ImGui::TableNextColumn();
						ImGui::Text(to_string(SizeCurves[i].y).c_str()); /* Start */

						ImGui::TableNextColumn();
						ImGui::Text(to_string(SizeCurves[i].z).c_str()); /* End */
					}

					ImGui::EndTable();
				}
				if (ImGui::Button("Delete"))
					if (m_pSelectedEffect->Get_SizeCurves().size() > m_iSelectedSizeCurve)
						m_pSelectedEffect->Remove_SizeCurve(m_iSelectedSizeCurve);

				ImGui::NewLine();

				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##SizeCurve", &m_fCurveValue, 0.05f, 0.f, 100.f, "Size: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##SizeStart", &m_fCurveStart, 0.05f, 0.f, 1.f, "Start: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##SizeEnd", &m_fCurveEnd, 0.05f, 0.f, 1.f, "End: %.02f"))
				{
				}
				ImGui::SameLine();

				if (ImGui::Button("Add"))
				{
					if (m_pSelectedEffect)
						m_pSelectedEffect->Add_SizeCurve(_float3(m_fCurveValue, m_fCurveStart, m_fCurveEnd));
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Alpha Curves"))
			{
				if (ImGui::BeginTable("AlphaCurvesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchSame, ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 6)))
				{
					ImGui::TableSetupScrollFreeze(0, 1);
					ImGui::TableSetupColumn("Alpha");
					ImGui::TableSetupColumn("Start");
					ImGui::TableSetupColumn("End");
					ImGui::TableHeadersRow();

					vector<_float3> AlphaCurves;

					if (m_pSelectedEffect)
						AlphaCurves = m_pSelectedEffect->Get_AlphaCurves();

					for (_uint i = 0; i < AlphaCurves.size(); i++)
					{
						ImGui::TableNextColumn();
						if (ImGui::Selectable(to_string(AlphaCurves[i].x).c_str(), i == m_iSelectedAlphaCurve, ImGuiSelectableFlags_SpanAllColumns)) /* Alpha */
							m_iSelectedAlphaCurve = i;

						ImGui::TableNextColumn();
						ImGui::Text(to_string(AlphaCurves[i].y).c_str()); /* Start */

						ImGui::TableNextColumn();
						ImGui::Text(to_string(AlphaCurves[i].z).c_str()); /* End */
					}

					ImGui::EndTable();
				}
				if (ImGui::Button("Delete"))
					if (m_pSelectedEffect->Get_AlphaCurves().size() > m_iSelectedAlphaCurve)
						m_pSelectedEffect->Remove_AlphaCurve(m_iSelectedAlphaCurve);

				ImGui::NewLine();

				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##AlphaCurve", &m_fCurveValue, 0.05f, 0.f, 100.f, "Alpha: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##AlphaStart", &m_fCurveStart, 0.05f, 0.f, 1.f, "Start: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##AlphaEnd", &m_fCurveEnd, 0.05f, 0.f, 1.f, "End: %.02f"))
				{
				}
				ImGui::SameLine();

				if (ImGui::Button("Add"))
				{
					if (m_pSelectedEffect)
						m_pSelectedEffect->Add_AlphaCurve(_float3(m_fCurveValue, m_fCurveStart, m_fCurveEnd));
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Turn Velocity Curves"))
			{
				if (ImGui::BeginTable("TurnVelocityCurvesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchSame, ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 6)))
				{
					ImGui::TableSetupScrollFreeze(0, 1);
					ImGui::TableSetupColumn("Turn Velocity");
					ImGui::TableSetupColumn("Start");
					ImGui::TableSetupColumn("End");
					ImGui::TableHeadersRow();

					vector<_float3> TurnVelocityCurves;

					if (m_pSelectedEffect)
						TurnVelocityCurves = m_pSelectedEffect->Get_TurnVelocityCurves();

					for (_uint i = 0; i < TurnVelocityCurves.size(); i++)
					{
						ImGui::TableNextColumn();
						if (ImGui::Selectable(to_string(TurnVelocityCurves[i].x).c_str(), i == m_iSelectedTurnVelocityCurve, ImGuiSelectableFlags_SpanAllColumns)) /* Alpha */
							m_iSelectedTurnVelocityCurve = i;

						ImGui::TableNextColumn();
						ImGui::Text(to_string(TurnVelocityCurves[i].y).c_str()); /* Start */

						ImGui::TableNextColumn();
						ImGui::Text(to_string(TurnVelocityCurves[i].z).c_str()); /* End */
					}

					ImGui::EndTable();
				}
				if (ImGui::Button("Delete"))
					if (m_pSelectedEffect->Get_TurnVelocityCurves().size() > m_iSelectedTurnVelocityCurve)
						m_pSelectedEffect->Remove_TurnVelocityCurve(m_iSelectedTurnVelocityCurve);

				ImGui::NewLine();

				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##TurnVelocityCurve", &m_fCurveValue, 0.05f, 0.f, 100.f, "Velocity: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##TurnVelocityStart", &m_fCurveStart, 0.05f, 0.f, 1.f, "Start: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##TurnVelocityEnd", &m_fCurveEnd, 0.05f, 0.f, 1.f, "End: %.02f"))
				{
				}
				ImGui::SameLine();

				if (ImGui::Button("Add"))
				{
					if (m_pSelectedEffect)
						m_pSelectedEffect->Add_TurnVelocityCurve(_float3(m_fCurveValue, m_fCurveStart, m_fCurveEnd));
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Noise Power Curves"))
			{
				if (ImGui::BeginTable("NoisePowerCurvesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchSame, ImVec2(0, ImGui::GetTextLineHeightWithSpacing() * 6)))
				{
					ImGui::TableSetupScrollFreeze(0, 1);
					ImGui::TableSetupColumn("Noise Power");
					ImGui::TableSetupColumn("Start");
					ImGui::TableSetupColumn("End");
					ImGui::TableHeadersRow();

					vector<_float3> NoisePowerCurves;

					if (m_pSelectedEffect)
						NoisePowerCurves = m_pSelectedEffect->Get_NoisePowerCurves();

					for (_uint i = 0; i < NoisePowerCurves.size(); i++)
					{
						ImGui::TableNextColumn();
						if (ImGui::Selectable(to_string(NoisePowerCurves[i].x).c_str(), i == m_iSelectedNoisePowerCurve, ImGuiSelectableFlags_SpanAllColumns)) /* Alpha */
							m_iSelectedNoisePowerCurve = i;

						ImGui::TableNextColumn();
						ImGui::Text(to_string(NoisePowerCurves[i].y).c_str()); /* Start */

						ImGui::TableNextColumn();
						ImGui::Text(to_string(NoisePowerCurves[i].z).c_str()); /* End */
					}

					ImGui::EndTable();
				}
				if (ImGui::Button("Delete"))
					if (m_pSelectedEffect->Get_NoisePowerCurves().size() > m_iSelectedNoisePowerCurve)
						m_pSelectedEffect->Remove_NoisePowerCurve(m_iSelectedNoisePowerCurve);

				ImGui::NewLine();

				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##NoisePowerCurve", &m_fCurveValue, 0.05f, 0.f, 100.f, "Power: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##NoisePowerStart", &m_fCurveStart, 0.05f, 0.f, 1.f, "Start: %.02f"))
				{
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				if (ImGui::DragFloat("##NoisePowerEnd", &m_fCurveEnd, 0.05f, 0.f, 1.f, "End: %.02f"))
				{
				}
				ImGui::SameLine();

				if (ImGui::Button("Add"))
				{
					if (m_pSelectedEffect)
						m_pSelectedEffect->Add_NoisePowerCurve(_float3(m_fCurveValue, m_fCurveStart, m_fCurveEnd));
				}
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::NewLine();
		if (ImGui::Button("Done"))
			ImGui::CloseCurrentPopup();
	
		ImGui::SetItemDefaultFocus();
		ImGui::EndPopup();
	}
}

void CImgui_Manager::Read_EffectsData()
{
	WIN32_FIND_DATA fileData;
	HANDLE hDir = FindFirstFile(TEXT("../../../Bin/Data/EffectData/*"), &fileData);

	/* No files found */
	if (hDir == INVALID_HANDLE_VALUE)
		return;

	do {
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		if (!lstrcmp(fileData.cFileName, TEXT(".")) || !lstrcmp(fileData.cFileName, TEXT("..")))
			continue;

		wstring wsFileName(fileData.cFileName);
		string sFileName(wsFileName.begin(), wsFileName.end());

		m_SavedEffects.push_back(sFileName);
	} while (FindNextFile(hDir, &fileData));

	FindClose(hDir);
}

void CImgui_Manager::Read_Textures(_tchar* pFolderPath)
{
	_tchar filePath[MAX_PATH] = TEXT("");
	wcscpy_s(filePath, MAX_PATH, pFolderPath); // Backup Path used for Sub-folders

	wcscat_s(pFolderPath, MAX_PATH, TEXT("*"));

	WIN32_FIND_DATA fileData;

	HANDLE hDir = FindFirstFile(pFolderPath, &fileData);

	/* No files found */
	if (hDir == INVALID_HANDLE_VALUE)
		return;

	do {
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // Directory
		{
			if (lstrcmp(fileData.cFileName, TEXT(".")) == 0 || lstrcmp(fileData.cFileName, TEXT("..")) == 0)
				continue;

			_tchar subFilePath[MAX_PATH] = TEXT("");
			wcscpy_s(subFilePath, MAX_PATH, filePath);
			wcscat_s(subFilePath, MAX_PATH, fileData.cFileName);
			wcscat_s(subFilePath, MAX_PATH, TEXT("/"));

			// Recursive Function Call
			Read_Textures(subFilePath);
		}
		else // File
		{
			_tchar szFileExt[MAX_PATH];
			_tchar szFileName[MAX_PATH];
			_wsplitpath_s(fileData.cFileName, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szFileExt, MAX_PATH);

			if (!wcscmp(szFileExt, TEXT(".dds")) || !wcscmp(szFileExt, TEXT(".png")))
			{
				wstring wsFileName = wstring(szFileName);
				string sFileName = string(wsFileName.begin(), wsFileName.end());

				m_TextureNames.push_back(sFileName);
			}
		}
	} while (FindNextFile(hDir, &fileData));

	FindClose(hDir);
}

void CImgui_Manager::Read_Meshes(_tchar* pFolderPath)
{
	_tchar filePath[MAX_PATH] = TEXT("");
	wcscpy_s(filePath, MAX_PATH, pFolderPath); // Backup Path used for Sub-folders

	wcscat_s(pFolderPath, MAX_PATH, TEXT("*"));

	WIN32_FIND_DATA fileData;

	HANDLE hDir = FindFirstFile(pFolderPath, &fileData);

	/* No files found */
	if (hDir == INVALID_HANDLE_VALUE)
		return;

	do {
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // Directory
		{
			if (lstrcmp(fileData.cFileName, TEXT(".")) == 0 || lstrcmp(fileData.cFileName, TEXT("..")) == 0)
				continue;

			_tchar subFilePath[MAX_PATH] = TEXT("");
			wcscpy_s(subFilePath, MAX_PATH, filePath);
			wcscat_s(subFilePath, MAX_PATH, fileData.cFileName);
			wcscat_s(subFilePath, MAX_PATH, TEXT("/"));

			// Recursive Function Call
			Read_Meshes(subFilePath);
		}
		else // File
		{
			_tchar szFileExt[MAX_PATH];
			_tchar szFileName[MAX_PATH];
			_wsplitpath_s(fileData.cFileName, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szFileExt, MAX_PATH);

			if (!wcscmp(szFileExt, TEXT(".dat")))
			{
				wstring wsFileName = wstring(szFileName);
				string sFileName = string(wsFileName.begin(), wsFileName.end());

				m_MeshNames.push_back(sFileName);
			}
		}
	} while (FindNextFile(hDir, &fileData));

	FindClose(hDir);
}

void CImgui_Manager::Set_Effect()
{
#pragma region Create Effect
	ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Create", TreeNodeFlags))
	{
		/* Create New Effect */
		ImGui::Text("Effect Name");
		static char cEffectName[MAX_PATH];
		ImGui::SetNextItemWidth(175);
		ImGui::InputText("##InputEffect", cEffectName, MAX_PATH);
		ImGui::SameLine();
		if (ImGui::Button("Create Effect"))
		{
			if (strcmp(cEffectName, ""))
			{
				m_sCurrentEffect = cEffectName; // Set Current Effect
				m_sCurrentEffect += ".dat";
				memset(cEffectName, 0, MAX_PATH);

				ImGui::OpenPopup("Create Effect");
			}
		}
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Text("Saved Effects");

		if (ImGui::BeginListBox("##SavedEffects", ImVec2(-FLT_MIN, 4 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int i = 0; i < m_SavedEffects.size(); i++)
			{
				if (ImGui::Selectable(m_SavedEffects[i].c_str(), m_iSavedEffect == i))
					m_iSavedEffect = i;

				if (m_iSavedEffect == i)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		if (ImGui::Button("Load Effect"))
			Load_Effect();

		ImGui::NewLine();
		ImGui::Separator();

		ImGui::Text("Current Effect:");
		ImGui::SameLine();
		ImGui::Text(m_sCurrentEffect.c_str());
		
		if (ImGui::Button("Save Effect"))
			Save_Effect();

		ImGui::NewLine();
	}
#pragma endregion Create
	
#pragma region Add
	if (ImGui::CollapsingHeader("Add"))
	{
		ImGui::Text("Available Resources");
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("EffectTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Effect Textures"))
			{
				/* Available Textures (Effect) */
				if (ImGui::BeginListBox("##Texture List", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() / 6)))
				{
					for (string sTextureName : m_TextureNames)
					{
						if (ImGui::Selectable(sTextureName.c_str(), m_sSelectedResource == sTextureName))
							m_sSelectedResource = sTextureName;

						if (m_sSelectedResource == sTextureName)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();

					if (ImGui::Button("Add Texture", ImVec2(100, 0)))
					{
						if (!m_sSelectedResource.empty())
						{
							CEffectTexture::TEXTUREEFFECTDESC tTextureEffectDesc;

							wstring wsTexturePrototype = wstring(m_sSelectedResource.begin(), m_sSelectedResource.end());
							wcscpy_s(tTextureEffectDesc.wcPrototypeId, MAX_PATH, wsTexturePrototype.c_str());

							CEffect* pEffect = nullptr;
							CGameInstance::Get_Instance()->Add_GameObject_Out(TEXT("Prototype_GameObject_EffectTexture"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), (CGameObject*&)pEffect, &tTextureEffectDesc);

							m_pEffectManager->Add_Effect(pEffect);
							pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_TEXTURE);
						}
					}
					ImGui::SameLine();

					if (ImGui::Button("Add Particle", ImVec2(100, 0)))
					{
						if (!m_sSelectedResource.empty())
						{
							CParticleSystem::PARTICLEDESC tParticleDesc;

							wstring wsTexturePrototype = wstring(m_sSelectedResource.begin(), m_sSelectedResource.end());
							wcscpy_s(tParticleDesc.wcPrototypeId, MAX_PATH, wsTexturePrototype.c_str());

							CEffect* pEffect = nullptr;
							CGameInstance::Get_Instance()->Add_GameObject_Out(TEXT("Prototype_GameObject_ParticleSystem"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), (CGameObject*&)pEffect, &tParticleDesc);

							m_pEffectManager->Add_Effect(pEffect);
							pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_PARTICLE);
						}
					}
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Effect Meshes"))
			{
				/* Available Meshes (Effect) */
				if (ImGui::BeginListBox("##Mesh List", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() / 6)))
				{
					for (string sMeshName : m_MeshNames)
					{
						if (ImGui::Selectable(sMeshName.c_str(), m_sSelectedResource == sMeshName))
							m_sSelectedResource = sMeshName;

						if (m_sSelectedResource == sMeshName)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndListBox();

					if (ImGui::Button("Add Mesh", ImVec2(100, 0)))
					{
						if (!m_sSelectedResource.empty())
						{
							CEffectMesh::MESHEFFECTDESC tMeshEffectDesc;

							wstring wsMeshPrototype = wstring(m_sSelectedResource.begin(), m_sSelectedResource.end());
							wcscpy_s(tMeshEffectDesc.wcPrototypeId, MAX_PATH, wsMeshPrototype.c_str());

							CEffect* pEffect = nullptr;
							CGameInstance::Get_Instance()->Add_GameObject_Out(TEXT("Prototype_GameObject_EffectMesh"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), (CGameObject*&)pEffect, &tMeshEffectDesc);

							m_pEffectManager->Add_Effect(pEffect);
							pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_MESH);
						}
					}
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::NewLine();
		ImGui::Separator();

		/* Instanced Effects */
		ImGui::Text("Instanced Effects");

		if (ImGui::BeginListBox("##Instanced Effect List", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() / 6)))
		{
			vector<CEffect*> Effects = CEffect_Manager::Get_Instance()->Get_InstancedEffects();
			for (CEffect* pEffect : Effects)
			{
				wstring wsEffectName = wstring(pEffect->Get_PrototypeId());
				string sEffectName = string(wsEffectName.begin(), wsEffectName.end());

				if (ImGui::Selectable(sEffectName.c_str(), m_sSelectedEffect == sEffectName, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(0, 22)))
				{
					m_sSelectedEffect = sEffectName;
					m_pSelectedEffect = pEffect;
					m_bIsPlaying = false;

					switch (m_pSelectedEffect->Get_EffectType())
					{
						case CEffect::EFFECT_TYPE::TYPE_TEXTURE:
						{
							wcscpy_s(m_tTextureEffectDesc.wcPrototypeId, MAX_PATH, m_pSelectedEffect->Get_PrototypeId());
							break;
						}	
						case CEffect::EFFECT_TYPE::TYPE_MESH:
						{
							wcscpy_s(m_tMeshEffectDesc.wcPrototypeId, MAX_PATH, m_pSelectedEffect->Get_PrototypeId());
							break;
						}
						case CEffect::EFFECT_TYPE::TYPE_PARTICLE:
						{
							wcscpy_s(m_tParticleDesc.wcPrototypeId, MAX_PATH, m_pSelectedEffect->Get_PrototypeId());
							break;
						}
					}

					// Set Transform of the Selected Effect
					CComponent* pComponent = pEffect->Find_Component(TEXT("Com_Transform"));
					if (!pComponent)
						return;
					CTransform* pTransform = dynamic_cast<CTransform*>(pComponent);
					if (!pTransform)
						return;

					m_pSelectedEffectTransform = pTransform;
				}
				ImGui::SameLine();
				ImGui::Checkbox("##EffectSelected", &pEffect->m_bIsSelected);
				

				if (m_sSelectedEffect == sEffectName)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		if (ImGui::Button("Delete", ImVec2(60, 0)))
		{
			if (m_pSelectedEffect)
			{
				m_pEffectManager->Remove_Effect(m_pSelectedEffect);
				m_pSelectedEffect = nullptr;
			}
		}

		ImGui::NewLine();
	}
#pragma endregion Add

#pragma region Customize
	if (ImGui::CollapsingHeader("Customize"))
	{
		if (ImGui::Button(m_bIsPlaying ? "Stop" : "Play", ImVec2(120, 0)))
		{
			if (m_pSelectedEffect)
			{
				m_bIsPlaying = !m_bIsPlaying;

				for (auto& pEffect : m_pEffectManager->Get_InstancedEffects())
				{
					if (pEffect->m_bIsSelected)
						pEffect->Set_Play(m_bIsPlaying);
				}
				
				if (m_bIsPlaying)
				{
					switch (m_pSelectedEffect->Get_EffectType())
					{
						case CEffect::EFFECT_TYPE::TYPE_TEXTURE:
						{
							((CEffectTexture*)m_pSelectedEffect)->Set_TextureEffectDesc(m_tTextureEffectDesc);
							break;
						}
						case CEffect::EFFECT_TYPE::TYPE_MESH:
						{
							((CEffectMesh*)m_pSelectedEffect)->Set_MeshEffectDesc(m_tMeshEffectDesc);
							break;
						}
						case CEffect::EFFECT_TYPE::TYPE_PARTICLE:
						{
							((CParticleSystem*)m_pSelectedEffect)->ResetParticleSystem();
							break;
						}
					}
				}
			}
		}
		
		if (m_pSelectedEffect)
		{
			switch (m_pSelectedEffect->Get_EffectType())
			{
			case CEffect::EFFECT_TYPE::TYPE_TEXTURE:
				Show_TextureCustomization();
				break;
			case CEffect::EFFECT_TYPE::TYPE_MESH:
				Show_MeshCustomization();
				break;
			case CEffect::EFFECT_TYPE::TYPE_PARTICLE:
				Show_ParticleCustomization();
				break;
			}
		}
		ImGui::NewLine();
		ImGui::Separator();
		if (ImGui::Button("Curve Editor", ImVec2(100, 0)))
		{
			if (m_pSelectedEffect)
				ImGui::OpenPopup("Curve Editor");
		}
	}
#pragma endregion Customize

	Draw_EffectModals();
}

_bool CImgui_Manager::Save_Effect()
{
	wstring wsCurrentEffect = wstring(m_sCurrentEffect.begin(), m_sCurrentEffect.end());

	/* Create Effect Data File */
	HANDLE hFileEffect = nullptr;
	_tchar LoadPathEffect[MAX_PATH] = TEXT("../../../Bin/Data/EffectData/");
	wcscat_s(LoadPathEffect, MAX_PATH, wsCurrentEffect.c_str());

	hFileEffect = CreateFile(LoadPathEffect, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileEffect == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwByte = 0;

	/* Write how many Effects are in this File (needed when Loading). */
	_uint iEffectsCount = (_uint)m_pEffectManager->Get_InstancedEffects().size();
	WriteFile(hFileEffect, &iEffectsCount, sizeof(_uint), &dwByte, nullptr);

	vector<CEffect*> Effects = m_pEffectManager->Get_InstancedEffects();
	/* For every Effect in this File: */
	for (_uint i = 0; i < Effects.size(); i++)
	{
		/* Write Effect Type (needed when Loading). */
		CEffect::EFFECT_TYPE eType = Effects[i]->Get_EffectType();
		WriteFile(hFileEffect, &eType, sizeof(CEffect::EFFECT_TYPE), &dwByte, nullptr);

		/* Write Effect Type-specific Description. */
		switch (Effects[i]->Get_EffectType())
		{
			case CEffect::EFFECT_TYPE::TYPE_TEXTURE:
			{
				WriteFile(hFileEffect, &m_tTextureEffectDesc, sizeof(CEffectTexture::TEXTUREEFFECTDESC), &dwByte, nullptr);
				break;
			}
			case CEffect::EFFECT_TYPE::TYPE_MESH:
			{
				WriteFile(hFileEffect, &m_tMeshEffectDesc, sizeof(CEffectMesh::MESHEFFECTDESC), &dwByte, nullptr);
				break;
			}
			case CEffect::EFFECT_TYPE::TYPE_PARTICLE:
			{
				CParticleSystem::PARTICLEDESC tParticleDesc = static_cast<CParticleSystem*>(Effects[i])->Get_ParticleDesc();
				WriteFile(hFileEffect, &m_tParticleDesc, sizeof(CParticleSystem::PARTICLEDESC), &dwByte, nullptr);
				break;
			}
		}

		/* Write how many Velocity Curves there are for this Effect (needed when Loading). */
		_uint iVelocityCurvesCount = (_uint)(Effects[i]->Get_VelocityCurves().size());
		WriteFile(hFileEffect, &iVelocityCurvesCount, sizeof(_uint), &dwByte, nullptr);

		/* Write Velocity Curves. */
		for (_uint j = 0; j < iVelocityCurvesCount; j++)
			WriteFile(hFileEffect, &Effects[i]->Get_VelocityCurves()[j], sizeof(_float3), &dwByte, nullptr);

		/* Write how many Size Curves there are for this Effect (needed when Loading). */
		_uint iSizeCurvesCount = (_uint)(Effects[i]->Get_SizeCurves().size());
		WriteFile(hFileEffect, &iSizeCurvesCount, sizeof(_uint), &dwByte, nullptr);

		/* Write Size Curves. */
		for (_uint j = 0; j < iSizeCurvesCount; j++)
			WriteFile(hFileEffect, &Effects[i]->Get_SizeCurves()[j], sizeof(_float3), &dwByte, nullptr);
		
		/* Write how many Alpha Curves there are for this Effect (needed when Loading). */
		_uint iAlphaCurvesCount = (_uint)(Effects[i]->Get_AlphaCurves().size());
		WriteFile(hFileEffect, &iAlphaCurvesCount, sizeof(_uint), &dwByte, nullptr);

		/* Write Alpha Curves. */
		for (_uint j = 0; j < iAlphaCurvesCount; j++)
			WriteFile(hFileEffect, &Effects[i]->Get_AlphaCurves()[j], sizeof(_float3), &dwByte, nullptr);

		/* Write how many Rotation Velocity Curves there are for this Effect (needed when Loading). */
		_uint iTurnVelocityCurvesCount = (_uint)(Effects[i]->Get_TurnVelocityCurves().size());
		WriteFile(hFileEffect, &iTurnVelocityCurvesCount, sizeof(_uint), &dwByte, nullptr);

		/* Write Rotation Velocity Curves. */
		for (_uint j = 0; j < iTurnVelocityCurvesCount; j++)
			WriteFile(hFileEffect, &Effects[i]->Get_TurnVelocityCurves()[j], sizeof(_float3), &dwByte, nullptr);

		/* Write how many Noise Power Curves there are for this Effect (needed when Loading). */
		_uint iNoisePowerCurvesCount = (_uint)(Effects[i]->Get_NoisePowerCurves().size());
		WriteFile(hFileEffect, &iNoisePowerCurvesCount, sizeof(_uint), &dwByte, nullptr);

		/* Write Noise Power Curves. */
		for (_uint j = 0; j < iNoisePowerCurvesCount; j++)
			WriteFile(hFileEffect, &Effects[i]->Get_NoisePowerCurves()[j], sizeof(_float3), &dwByte, nullptr);
	}

	CloseHandle(hFileEffect);

	m_SavedEffects.push_back(m_sCurrentEffect);
}

_bool CImgui_Manager::Load_Effect()
{
	wstring wsSelectedSavedEffect = wstring(m_SavedEffects[m_iSavedEffect].begin(), m_SavedEffects[m_iSavedEffect].end());

	/* Load Effect File. */
	HANDLE hFileEffect = nullptr;
	_tchar LoadPathEffect[MAX_PATH] = TEXT("../../../Bin/Data/EffectData/");
	wcscat_s(LoadPathEffect, MAX_PATH, wsSelectedSavedEffect.c_str());

	hFileEffect = CreateFile(LoadPathEffect, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileEffect == INVALID_HANDLE_VALUE)
		return false;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	DWORD dwByte = 0;
	_uint iEffectsCount = 0;

	/* Read how many Effects there are in this File. */
	ReadFile(hFileEffect, &iEffectsCount, sizeof(_uint), &dwByte, nullptr);

	while (true)
	{
		if (!dwByte)
			break;

		/* For every Effect in this File: */
		for (_uint i = 0; i < iEffectsCount; i++)
		{
			/* Read Effect Type. */
			CEffect::EFFECT_TYPE eType;
			ReadFile(hFileEffect, &eType, sizeof(CEffect::EFFECT_TYPE), &dwByte, nullptr);

			CEffectTexture::TEXTUREEFFECTDESC tTextureEffectDesc;
			CEffectMesh::MESHEFFECTDESC tMeshEffectDesc;
			CParticleSystem::PARTICLEDESC tParticleDesc;
			CEffect* pEffect = nullptr;

			/* Read Effect Type-specific Description. */
			switch (eType)
			{
				case CEffect::EFFECT_TYPE::TYPE_TEXTURE:
				{
					ReadFile(hFileEffect, &tTextureEffectDesc, sizeof(CEffectTexture::TEXTUREEFFECTDESC), &dwByte, nullptr);

					if (!dwByte)
						break;

					pGameInstance->Add_GameObject_Out(TEXT("Prototype_GameObject_EffectTexture"), LEVEL_GAMEPLAY, TEXT("Layer_Effects"), (CGameObject*&)pEffect, &tTextureEffectDesc);

					m_tTextureEffectDesc = tTextureEffectDesc;
					m_pEffectManager->Add_Effect(pEffect);
					pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_TEXTURE);

					wstring wsMaskTexture = wstring(m_tTextureEffectDesc.wcMaskTexture);
					string sMaskTexture = string(wsMaskTexture.begin(), wsMaskTexture.end());
					m_sSelectedMaskTexture = sMaskTexture;

					wstring wsNoiseTexture = wstring(m_tTextureEffectDesc.wcNoiseTexture);
					string sNoiseTexture = string(wsNoiseTexture.begin(), wsNoiseTexture.end());
					m_sSelectedNoiseTexture = sNoiseTexture;

					wstring wsDissolveTexture = wstring(m_tTextureEffectDesc.wcDissolveTexture);
					string sDissolveTexture = string(wsDissolveTexture.begin(), wsDissolveTexture.end());
					m_sSelectedDissolveTexture = sDissolveTexture;
					break;
				}
				case CEffect::EFFECT_TYPE::TYPE_MESH:
				{
					ReadFile(hFileEffect, &tMeshEffectDesc, sizeof(CEffectMesh::MESHEFFECTDESC), &dwByte, nullptr);

					if (!dwByte)
						break;

					pGameInstance->Add_GameObject_Out(TEXT("Prototype_GameObject_EffectMesh"), LEVEL_GAMEPLAY, TEXT("Layer_Effects"), (CGameObject*&)pEffect, &tMeshEffectDesc);

					m_tMeshEffectDesc = tMeshEffectDesc;
					m_pEffectManager->Add_Effect(pEffect);
					pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_MESH);

					wstring wsMaskTexture = wstring(m_tMeshEffectDesc.wcMaskTexture);
					string sMaskTexture = string(wsMaskTexture.begin(), wsMaskTexture.end());
					m_sSelectedMaskTexture = sMaskTexture;

					wstring wsNoiseTexture = wstring(m_tMeshEffectDesc.wcNoiseTexture);
					string sNoiseTexture = string(wsNoiseTexture.begin(), wsNoiseTexture.end());
					m_sSelectedNoiseTexture = sNoiseTexture;

					wstring wsDissolveTexture = wstring(m_tMeshEffectDesc.wcDissolveTexture);
					string sDissolveTexture = string(wsDissolveTexture.begin(), wsDissolveTexture.end());
					m_sSelectedDissolveTexture = sDissolveTexture;
					break;
				}
				case CEffect::EFFECT_TYPE::TYPE_PARTICLE:
				{
					ReadFile(hFileEffect, &tParticleDesc, sizeof(CParticleSystem::PARTICLEDESC), &dwByte, nullptr);

					if (!dwByte)
						break;

					pGameInstance->Add_GameObject_Out(TEXT("Prototype_GameObject_ParticleSystem"), LEVEL_GAMEPLAY, TEXT("Layer_Effects"), (CGameObject*&)pEffect, &tParticleDesc);

					m_tParticleDesc = tParticleDesc;
					m_pEffectManager->Add_Effect(pEffect);
					pEffect->Set_EffectType(CEffect::EFFECT_TYPE::TYPE_PARTICLE);

					m_sCurrentSpawnType = m_SpawnTypes[m_tParticleDesc.m_eSpawnType];
					break;
				}
			}

			/* Read how many Velocity Curves there are for this Effect. */
			_uint iVelocityCurvesCount = 0;
			ReadFile(hFileEffect, &iVelocityCurvesCount, sizeof(_uint), &dwByte, nullptr);

			/* Read Velocity Curves. */
			vector<_float3> VelocityCurves;
			_float3 VelocityCurve;
			for (_uint j = 0; j < iVelocityCurvesCount; j++)
			{
				ReadFile(hFileEffect, &VelocityCurve, sizeof(_float3), &dwByte, nullptr);
				VelocityCurves.push_back(VelocityCurve);
			}
			if (!VelocityCurves.empty())
				pEffect->Set_VelocityCurves(VelocityCurves);

			/* Read how many Size Curves there are for this Effect. */
			_uint iSizeCurvesCount = 0;
			ReadFile(hFileEffect, &iSizeCurvesCount, sizeof(_uint), &dwByte, nullptr);

			/* Read Size Curves. */
			vector<_float3> SizeCurves;
			_float3 SizeCurve;
			for (_uint j = 0; j < iSizeCurvesCount; j++)
			{
				ReadFile(hFileEffect, &SizeCurve, sizeof(_float3), &dwByte, nullptr);
				SizeCurves.push_back(SizeCurve);
			}
			if (!SizeCurves.empty())
				pEffect->Set_SizeCurves(SizeCurves);

			/* Read how many Alpha Curves there are for this Effect. */
			_uint iAlphaCurvesCount = 0;
			ReadFile(hFileEffect, &iAlphaCurvesCount, sizeof(_uint), &dwByte, nullptr);

			/* Read Alpha Curves. */
			vector<_float3> AlphaCurves;
			_float3 AlphaCurve;
			for (_uint j = 0; j < iAlphaCurvesCount; j++)
			{
				ReadFile(hFileEffect, &AlphaCurve, sizeof(_float3), &dwByte, nullptr);
				AlphaCurves.push_back(AlphaCurve);
			}
			if (!AlphaCurves.empty())
				pEffect->Set_AlphaCurves(AlphaCurves);

			/* Read how many Rotation Velocity Curves there are for this Effect. */
			_uint iTurnVelocityCurvesCount = 0;
			ReadFile(hFileEffect, &iTurnVelocityCurvesCount, sizeof(_uint), &dwByte, nullptr);

			/* Read Alpha Curves. */
			vector<_float3> TurnVelocityCurves;
			_float3 TurnVelocityCurve;
			for (_uint j = 0; j < iTurnVelocityCurvesCount; j++)
			{
				ReadFile(hFileEffect, &TurnVelocityCurve, sizeof(_float3), &dwByte, nullptr);
				TurnVelocityCurves.push_back(TurnVelocityCurve);
			}
			if (!TurnVelocityCurves.empty())
				pEffect->Set_TurnVelocityCurves(TurnVelocityCurves);

			/* Read how many Noise Power Curves there are for this Effect. */
			_uint iNoisePowerCurvesCount = 0;
			ReadFile(hFileEffect, &iNoisePowerCurvesCount, sizeof(_uint), &dwByte, nullptr);

			/* Read Noise Power Curves. */
			vector<_float3> NoisePowerCurves;
			_float3 NoisePowerCurve;
			for (_uint j = 0; j < iNoisePowerCurvesCount; j++)
			{
				ReadFile(hFileEffect, &NoisePowerCurve, sizeof(_float3), &dwByte, nullptr);
				NoisePowerCurves.push_back(NoisePowerCurve);
			}
			if (!NoisePowerCurves.empty())
				pEffect->Set_NoisePowerCurves(NoisePowerCurves);
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	CloseHandle(hFileEffect);

	m_sCurrentEffect = m_SavedEffects[m_iSavedEffect];

	return true;
}

void CImgui_Manager::Show_TextureCustomization()
{
	ImGui::NewLine();
	ImGui::Separator();

	ImGui::Text("Texture Customization");
	ImGui::NewLine();

	ImGui::Text("Shader");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##Shader", m_sCurrentShaderTexture.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_ShadersTexture.begin(); iter != m_ShadersTexture.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sCurrentShaderTexture))
			{
				m_sCurrentShaderTexture = *iter;
				m_pSelectedEffect->Set_ShaderId(iCounter);
			}

			if (iter->c_str() == m_sCurrentShaderTexture)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}
	ImGui::NewLine();

	ImGui::Text("Color");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TextureColorR", &m_tTextureEffectDesc.vColor.x, 0.05f, 0, 1, "R: %.02f"))
	{
		CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
		if (pEffectTexture)
			pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TextureColorG", &m_tTextureEffectDesc.vColor.y, 0.05f, 0, 1, "G: %.02f"))
	{
		CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
		if (pEffectTexture)
			pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TextureColorB", &m_tTextureEffectDesc.vColor.z, 0.05f, 0, 1, "B: %.02f"))
	{
		CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
		if (pEffectTexture)
			pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TextureAlpha", &m_tTextureEffectDesc.fAlpha, 0.05f, 0, 1, "A: %.02f"))
	{
		CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
		if (pEffectTexture)
		{
			m_tTextureEffectDesc.fAlphaInitial = m_tTextureEffectDesc.fAlpha;
			pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
		}
	}
	ImGui::Text("Alpha Discard");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TextureAlphaDiscard", &m_tTextureEffectDesc.m_fAlphaDiscard, 0.05f, 0, 1, "A: %.02f"))
	{
		CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
		if (pEffectTexture)
			pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
	}
	ImGui::NewLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TextureLifetime", &m_tTextureEffectDesc.fLifetime, 0.05f, 0, 0, "Lifetime: %.02f"))
	{
		CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
		if (pEffectTexture)
			pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
	}
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TextureSize", &m_tTextureEffectDesc.fSize, 0.05f, .1f, 100.f, "Size: %.02f"))
	{
		CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
		if (pEffectTexture)
		{
			m_tTextureEffectDesc.fInitialSize = m_tTextureEffectDesc.fSize;
			pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
		}
	}
	ImGui::NewLine();

	/* Pseudo Diffuse Map. */
	ImGui::Text("Mask Texture");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);
	if (ImGui::BeginCombo("##MaskTexture", m_sSelectedMaskTexture.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_TextureNames.begin(); iter != m_TextureNames.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sSelectedMaskTexture))
			{
				m_sSelectedMaskTexture = *iter;
				wstring wsMaskTexture = wstring(m_sSelectedMaskTexture.begin(), m_sSelectedMaskTexture.end());

				CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
				if (pEffectTexture)
				{
					wcscpy_s(m_tTextureEffectDesc.wcMaskTexture, MAX_PATH, wsMaskTexture.c_str());
					pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
				}

				m_pSelectedEffect->Add_MaskTexture();
			}

			if (iter->c_str() == m_sSelectedMaskTexture)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Noise Texture");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);
	if (ImGui::BeginCombo("##NoiseTexture", m_sSelectedNoiseTexture.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_TextureNames.begin(); iter != m_TextureNames.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sSelectedNoiseTexture))
			{
				m_sSelectedNoiseTexture = *iter;
				wstring wsNoiseTexture = wstring(m_sSelectedNoiseTexture.begin(), m_sSelectedNoiseTexture.end());

				CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
				if (pEffectTexture)
				{
					wcscpy_s(m_tTextureEffectDesc.wcNoiseTexture, MAX_PATH, wsNoiseTexture.c_str());
					pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
				}

				m_pSelectedEffect->Add_NoiseTexture();
			}

			if (iter->c_str() == m_sSelectedNoiseTexture)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(120.f);
	if (ImGui::DragFloat("##NoiseSpeed", &m_tTextureEffectDesc.fNoiseSpeed, 0.05f, 0, 10, "Noise Speed: %.02f"))
	{
		CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
		if (pEffectTexture)
			pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(120.f);
	if (ImGui::DragFloat("##NoisePower", &m_tTextureEffectDesc.fNoisePower, 0.05f, 0, 100, "Noise Power: %.02f"))
	{
		CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
		if (pEffectTexture)
		{
			m_tTextureEffectDesc.fNoisePowerInitial = m_tTextureEffectDesc.fNoisePower;
			pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
		}
	}

	ImGui::Text("Dissolve Texture");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);
	if (ImGui::BeginCombo("##DissolveTexture", m_sSelectedDissolveTexture.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_TextureNames.begin(); iter != m_TextureNames.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sSelectedDissolveTexture))
			{
				m_sSelectedDissolveTexture = *iter;
				wstring wsDissolveTexture = wstring(m_sSelectedDissolveTexture.begin(), m_sSelectedDissolveTexture.end());

				CEffectTexture* pEffectTexture = dynamic_cast<CEffectTexture*>(m_pSelectedEffect);
				if (pEffectTexture)
				{
					wcscpy_s(m_tTextureEffectDesc.wcDissolveTexture, MAX_PATH, wsDissolveTexture.c_str());
					pEffectTexture->Set_TextureEffectDesc(m_tTextureEffectDesc);
				}

				m_pSelectedEffect->Add_DissolveTexture();
			}

			if (iter->c_str() == m_sSelectedDissolveTexture)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}
}

void CImgui_Manager::Show_MeshCustomization()
{
	ImGui::NewLine();
	ImGui::Separator();

	ImGui::Text("Mesh Customization");
	ImGui::NewLine();

	ImGui::Text("Shader");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##Shader", m_sCurrentShaderMesh.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_ShadersMesh.begin(); iter != m_ShadersMesh.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sCurrentShaderMesh))
			{
				m_sCurrentShaderMesh = *iter;
				m_pSelectedEffect->Set_ShaderId(iCounter);
			}

			if (iter->c_str() == m_sCurrentShaderMesh)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}
	ImGui::NewLine();

	ImGui::Text("Color");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##MeshColorR", &m_tMeshEffectDesc.vColor.x, 0.05f, 0, 1, "R: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##MeshColorG", &m_tMeshEffectDesc.vColor.y, 0.05f, 0, 1, "G: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##MeshColorB", &m_tMeshEffectDesc.vColor.z, 0.05f, 0, 1, "B: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##MeshAlpha", &m_tMeshEffectDesc.fAlpha, 0.05f, 0, 1, "A: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
		{
			m_tMeshEffectDesc.fAlphaInitial = m_tMeshEffectDesc.fAlpha;
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
		}
	}

	_float3 vScale, vRotation, vTranslation;
	if (ImGui::RadioButton("Scale", m_eCurrentTransformation == TRANS_SCALE))
	{
		m_eCurrentTransformation = TRANS_SCALE;

		if (m_pSelectedEffectTransform)
		{
			m_fX = m_pSelectedEffectTransform->Get_Scale(CTransform::STATE::STATE_RIGHT);
			m_fY = m_pSelectedEffectTransform->Get_Scale(CTransform::STATE::STATE_UP);
			m_fZ = m_pSelectedEffectTransform->Get_Scale(CTransform::STATE::STATE_LOOK);
		}
	}
	ImGui::SameLine();

	if (ImGui::RadioButton("Rotation", m_eCurrentTransformation == TRANS_ROTATION))
	{
		m_eCurrentTransformation = TRANS_ROTATION;

		if (m_pSelectedEffectTransform)
		{
			m_fX = m_pSelectedEffectTransform->Get_CurrentRotationX();
			m_fY = m_pSelectedEffectTransform->Get_CurrentRotationY();
			m_fZ = m_pSelectedEffectTransform->Get_CurrentRotationZ();
		}
	}
	ImGui::SameLine();

	if (ImGui::RadioButton("Translation", m_eCurrentTransformation == TRANS_TRANSLATION))
	{
		m_eCurrentTransformation = TRANS_TRANSLATION;

		if (m_pSelectedEffectTransform)
		{
			_float3 vCurrentTranslation;
			XMStoreFloat3(&vCurrentTranslation, m_pSelectedEffectTransform->Get_State(CTransform::STATE::STATE_TRANSLATION));
			m_fX = vCurrentTranslation.x;
			m_fY = vCurrentTranslation.y;
			m_fZ = vCurrentTranslation.z;
		}
	}

	ImGui::SetNextItemWidth(80);
	if (ImGui::DragFloat("##X", &m_fX, m_eCurrentTransformation == TRANS_ROTATION ? 1 : 0.05f, m_eCurrentTransformation == TRANS_ROTATION ? -360.f : 0.f, m_eCurrentTransformation == TRANS_ROTATION ? 360.f : 0.f, "X: %.03f"))
	{
		switch (m_eCurrentTransformation)
		{
		case TRANS_SCALE:
		{
			if (m_pSelectedEffectTransform)
				m_pSelectedEffectTransform->Set_Scale(CTransform::STATE::STATE_RIGHT, m_fX);
			break;
		}
		case TRANS_ROTATION:
		{
			if (m_pSelectedEffectTransform)
			{
				_vector vRotationX = XMVectorSet(1.f, 0.f, 0.f, 0.f);
				m_pSelectedEffectTransform->Set_Rotation(_float3(m_fX, m_fY, m_fZ));
			}

			break;
		}
		case TRANS_TRANSLATION:
		{
			if (m_pSelectedEffectTransform)
			{
				XMStoreFloat3(&vTranslation, m_pSelectedEffectTransform->Get_State(CTransform::STATE::STATE_TRANSLATION));
				_vector vNewTranslation = XMVectorSet(m_fX, vTranslation.y, vTranslation.z, 1.f);
				m_pSelectedEffectTransform->Set_State(CTransform::STATE::STATE_TRANSLATION, vNewTranslation);
			}

			break;
		}
		}
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(80);
	if (ImGui::DragFloat("##Y", &m_fY, m_eCurrentTransformation == TRANS_ROTATION ? 1 : 0.05f, m_eCurrentTransformation == TRANS_ROTATION ? -360.f : 0.f, m_eCurrentTransformation == TRANS_ROTATION ? 360.f : 0.f, "Y: %.03f"))
	{
		switch (m_eCurrentTransformation)
		{
		case TRANS_SCALE:
		{
			if (m_pSelectedEffectTransform)
				m_pSelectedEffectTransform->Set_Scale(CTransform::STATE::STATE_UP, m_fY);
			break;
		}
		case TRANS_ROTATION:
		{
			if (m_pSelectedEffectTransform)
				m_pSelectedEffectTransform->Set_Rotation(_float3(m_fX, m_fY, m_fZ));
			break;
		}
		case TRANS_TRANSLATION:
		{
			if (m_pSelectedEffectTransform)
			{
				XMStoreFloat3(&vTranslation, m_pSelectedEffectTransform->Get_State(CTransform::STATE::STATE_TRANSLATION));
				_vector vNewTranslation = XMVectorSet(vTranslation.x, m_fY, vTranslation.z, 1.f);
				m_pSelectedEffectTransform->Set_State(CTransform::STATE::STATE_TRANSLATION, vNewTranslation);
			}

			break;
		}
		}
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(80);
	if (ImGui::DragFloat("##Z", &m_fZ, m_eCurrentTransformation == TRANS_ROTATION ? 1 : 0.05f, m_eCurrentTransformation == TRANS_ROTATION ? -360.f : 0.f, m_eCurrentTransformation == TRANS_ROTATION ? 360.f : 0.f, "Z: %.03f"))
	{
		switch (m_eCurrentTransformation)
		{
		case TRANS_SCALE:
		{
			if (m_pSelectedEffectTransform)
				m_pSelectedEffectTransform->Set_Scale(CTransform::STATE::STATE_LOOK, m_fZ);
			break;
		}
		case TRANS_ROTATION:
		{
			if (m_pSelectedEffectTransform)
				m_pSelectedEffectTransform->Set_Rotation(_float3(m_fX, m_fY, m_fZ));
			break;
		}
		case TRANS_TRANSLATION:
		{
			if (m_pSelectedEffectTransform)
			{
				XMStoreFloat3(&vTranslation, m_pSelectedEffectTransform->Get_State(CTransform::STATE::STATE_TRANSLATION));
				_vector vNewTranslation = XMVectorSet(vTranslation.x, vTranslation.y, m_fZ, 1.f);
				m_pSelectedEffectTransform->Set_State(CTransform::STATE::STATE_TRANSLATION, vNewTranslation);
			}
			break;
		}
		}
	}
	ImGui::NewLine();

	ImGui::Text("Turn");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TurnX", &m_tMeshEffectDesc.vTurn.x, 0.05f, 0, 1, "X: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TurnY", &m_tMeshEffectDesc.vTurn.y, 0.05f, 0, 1, "Y: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##TurnZ", &m_tMeshEffectDesc.vTurn.z, 0.05f, 0, 1, "Z: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
	}
	ImGui::SetNextItemWidth(140.f);
	if (ImGui::DragFloat("##TurnVelocity", &m_tMeshEffectDesc.fTurnVelocity, 0.05f, 0, 360, "Turn Velocity: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
		{
			m_tMeshEffectDesc.fTurnVelocityInitial = m_tMeshEffectDesc.fTurnVelocity;
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
		}
			
	}
	ImGui::NewLine();

	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##MeshLifetime", &m_tMeshEffectDesc.fLifetime, 0.05f, 0, 0, "Lifetime: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
	}
	ImGui::NewLine();

	/* Pseudo Diffuse Map. */
	ImGui::Text("Mask Texture");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);
	if (ImGui::BeginCombo("##MaskTexture", m_sSelectedMaskTexture.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_TextureNames.begin(); iter != m_TextureNames.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sSelectedMaskTexture))
			{
				m_sSelectedMaskTexture = *iter;
				wstring wsMaskTexture = wstring(m_sSelectedMaskTexture.begin(), m_sSelectedMaskTexture.end());

				CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
				if (pEffectMesh)
				{
					wcscpy_s(m_tMeshEffectDesc.wcMaskTexture, MAX_PATH, wsMaskTexture.c_str());
					pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
				}

				m_pSelectedEffect->Add_MaskTexture();
			}

			if (iter->c_str() == m_sSelectedMaskTexture)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Noise Texture");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);
	if (ImGui::BeginCombo("##NoiseTexture", m_sSelectedNoiseTexture.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_TextureNames.begin(); iter != m_TextureNames.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sSelectedNoiseTexture))
			{
				m_sSelectedNoiseTexture = *iter;
				wstring wsNoiseTexture = wstring(m_sSelectedNoiseTexture.begin(), m_sSelectedNoiseTexture.end());

				CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
				if (pEffectMesh)
				{
					wcscpy_s(m_tMeshEffectDesc.wcNoiseTexture, MAX_PATH, wsNoiseTexture.c_str());
					pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
				}
	
				m_pSelectedEffect->Add_NoiseTexture();
			}

			if (iter->c_str() == m_sSelectedNoiseTexture)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(120.f);
	if (ImGui::DragFloat("##NoiseSpeed", &m_tMeshEffectDesc.fNoiseSpeed, 0.05f, 0, 10, "Noise Speed: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);	
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(120.f);
	if (ImGui::DragFloat("##NoisePower", &m_tMeshEffectDesc.fNoisePower, 0.05f, 0, 100, "Noise Power: %.02f"))
	{
		CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
		if (pEffectMesh)
		{
			m_tMeshEffectDesc.fNoisePowerInitial = m_tMeshEffectDesc.fNoisePower;
			pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
		}
	}

	ImGui::Text("Dissolve Texture");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);
	if (ImGui::BeginCombo("##DissolveTexture", m_sSelectedDissolveTexture.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_TextureNames.begin(); iter != m_TextureNames.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sSelectedDissolveTexture))
			{
				m_sSelectedDissolveTexture = *iter;
				wstring wsDissolveTexture = wstring(m_sSelectedDissolveTexture.begin(), m_sSelectedDissolveTexture.end());

				CEffectMesh* pEffectMesh = dynamic_cast<CEffectMesh*>(m_pSelectedEffect);
				if (pEffectMesh)
				{
					wcscpy_s(m_tMeshEffectDesc.wcDissolveTexture, MAX_PATH, wsDissolveTexture.c_str());
					pEffectMesh->Set_MeshEffectDesc(m_tMeshEffectDesc);
				}

				m_pSelectedEffect->Add_DissolveTexture();
			}

			if (iter->c_str() == m_sSelectedDissolveTexture)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}
}

void CImgui_Manager::Show_ParticleCustomization()
{
	ImGui::NewLine();
	ImGui::Separator();

	ImGui::Text("Particles Customization");
	ImGui::NewLine();

	ImGui::Text("Shader");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##Shader", m_sCurrentShaderParticle.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_ShadersParticle.begin(); iter != m_ShadersParticle.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sCurrentShaderParticle))
			{
				m_sCurrentShaderParticle = *iter;
				m_pSelectedEffect->Set_ShaderId(iCounter);
			}

			if (iter->c_str() == m_sCurrentShaderParticle)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}
	ImGui::NewLine();

	ImGui::Text("Spawn Type");
	ImGui::SameLine();
	if (ImGui::BeginCombo("##SpawnType", m_sCurrentSpawnType.c_str()))
	{
		_uint iCounter = 0;
		for (auto& iter = m_SpawnTypes.begin(); iter != m_SpawnTypes.end(); iter++)
		{
			if (ImGui::Selectable(iter->c_str(), iter->c_str() == m_sCurrentSpawnType))
			{
				m_sCurrentSpawnType = *iter;

				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
				if (pParticleSystem)
				{
					m_tParticleDesc.m_eSpawnType = iCounter;
					pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
				}
			}

			if (iter->c_str() == m_sCurrentSpawnType)
				ImGui::SetItemDefaultFocus();

			iCounter++;
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Color");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##ParticlesColorR", &m_tParticleDesc.m_vColor.x, 0.05f, 0, 1, "R: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##ParticlesColorG", &m_tParticleDesc.m_vColor.y, 0.05f, 0, 1, "G: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##ParticlesColorB", &m_tParticleDesc.m_vColor.z, 0.05f, 0, 1, "B: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##ParticleAlpha", &m_tParticleDesc.m_fAlpha, 0.05f, 0, 1, "A: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	ImGui::Text("Alpha Discard");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::DragFloat("##ParticleAlphaDiscard", &m_tParticleDesc.m_fAlphaDiscard, 0.05f, 0, 1, "A: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}

	ImGui::NewLine();
	if (ImGui::DragInt("##ParticlesNum", &m_tParticleDesc.m_iMaxParticles, 1, 0, 1000, "Max Particles: %d"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (m_sCurrentSpawnType == "BURST")
		ImGui::BeginDisabled();
	if (ImGui::DragFloat("##ParticlesPerSecond", &m_tParticleDesc.m_fParticlesPerSecond, 0.05f, 0, m_tParticleDesc.m_iMaxParticles, "Particles/Sec: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (m_sCurrentSpawnType == "BURST")
		ImGui::EndDisabled();
	if (ImGui::DragFloat("##ParticlesLifetime", &m_tParticleDesc.m_fParticlesLifetime, 0.05f, 0, 0, "Particles Lifetime: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (ImGui::DragFloat("##ParticleDeviationX", &m_tParticleDesc.m_fParticleDeviationX, 0.05f, 0, 0, "X Variation: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (ImGui::DragFloat("##ParticleDeviationY", &m_tParticleDesc.m_fParticleDeviationY, 0.05f, 0, 0, "Y Variation: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (ImGui::DragFloat("##ParticleDeviationZ", &m_tParticleDesc.m_fParticleDeviationZ, 0.05f, 0, 0, "Z Variation: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (m_tParticleDesc.m_bRandomDirectionX)
		ImGui::BeginDisabled();
	if (ImGui::DragFloat("##ParticleDirectionX", &m_tParticleDesc.m_vParticleDirection.x, 0.05f, -1, 1, "Direction X: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (m_tParticleDesc.m_bRandomDirectionX)
		ImGui::EndDisabled();
	ImGui::SameLine();
	ImGui::Text("Random");
	ImGui::SameLine();
	if (ImGui::Checkbox("##RandomDirectionX", &m_tParticleDesc.m_bRandomDirectionX))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}

	if (m_tParticleDesc.m_bRandomDirectionY)
		ImGui::BeginDisabled();
	if (ImGui::DragFloat("##ParticleDirectionY", &m_tParticleDesc.m_vParticleDirection.y, 0.05f, -1, 1, "Direction Y: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (m_tParticleDesc.m_bRandomDirectionY)
		ImGui::EndDisabled();
	ImGui::SameLine();
	ImGui::Text("Random");
	ImGui::SameLine();
	if (ImGui::Checkbox("##RandomDirectionY", &m_tParticleDesc.m_bRandomDirectionY))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}

	if (m_tParticleDesc.m_bRandomDirectionZ)
		ImGui::BeginDisabled();
	if (ImGui::DragFloat("##ParticleDirectionZ", &m_tParticleDesc.m_vParticleDirection.z, 0.05f, -1, 1, "Direction Z: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (m_tParticleDesc.m_bRandomDirectionZ)
		ImGui::EndDisabled();
	ImGui::SameLine();
	ImGui::Text("Random");
	ImGui::SameLine();
	if (ImGui::Checkbox("##RandomDirectionZ", &m_tParticleDesc.m_bRandomDirectionZ))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}

	if (ImGui::DragFloat("##Velocity", &m_tParticleDesc.m_fParticleVelocity, 0.05f, 0, 100.f, "Velocity: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (ImGui::DragFloat("##VelocityVariation", &m_tParticleDesc.m_fParticleVelocityVariation, 0.05f, 0, 100.f, "Velocity Variation: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (ImGui::DragFloat("##Size", &m_tParticleDesc.m_fParticleSize, 0.05f, .1f, 100.f, "Size: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
	if (ImGui::DragFloat("##SizeVariation", &m_tParticleDesc.m_fParticleSizeVariation, 0.05f, 0, 100.f, "Size Variation: %.02f"))
	{
		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(m_pSelectedEffect);
		if (pParticleSystem)
			pParticleSystem->Set_ParticleDesc(m_tParticleDesc);
	}
}

void CImgui_Manager::Create_Model(const _tchar* pPrototypeTag, const _tchar* pLayerTag, _bool bCreatePrototype)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_InitDesc.vPosition.x += m_vfOffset.x;
	m_InitDesc.vPosition.y += m_vfOffset.y;
	m_InitDesc.vPosition.z += m_vfOffset.z;
	m_pModel_Manager->Set_InitModelDesc(m_InitDesc);
	
	_matrix			PivotMatrix = XMMatrixIdentity();
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();
	m_pModel_Manager->Create_Model(iLevel, pPrototypeTag, pLayerTag, m_pDevice, m_pContext, CModel::TYPE_NONANIM, PivotMatrix, bCreatePrototype);

	RELEASE_INSTANCE(CGameInstance);
}

void CImgui_Manager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	for (auto& iter : m_TempLayerTags)
		Safe_Delete(iter);
	m_TempLayerTags.clear();

	for (auto& iter : m_TerrainTags)
		Safe_Delete(iter);
	m_TerrainTags.clear();

	m_stLayerTags.clear();
	//CleanupDeviceD3D();
	//::DestroyWindow(hwnd);
	//::UnregisterClass(wc.lpszClassName, wc.hInstance);

	Safe_Release(m_pModel_Manager);
	Safe_Release(m_pTerrain_Manager);
	Safe_Release(m_pNavigation_Manager);
	Safe_Release(m_pCamera_Manager);
	Safe_Release(m_pEffectManager);
	CEffect_Manager::Get_Instance()->Destroy_Instance();
	CCamera_Manager::Get_Instance()->Destroy_Instance();
	CTerrain_Manager::Get_Instance()->Destroy_Instance();
	CModelManager::Get_Instance()->Destroy_Instance();
	CNavigation_Manager::Get_Instance()->Destroy_Instance();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
