#include "stdafx.h"
#include "..\Public\Imgui_Manager.h"
#include "imgui.h"
#include "GameInstance.h"
#include "NonMovableObject.h"
#include "Light.h"
#include "MeshMap.h"
#include "Player.h"
#include "NaviPoint.h"
#include "MeshContainer.h"
#include "Room.h"
#include "Door.h"
#include "HierarchyNode.h"
#include "Channel.h"

IMPLEMENT_SINGLETON(CImgui_Manager)

CImgui_Manager::CImgui_Manager()
{
	ZeroMemory(m_fPlaceRotation, sizeof(_float) * 3);

	ZeroMemory(m_fChangeRotation, sizeof(_float) * 3);
	ZeroMemory(m_fChangePosition, sizeof(_float) * 3);

	ZeroMemory(m_fLightPosition, sizeof(_float) * 3);
	ZeroMemory(m_fLightChangePosition, sizeof(_float) * 3);

	ZeroMemory(m_fLightDirection, sizeof(_float) * 3);
	ZeroMemory(m_fLightChangeDirection, sizeof(_float) * 3);

	ZeroMemory(m_fLightDiffuse, sizeof(_float) * 3);
	ZeroMemory(m_fLightAmbient, sizeof(_float) * 3);
	ZeroMemory(m_fLightSpecular, sizeof(_float) * 3);

	ZeroMemory(m_fLightChangeDiffuse, sizeof(_float) * 3);
	ZeroMemory(m_fLightChangeAmbient, sizeof(_float) * 3);
	ZeroMemory(m_fLightChangeSpecular, sizeof(_float) * 3);
}

HRESULT CImgui_Manager::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//m_temp = io.ConfigFlags;
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	return S_OK;
}

void CImgui_Manager::Tick(_float fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ShowGui();

	ImGui::EndFrame();
}

void CImgui_Manager::Render()
{
	// Rendering


	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CImgui_Manager::ShowGui()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	ImGui::Begin("Total", NULL, ImGuiWindowFlags_None);

	if (ImGui::Button("Load Model"))
		m_isLoad = true;
	ImGui::SameLine();
	if (ImGui::Button("Save Animation"))
	{
		_tchar pAnimationDataFilePath[MAX_PATH] = TEXT("../../../Bin/Bin_Data/Anim/");

		CPlayer::PLAYERDESC Pivot;
		memcpy(&Pivot, &((CPlayer*)(pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front()))->Get_Pivot(), sizeof(CPlayer::PLAYERDESC));

		switch (Pivot.eModel)
		{
		case MODEL_ALPHEN:
			lstrcat(pAnimationDataFilePath, TEXT("Alphen/Alphen_Animation_Add.dat"));
			break;
		case MODEL_ICEWOLF:
			lstrcat(pAnimationDataFilePath, TEXT("Ice_Wolf/Ice_Wolf_Animation_Add.dat"));
			break;
		case MODEL_SION:
			lstrcat(pAnimationDataFilePath, TEXT("Sion/Sion_Animation_Add.dat"));
			break;

		case MODEL_BERSERKER:
			lstrcat(pAnimationDataFilePath, TEXT("Berserker/Berserker_Animation_Add.dat"));
			break;
			
		case MODEL_HAWK:
			lstrcat(pAnimationDataFilePath, TEXT("Hawk/Hawk_Animation_Add.dat"));
			break;

		case MODEL_ASTRAL_DOUBT:
			lstrcat(pAnimationDataFilePath, TEXT("Astral_Doubt/Astral_Doubt_Animation_Add.dat"));
			break;
		}

		CModel* pPlayerModel = (CModel*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Model"));
		if (nullptr != pPlayerModel)
		{
			_ulong dwByte = 0;
			HANDLE hFile = CreateFile(pAnimationDataFilePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
			{
				ERR_MSG(TEXT("Failed to Save : Animation_Add"));
				return;
			}

			for (auto& Animation : pPlayerModel->Get_Animations())
			{
				vector<_float> TickPerSeconds = Animation->Get_TickPerSeconds();
				vector<_float> ChangeTimes = Animation->Get_ChangeTickTimes();
				vector<CAnimation::ANIMEVENT> AnimEvent = Animation->Get_AnimEvents();

				_int TickPerSecondsSize = TickPerSeconds.size();
				_int ChangeTimesSize = ChangeTimes.size();
				_int EventsSize = AnimEvent.size();

				_float fDuration = Animation->Get_Duration();
				WriteFile(hFile, &fDuration, sizeof(_float), &dwByte, nullptr);

				_int iNumChannels = Animation->Get_Channels().size();
				//WriteFile(hFile, &iNumChannels, sizeof(_int), &dwByte, nullptr);

				for (_int i = 0; i < iNumChannels; ++i)
				{
					_int iNumKeyFrames = Animation->Get_Channels()[i]->Get_KeyFrames().size();
					for (_int j = 0; j < iNumKeyFrames; ++j)
						WriteFile(hFile, &Animation->Get_Channels()[i]->Get_KeyFrames()[j], sizeof(KEYFRAME), &dwByte, nullptr);
				}

				WriteFile(hFile, &TickPerSecondsSize, sizeof(_int), &dwByte, nullptr);
				for (_int i = 0; i < TickPerSecondsSize; ++i)
					WriteFile(hFile, &TickPerSeconds[i], sizeof(_float), &dwByte, nullptr);
				
				WriteFile(hFile, &ChangeTimesSize, sizeof(_int), &dwByte, nullptr);
				for (_int i = 0; i < ChangeTimesSize; ++i)
					WriteFile(hFile, &ChangeTimes[i], sizeof(_float), &dwByte, nullptr);
				
				WriteFile(hFile, &EventsSize, sizeof(_int), &dwByte, nullptr);
				for (_int i = 0; i < EventsSize; ++i)
					WriteFile(hFile, &AnimEvent[i], sizeof(CAnimation::ANIMEVENT), &dwByte, nullptr);
			}

			CloseHandle(hFile);
		}

		m_isSaveComplete = true;
	}
	
	if (m_isLoad)
	{
		ImGui::Begin("Load", &m_isLoad);

		char** pItems = new char*[m_AnimObj.size()];

		for (_int i = 0; i < m_AnimObj.size(); ++i)
			pItems[i] = m_AnimObj[i];

		if (ImGui::ListBox("Load Model", &m_iLoadModel, pItems, _int(m_AnimObj.size())))
		{
			if (!(CPlayer*)(pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Player")).empty()))
				((CPlayer*)(pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front()))->Set_Dead();

			CPlayer::PLAYERDESC Pivot;
			ZeroMemory(&Pivot, sizeof(CPlayer::PLAYERDESC));

			Pivot.tPivot.vPosition = _float3(0.f, 0.f, 0.f);
			Pivot.tPivot.vScale = _float3(1.f, 1.f, 1.f);
			Pivot.tPivot.vRotation = _float3(0.f, 0.f, 0.f);

			Pivot.eModel = (MODEL)m_iLoadModel;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, TEXT("Layer_Player"), &Pivot)))
				return;

			if (m_iPreModel != m_iLoadModel)
			{
				m_iBoneChoice = -1;
				m_iAnimationChoice = -1;
			}
			
			m_iPreModel = m_iLoadModel;
		}

		Safe_Delete_Array(pItems);

		if (ImGui::Button("Close"))
		{
			m_isLoad = false;
			m_iLoadModel = -1;
		}
			
		ImGui::End();
	}

	if (m_isSaveComplete)
	{
		ImGui::Begin("Save Animation", &m_isSaveComplete);

		ImGui::Text("Save Complete");

		if (ImGui::Button("Close"))
			m_isSaveComplete = false;

		ImGui::End();
	}

	ImGuiTabBarFlags tab_Total_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("TotalTabBar", tab_Total_bar_flags))
	{
		if (ImGui::BeginTabItem("Animation"))
		{
			CModel* pPlayerModel = (CModel*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Model"));
			if (nullptr != pPlayerModel)
			{
				vector<CHierarchyNode*> ModelBones = pPlayerModel->Get_Bones();
				vector<CAnimation*> ModelAnimations = pPlayerModel->Get_Animations();

				char** pBoneItems = new char*[ModelBones.size()];
				char** pAnimItems = new char*[ModelAnimations.size()];

				for (_int i = 0; i < ModelBones.size(); ++i)
				{
					pBoneItems[i] = new char[MAX_PATH];
					strcpy_s(pBoneItems[i], sizeof(char) * MAX_PATH, ModelBones[i]->Get_Name());
				}

				if (ImGui::ListBox("Bone", &m_iBoneChoice, pBoneItems, _int(ModelBones.size())))
				{
					_matrix BoneTransformation = ModelBones[m_iBoneChoice]->Get_TransformationMatrix();

					_vector Scale, RotQuat, Pos;
					XMMatrixDecompose(&Scale, &RotQuat, &Pos, BoneTransformation);

					_float4 BoneScale, BonePos;
					XMStoreFloat4(&BoneScale, Scale);
					XMStoreFloat4(&BonePos, Pos);

					_float4 RotationQuat, RotationDot;
					XMStoreFloat4(&RotationQuat, RotQuat);
					XMStoreFloat4(&RotationDot, XMVector3Dot(RotQuat, RotQuat));

					m_fBoneScale[0] = BoneScale.x;
					m_fBoneScale[1] = BoneScale.y;
					m_fBoneScale[2] = BoneScale.z;

					m_fBoneRotation[0] = asinf(2.f * (RotationQuat.w * RotationQuat.x - RotationQuat.y * RotationQuat.z));
					m_fBoneRotation[1] = atan2f(2.f * (RotationQuat.x * RotationQuat.z + RotationQuat.w * RotationQuat.y), (-RotationDot.x - RotationDot.y + RotationDot.z + RotationDot.w));
					m_fBoneRotation[2] = atan2f(2.f * (RotationQuat.x * RotationQuat.y + RotationQuat.w * RotationQuat.z), (-RotationDot.x + RotationDot.y - RotationDot.z + RotationDot.w));

					m_fBonePosition[0] = BonePos.x;
					m_fBonePosition[1] = BonePos.y;
					m_fBonePosition[2] = BonePos.z;

					((CPlayer*)pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front())->Change_Bone(ModelBones[m_iBoneChoice]->Get_Name());
				}

				for (_int i = 0; i < ModelBones.size(); ++i)
					Safe_Delete_Array(pBoneItems[i]);

				Safe_Delete_Array(pBoneItems);

				ImGui::InputFloat3("Scale", m_fBoneScale);
				ImGui::InputFloat3("Rotation", m_fBoneRotation);
				ImGui::InputFloat3("Position", m_fBonePosition);

				if (ImGui::Button("Weapon"))
					m_isWeaponWindow = true;

				for (_int i = 0; i < ModelAnimations.size(); ++i)
				{
					pAnimItems[i] = new char[MAX_PATH];
					strcpy_s(pAnimItems[i], sizeof(char) * MAX_PATH, ModelAnimations[i]->Get_Name());
				}
				ImGui::SetNextItemWidth(600.f);
				if (ImGui::ListBox("Animation", &m_iAnimationChoice, pAnimItems, _int(ModelAnimations.size())))
				{
					((CPlayer*)pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front())->Set_AnimIndex(m_iAnimationChoice);

					m_fAnimationDuration = ModelAnimations[m_iAnimationChoice]->Get_Duration();

					m_iEventChoice = -1;
				}
					

				for (_int i = 0; i < ModelAnimations.size(); ++i)
					Safe_Delete_Array(pAnimItems[i]);

				Safe_Delete_Array(pAnimItems);
				
				if (ImGui::DragFloat("Duration", &m_fAnimationDuration, 0.005f, 0.001f, 200.f))
					ModelAnimations[m_iAnimationChoice]->Change_Duration(m_fAnimationDuration);
				ImGui::SameLine();
				ImGui::Checkbox("Animation Play", &m_isAnimationPlay);
				
				if (m_isAnimationPlay)
				{
					if (-1 != m_iAnimationChoice)
					{
						((CPlayer*)pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front())->StartAnim();
						m_fAnimationTime = ModelAnimations[m_iAnimationChoice]->Get_CurrentTime();
						if (m_fAnimationTime > m_fAnimationDuration)
							m_fAnimationTime = 0.f;
					}
				}
				else
					((CPlayer*)pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front())->StopAnim();

				if (ImGui::SliderFloat("Playing", &m_fAnimationTime, 0.f, m_fAnimationDuration))
				{
					ModelAnimations[m_iAnimationChoice]->Reset();
					ModelAnimations[m_iAnimationChoice]->Set_CurrentTime(m_fAnimationTime);
					((CPlayer*)pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front())->PlayAnimation();
				}
					
				
				if (ImGui::Button("Animation Edit"))
					m_isAnimationEdit = true;

				ImGui::SameLine();

				if (ImGui::Button("Animation Event"))
					m_isEventWindow = true;
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();

	if (m_isAnimationEdit)
	{
		ImGui::Begin("Animation Edit", &m_isAnimationEdit);
		
		CModel* pPlayerModel = (CModel*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Model"));
		if ((nullptr != pPlayerModel) && (-1 != m_iAnimationChoice))
		{
			CAnimation* ChoiceModelAnimation = pPlayerModel->Get_Animations()[m_iAnimationChoice];

			vector<_float> TimeVector = ChoiceModelAnimation->Get_ChangeTickTimes();
			vector<_float> TickVector = ChoiceModelAnimation->Get_TickPerSeconds();

			char** pTime = new char*[TimeVector.size()];
			char** pTick = new char*[TickVector.size()];

			for (_int i = 0; i < TimeVector.size(); ++i)
			{
				pTime[i] = new char[MAX_PATH];
				snprintf(pTime[i], MAX_PATH, "%f", TimeVector[i]);
			}
				
			for (_int i = 0; i < TickVector.size(); ++i)
			{
				pTick[i] = new char[MAX_PATH];
				snprintf(pTick[i], MAX_PATH, "%f", TickVector[i]);
			}

			if (ImGui::ListBox("Time", &m_iTimeChoice, pTime, TimeVector.size()))
			{
				m_iTickChoice = m_iTimeChoice;

				m_fAnimationTickTime = TimeVector[m_iTimeChoice];
				m_fAnimationTick = TickVector[m_iTimeChoice];
			}
				
			if (ImGui::ListBox("TickPerSecond", &m_iTickChoice, pTick, TickVector.size()))
			{
				m_iTimeChoice = m_iTickChoice;

				m_fAnimationTickTime = TimeVector[m_iTickChoice];
				m_fAnimationTick = TickVector[m_iTickChoice];
			}
			
			ImGui::DragFloat("Playing Time", &m_fAnimationTickTime, 0.05f, 0.f, m_fAnimationDuration);
			ImGui::DragFloat("Playing TickPerSecond", &m_fAnimationTick, 0.05f, 0.f, 100.f);

			_float fOriginalTickPerSecond = ChoiceModelAnimation->Get_OriginalTickPerSecond();
			ImGui::InputFloat("Original TickPerSecond", &fOriginalTickPerSecond);
			
			if (ImGui::Button("Add"))
				ChoiceModelAnimation->Add_TickPerSecond(m_fAnimationTickTime, m_fAnimationTick);
			ImGui::SameLine();
			if (ImGui::Button("Fix"))
				ChoiceModelAnimation->Fix_TickPerSecond(m_iTimeChoice, m_fAnimationTickTime, m_fAnimationTick);
			ImGui::SameLine();
			if (ImGui::Button("Delete"))
			{
				ChoiceModelAnimation->Delete_TickPerSecond(m_iTimeChoice);
				
				m_fAnimationTickTime = 0.f;
				m_fAnimationTick = 0.f;
			}

			for (_int i = 0; i < TimeVector.size(); ++i)
				Safe_Delete_Array(pTime[i]);
			Safe_Delete_Array(pTime);

			for (_int i = 0; i < TickVector.size(); ++i)
				Safe_Delete_Array(pTick[i]);
			Safe_Delete_Array(pTick);
		}	

		if (ImGui::Button("Close"))
		{
			m_iTickChoice = -1;
			m_iTimeChoice = -1;

			m_isAnimationEdit = false;
		}
		ImGui::End();
	}

	if (m_isWeaponWindow)
	{
		ImGui::Begin("Weapon", &m_isWeaponWindow);

		char** pWeapon = new char*[m_WeaponObj.size()];
		
		for (_int i = 0; i < m_WeaponObj.size(); ++i)
			pWeapon[i] = m_WeaponObj[i];

		if (ImGui::ListBox("Weapon List", &m_iWeaponChoice, pWeapon, m_WeaponObj.size()))
		{
			CModel* pPlayerModel = (CModel*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Model"));
			if (nullptr != pPlayerModel)
			{
				_tchar ModelName[MAX_PATH] = TEXT("");
				MultiByteToWideChar(CP_ACP, 0, m_WeaponObj[m_iWeaponChoice], (int)strlen(m_WeaponObj[m_iWeaponChoice]), ModelName, MAX_PATH);

				_tchar ModelTag[MAX_PATH] = TEXT("Prototype_Component_Model_");

				lstrcat(ModelTag, ModelName);

				vector<CHierarchyNode*> ModelBones = pPlayerModel->Get_Bones();

				if (-1 != m_iBoneChoice)
					((CPlayer*)pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front())->Ready_Parts(ModelBones[m_iBoneChoice]->Get_Name(), ModelTag);
			}
		}

		Safe_Delete_Array(pWeapon);

		if (ImGui::Button("Close"))
		{
			m_iWeaponChoice = -1;
			m_isWeaponWindow = false;
		}

		ImGui::End();
	}

	if (m_isEventWindow)
	{
		ImGui::Begin("Event", &m_isEventWindow);

		CModel* pPlayerModel = (CModel*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Model"));
		if ((nullptr != pPlayerModel) && (-1 != m_iAnimationChoice))
		{
			CAnimation* ChoiceModelAnimation = pPlayerModel->Get_Animations()[m_iAnimationChoice];

			vector<CAnimation::ANIMEVENT> AnimEventVector = ChoiceModelAnimation->Get_AnimEvents();
			_int iAnimEvents = AnimEventVector.size();

			char** pStartTime = new char*[iAnimEvents];
			char** pEndTime = new char*[iAnimEvents];
			char** pEventType = new char*[iAnimEvents];

			for (_int i = 0; i < iAnimEvents; ++i)
			{
				pStartTime[i] = new char[MAX_PATH];
				snprintf(pStartTime[i], MAX_PATH, "%f", AnimEventVector[i].fStartTime);
			}

			for (_int i = 0; i < iAnimEvents; ++i)
			{
				pEndTime[i] = new char[MAX_PATH];
				snprintf(pEndTime[i], MAX_PATH, "%f", AnimEventVector[i].fEndTime);
			}

			for (_int i = 0; i < iAnimEvents; ++i)
			{
				switch (AnimEventVector[i].eType)
				{
				case CAnimation::EVENT_SOUND:
					pEventType[i] = "EVENT_SOUND";
					break;
				case CAnimation::EVENT_EFFECT:
					pEventType[i] = "EVENT_EFFECT";
					break;
				case CAnimation::EVENT_COLLIDER:
					pEventType[i] = "EVENT_COLLIDER";
					break;
				case CAnimation::EVENT_STATE:
					pEventType[i] = "EVENT_STATE";
					break;
				case CAnimation::EVENT_INPUT:
					pEventType[i] = "EVENT_INPUT";
					break;
				}
			}

			if (ImGui::ListBox("Event Start", &m_iEventChoice, pStartTime, iAnimEvents))
			{
				m_fEventStartTime = AnimEventVector[m_iEventChoice].fStartTime;
				m_fEventEndTime = AnimEventVector[m_iEventChoice].fEndTime;
			}
			if (ImGui::ListBox("Event End", &m_iEventChoice, pEndTime, iAnimEvents))
			{
				m_fEventStartTime = AnimEventVector[m_iEventChoice].fStartTime;
				m_fEventEndTime = AnimEventVector[m_iEventChoice].fEndTime;
			}
			if (ImGui::ListBox("Event Type", &m_iEventChoice, pEventType, iAnimEvents))
			{
				m_fEventStartTime = AnimEventVector[m_iEventChoice].fStartTime;
				m_fEventEndTime = AnimEventVector[m_iEventChoice].fEndTime;
			}

			ImGui::DragFloat("Start Time", &m_fEventStartTime, 0.05f, 0.f, m_fAnimationDuration);
			ImGui::DragFloat("End Time", &m_fEventEndTime, 0.05f, 0.f, m_fAnimationDuration);

			static char szEventName[MAX_PATH] = "";
			if (-1 != m_iEventChoice && m_iEventChoice < AnimEventVector.size())
				memcpy(szEventName, AnimEventVector[m_iEventChoice].szName, sizeof(char) * MAX_PATH);
			
			ImGui::SetNextItemWidth(175);
			ImGui::InputText("##Event Tag", szEventName, MAX_PATH);

			ImGui::RadioButton("Event_Sound", &m_iEventType, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Event_Effect", &m_iEventType, 1);
			ImGui::SameLine();
			ImGui::RadioButton("Event_Collider", &m_iEventType, 2);
			ImGui::RadioButton("Event_State", &m_iEventType, 3);
			ImGui::SameLine();
			ImGui::RadioButton("Event_Input", &m_iEventType, 4);

			ImGui::Text("Event Name");
			static char cEventName[MAX_PATH] = "";
			ImGui::SetNextItemWidth(175);
			ImGui::InputText("##InputEvent", cEventName, MAX_PATH);

			if (ImGui::Button("Add"))
			{
				CAnimation::ANIMEVENT AnimEvent;
				ZeroMemory(&AnimEvent, sizeof(CAnimation::ANIMEVENT));

				AnimEvent.eType = (CAnimation::EVENTTYPE)m_iEventType;
				AnimEvent.fStartTime = m_fEventStartTime;
				AnimEvent.fEndTime = m_fEventEndTime;
				memcpy(AnimEvent.szName, cEventName, sizeof(char) * MAX_PATH);

				ChoiceModelAnimation->Add_Event(AnimEvent);
			}
			ImGui::SameLine();
			if (ImGui::Button("Fix"))
			{
				CAnimation::ANIMEVENT AnimEvent;
				ZeroMemory(&AnimEvent, sizeof(CAnimation::ANIMEVENT));

				AnimEvent.eType = (CAnimation::EVENTTYPE)m_iEventType;
				AnimEvent.fStartTime = m_fEventStartTime;
				AnimEvent.fEndTime = m_fEventEndTime;
				memcpy(AnimEvent.szName, cEventName, sizeof(char) * MAX_PATH);

				ChoiceModelAnimation->Fix_Event(m_iEventChoice, AnimEvent);
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete"))
			{
				ChoiceModelAnimation->Delete_Event(m_iEventChoice);

				m_fEventStartTime = 0.f;
				m_fEventEndTime = 0.f;
				m_iEventType = CAnimation::EVENTTYPE::EVENT_END;
				m_iEventChoice = -1;
			}

			for (_int i = 0; i < iAnimEvents; ++i)
			{
				Safe_Delete_Array(pStartTime[i]);
				Safe_Delete_Array(pEndTime[i]);
			}

			Safe_Delete_Array(pStartTime);
			Safe_Delete_Array(pEndTime);
			Safe_Delete_Array(pEventType);
		}

		if (ImGui::Button("Close"))
		{
			m_isEventWindow = false;
			m_iEventChoice = -1;
		}

		ImGui::End();
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CImgui_Manager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//CleanupDeviceD3D();
	//::DestroyWindow(hwnd);
	//::UnregisterClass(wc.lpszClassName, wc.hInstance);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
