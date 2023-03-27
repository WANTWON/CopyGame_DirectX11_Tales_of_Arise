#include "stdafx.h"
#include "..\Public\UI_Dialogue.h"

#include "GameInstance.h"
#include <fstream>
#include <iostream>
#include <string>
#include "UI_QuestClear.h"
#include "UI_Dialoguepopup.h"
#include "CameraManager.h"
#include "Level_Restaurant.h"
#include "Level_WorkTool.h"
#include "Player.h"
#include "MonsterLaw.h"
#include "AiRinwell.h"
#include <tchar.h>


CUI_Dialogue::CUI_Dialogue(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Dialogue::CUI_Dialogue(const CUI_Dialogue & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Dialogue::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Dialogue::Initialize(void * pArg)
{


	m_FontR = 0.9019607843137255f;
	m_FontG = 0.8235294117647059f;
	m_FontB = 0.6588235294117647f;
	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fFontPos.x = 500.f;
	m_fAlpha = 0;

	m_bfadein = true;
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LIFEBOTTLE, ITEMTYPE_POTION, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LIFEBOTTLE, ITEMTYPE_POTION, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LIFEBOTTLE, ITEMTYPE_POTION, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LIFEBOTTLE, ITEMTYPE_POTION, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LIFEBOTTLE, ITEMTYPE_POTION, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LIFEBOTTLE, ITEMTYPE_POTION, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LIFEBOTTLE, ITEMTYPE_POTION, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LIFEBOTTLE, ITEMTYPE_POTION, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_POTATO, ITEMTYPE_VEGITABLE, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_DULLSWORD, ITEMTYPE_SWORD, false, false);
	CUI_Manager::Get_Instance()->AddItem(ITEMNAME_PYOGOMUSHROOM, ITEMTYPE_ARMOR, false, false);

	Read_TextFiles_for_dialogue();
	Read_TextFiles_for_Quest1Clear();
	Read_TextFiles_for_Quest2Strat(); 
	Read_TextFiles_for_Quest2Clear();
	Read_TextFiles_for_Quest3Start();
	Read_TextFiles_for_Quest3Clear();
	Read_TextFiles_for_Stage1Last();
	Read_TextFiles_for_Quest4Start();
	Read_TextFiles_for_Minigame1start();
	Read_TextFiles_for_Minigame1end();
	Read_TextFiles_for_Minigame2start();
	Read_TextFiles_for_Minigame2end();
	Read_TextFiles_for_LawBossBattleStart();
	Read_TextFiles_for_LawBossBattleEvent();
	Read_TextFiles_for_LawBossBattleEnd();
	Read_TextFiles_for_LastQuestStart();


	 Read_TextFiles_for_Meetlastboss();
	 Read_TextFiles_for_Secondbosson();
	 Read_TextFiles_for_Lastoverlimit();

	 Read_TextFiles_for_lastbossclear();

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Dialogue::Tick(_float fTimeDelta)
{
	if (m_bSexyEventEnd)
	{
		m_fSexyOffTimer += fTimeDelta;
		if (m_fSexyOffTimer > 1.5f)
		{
			m_bSexyEventEnd = false;
			m_fSexyOffTimer = 0.f;
			CPlayerManager::Get_Instance()->Set_Changetoboss(true);
			dynamic_cast<CMonsterLaw*>(CBattleManager::Get_Instance()->Get_BossMonster())->Set_EventFinish(true);
			CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Rinwell"));
			CBaseObj* pObject = dynamic_cast<CBaseObj*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_LAWBATTLE, TEXT("Layer_Rinwell")));
			dynamic_cast<CMonster*>(pObject)->Change_Navigation(LEVEL_LAWBATTLE);
			dynamic_cast<CMonster*>(pObject)->Compute_CurrentIndex(LEVEL_LAWBATTLE);
			dynamic_cast<CMonster*>(pObject)->Set_BattleMode(true);

			dynamic_cast<CAiRinwell*>(pObject)->Set_AirMode(true);
			dynamic_cast<CAiRinwell*>(pObject)->Set_50DownCutScene(true);

			CBattleManager::Get_Instance()->Add_BattleMonster(pObject);

			dynamic_cast<CUI_Dialoguepopup*>(CUI_Manager::Get_Instance()->Get_Dialoguepopup())->Open_Dialogue(8, true, 0, 1);
			CBattleManager::Get_Instance()->Set_IsStrike(false);

			CGameInstance::Get_Instance()->StopAll();
			CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_LawBattle.wav"), g_fSoundVolume);
			CBattleManager::Get_Instance()->Set_Rinwellboss(true);

		}

	}

	
	if (CGameInstance::Get_Instance()->Key_Up(DIK_PGUP))
	{
		dynamic_cast<CUI_Dialoguepopup*>(CUI_Manager::Get_Instance()->Get_Dialoguepopup())->Open_Dialogue(2, true, 1, 0);
	}



		
		

	//m_vCurrentDialouge = m_vDialogue;
	if (m_btick)
	{
		

		m_fbrightpos += 0.015f;

		if (m_fbrightpos > 2.5f)
			m_fbrightpos = 0.f;


		//std::mbstowcs;
		if (m_bpingdown)
		{
			m_fCursorpingpong += 0.25f;

			if (m_fCursorpingpong >= 5.f)
			{
				m_fCursorpingpong = 5.f;
				m_bpingdown = false;
			}
		}
		else
		{
			m_fCursorpingpong -= 0.25f;
			if (m_fCursorpingpong <= 0.f)
			{
				m_fCursorpingpong = 0.f;
				m_bpingdown = true;;
			}
		}


		--m_fFlowCurrent;

		if (m_fFlowCurrent <= 0)
			m_fFlowCurrent = 100.f;

		if (m_bfadein)
		{
			m_fAlpha += 0.02f; //????
			m_fFade += 0.8f;
		}
		else if (m_bfadeout)
		{
			m_fAlpha -= 0.02f;
			m_fFade -= 0.8f;
		}



		//	m_fAlpha = 0.5f;

		//if (CGameInstance::Get_Instance()->Key_Up(DIK_4)) // ???????
		//{
		//	m_bfadeout = true;
		//}
		//if (CGameInstance::Get_Instance()->Key_Up(DIK_5)) // ????????
		//{
		//	m_bfadein = true;
		//}
		if (CGameInstance::Get_Instance()->Key_Up(DIK_6)) // ????????
		{
			if (m_iVectorIndex == 13 && m_iDialogueindex == 1)
			{
				CPlayerManager::Get_Instance()->Get_EnumPlayer(2)->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(50.f, 0.f, 50.f, 1.f));
				CPlayerManager::Get_Instance()->Get_EnumPlayer(2)->Get_Transform()->LookDir(XMVectorSet(0.f, 0.f, 1.f, 0.f));
				CCameraManager* pCameraManager = CCameraManager::Get_Instance();
				pCameraManager->Set_CamState(CCameraManager::CAM_ACTION);
				pCameraManager->Play_ActionCamera(TEXT("SexyLaw2.dat"), XMMatrixIdentity());
				CPlayerManager::Get_Instance()->Get_EnumPlayer(2)->Set_IsActionMode(true);
				CPlayerManager::Get_Instance()->Get_EnumPlayer(2)->AI_RINWELL_Event();
			}
		
			++m_iDialogueindex;

			if (m_iDialogueindex == vectorsize)
			{
				m_bfadeout = true;
				CCameraManager* pCameraManager = CCameraManager::Get_Instance();
				if (pCameraManager->Get_CamState() == CCameraManager::CAM_ACTION)
				{
					CPlayerManager::Get_Instance()->Get_ActivePlayer()->Set_IsActionMode(false);
					CCamera* pCamera = pCameraManager->Get_CurrentCamera();
					dynamic_cast<CCamera_Action*>(pCamera)->Set_Play(false);
					CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_DYNAMIC);
				}
				
				--m_iDialogueindex;
				CUI_QuestClear::QUESTCLEARDESC cleardesc;
				ZeroMemory(&cleardesc, sizeof(CUI_QuestClear::QUESTCLEARDESC));
				switch (m_iVectorIndex)
				{
				case 0:
					CUI_Manager::Get_Instance()->Set_QuestIndex(1);
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTSTART"), LEVEL_SNOWFIELD, (TEXT("ssssss")))))
						return OBJ_NOEVENT;
					
					//CGame
					break;

				case 1:
					
					cleardesc.eName1 = ITEMNAME_LEMONJELLY;
					cleardesc.eType1 = ITEMTYPE_JELLY;
					cleardesc.iGaingald = 700;
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTCLEAR"), LEVEL_SNOWFIELD, (TEXT("ssss")), &cleardesc)))
						return OBJ_NOEVENT;
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY,false,false);
					CUI_Manager::Get_Instance()->Set_QuestComplete(0, true);
					break;

				case 2:
					CUI_Manager::Get_Instance()->Set_QuestIndex(2);
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTSTART"), LEVEL_SNOWFIELD, (TEXT("ssssss")))))
						return OBJ_NOEVENT;

					break;

				case 3:

					cleardesc.eName1 = ITEMNAME_PYOGOMUSHROOM;
					cleardesc.eType1 = ITEMTYPE_MATERIAL;
					cleardesc.iGaingald = 100;
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTCLEAR"), LEVEL_SNOWFIELD, (TEXT("ssss")), &cleardesc)))
						return OBJ_NOEVENT;
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
					CUI_Manager::Get_Instance()->Set_QuestComplete(1, true);
					break;

				case 4:

					CUI_Manager::Get_Instance()->Set_QuestIndex(3);
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTSTART"), LEVEL_SNOWFIELD, (TEXT("ssssss")))))
						return OBJ_NOEVENT;

					break;

				case 5:
				{
					CUI_Manager::Get_Instance()->Set_QuestIndex(3);
					CUI_QuestClear::QUESTCLEARDESC garr;
					ZeroMemory(&garr, sizeof(CUI_QuestClear::QUESTCLEARDESC));
					garr.eName1 = ITEMNAME_REDSAGE;
					garr.eType1 = ITEMTYPE_LEAVES;
					garr.iGaingald = 500;
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTCLEAR"), LEVEL_SNOWFIELD, (TEXT("ssss")), &garr)))
						return OBJ_NOEVENT;
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
					CUI_Manager::Get_Instance()->Set_QuestComplete(2, true);

					break;
				}
					

				case 6:
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_SystemMessagebox"), LEVEL_SNOWFIELD, (TEXT("ssss")))))
						return OBJ_NOEVENT;

					break;

				case 7:
					CUI_Manager::Get_Instance()->Set_QuestIndex(4);
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTSTART"), LEVEL_SNOWFIELD, (TEXT("ssssss")))))
						return OBJ_NOEVENT;
					

					break;

				case 8:
					dynamic_cast<CLevel_Restaurant*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Set_MiniGameStart(true);
					CUI_Manager::Get_Instance()->Set_Dialogue_section(9);
					break;

				case 9:
					CUI_Manager::Get_Instance()->Set_Dialogue_section(10);
					break;
				case 10:
					dynamic_cast<CLevel_WorkTool*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Set_MiniGameStart(true);
					break;

				case 11:
					CUI_Manager::Get_Instance()->Set_Dialogue_section(12);
					break;

				case 12:
				{
					CLevel* pCurrentLevel = CGameInstance::Get_Instance()->Get_CurrentLevel();
					pCurrentLevel->Set_NextLevel(true, LEVEL_LAWBATTLE);
					break;
				}
					

				case 13:
				{
					m_bSexyEventEnd = true;
					break;
				}
					
				case 14:
				{
					CUI_Manager::Get_Instance()->Set_QuestIndex(4);
					CUI_QuestClear::QUESTCLEARDESC garrr;
					ZeroMemory(&garrr, sizeof(CUI_QuestClear::QUESTCLEARDESC));
					garrr.eName1 = ITEMNAME_REDSAGE;
					garrr.eType1 = ITEMTYPE_LEAVES;
					garrr.iGaingald = 500;
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTCLEAR"), LEVEL_SNOWFIELD, (TEXT("ssss")), &garrr)))
						return OBJ_NOEVENT;
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, false, false);
					CUI_Manager::Get_Instance()->Set_QuestComplete(3, true);
					break;
				}
					

				case 15:
					CUI_Manager::Get_Instance()->Set_QuestIndex(5);
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTSTART"), LEVEL_SNOWFIELD, (TEXT("ses")))))
						return OBJ_NOEVENT;

				}

				CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_End.wav"), SOUND_UI, 0.4f);
			}
			else
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_Next.wav"), SOUND_UI, 0.4f);
		}
	}


	
	//if (CGameInstance::Get_Instance()->Key_Up(DIK_9)) // ????????
	//{
	//	++m_iVectorIndex;
	//}



	
	return OBJ_NOEVENT;
}

void CUI_Dialogue::Late_Tick(_float fTimeDelta)
{
	int iii = sizeof(m_vCurrentDialogue[0][0]);
	iii = sizeof(m_vCurrentDialogue[0][1]);
	 iii = sizeof(m_vCurrentDialogue[0][0][1]);
	if (m_btick)
	{
		if (m_fAlpha >= 0.5f && m_bfadein)
		{
			m_fAlpha = 0.5f;
			m_bfadein = false;
		}

		if (m_fAlpha <= 0.f && m_bfadeout)
		{
			m_fAlpha = 0.f;
			m_bfadeout = false;
			m_btick = false;
			m_iDialogueindex = 0;
		}

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_FRONT, this);
	}
	
	

}

HRESULT CUI_Dialogue::Render()
{

	_float alpha = m_fAlpha * 2.f;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	
	

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	



	if(FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;


	m_fSize.x = 900.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 650.f;
	m_fPosition.y = 620.f + m_fFade;;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	

	m_pShaderCom->Begin(UI_DIALOGUEBOX);

	m_pVIBufferCom->Render();

	m_fSize.x = 400.f;
	m_fSize.y = 40.f;
	m_fPosition.x = 430.f;
	m_fPosition.y = 545.f + m_fFade;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_DIALOGUEBOX);

	m_pVIBufferCom->Render();

	//m_fSize.x = 44.f;
	//m_fSize.y = 28.f;
	//m_fPosition.x = 430.f;
	//m_fPosition.y = 545.f + m_fFade;
	//_float alpha = m_fAlpha * 2.f;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
	//	return E_FAIL;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
	//	return E_FAIL;
	///*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
	//return E_FAIL;*/
	//m_pShaderCom->Begin(UI_BRIGHT);
	//m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	m_fSize.x = 400.f;
	m_fSize.y = 6.f;
	m_fPosition.x = 450.f;
	m_fPosition.y = 560.f + m_fFade;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_DIALOGUELINE);

	m_pVIBufferCom->Render();


	
	m_fSize.x = 32.f;
	m_fSize.y = 16.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 670.f + m_fFade + m_fCursorpingpong;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(2))))
		return E_FAIL;

	/*m_fFlowMAX = 1
		m_fFlowCurrent*/

	m_pShaderCom->Begin(UI_CURSORNOTMOVE);

	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_CURSOR);

	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_fFontPos.x = 465.f;
	//m_fFontPos.y = 575.f + m_fFade;
	
	Render_Fonts(m_iDialogueindex);



	return S_OK;
}

HRESULT CUI_Dialogue::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dialoguebox"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_dialogueportrait"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	

	

	return S_OK;
}

HRESULT CUI_Dialogue::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fFlowCurrent, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fFlowMAX, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

void CUI_Dialogue::Read_TextFiles_for_dialogue()
{
	std::ifstream file("../../../Bin/test.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
		//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}
	

	std::ifstream file1("../../../Bin/test1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
		//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/test2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/test3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file4("../../../Bin/test4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file5("../../../Bin/test5.txt");
	if (file5.is_open())
	{
		while (file5.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[5].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file5.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file6("../../../Bin/test6.txt");
	if (file6.is_open())
	{
		while (file6.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[6].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file6.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file7("../../../Bin/test7.txt");
	if (file7.is_open())
	{
		while (file7.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue[7].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file7.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}






	
	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue[0]);
	matrix.push_back(m_vDialogue[1]);
	matrix.push_back(m_vDialogue[2]);
	matrix.push_back(m_vDialogue[3]);
	matrix.push_back(m_vDialogue[4]);
	matrix.push_back(m_vDialogue[5]);
	matrix.push_back(m_vDialogue[6]);
	matrix.push_back(m_vDialogue[7]);

	m_vCurrentDialogue.push_back(matrix);


}

void CUI_Dialogue::Read_TextFiles_for_Quest1Clear()
{
	std::ifstream file("../../../Bin/quest1clear0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest1clear1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/quest1clear2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/quest1clear3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file4("../../../Bin/quest1clear4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file5("../../../Bin/quest1clear5.txt");
	if (file5.is_open())
	{
		while (file5.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge1[5].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file5.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	







	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge1[0]);
	matrix.push_back(m_vDialouge1[1]);
	matrix.push_back(m_vDialouge1[2]);
	matrix.push_back(m_vDialouge1[3]);
	matrix.push_back(m_vDialouge1[4]);
	matrix.push_back(m_vDialouge1[5]);
	

	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Quest2Strat()
{
	std::ifstream file("../../../Bin/quest2start0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge2[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest2start1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge2[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/quest2start2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge2[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/quest2start3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge2[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}



	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge2[0]);
	matrix.push_back(m_vDialouge2[1]);
	matrix.push_back(m_vDialouge2[2]);
	matrix.push_back(m_vDialouge2[3]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Quest2Clear()
{
	std::ifstream file("../../../Bin/quest2clear0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge3[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest2clear1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge3[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	



	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge3[0]);
	matrix.push_back(m_vDialouge3[1]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Quest3Start()
{
	std::ifstream file("../../../Bin/quest3start0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge4[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest3start1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge4[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/quest3start2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge4[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/quest3start3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge4[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}



	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge4[0]);
	matrix.push_back(m_vDialouge4[1]);
	matrix.push_back(m_vDialouge4[2]);
	matrix.push_back(m_vDialouge4[3]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Quest3Clear()
{
	std::ifstream file("../../../Bin/quest3clear0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest3clear1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/quest3clear2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/quest3clear3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file4("../../../Bin/quest3clear4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file5("../../../Bin/quest3clear5.txt");
	if (file5.is_open())
	{
		while (file5.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[5].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file5.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file6("../../../Bin/quest3clear6.txt");
	if (file6.is_open())
	{
		while (file6.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge5[6].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file6.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}
	//m_vCurrentDialogue.








	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge5[0]);
	matrix.push_back(m_vDialouge5[1]);
	matrix.push_back(m_vDialouge5[2]);
	matrix.push_back(m_vDialouge5[3]);
	matrix.push_back(m_vDialouge5[4]);
	matrix.push_back(m_vDialouge5[5]); 
	matrix.push_back(m_vDialouge5[6]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Stage1Last()
{

	std::ifstream file("../../../Bin/stage1last0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge6[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/stage1last1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge6[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/stage1last2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge6[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/stage1last3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge6[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file4("../../../Bin/stage1last4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge6[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}




	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge6[0]);
	matrix.push_back(m_vDialouge6[1]);
	matrix.push_back(m_vDialouge6[2]);
	matrix.push_back(m_vDialouge6[3]);
	matrix.push_back(m_vDialouge6[4]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Quest4Start()
{
	std::ifstream file("../../../Bin/quest4start0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge7[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/quest4start1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge7[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/quest4start2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge7[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/quest4start3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge7[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file4("../../../Bin/quest4start4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialouge7[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	//std::ifstream file5("../../../Bin/quest4start5.txt");
	//if (file5.is_open())
	//{
	//	while (file5.getline(fuck, 256))
	//	{
	//		_tchar* pszDialog = new _tchar[MAX_PATH];
	//		m_vDialouge7[5].push_back(pszDialog);
	//		ConverCtoWC(fuck);
	//		memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
	//		//	Safe_Delete_Array(pszDialog);
	//	}
	//	file5.close();
	//}
	//else
	//{
	//	std::cout << "Unable to open file\n";
	//}









	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialouge7[0]);
	matrix.push_back(m_vDialouge7[1]);
	matrix.push_back(m_vDialouge7[2]);
	matrix.push_back(m_vDialouge7[3]);
	matrix.push_back(m_vDialouge7[4]);
	//matrix.push_back(m_vDialouge7[5]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Minigame1start()
{
	std::ifstream file("../../../Bin/minigamestart0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue8[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/minigamestart1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue8[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/minigamestart2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue8[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}




	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue8[0]);
	matrix.push_back(m_vDialogue8[1]);
	matrix.push_back(m_vDialogue8[2]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Minigame1end()
{
	std::ifstream file("../../../Bin/minigameend0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue9[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/minigameend1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue9[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/minigameend2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue9[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/minigameend3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue9[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file4("../../../Bin/minigameend4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue9[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file5("../../../Bin/minigameend5.txt");
	if (file5.is_open())
	{
		while (file5.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue9[5].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file5.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}









	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue9[0]);
	matrix.push_back(m_vDialogue9[1]);
	matrix.push_back(m_vDialogue9[2]);
	matrix.push_back(m_vDialogue9[3]);
	matrix.push_back(m_vDialogue9[4]);
	matrix.push_back(m_vDialogue9[5]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Minigame2start()
{
	std::ifstream file("../../../Bin/minigame1start0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue10[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/minigame1start1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue10[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/minigame1start2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue10[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/minigame1start3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue10[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}









	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue10[0]);
	matrix.push_back(m_vDialogue10[1]);
	matrix.push_back(m_vDialogue10[2]);
	matrix.push_back(m_vDialogue10[3]);



	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Minigame2end()
{
	std::ifstream file("../../../Bin/minigame1end0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue11[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/minigame1end1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue11[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/minigame1end2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue11[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/minigame1end3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue11[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file4("../../../Bin/minigame1end4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue11[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file5("../../../Bin/minigame1end5.txt");
	if (file5.is_open())
	{
		while (file5.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue11[5].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file5.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}









	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue11[0]);
	matrix.push_back(m_vDialogue11[1]);
	matrix.push_back(m_vDialogue11[2]);
	matrix.push_back(m_vDialogue11[3]);
	matrix.push_back(m_vDialogue11[4]);
	matrix.push_back(m_vDialogue11[5]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_LawBossBattleStart()
{
	std::ifstream file("../../../Bin/meetbosslaw0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue12[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/meetbosslaw1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue12[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/meetbosslaw2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue12[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/meetbosslaw3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue12[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file4("../../../Bin/meetbosslaw4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue12[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file5("../../../Bin/meetbosslaw5.txt");
	if (file5.is_open())
	{
		while (file5.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue12[5].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file5.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file6("../../../Bin/meetbosslaw6.txt");
	if (file6.is_open())
	{
		while (file6.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue12[6].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file6.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}






	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue12[0]);
	matrix.push_back(m_vDialogue12[1]);
	matrix.push_back(m_vDialogue12[2]);
	matrix.push_back(m_vDialogue12[3]);
	matrix.push_back(m_vDialogue12[4]);
	matrix.push_back(m_vDialogue12[5]);
	matrix.push_back(m_vDialogue12[6]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_LawBossBattleEvent()
{
	std::ifstream file("../../../Bin/LawSexy0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue13[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/LawSexy1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue13[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/LawSexy2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue13[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/LawSexy3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue13[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}









	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue13[0]);
	matrix.push_back(m_vDialogue13[1]);
	matrix.push_back(m_vDialogue13[2]);
	matrix.push_back(m_vDialogue13[3]);



	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_LawBossBattleEnd()
{
	std::ifstream file("../../../Bin/bosslawend0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue14[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/bosslawend1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue14[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/bosslawend2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue14[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/bosslawend3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue14[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}









	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue14[0]);
	matrix.push_back(m_vDialogue14[1]);
	matrix.push_back(m_vDialogue14[2]);
	matrix.push_back(m_vDialogue14[3]);



	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_LastQuestStart()
{
	std::ifstream file("../../../Bin/lastqueststart0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/lastqueststart1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/lastqueststart2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/lastqueststart3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file4("../../../Bin/lastqueststart4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file5("../../../Bin/lastqueststart5.txt");
	if (file5.is_open())
	{
		while (file5.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[5].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file5.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file6("../../../Bin/lastqueststart6.txt");
	if (file6.is_open())
	{
		while (file6.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[6].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file6.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}
	std::ifstream file7("../../../Bin/lastqueststart7.txt");
	if (file7.is_open())
	{
		while (file7.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[7].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file7.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file8("../../../Bin/lastqueststart8.txt");
	if (file8.is_open())
	{
		while (file8.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[8].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file8.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file9("../../../Bin/lastqueststart9.txt");
	if (file9.is_open())
	{
		while (file9.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[9].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file9.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file10("../../../Bin/lastqueststart10.txt");
	if (file10.is_open())
	{
		while (file10.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[10].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file10.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file11("../../../Bin/lastqueststart11.txt");
	if (file11.is_open())
	{
		while (file11.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[11].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file11.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file12("../../../Bin/lastqueststart12.txt");
	if (file12.is_open())
	{
		while (file12.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[12].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file12.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file13("../../../Bin/lastqueststart13.txt");
	if (file13.is_open())
	{
		while (file13.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[13].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file13.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file14("../../../Bin/lastqueststart14.txt");
	if (file14.is_open())
	{
		while (file14.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[14].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file14.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file15("../../../Bin/lastqueststart15.txt");
	if (file15.is_open())
	{
		while (file15.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue15[15].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file15.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}






	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue15[0]);
	matrix.push_back(m_vDialogue15[1]);
	matrix.push_back(m_vDialogue15[2]);
	matrix.push_back(m_vDialogue15[3]);
	matrix.push_back(m_vDialogue15[4]);
	matrix.push_back(m_vDialogue15[5]);
	matrix.push_back(m_vDialogue15[6]);
	matrix.push_back(m_vDialogue15[7]);
	matrix.push_back(m_vDialogue15[8]);
	matrix.push_back(m_vDialogue15[9]);
	matrix.push_back(m_vDialogue15[10]);
	matrix.push_back(m_vDialogue15[11]);
	matrix.push_back(m_vDialogue15[12]);
	matrix.push_back(m_vDialogue15[13]);
	matrix.push_back(m_vDialogue15[14]);
	matrix.push_back(m_vDialogue15[15]);


	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Meetlastboss()
{
	std::ifstream file("../../../Bin/meetlastboss0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue16[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/meetlastboss1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue16[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/meetlastboss2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue16[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/meetlastboss3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue16[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}









	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue16[0]);
	matrix.push_back(m_vDialogue16[1]);
	matrix.push_back(m_vDialogue16[2]);
	matrix.push_back(m_vDialogue16[3]);



	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Secondbosson()
{
	std::ifstream file("../../../Bin/secondbosson0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue17[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/secondbosson1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue17[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	

	









	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue17[0]);
	matrix.push_back(m_vDialogue17[1]);




	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_Lastoverlimit()
{
	std::ifstream file("../../../Bin/lastoverlim0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue18[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/lastoverlim1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue18[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}













	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue18[0]);
	matrix.push_back(m_vDialogue18[1]);




	m_vCurrentDialogue.push_back(matrix);
}

void CUI_Dialogue::Read_TextFiles_for_lastbossclear()
{
	std::ifstream file("../../../Bin/lastbossend0.txt");
	if (file.is_open())
	{
		while (file.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue19[0].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file1("../../../Bin/lastbossend1.txt");
	if (file1.is_open())
	{
		while (file1.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue19[1].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file1.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	//m_vCurrentDialogue.
	std::ifstream file2("../../../Bin/lastbossend2.txt");
	if (file2.is_open())
	{
		while (file2.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue19[2].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file2.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}

	std::ifstream file3("../../../Bin/lastbossend3.txt");
	if (file3.is_open())
	{
		while (file3.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue19[3].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file3.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}


	std::ifstream file4("../../../Bin/lastbossend4.txt");
	if (file4.is_open())
	{
		while (file4.getline(fuck, 256))
		{
			_tchar* pszDialog = new _tchar[MAX_PATH];
			m_vDialogue19[4].push_back(pszDialog);
			ConverCtoWC(fuck);
			memcpy(pszDialog, m_szTXT, sizeof(_tchar)*MAX_PATH);
			//	Safe_Delete_Array(pszDialog);
		}
		file4.close();
	}
	else
	{
		std::cout << "Unable to open file\n";
	}






	vector<vector<_tchar*>> matrix;
	matrix.push_back(m_vDialogue19[0]);
	matrix.push_back(m_vDialogue19[1]);
	matrix.push_back(m_vDialogue19[2]);
	matrix.push_back(m_vDialogue19[3]);
	matrix.push_back(m_vDialogue19[4]);


	m_vCurrentDialogue.push_back(matrix);
}

wchar_t * CUI_Dialogue::ConverCtoWC(char * str)
{

	//wchar_t?? ???? ????
	wchar_t* pStr;
	//??? ????? ??? ??? ???? ???
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
	//wchar_t ??? ???
	pStr = new WCHAR[MAX_PATH];
	//?? ???
	MultiByteToWideChar(CP_ACP, 0, str, _uint(strlen(str) + 1), m_szTXT, MAX_PATH);
	
	Safe_Delete_Array(pStr);
	return pStr;

}

void CUI_Dialogue::Render_Fonts(_uint index)
{
	/*_uint test = 0;
	_uint test2 = sizeof(m_vDialogue);
	for (auto vec : m_vDialogue) test++;*/
	m_fFontsize = 0.75f;
	m_fFontOffsetY = 30.f;
	switch (m_vCurrentDialogue[m_iVectorIndex][index].size()-1)
	{
	case 0:
		return;
		break;
	case 1:
		m_fFontPos.y = 595.f + m_fFade;
		break;
	case 2:
		m_fFontPos.y = 580.f + m_fFade; 
		break;
	case 3:
		m_fFontPos.y = 570.f + m_fFade;
		break;

	}

	for (_uint i = 0; i < m_vCurrentDialogue[m_iVectorIndex][index].size()-1; ++i)
	{
		CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vCurrentDialogue[m_iVectorIndex][index][i], XMVectorSet(m_fFontPos.x, m_fFontPos.y + (m_fFontOffsetY * (_float)i), 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);
	}

	
	CGameInstance::Get_Instance()->Render_Font(TEXT("Font_Nexon"), m_vCurrentDialogue[m_iVectorIndex][index][m_vCurrentDialogue[m_iVectorIndex][index].size()-1], XMVectorSet(390.f, 535.f+m_fFade, 0.f, 1.f), XMVectorSet(m_FontR*(m_fAlpha*2.f), m_FontG*(m_fAlpha*2.f), m_FontB*(m_fAlpha*2.f), m_fAlpha * 2.f), m_fFontsize);
	
	if (!_tcscmp(m_vCurrentDialogue[m_iVectorIndex][index][m_vCurrentDialogue[m_iVectorIndex][index].size() - 1], TEXT("¾ËÆæ")))
	{
		m_iPortraitnum = 0;
	}
	else if (!_tcscmp(m_vCurrentDialogue[m_iVectorIndex][index][m_vCurrentDialogue[m_iVectorIndex][index].size() - 1], TEXT("½Ã¿Â")))
	{
		m_iPortraitnum = 1;
	}
	else if (!_tcscmp(m_vCurrentDialogue[m_iVectorIndex][index][m_vCurrentDialogue[m_iVectorIndex][index].size() - 1], TEXT("¸°À£")))
	{
		m_iPortraitnum = 2;
	}
	else if (!_tcscmp(m_vCurrentDialogue[m_iVectorIndex][index][m_vCurrentDialogue[m_iVectorIndex][index].size() - 1], TEXT("·Î¿ì")))
	{
		m_iPortraitnum = 3;
	}
	else if (!_tcscmp(m_vCurrentDialogue[m_iVectorIndex][index][m_vCurrentDialogue[m_iVectorIndex][index].size() - 1], TEXT("Å°»ç¶ó")))
	{
		m_iPortraitnum = 4;
	}
	else if (!_tcscmp(m_vCurrentDialogue[m_iVectorIndex][index][m_vCurrentDialogue[m_iVectorIndex][index].size() - 1], TEXT("µà¿ÀÇÒ¶÷")))
	{
		m_iPortraitnum = 5;
	}
	else
		m_iPortraitnum = 100;

	if (m_iDialogueindex == 14)
		m_iPortraitnum = 5;
	else if (m_iDialogueindex == 15)
		m_iPortraitnum = 4;

	if(m_iPortraitnum <= 5)
	{

		m_fSize.x = 600.f;
		m_fSize.y = 540.f;
		m_fPosition.x = 190.f;
		m_fPosition.y = 600.f + m_fFade;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		_float alpha = m_fAlpha * 2.f;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
			return;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_iPortraitnum))))
			return;
	
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return;
		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();
	}

	




}

void CUI_Dialogue::Open_Dialogue(_uint index)
{
	m_iVectorIndex = index; m_btick = true;

	m_bfadein = true;
	_uint test = 0;

	for (auto vec : (m_vCurrentDialogue[index])) test++;
	
	vectorsize = test;
	CGameInstance::Get_Instance()->PlaySounds(TEXT("Open_Dialogue.wav"), SOUND_EFFECT, 0.3f);
}



CUI_Dialogue * CUI_Dialogue::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Dialogue*	pInstance = new CUI_Dialogue(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Dialogue"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_Dialogue::Clone(void * pArg)
{
	CUI_Dialogue*	pInstance = new CUI_Dialogue(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Dialogue"));
		Safe_Release(pInstance);
	}

	CUI_Manager::Get_Instance()->Set_Dialogue(pInstance);

	return pInstance;
}

void CUI_Dialogue::Free()
{

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	//for (int j = 0; j < m_vDialogue[j].size(); ++j)
	//{
	//	for (int i = 0; i<m_vDialogue[i].size(); ++i)
	//	{
	//		//	Safe_Delete(m_vDialogue[i]);


	//		_tchar* temp = m_vDialogue[j][i];
	//		delete[] temp;

	//	}
	//}

	//
	//for (int i = 0; i < m_vDialogue[1].size(); ++i)
	//{
	////	Safe_Delete(m_vDialogue[i]);
	//	
	//	_tchar* temp = m_vDialogue[1][i];
	//	delete[] temp;
	//}
	////delete[] pszDialog;
	//_uint size = m_vCurrentDialogue[0].size();
	//size = m_vCurrentDialogue[0][0].size();
	//size = m_vCurrentDialogue[0][1].size();
	//size = m_vCurrentDialogue[1][0].size();
	//size = m_vCurrentDialogue[1][1].size();
	////size = m_vCurrentDialogue[1][1].size();

	for (int i = 0; i < m_vCurrentDialogue.size(); ++i)
	{
		for (int j = 0; j <m_vCurrentDialogue[i].size(); ++j)
		{
			/*if (j == 2)
				continue;*/

				vector<_tchar*> temp = m_vCurrentDialogue[i][j];
				for (auto& iter : temp)
				{
					delete[] iter;
				}
				temp.clear();
		}
			

	}
	//	m_vCurrentDialogue[i][j][k]




	__super::Free();


	}
	
//}


