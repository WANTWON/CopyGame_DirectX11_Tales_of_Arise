+ACM-include +ACI-stdafx.h+ACI-
+ACM-include +ACI-..+AFw-Public+AFw-UI+AF8-Dialogue.h+ACI-

+ACM-include +ACI-GameInstance.h+ACI-
+ACM-include +ADw-fstream+AD4-
+ACM-include +ADw-iostream+AD4-
+ACM-include +ADw-string+AD4-
+ACM-include +ACI-UI+AF8-QuestClear.h+ACI-
+ACM-include +ACI-UI+AF8-Dialoguepopup.h+ACI-
+ACM-include +ACI-CameraManager.h+ACI-
+ACM-include +ACI-Level+AF8-Restaurant.h+ACI-
+ACM-include +ACI-Level+AF8-WorkTool.h+ACI-
+ACM-include +ACI-Player.h+ACI-
+ACM-include +ACI-MonsterLaw.h+ACI-
+ACM-include +ACI-AiRinwell.h+ACI-
+ACM-include +ADw-tchar.h+AD4-
+ACM-include +ACI-UI+AF8-PartyjoinMsg+AF8-Law.h+ACI-


CUI+AF8-Dialogue::CUI+AF8-Dialogue(ID3D11Device +ACo- pDevice, ID3D11DeviceContext +ACo- pContext)
	: CUI+AF8-Base(pDevice, pContext)
+AHs-
+AH0-

CUI+AF8-Dialogue::CUI+AF8-Dialogue(const CUI+AF8-Dialogue +ACY- rhs)
	: CUI+AF8-Base(rhs)
+AHs-
+AH0-

HRESULT CUI+AF8-Dialogue::Initialize+AF8-Prototype()
+AHs-
	return S+AF8-OK+ADs-
+AH0-

HRESULT CUI+AF8-Dialogue::Initialize(void +ACo- pArg)
+AHs-


	m+AF8-FontR +AD0- 0.9019607843137255f+ADs-
	m+AF8-FontG +AD0- 0.8235294117647059f+ADs-
	m+AF8-FontB +AD0- 0.6588235294117647f+ADs-
	m+AF8-fSize.x +AD0- 720.f+ADs-
	m+AF8-fSize.y +AD0- 150.f+ADs-
	m+AF8-fPosition.x +AD0- 200.f+ADs-
	m+AF8-fPosition.y +AD0- 600.f+ADs-
	m+AF8-fFontPos.x +AD0- 500.f+ADs-
	m+AF8-fAlpha +AD0- 0+ADs-

	m+AF8-bfadein +AD0- true+ADs-
	CUI+AF8-Manager::Get+AF8-Instance()-+AD4-AddItem(ITEMNAME+AF8-LEMONJELLY, ITEMTYPE+AF8-JELLY, false, false)+ADs-
	CUI+AF8-Manager::Get+AF8-Instance()-+AD4-AddItem(ITEMNAME+AF8-LIFEBOTTLE, ITEMTYPE+AF8-POTION, false, false)+ADs-
	CUI+AF8-Manager::Get+AF8-Instance()-+AD4-AddItem(ITEMNAME+AF8-POTATO, ITEMTYPE+AF8-VEGITABLE, false, false)+ADs-
	CUI+AF8-Manager::Get+AF8-Instance()-+AD4-AddItem(ITEMNAME+AF8-DULLSWORD, ITEMTYPE+AF8-SWORD, false, false)+ADs-
	CUI+AF8-Manager::Get+AF8-Instance()-+AD4-AddItem(ITEMNAME+AF8-PYOGOMUSHROOM, ITEMTYPE+AF8-ARMOR, false, false)+ADs-

	Read+AF8-TextFiles+AF8-for+AF8-dialogue()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Quest1Clear()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Quest2Strat()+ADs- 
	Read+AF8-TextFiles+AF8-for+AF8-Quest2Clear()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Quest3Start()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Quest3Clear()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Stage1Last()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Quest4Start()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Minigame1start()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Minigame1end()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Minigame2start()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-Minigame2end()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-LawBossBattleStart()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-LawBossBattleEvent()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-LawBossBattleEnd()+ADs-
	Read+AF8-TextFiles+AF8-for+AF8-LastQuestStart()+ADs-
	if (FAILED(+AF8AXw-super::Initialize(pArg)))
		return E+AF8-FAIL+ADs-

	return S+AF8-OK+ADs-
+AH0-

int CUI+AF8-Dialogue::Tick(+AF8-float fTimeDelta)
+AHs-
	if (m+AF8-bSexyEventEnd)
	+AHs-
		m+AF8-fSexyOffTimer +-+AD0- fTimeDelta+ADs-
		if (m+AF8-fSexyOffTimer +AD4- 3.5f)
		+AHs-
			m+AF8-bSexyEventEnd +AD0- false+ADs-
			m+AF8-fSexyOffTimer +AD0- 0.f+ADs-
			CPlayerManager::Get+AF8-Instance()-+AD4-Set+AF8-Changetoboss(true)+ADs-
			dynamic+AF8-cast+ADw-CMonsterLaw+ACoAPg-(CBattleManager::Get+AF8-Instance()-+AD4-Get+AF8-BossMonster())-+AD4-Set+AF8-EventFinish(true)+ADs-
			CObject+AF8-Pool+AF8-Manager::Get+AF8-Instance()-+AD4-Reuse+AF8-Pooling+AF8-Layer(LEVEL+AF8-LAWBATTLE, TEXT(+ACI-Layer+AF8-Rinwell+ACI-))+ADs-
			CBaseObj+ACo- pObject +AD0- dynamic+AF8-cast+ADw-CBaseObj+ACoAPg-(CGameInstance::Get+AF8-Instance()-+AD4-Get+AF8-Object(LEVEL+AF8-LAWBATTLE, TEXT(+ACI-Layer+AF8-Rinwell+ACI-)))+ADs-
			dynamic+AF8-cast+ADw-CMonster+ACoAPg-(pObject)-+AD4-Change+AF8-Navigation(LEVEL+AF8-LAWBATTLE)+ADs-
			dynamic+AF8-cast+ADw-CMonster+ACoAPg-(pObject)-+AD4-Compute+AF8-CurrentIndex(LEVEL+AF8-LAWBATTLE)+ADs-
			dynamic+AF8-cast+ADw-CMonster+ACoAPg-(pObject)-+AD4-Set+AF8-BattleMode(true)+ADs-

			dynamic+AF8-cast+ADw-CAiRinwell+ACoAPg-(pObject)-+AD4-Set+AF8-AirMode(true)+ADs-
			dynamic+AF8-cast+ADw-CAiRinwell+ACoAPg-(pObject)-+AD4-Set+AF8-50DownCutScene(true)+ADs-

			CBattleManager::Get+AF8-Instance()-+AD4-Add+AF8-BattleMonster(pObject)+ADs-

			dynamic+AF8-cast+ADw-CUI+AF8-Dialoguepopup+ACoAPg-(CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Get+AF8-Dialoguepopup())-+AD4-Open+AF8-Dialogue(8, true, 0, 1)+ADs-
			CBattleManager::Get+AF8-Instance()-+AD4-Set+AF8-IsStrike(false)+ADs-

			CGameInstance::Get+AF8-Instance()-+AD4-StopAll()+ADs-
			CGameInstance::Get+AF8-Instance()-+AD4-PlayBGM(TEXT(+ACI-BGM+AF8-LawBattle.wav+ACI-), g+AF8-fSoundVolume)+ADs-
			CBattleManager::Get+AF8-Instance()-+AD4-Set+AF8-Rinwellboss(true)+ADs-

		+AH0-

	+AH0-

	
	if (CGameInstance::Get+AF8-Instance()-+AD4-Key+AF8-Up(DIK+AF8-PGUP))
	+AHs-
		dynamic+AF8-cast+ADw-CUI+AF8-Dialoguepopup+ACoAPg-(CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Get+AF8-Dialoguepopup())-+AD4-Open+AF8-Dialogue(2, true, 1, 0)+ADs-
	+AH0-
		
		

	//m+AF8-vCurrentDialouge +AD0- m+AF8-vDialogue+ADs-
	if (m+AF8-btick)
	+AHs-
		

		m+AF8-fbrightpos +-+AD0- 0.015f+ADs-

		if (m+AF8-fbrightpos +AD4- 2.5f)
			m+AF8-fbrightpos +AD0- 0.f+ADs-


		//std::mbstowcs+ADs-
		if (m+AF8-bpingdown)
		+AHs-
			m+AF8-fCursorpingpong +-+AD0- 0.25f+ADs-

			if (m+AF8-fCursorpingpong +AD4APQ- 5.f)
			+AHs-
				m+AF8-fCursorpingpong +AD0- 5.f+ADs-
				m+AF8-bpingdown +AD0- false+ADs-
			+AH0-
		+AH0-
		else
		+AHs-
			m+AF8-fCursorpingpong -+AD0- 0.25f+ADs-
			if (m+AF8-fCursorpingpong +ADwAPQ- 0.f)
			+AHs-
				m+AF8-fCursorpingpong +AD0- 0.f+ADs-
				m+AF8-bpingdown +AD0- true+ADsAOw-
			+AH0-
		+AH0-


		--m+AF8-fFlowCurrent+ADs-

		if (m+AF8-fFlowCurrent +ADwAPQ- 0)
			m+AF8-fFlowCurrent +AD0- 100.f+ADs-

		if (m+AF8-bfadein)
		+AHs-
			m+AF8-fAlpha +-+AD0- 0.02f+ADs- //+//3//f/9bac-
			m+AF8-fFade +-+AD0- 0.8f+ADs-
		+AH0-
		else if (m+AF8-bfadeout)
		+AHs-
			m+AF8-fAlpha -+AD0- 0.02f+ADs-
			m+AF8-fFade -+AD0- 0.8f+ADs-
		+AH0-



		//	m+AF8-fAlpha +AD0- 0.5f+ADs-

		//if (CGameInstance::Get+AF8-Instance()-+AD4-Key+AF8-Up(DIK+AF8-4)) // +//3//f/9//3//f/9//0-
		//+AHs-
		//	m+AF8-bfadeout +AD0- true+ADs-
		//+AH0-
		//if (CGameInstance::Get+AF8-Instance()-+AD4-Key+AF8-Up(DIK+AF8-5)) // +//3//f/9//3//f/9//3//Q-
		//+AHs-
		//	m+AF8-bfadein +AD0- true+ADs-
		//+AH0-
		if (CGameInstance::Get+AF8-Instance()-+AD4-Key+AF8-Up(DIK+AF8-6)) // +//3//f/9//3//f/9//3//Q-
		+AHs-
			if (m+AF8-iVectorIndex +AD0APQ- 13 +ACYAJg- m+AF8-iDialogueindex +AD0APQ- 1)
			+AHs-
				CPlayerManager::Get+AF8-Instance()-+AD4-Get+AF8-EnumPlayer(2)-+AD4-Get+AF8-Transform()-+AD4-Set+AF8-State(CTransform::STATE+AF8-TRANSLATION, XMVectorSet(50.f, 0.f, 50.f, 1.f))+ADs-
				CPlayerManager::Get+AF8-Instance()-+AD4-Get+AF8-EnumPlayer(2)-+AD4-Get+AF8-Transform()-+AD4-LookDir(XMVectorSet(0.f, 0.f, 1.f, 0.f))+ADs-
				CCameraManager+ACo- pCameraManager +AD0- CCameraManager::Get+AF8-Instance()+ADs-
				pCameraManager-+AD4-Set+AF8-CamState(CCameraManager::CAM+AF8-ACTION)+ADs-
				pCameraManager-+AD4-Play+AF8-ActionCamera(TEXT(+ACI-SexyLaw2.dat+ACI-), XMMatrixIdentity())+ADs-
				CPlayerManager::Get+AF8-Instance()-+AD4-Get+AF8-EnumPlayer(2)-+AD4-Set+AF8-IsActionMode(true)+ADs-
				CPlayerManager::Get+AF8-Instance()-+AD4-Get+AF8-EnumPlayer(2)-+AD4-AI+AF8-RINWELL+AF8-Event()+ADs-
			+AH0-
		
			+-+-m+AF8-iDialogueindex+ADs-

			if (m+AF8-iDialogueindex +AD0APQ- vectorsize)
			+AHs-
				m+AF8-bfadeout +AD0- true+ADs-
				CCameraManager+ACo- pCameraManager +AD0- CCameraManager::Get+AF8-Instance()+ADs-
				if (pCameraManager-+AD4-Get+AF8-CamState() +AD0APQ- CCameraManager::CAM+AF8-ACTION)
				+AHs-
					CPlayerManager::Get+AF8-Instance()-+AD4-Get+AF8-ActivePlayer()-+AD4-Set+AF8-IsActionMode(false)+ADs-
					CCamera+ACo- pCamera +AD0- pCameraManager-+AD4-Get+AF8-CurrentCamera()+ADs-
					dynamic+AF8-cast+ADw-CCamera+AF8-Action+ACoAPg-(pCamera)-+AD4-Set+AF8-Play(false)+ADs-
					CCameraManager::Get+AF8-Instance()-+AD4-Set+AF8-CamState(CCameraManager::CAM+AF8-DYNAMIC)+ADs-
				+AH0-
				
				--m+AF8-iDialogueindex+ADs-
				CUI+AF8-QuestClear::QUESTCLEARDESC cleardesc+ADs-
				ZeroMemory(+ACY-cleardesc, sizeof(CUI+AF8-QuestClear::QUESTCLEARDESC))+ADs-
				switch (m+AF8-iVectorIndex)
				+AHs-
				case 0:
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestIndex(1)+ADs-
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-QUESTSTART+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ssssss+ACI-)))))
						return OBJ+AF8-NOEVENT+ADs-
					
					//CGame
					break+ADs-

				case 1:
					
					cleardesc.eName1 +AD0- ITEMNAME+AF8-LEMONJELLY+ADs-
					cleardesc.eType1 +AD0- ITEMTYPE+AF8-JELLY+ADs-
					cleardesc.iGaingald +AD0- 700+ADs-
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-QUESTCLEAR+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ssss+ACI-)), +ACY-cleardesc)))
						return OBJ+AF8-NOEVENT+ADs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-AddItem(ITEMNAME+AF8-LEMONJELLY, ITEMTYPE+AF8-JELLY,false,false)+ADs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestComplete(0, true)+ADs-
					break+ADs-

				case 2:
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestIndex(2)+ADs-
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-QUESTSTART+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ssssss+ACI-)))))
						return OBJ+AF8-NOEVENT+ADs-

					break+ADs-

				case 3:

					cleardesc.eName1 +AD0- ITEMNAME+AF8-PYOGOMUSHROOM+ADs-
					cleardesc.eType1 +AD0- ITEMTYPE+AF8-MATERIAL+ADs-
					cleardesc.iGaingald +AD0- 100+ADs-
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-QUESTCLEAR+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ssss+ACI-)), +ACY-cleardesc)))
						return OBJ+AF8-NOEVENT+ADs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-AddItem(ITEMNAME+AF8-LEMONJELLY, ITEMTYPE+AF8-JELLY, false, false)+ADs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestComplete(1, true)+ADs-
					break+ADs-

				case 4:

					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestIndex(3)+ADs-
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-QUESTSTART+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ssssss+ACI-)))))
						return OBJ+AF8-NOEVENT+ADs-

					break+ADs-

				case 5:
				+AHs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestIndex(3)+ADs-
					CUI+AF8-QuestClear::QUESTCLEARDESC garr+ADs-
					ZeroMemory(+ACY-garr, sizeof(CUI+AF8-QuestClear::QUESTCLEARDESC))+ADs-
					garr.eName1 +AD0- ITEMNAME+AF8-REDSAGE+ADs-
					garr.eType1 +AD0- ITEMTYPE+AF8-LEAVES+ADs-
					garr.iGaingald +AD0- 500+ADs-
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-QUESTCLEAR+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ssss+ACI-)), +ACY-garr)))
						return OBJ+AF8-NOEVENT+ADs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-AddItem(ITEMNAME+AF8-LEMONJELLY, ITEMTYPE+AF8-JELLY, false, false)+ADs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestComplete(2, true)+ADs-

					break+ADs-
				+AH0-
					

				case 6:
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-SystemMessagebox+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ssss+ACI-)))))
						return OBJ+AF8-NOEVENT+ADs-

					break+ADs-

				case 7:
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestIndex(4)+ADs-
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-QUESTSTART+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ssssss+ACI-)))))
						return OBJ+AF8-NOEVENT+ADs-
					

					break+ADs-

				case 8:
					dynamic+AF8-cast+ADw-CLevel+AF8-Restaurant+ACoAPg-(CGameInstance::Get+AF8-Instance()-+AD4-Get+AF8-CurrentLevel())-+AD4-Set+AF8-MiniGameStart(true)+ADs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-Dialogue+AF8-section(9)+ADs-
					break+ADs-

				case 9:
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-Dialogue+AF8-section(10)+ADs-
					break+ADs-
				case 10:
					dynamic+AF8-cast+ADw-CLevel+AF8-WorkTool+ACoAPg-(CGameInstance::Get+AF8-Instance()-+AD4-Get+AF8-CurrentLevel())-+AD4-Set+AF8-MiniGameStart(true)+ADs-
					break+ADs-

				case 11:
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-Dialogue+AF8-section(12)+ADs-
					break+ADs-

				case 12:
				+AHs-
					CLevel+ACo- pCurrentLevel +AD0- CGameInstance::Get+AF8-Instance()-+AD4-Get+AF8-CurrentLevel()+ADs-
					pCurrentLevel-+AD4-Set+AF8-NextLevel(true, LEVEL+AF8-LAWBATTLE)+ADs-
					break+ADs-
				+AH0-
					

				case 13:
				+AHs-
					m+AF8-bSexyEventEnd +AD0- true+ADs-
					break+ADs-
				+AH0-
					
				case 14:
				+AHs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestIndex(4)+ADs-
					CUI+AF8-QuestClear::QUESTCLEARDESC garrr+ADs-
					ZeroMemory(+ACY-garrr, sizeof(CUI+AF8-QuestClear::QUESTCLEARDESC))+ADs-
					garrr.eName1 +AD0- ITEMNAME+AF8-REDSAGE+ADs-
					garrr.eType1 +AD0- ITEMTYPE+AF8-LEAVES+ADs-
					garrr.iGaingald +AD0- 500+ADs-
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-QUESTCLEAR+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ssss+ACI-)), +ACY-garrr)))
						return OBJ+AF8-NOEVENT+ADs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-AddItem(ITEMNAME+AF8-LEMONJELLY, ITEMTYPE+AF8-JELLY, false, false)+ADs-
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestComplete(3, true)+ADs-
					break+ADs-
				+AH0-
					

				case 15:
					CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-QuestIndex(5)+ADs-
					if (FAILED(CGameInstance::Get+AF8-Instance()-+AD4-Add+AF8-GameObject(TEXT(+ACI-Prototype+AF8-GameObject+AF8-UI+AF8-QUESTSTART+ACI-), LEVEL+AF8-SNOWFIELD, (TEXT(+ACI-ses+ACI-)))))
						return OBJ+AF8-NOEVENT+ADs-
					break+ADs-


				case 16:
					
					//	CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-Dialogue+AF8-section(6)+ADs- //after quest 3 clear
					break+ADs-

				+AH0-

				CGameInstance::Get+AF8-Instance()-+AD4-PlaySounds(TEXT(+ACI-Chat+AF8-End.wav+ACI-), SOUND+AF8-UI, 0.4f)+ADs-
			+AH0-
			else
				CGameInstance::Get+AF8-Instance()-+AD4-PlaySounds(TEXT(+ACI-Chat+AF8-Next.wav+ACI-), SOUND+AF8-UI, 0.4f)+ADs-
		+AH0-
	+AH0-


	
	//if (CGameInstance::Get+AF8-Instance()-+AD4-Key+AF8-Up(DIK+AF8-9)) // +//3//f/9//3//f/9//3//Q-
	//+AHs-
	//	+-+-m+AF8-iVectorIndex+ADs-
	//+AH0-



	
	return OBJ+AF8-NOEVENT+ADs-
+AH0-

void CUI+AF8-Dialogue::Late+AF8-Tick(+AF8-float fTimeDelta)
+AHs-
	int iii +AD0- sizeof(m+AF8-vCurrentDialogue+AFs-0+AF0AWw-0+AF0-)+ADs-
	iii +AD0- sizeof(m+AF8-vCurrentDialogue+AFs-0+AF0AWw-1+AF0-)+ADs-
	 iii +AD0- sizeof(m+AF8-vCurrentDialogue+AFs-0+AF0AWw-0+AF0AWw-1+AF0-)+ADs-
	if (m+AF8-btick)
	+AHs-
		if (m+AF8-fAlpha +AD4APQ- 0.5f +ACYAJg- m+AF8-bfadein)
		+AHs-
			m+AF8-fAlpha +AD0- 0.5f+ADs-
			m+AF8-bfadein +AD0- false+ADs-
		+AH0-

		if (m+AF8-fAlpha +ADwAPQ- 0.f +ACYAJg- m+AF8-bfadeout)
		+AHs-
			m+AF8-fAlpha +AD0- 0.f+ADs-
			m+AF8-bfadeout +AD0- false+ADs-
			m+AF8-btick +AD0- false+ADs-
			m+AF8-iDialogueindex +AD0- 0+ADs-
		+AH0-

		if (nullptr +ACEAPQ- m+AF8-pRendererCom)
			m+AF8-pRendererCom-+AD4-Add+AF8-RenderGroup(CRenderer::RENDER+AF8-UI+AF8-BACK, this)+ADs-
	+AH0-
	
	

+AH0-

HRESULT CUI+AF8-Dialogue::Render()
+AHs-

	+AF8-float alpha +AD0- m+AF8-fAlpha +ACo- 2.f+ADs-

	if (nullptr +AD0APQ- m+AF8-pShaderCom +AHwAfA-
		nullptr +AD0APQ- m+AF8-pVIBufferCom)
		return E+AF8-FAIL+ADs-

	
	

	if (FAILED(SetUp+AF8-ShaderResources()))
		return E+AF8-FAIL+ADs-

	



	if(FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fAlpha+ACI-, +ACY-m+AF8-fAlpha, sizeof(+AF8-float))))
		return E+AF8-FAIL+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-ShaderResourceView(+ACI-g+AF8-DiffuseTexture+ACI-, m+AF8-pTextureCom-+AD4-Get+AF8-SRV(0))))
		return E+AF8-FAIL+ADs-


	m+AF8-fSize.x +AD0- 900.f+ADs-
	m+AF8-fSize.y +AD0- 150.f+ADs-
	m+AF8-fPosition.x +AD0- 650.f+ADs-
	m+AF8-fPosition.y +AD0- 620.f +- m+AF8-fFade+ADsAOw-

	m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-RIGHT, m+AF8-fSize.x)+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-UP, m+AF8-fSize.y)+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-State(CTransform::STATE+AF8-TRANSLATION, XMVectorSet(m+AF8-fPosition.x - g+AF8-iWinSizeX +ACo- 0.5f, -m+AF8-fPosition.y +- g+AF8-iWinSizeY +ACo- 0.5f, 0.f, 1.f))+ADs-
	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-WorldMatrix+ACI-, +ACY-m+AF8-pTransformCom-+AD4-Get+AF8-World4x4+AF8-TP(), sizeof(+AF8-float4x4))))
		return E+AF8-FAIL+ADs-
	

	m+AF8-pShaderCom-+AD4-Begin(UI+AF8-DIALOGUEBOX)+ADs-

	m+AF8-pVIBufferCom-+AD4-Render()+ADs-

	m+AF8-fSize.x +AD0- 400.f+ADs-
	m+AF8-fSize.y +AD0- 40.f+ADs-
	m+AF8-fPosition.x +AD0- 430.f+ADs-
	m+AF8-fPosition.y +AD0- 545.f +- m+AF8-fFade+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-RIGHT, m+AF8-fSize.x)+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-UP, m+AF8-fSize.y)+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-State(CTransform::STATE+AF8-TRANSLATION, XMVectorSet(m+AF8-fPosition.x - g+AF8-iWinSizeX +ACo- 0.5f, -m+AF8-fPosition.y +- g+AF8-iWinSizeY +ACo- 0.5f, 0.f, 1.f))+ADs-
	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-WorldMatrix+ACI-, +ACY-m+AF8-pTransformCom-+AD4-Get+AF8-World4x4+AF8-TP(), sizeof(+AF8-float4x4))))
		return E+AF8-FAIL+ADs-
	m+AF8-pShaderCom-+AD4-Begin(UI+AF8-DIALOGUEBOX)+ADs-

	m+AF8-pVIBufferCom-+AD4-Render()+ADs-

	//m+AF8-fSize.x +AD0- 44.f+ADs-
	//m+AF8-fSize.y +AD0- 28.f+ADs-
	//m+AF8-fPosition.x +AD0- 430.f+ADs-
	//m+AF8-fPosition.y +AD0- 545.f +- m+AF8-fFade+ADs-
	//+AF8-float alpha +AD0- m+AF8-fAlpha +ACo- 2.f+ADs-
	//if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fAlpha+ACI-, +ACY-alpha, sizeof(+AF8-float))))
	//	return E+AF8-FAIL+ADs-
	//m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-RIGHT, m+AF8-fSize.x)+ADs-
	//m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-UP, m+AF8-fSize.y)+ADs-
	//m+AF8-pTransformCom-+AD4-Set+AF8-State(CTransform::STATE+AF8-TRANSLATION, XMVectorSet(m+AF8-fPosition.x - g+AF8-iWinSizeX +ACo- 0.5f, -m+AF8-fPosition.y +- g+AF8-iWinSizeY +ACo- 0.5f, 0.f, 1.f))+ADs-
	//if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-WorldMatrix+ACI-, +ACY-m+AF8-pTransformCom-+AD4-Get+AF8-World4x4+AF8-TP(), sizeof(+AF8-float4x4))))
	//	return E+AF8-FAIL+ADs-
	//if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-ShaderResourceView(+ACI-g+AF8-DiffuseTexture+ACI-, m+AF8-pTextureCom2-+AD4-Get+AF8-SRV(0))))
	//	return E+AF8-FAIL+ADs-

	//if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fBright+ACI-, +ACY-m+AF8-fbrightpos, sizeof(+AF8-float))))
	//	return E+AF8-FAIL+ADs-
	///+ACo-if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fBrightlerp+ACI-, +ACY-m+AF8-fbrightlerp, sizeof(+AF8-float))))
	//return E+AF8-FAIL+ADsAKg-/
	//m+AF8-pShaderCom-+AD4-Begin(UI+AF8-BRIGHT)+ADs-
	//m+AF8-pVIBufferCom-+AD4-Render()+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fAlpha+ACI-, +ACY-m+AF8-fAlpha, sizeof(+AF8-float))))
		return E+AF8-FAIL+ADs-


	m+AF8-fSize.x +AD0- 400.f+ADs-
	m+AF8-fSize.y +AD0- 6.f+ADs-
	m+AF8-fPosition.x +AD0- 450.f+ADs-
	m+AF8-fPosition.y +AD0- 560.f +- m+AF8-fFade+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-RIGHT, m+AF8-fSize.x)+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-UP, m+AF8-fSize.y)+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-State(CTransform::STATE+AF8-TRANSLATION, XMVectorSet(m+AF8-fPosition.x - g+AF8-iWinSizeX +ACo- 0.5f, -m+AF8-fPosition.y +- g+AF8-iWinSizeY +ACo- 0.5f, 0.f, 1.f))+ADs-
	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-WorldMatrix+ACI-, +ACY-m+AF8-pTransformCom-+AD4-Get+AF8-World4x4+AF8-TP(), sizeof(+AF8-float4x4))))
		return E+AF8-FAIL+ADs-
	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-ShaderResourceView(+ACI-g+AF8-DiffuseTexture+ACI-, m+AF8-pTextureCom-+AD4-Get+AF8-SRV(1))))
		return E+AF8-FAIL+ADs-

	m+AF8-pShaderCom-+AD4-Begin(UI+AF8-DIALOGUELINE)+ADs-

	m+AF8-pVIBufferCom-+AD4-Render()+ADs-


	
	m+AF8-fSize.x +AD0- 32.f+ADs-
	m+AF8-fSize.y +AD0- 16.f+ADs-
	m+AF8-fPosition.x +AD0- 640.f+ADs-
	m+AF8-fPosition.y +AD0- 670.f +- m+AF8-fFade +- m+AF8-fCursorpingpong+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-RIGHT, m+AF8-fSize.x)+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-UP, m+AF8-fSize.y)+ADs-
	m+AF8-pTransformCom-+AD4-Set+AF8-State(CTransform::STATE+AF8-TRANSLATION, XMVectorSet(m+AF8-fPosition.x - g+AF8-iWinSizeX +ACo- 0.5f, -m+AF8-fPosition.y +- g+AF8-iWinSizeY +ACo- 0.5f, 0.f, 1.f))+ADs-
	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-WorldMatrix+ACI-, +ACY-m+AF8-pTransformCom-+AD4-Get+AF8-World4x4+AF8-TP(), sizeof(+AF8-float4x4))))
		return E+AF8-FAIL+ADs-
	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-ShaderResourceView(+ACI-g+AF8-DiffuseTexture+ACI-, m+AF8-pTextureCom-+AD4-Get+AF8-SRV(2))))
		return E+AF8-FAIL+ADs-

	/+ACo-m+AF8-fFlowMAX +AD0- 1
		m+AF8-fFlowCurrent+ACo-/

	m+AF8-pShaderCom-+AD4-Begin(UI+AF8-DIALOGUECURSORNONMOVE)+ADs-

	m+AF8-pVIBufferCom-+AD4-Render()+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fAlpha+ACI-, +ACY-alpha, sizeof(+AF8-float))))
		return E+AF8-FAIL+ADs-

	m+AF8-pShaderCom-+AD4-Begin(UI+AF8-DIALOGUECURSOR)+ADs-

	m+AF8-pVIBufferCom-+AD4-Render()+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fAlpha+ACI-, +ACY-m+AF8-fAlpha, sizeof(+AF8-float))))
		return E+AF8-FAIL+ADs-

	m+AF8-fFontPos.x +AD0- 465.f+ADs-
	//m+AF8-fFontPos.y +AD0- 575.f +- m+AF8-fFade+ADs-
	
	Render+AF8-Fonts(m+AF8-iDialogueindex)+ADs-



	return S+AF8-OK+ADs-
+AH0-

HRESULT CUI+AF8-Dialogue::Ready+AF8-Components(void +ACo- pArg)
+AHs-
	/+ACo- For.Com+AF8-Renderer +ACo-/
	if (FAILED(+AF8AXw-super::Add+AF8-Components(TEXT(+ACI-Com+AF8-Renderer+ACI-), LEVEL+AF8-STATIC, TEXT(+ACI-Prototype+AF8-Component+AF8-Renderer+ACI-), (CComponent+ACoAKg-)+ACY-m+AF8-pRendererCom)))
		return E+AF8-FAIL+ADs-

	/+ACo- For.Com+AF8-Transform +ACo-/
	if (FAILED(+AF8AXw-super::Add+AF8-Components(TEXT(+ACI-Com+AF8-Transform+ACI-), LEVEL+AF8-STATIC, TEXT(+ACI-Prototype+AF8-Component+AF8-Transform+ACI-), (CComponent+ACoAKg-)+ACY-m+AF8-pTransformCom)))
		return E+AF8-FAIL+ADs-

	/+ACo- For.Com+AF8-Shader +ACo-/
	if (FAILED(+AF8AXw-super::Add+AF8-Components(TEXT(+ACI-Com+AF8-Shader+ACI-), LEVEL+AF8-STATIC, TEXT(+ACI-Prototype+AF8-Component+AF8-Shader+AF8-UI+ACI-), (CComponent+ACoAKg-)+ACY-m+AF8-pShaderCom)))
		return E+AF8-FAIL+ADs-

	/+ACo- For.Com+AF8-Texture +ACo-/
	if (FAILED(+AF8AXw-super::Add+AF8-Components(TEXT(+ACI-Com+AF8-Texture+ACI-), LEVEL+AF8-STATIC, TEXT(+ACI-Prototype+AF8-Component+AF8-Texture+AF8-Dialoguebox+ACI-), (CComponent+ACoAKg-)+ACY-m+AF8-pTextureCom)))
		return E+AF8-FAIL+ADs-

	/+ACo- For.Com+AF8-VIBuffer +ACo-/
	if (FAILED(+AF8AXw-super::Add+AF8-Components(TEXT(+ACI-Com+AF8-VIBuffer+ACI-), LEVEL+AF8-STATIC, TEXT(+ACI-Prototype+AF8-Component+AF8-VIBuffer+AF8-Rect+ACI-), (CComponent+ACoAKg-)+ACY-m+AF8-pVIBufferCom)))
		return E+AF8-FAIL+ADs-

	/+ACo- For.Com+AF8-Texture +ACo-/
	if (FAILED(+AF8AXw-super::Add+AF8-Components(TEXT(+ACI-Com+AF8-Texture1+ACI-), LEVEL+AF8-STATIC, TEXT(+ACI-Prototype+AF8-Component+AF8-Texture+AF8-hpgrad+ACI-), (CComponent+ACoAKg-)+ACY-m+AF8-pTextureCom1)))
		return E+AF8-FAIL+ADs-

	/+ACo- For.Com+AF8-Texture +ACo-/
	if (FAILED(+AF8AXw-super::Add+AF8-Components(TEXT(+ACI-Com+AF8-Texture2+ACI-), LEVEL+AF8-STATIC, TEXT(+ACI-Prototype+AF8-Component+AF8-Texture+AF8-charactername+ACI-), (CComponent+ACoAKg-)+ACY-m+AF8-pTextureCom2)))
		return E+AF8-FAIL+ADs-

	/+ACo- For.Com+AF8-Texture +ACo-/
	if (FAILED(+AF8AXw-super::Add+AF8-Components(TEXT(+ACI-Com+AF8-Texture3+ACI-), LEVEL+AF8-STATIC, TEXT(+ACI-Prototype+AF8-Component+AF8-Texture+AF8-dialogueportrait+ACI-), (CComponent+ACoAKg-)+ACY-m+AF8-pTextureCom3)))
		return E+AF8-FAIL+ADs-

	

	

	return S+AF8-OK+ADs-
+AH0-

HRESULT CUI+AF8-Dialogue::SetUp+AF8-ShaderResources()
+AHs-
	if (nullptr +AD0APQ- m+AF8-pShaderCom)
		return E+AF8-FAIL+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-WorldMatrix+ACI-, +ACY-m+AF8-pTransformCom-+AD4-Get+AF8-World4x4+AF8-TP(), sizeof(+AF8-float4x4))))
		return E+AF8-FAIL+ADs-
	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-ViewMatrix+ACI-, +ACY-m+AF8-ViewMatrix, sizeof(+AF8-float4x4))))
		return E+AF8-FAIL+ADs-
	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-ProjMatrix+ACI-, +ACY-m+AF8-ProjMatrix, sizeof(+AF8-float4x4))))
		return E+AF8-FAIL+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-ShaderResourceView(+ACI-g+AF8-DiffuseTexture+ACI-, m+AF8-pTextureCom-+AD4-Get+AF8-SRV(0))))
		return E+AF8-FAIL+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-ShaderResourceView(+ACI-g+AF8-GradationTexture+ACI-, m+AF8-pTextureCom1-+AD4-Get+AF8-SRV(0))))
		return E+AF8-FAIL+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fAlpha+ACI-, +ACY-m+AF8-fAlpha, sizeof(+AF8-float))))
		return E+AF8-FAIL+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fCurrentHp+ACI-, +ACY-m+AF8-fFlowCurrent, sizeof(+AF8-float))))
		return E+AF8-FAIL+ADs-

	if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fMaxHp+ACI-, +ACY-m+AF8-fFlowMAX, sizeof(+AF8-float))))
		return E+AF8-FAIL+ADs-


	return S+AF8-OK+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-dialogue()
+AHs-
	std::ifstream file(+ACI-../../../Bin/test.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
		//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-
	

	std::ifstream file1(+ACI-../../../Bin/test1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
		//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/test2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/test3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file4(+ACI-../../../Bin/test4.txt+ACI-)+ADs-
	if (file4.is+AF8-open())
	+AHs-
		while (file4.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue+AFs-4+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file4.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file5(+ACI-../../../Bin/test5.txt+ACI-)+ADs-
	if (file5.is+AF8-open())
	+AHs-
		while (file5.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue+AFs-5+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file5.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file6(+ACI-../../../Bin/test6.txt+ACI-)+ADs-
	if (file6.is+AF8-open())
	+AHs-
		while (file6.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue+AFs-6+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file6.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file7(+ACI-../../../Bin/test7.txt+ACI-)+ADs-
	if (file7.is+AF8-open())
	+AHs-
		while (file7.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue+AFs-7+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file7.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-






	
	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue+AFs-3+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue+AFs-4+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue+AFs-5+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue+AFs-6+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue+AFs-7+AF0-)+ADs-

	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-


+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Quest1Clear()
+AHs-
	std::ifstream file(+ACI-../../../Bin/quest1clear0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge1+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/quest1clear1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge1+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/quest1clear2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge1+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/quest1clear3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge1+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file4(+ACI-../../../Bin/quest1clear4.txt+ACI-)+ADs-
	if (file4.is+AF8-open())
	+AHs-
		while (file4.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge1+AFs-4+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file4.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file5(+ACI-../../../Bin/quest1clear5.txt+ACI-)+ADs-
	if (file5.is+AF8-open())
	+AHs-
		while (file5.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge1+AFs-5+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file5.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	







	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge1+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge1+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge1+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge1+AFs-3+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge1+AFs-4+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge1+AFs-5+AF0-)+ADs-
	

	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Quest2Strat()
+AHs-
	std::ifstream file(+ACI-../../../Bin/quest2start0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge2+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/quest2start1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge2+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/quest2start2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge2+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/quest2start3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge2+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-



	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge2+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge2+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge2+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge2+AFs-3+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Quest2Clear()
+AHs-
	std::ifstream file(+ACI-../../../Bin/quest2clear0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge3+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/quest2clear1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge3+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	



	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge3+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge3+AFs-1+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Quest3Start()
+AHs-
	std::ifstream file(+ACI-../../../Bin/quest3start0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge4+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/quest3start1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge4+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/quest3start2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge4+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/quest3start3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge4+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-



	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge4+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge4+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge4+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge4+AFs-3+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Quest3Clear()
+AHs-
	std::ifstream file(+ACI-../../../Bin/quest3clear0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge5+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/quest3clear1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge5+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/quest3clear2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge5+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/quest3clear3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge5+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file4(+ACI-../../../Bin/quest3clear4.txt+ACI-)+ADs-
	if (file4.is+AF8-open())
	+AHs-
		while (file4.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge5+AFs-4+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file4.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file5(+ACI-../../../Bin/quest3clear5.txt+ACI-)+ADs-
	if (file5.is+AF8-open())
	+AHs-
		while (file5.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge5+AFs-5+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file5.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file6(+ACI-../../../Bin/quest3clear6.txt+ACI-)+ADs-
	if (file6.is+AF8-open())
	+AHs-
		while (file6.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge5+AFs-6+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file6.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-
	//m+AF8-vCurrentDialogue.








	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge5+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge5+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge5+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge5+AFs-3+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge5+AFs-4+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge5+AFs-5+AF0-)+ADs- 
	matrix.push+AF8-back(m+AF8-vDialouge5+AFs-6+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Stage1Last()
+AHs-

	std::ifstream file(+ACI-../../../Bin/stage1last0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge6+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/stage1last1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge6+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/stage1last2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge6+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/stage1last3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge6+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file4(+ACI-../../../Bin/stage1last4.txt+ACI-)+ADs-
	if (file4.is+AF8-open())
	+AHs-
		while (file4.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge6+AFs-4+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file4.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-




	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge6+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge6+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge6+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge6+AFs-3+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge6+AFs-4+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Quest4Start()
+AHs-
	std::ifstream file(+ACI-../../../Bin/quest4start0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge7+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/quest4start1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge7+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/quest4start2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge7+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/quest4start3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge7+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file4(+ACI-../../../Bin/quest4start4.txt+ACI-)+ADs-
	if (file4.is+AF8-open())
	+AHs-
		while (file4.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialouge7+AFs-4+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file4.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	//std::ifstream file5(+ACI-../../../Bin/quest4start5.txt+ACI-)+ADs-
	//if (file5.is+AF8-open())
	//+AHs-
	//	while (file5.getline(fuck, 256))
	//	+AHs-
	//		+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
	//		m+AF8-vDialouge7+AFs-5+AF0-.push+AF8-back(pszDialog)+ADs-
	//		ConverCtoWC(fuck)+ADs-
	//		memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
	//		//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
	//	+AH0-
	//	file5.close()+ADs-
	//+AH0-
	//else
	//+AHs-
	//	std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	//+AH0-









	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge7+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge7+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge7+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge7+AFs-3+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialouge7+AFs-4+AF0-)+ADs-
	//matrix.push+AF8-back(m+AF8-vDialouge7+AFs-5+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Minigame1start()
+AHs-
	std::ifstream file(+ACI-../../../Bin/minigamestart0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue8+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/minigamestart1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue8+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/minigamestart2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue8+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-




	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue8+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue8+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue8+AFs-2+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Minigame1end()
+AHs-
	std::ifstream file(+ACI-../../../Bin/minigameend0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue9+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/minigameend1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue9+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/minigameend2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue9+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/minigameend3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue9+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file4(+ACI-../../../Bin/minigameend4.txt+ACI-)+ADs-
	if (file4.is+AF8-open())
	+AHs-
		while (file4.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue9+AFs-4+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file4.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file5(+ACI-../../../Bin/minigameend5.txt+ACI-)+ADs-
	if (file5.is+AF8-open())
	+AHs-
		while (file5.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue9+AFs-5+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file5.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-









	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue9+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue9+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue9+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue9+AFs-3+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue9+AFs-4+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue9+AFs-5+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Minigame2start()
+AHs-
	std::ifstream file(+ACI-../../../Bin/minigame1start0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue10+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/minigame1start1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue10+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/minigame1start2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue10+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/minigame1start3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue10+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-









	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue10+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue10+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue10+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue10+AFs-3+AF0-)+ADs-



	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-Minigame2end()
+AHs-
	std::ifstream file(+ACI-../../../Bin/minigame1end0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue11+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/minigame1end1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue11+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/minigame1end2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue11+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/minigame1end3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue11+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file4(+ACI-../../../Bin/minigame1end4.txt+ACI-)+ADs-
	if (file4.is+AF8-open())
	+AHs-
		while (file4.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue11+AFs-4+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file4.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file5(+ACI-../../../Bin/minigame1end5.txt+ACI-)+ADs-
	if (file5.is+AF8-open())
	+AHs-
		while (file5.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue11+AFs-5+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file5.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-









	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue11+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue11+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue11+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue11+AFs-3+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue11+AFs-4+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue11+AFs-5+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-LawBossBattleStart()
+AHs-
	std::ifstream file(+ACI-../../../Bin/meetbosslaw0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue12+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/meetbosslaw1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue12+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/meetbosslaw2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue12+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/meetbosslaw3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue12+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file4(+ACI-../../../Bin/meetbosslaw4.txt+ACI-)+ADs-
	if (file4.is+AF8-open())
	+AHs-
		while (file4.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue12+AFs-4+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file4.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file5(+ACI-../../../Bin/meetbosslaw5.txt+ACI-)+ADs-
	if (file5.is+AF8-open())
	+AHs-
		while (file5.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue12+AFs-5+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file5.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file6(+ACI-../../../Bin/meetbosslaw6.txt+ACI-)+ADs-
	if (file6.is+AF8-open())
	+AHs-
		while (file6.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue12+AFs-6+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file6.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-






	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue12+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue12+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue12+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue12+AFs-3+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue12+AFs-4+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue12+AFs-5+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue12+AFs-6+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-LawBossBattleEvent()
+AHs-
	std::ifstream file(+ACI-../../../Bin/LawSexy0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue13+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/LawSexy1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue13+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/LawSexy2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue13+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/LawSexy3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue13+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-









	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue13+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue13+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue13+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue13+AFs-3+AF0-)+ADs-



	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-LawBossBattleEnd()
+AHs-
	std::ifstream file(+ACI-../../../Bin/bosslawend0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue14+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/bosslawend1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue14+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/bosslawend2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue14+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/bosslawend3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue14+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-









	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue14+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue14+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue14+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue14+AFs-3+AF0-)+ADs-



	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

void CUI+AF8-Dialogue::Read+AF8-TextFiles+AF8-for+AF8-LastQuestStart()
+AHs-
	std::ifstream file(+ACI-../../../Bin/lastqueststart0.txt+ACI-)+ADs-
	if (file.is+AF8-open())
	+AHs-
		while (file.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-0+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file1(+ACI-../../../Bin/lastqueststart1.txt+ACI-)+ADs-
	if (file1.is+AF8-open())
	+AHs-
		while (file1.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-1+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file1.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file2(+ACI-../../../Bin/lastqueststart2.txt+ACI-)+ADs-
	if (file2.is+AF8-open())
	+AHs-
		while (file2.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-2+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file2.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file3(+ACI-../../../Bin/lastqueststart3.txt+ACI-)+ADs-
	if (file3.is+AF8-open())
	+AHs-
		while (file3.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-3+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file3.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file4(+ACI-../../../Bin/lastqueststart4.txt+ACI-)+ADs-
	if (file4.is+AF8-open())
	+AHs-
		while (file4.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-4+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file4.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file5(+ACI-../../../Bin/lastqueststart5.txt+ACI-)+ADs-
	if (file5.is+AF8-open())
	+AHs-
		while (file5.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-5+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file5.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file6(+ACI-../../../Bin/lastqueststart6.txt+ACI-)+ADs-
	if (file6.is+AF8-open())
	+AHs-
		while (file6.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-6+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file6.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-
	std::ifstream file7(+ACI-../../../Bin/lastqueststart7.txt+ACI-)+ADs-
	if (file7.is+AF8-open())
	+AHs-
		while (file7.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-7+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file7.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file8(+ACI-../../../Bin/lastqueststart8.txt+ACI-)+ADs-
	if (file8.is+AF8-open())
	+AHs-
		while (file8.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-8+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file8.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	//m+AF8-vCurrentDialogue.
	std::ifstream file9(+ACI-../../../Bin/lastqueststart9.txt+ACI-)+ADs-
	if (file9.is+AF8-open())
	+AHs-
		while (file9.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-9+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file9.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file10(+ACI-../../../Bin/lastqueststart10.txt+ACI-)+ADs-
	if (file10.is+AF8-open())
	+AHs-
		while (file10.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-10+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file10.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file11(+ACI-../../../Bin/lastqueststart11.txt+ACI-)+ADs-
	if (file11.is+AF8-open())
	+AHs-
		while (file11.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-11+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file11.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-


	std::ifstream file12(+ACI-../../../Bin/lastqueststart12.txt+ACI-)+ADs-
	if (file12.is+AF8-open())
	+AHs-
		while (file12.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-12+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file12.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file13(+ACI-../../../Bin/lastqueststart13.txt+ACI-)+ADs-
	if (file13.is+AF8-open())
	+AHs-
		while (file13.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-13+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file13.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file14(+ACI-../../../Bin/lastqueststart14.txt+ACI-)+ADs-
	if (file14.is+AF8-open())
	+AHs-
		while (file14.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-14+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file14.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-

	std::ifstream file15(+ACI-../../../Bin/lastqueststart15.txt+ACI-)+ADs-
	if (file15.is+AF8-open())
	+AHs-
		while (file15.getline(fuck, 256))
		+AHs-
			+AF8-tchar+ACo- pszDialog +AD0- new +AF8-tchar+AFs-MAX+AF8-PATH+AF0AOw-
			m+AF8-vDialogue15+AFs-15+AF0-.push+AF8-back(pszDialog)+ADs-
			ConverCtoWC(fuck)+ADs-
			memcpy(pszDialog, m+AF8-szTXT, sizeof(+AF8-tchar)+ACo-MAX+AF8-PATH)+ADs-
			//	Safe+AF8-Delete+AF8-Array(pszDialog)+ADs-
		+AH0-
		file15.close()+ADs-
	+AH0-
	else
	+AHs-
		std::cout +ADwAPA- +ACI-Unable to open file+AFw-n+ACIAOw-
	+AH0-






	vector+ADw-vector+ADwAXw-tchar+ACoAPgA+- matrix+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-0+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-1+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-2+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-3+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-4+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-5+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-6+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-7+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-8+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-9+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-10+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-11+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-12+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-13+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-14+AF0-)+ADs-
	matrix.push+AF8-back(m+AF8-vDialogue15+AFs-15+AF0-)+ADs-


	m+AF8-vCurrentDialogue.push+AF8-back(matrix)+ADs-
+AH0-

wchar+AF8-t +ACo- CUI+AF8-Dialogue::ConverCtoWC(char +ACo- str)
+AHs-

	//wchar+AF8-t+//3//Q- +//3//f/9//0- +//3//f/9//0-
	wchar+AF8-t+ACo- pStr+ADs-
	//+//3//QG8- +//3//f/9//0Brg- +AWn//f/9- +//3//f/9- +//3//f/9//0- +//3//QIv-
	int strSize +AD0- MultiByteToWideChar(CP+AF8-ACP, 0, str, -1, NULL, NULL)+ADs-
	//wchar+AF8-t +//0HuP/9- +//0EtP/9-
	pStr +AD0- new WCHAR+AFs-MAX+AF8-PATH+AF0AOw-
	//+//3//Q- +//3//QIv-
	MultiByteToWideChar(CP+AF8-ACP, 0, str, +AF8-uint(strlen(str) +- 1), m+AF8-szTXT, MAX+AF8-PATH)+ADs-
	
	Safe+AF8-Delete+AF8-Array(pStr)+ADs-
	return pStr+ADs-

+AH0-

void CUI+AF8-Dialogue::Render+AF8-Fonts(+AF8-uint index)
+AHs-
	/+ACoAXw-uint test +AD0- 0+ADs-
	+AF8-uint test2 +AD0- sizeof(m+AF8-vDialogue)+ADs-
	for (auto vec : m+AF8-vDialogue) test+-+-+ADsAKg-/
	m+AF8-fFontsize +AD0- 0.75f+ADs-
	m+AF8-fFontOffsetY +AD0- 30.f+ADs-
	switch (m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0-.size()-1)
	+AHs-
	case 0:
		return+ADs-
		break+ADs-
	case 1:
		m+AF8-fFontPos.y +AD0- 595.f +- m+AF8-fFade+ADs-
		break+ADs-
	case 2:
		m+AF8-fFontPos.y +AD0- 580.f +- m+AF8-fFade+ADs- 
		break+ADs-
	case 3:
		m+AF8-fFontPos.y +AD0- 570.f +- m+AF8-fFade+ADs-
		break+ADs-

	+AH0-

	for (+AF8-uint i +AD0- 0+ADs- i +ADw- m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0-.size()-1+ADs- +-+-i)
	+AHs-
		CGameInstance::Get+AF8-Instance()-+AD4-Render+AF8-Font(TEXT(+ACI-Font+AF8-Nexon+ACI-), m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0AWw-i+AF0-, XMVectorSet(m+AF8-fFontPos.x, m+AF8-fFontPos.y +- (m+AF8-fFontOffsetY +ACo- (+AF8-float)i), 0.f, 1.f), XMVectorSet(m+AF8-FontR+ACo-(m+AF8-fAlpha+ACo-2.f), m+AF8-FontG+ACo-(m+AF8-fAlpha+ACo-2.f), m+AF8-FontB+ACo-(m+AF8-fAlpha+ACo-2.f), m+AF8-fAlpha +ACo- 2.f), m+AF8-fFontsize)+ADs-
	+AH0-

	
	CGameInstance::Get+AF8-Instance()-+AD4-Render+AF8-Font(TEXT(+ACI-Font+AF8-Nexon+ACI-), m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0AWw-m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0-.size()-1+AF0-, XMVectorSet(390.f, 535.f+-m+AF8-fFade, 0.f, 1.f), XMVectorSet(m+AF8-FontR+ACo-(m+AF8-fAlpha+ACo-2.f), m+AF8-FontG+ACo-(m+AF8-fAlpha+ACo-2.f), m+AF8-FontB+ACo-(m+AF8-fAlpha+ACo-2.f), m+AF8-fAlpha +ACo- 2.f), m+AF8-fFontsize)+ADs-
	
	//+AF8-tchar+ACo- asd +AD0- m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0AWw-m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0-.size() - 1+AF0AOw-

	if (+ACEAXw-tcscmp(m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0AWw-m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0-.size() - 1+AF0-, TEXT(+ACLFTNOcACI-)))
	+AHs-
		m+AF8-iPortraitnum +AD0- 0+ADs-
	+AH0-
	else if (+ACEAXw-tcscmp(m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0AWw-m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0-.size() - 1+AF0-, TEXT(+ACLC3MYoACI-)))
	+AHs-
		m+AF8-iPortraitnum +AD0- 1+ADs-
	+AH0-
	else if (+ACEAXw-tcscmp(m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0AWw-m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0-.size() - 1+AF0-, TEXT(+ACK5sMbwACI-)))
	+AHs-
		m+AF8-iPortraitnum +AD0- 2+ADs-
	+AH0-
	else if (+ACEAXw-tcscmp(m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0AWw-m+AF8-vCurrentDialogue+AFs-m+AF8-iVectorIndex+AF0AWw-index+AF0-.size() - 1+AF0-, TEXT(+ACK4XMawACI-)))
	+AHs-
		m+AF8-iPortraitnum +AD0- 3+ADs-
	+AH0-
	else
		m+AF8-iPortraitnum +AD0- 100+ADs-

	

	if(m+AF8-iPortraitnum +ADwAPQ- 3)
	+AHs-

		m+AF8-fSize.x +AD0- 600.f+ADs-
		m+AF8-fSize.y +AD0- 540.f+ADs-
		m+AF8-fPosition.x +AD0- 190.f+ADs-
		m+AF8-fPosition.y +AD0- 600.f +- m+AF8-fFade+ADs-

		m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-RIGHT, m+AF8-fSize.x)+ADs-
		m+AF8-pTransformCom-+AD4-Set+AF8-Scale(CTransform::STATE+AF8-UP, m+AF8-fSize.y)+ADs-
		m+AF8-pTransformCom-+AD4-Set+AF8-State(CTransform::STATE+AF8-TRANSLATION, XMVectorSet(m+AF8-fPosition.x - g+AF8-iWinSizeX +ACo- 0.5f, -m+AF8-fPosition.y +- g+AF8-iWinSizeY +ACo- 0.5f, 0.f, 1.f))+ADs-
		+AF8-float alpha +AD0- m+AF8-fAlpha +ACo- 2.f+ADs-
		if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-fAlpha+ACI-, +ACY-alpha, sizeof(+AF8-float))))
			return+ADs-
		if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-ShaderResourceView(+ACI-g+AF8-DiffuseTexture+ACI-, m+AF8-pTextureCom3-+AD4-Get+AF8-SRV(m+AF8-iPortraitnum))))
			return+ADs-
	
		if (FAILED(m+AF8-pShaderCom-+AD4-Set+AF8-RawValue(+ACI-g+AF8-WorldMatrix+ACI-, +ACY-m+AF8-pTransformCom-+AD4-Get+AF8-World4x4+AF8-TP(), sizeof(+AF8-float4x4))))
			return+ADs-
		m+AF8-pShaderCom-+AD4-Begin(UI+AF8-ALPHASET)+ADs-

		m+AF8-pVIBufferCom-+AD4-Render()+ADs-
	+AH0-

	




+AH0-

void CUI+AF8-Dialogue::Open+AF8-Dialogue(+AF8-uint index)
+AHs-
	m+AF8-iVectorIndex +AD0- index+ADs- m+AF8-btick +AD0- true+ADs-

	m+AF8-bfadein +AD0- true+ADs-
	+AF8-uint test +AD0- 0+ADs-

	for (auto vec : (m+AF8-vCurrentDialogue+AFs-index+AF0-)) test+-+-+ADs-
	
	vectorsize +AD0- test+ADs-
	CGameInstance::Get+AF8-Instance()-+AD4-PlaySounds(TEXT(+ACI-Open+AF8-Dialogue.wav+ACI-), SOUND+AF8-EFFECT, 0.3f)+ADs-
+AH0-



CUI+AF8-Dialogue +ACo- CUI+AF8-Dialogue::Create(ID3D11Device +ACo- pDevice, ID3D11DeviceContext +ACo- pContext)
+AHs-
	CUI+AF8-Dialogue+ACo-	pInstance +AD0- new CUI+AF8-Dialogue(pDevice, pContext)+ADs-

	if (FAILED(pInstance-+AD4-Initialize+AF8-Prototype()))
	+AHs-
		ERR+AF8-MSG(TEXT(+ACI-Failed to Created : CUI+AF8-Dialogue+ACI-))+ADs-
		Safe+AF8-Release(pInstance)+ADs-
	+AH0-

	return pInstance+ADs-
+AH0-



CGameObject +ACo- CUI+AF8-Dialogue::Clone(void +ACo- pArg)
+AHs-
	CUI+AF8-Dialogue+ACo-	pInstance +AD0- new CUI+AF8-Dialogue(+ACo-this)+ADs-

	if (FAILED(pInstance-+AD4-Initialize(pArg)))
	+AHs-
		ERR+AF8-MSG(TEXT(+ACI-Failed to Cloned : CUI+AF8-Dialogue+ACI-))+ADs-
		Safe+AF8-Release(pInstance)+ADs-
	+AH0-

	CUI+AF8-Manager::Get+AF8-Instance()-+AD4-Set+AF8-Dialogue(pInstance)+ADs-

	return pInstance+ADs-
+AH0-

void CUI+AF8-Dialogue::Free()
+AHs-

	Safe+AF8-Release(m+AF8-pTextureCom1)+ADs-
	Safe+AF8-Release(m+AF8-pTextureCom2)+ADs-
	Safe+AF8-Release(m+AF8-pTextureCom3)+ADs-
	//for (int j +AD0- 0+ADs- j +ADw- m+AF8-vDialogue+AFs-j+AF0-.size()+ADs- +-+-j)
	//+AHs-
	//	for (int i +AD0- 0+ADs- i+ADw-m+AF8-vDialogue+AFs-i+AF0-.size()+ADs- +-+-i)
	//	+AHs-
	//		//	Safe+AF8-Delete(m+AF8-vDialogue+AFs-i+AF0-)+ADs-


	//		+AF8-tchar+ACo- temp +AD0- m+AF8-vDialogue+AFs-j+AF0AWw-i+AF0AOw-
	//		delete+AFsAXQ- temp+ADs-

	//	+AH0-
	//+AH0-

	//
	//for (int i +AD0- 0+ADs- i +ADw- m+AF8-vDialogue+AFs-1+AF0-.size()+ADs- +-+-i)
	//+AHs-
	////	Safe+AF8-Delete(m+AF8-vDialogue+AFs-i+AF0-)+ADs-
	//	
	//	+AF8-tchar+ACo- temp +AD0- m+AF8-vDialogue+AFs-1+AF0AWw-i+AF0AOw-
	//	delete+AFsAXQ- temp+ADs-
	//+AH0-
	////delete+AFsAXQ- pszDialog+ADs-
	//+AF8-uint size +AD0- m+AF8-vCurrentDialogue+AFs-0+AF0-.size()+ADs-
	//size +AD0- m+AF8-vCurrentDialogue+AFs-0+AF0AWw-0+AF0-.size()+ADs-
	//size +AD0- m+AF8-vCurrentDialogue+AFs-0+AF0AWw-1+AF0-.size()+ADs-
	//size +AD0- m+AF8-vCurrentDialogue+AFs-1+AF0AWw-0+AF0-.size()+ADs-
	//size +AD0- m+AF8-vCurrentDialogue+AFs-1+AF0AWw-1+AF0-.size()+ADs-
	////size +AD0- m+AF8-vCurrentDialogue+AFs-1+AF0AWw-1+AF0-.size()+ADs-

	for (int i +AD0- 0+ADs- i +ADw- m+AF8-vCurrentDialogue.size()+ADs- +-+-i)
	+AHs-
		for (int j +AD0- 0+ADs- j +ADw-m+AF8-vCurrentDialogue+AFs-i+AF0-.size()+ADs- +-+-j)
		+AHs-
			/+ACo-if (j +AD0APQ- 2)
				continue+ADsAKg-/

				vector+ADwAXw-tchar+ACoAPg- temp +AD0- m+AF8-vCurrentDialogue+AFs-i+AF0AWw-j+AF0AOw-
				for (auto+ACY- iter : temp)
				+AHs-
					delete+AFsAXQ- iter+ADs-
				+AH0-
				temp.clear()+ADs-
		+AH0-
			

	+AH0-
	//	m+AF8-vCurrentDialogue+AFs-i+AF0AWw-j+AF0AWw-k+AF0-




	+AF8AXw-super::Free()+ADs-


	+AH0-
	
//+AH0-


