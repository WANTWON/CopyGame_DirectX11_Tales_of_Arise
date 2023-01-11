#include "stdafx.h"
#include "..\Public\PrizeItem.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_Manager.h"
#include "InvenItem.h"
#include "MessageBox.h"
#include "UIButton.h"
#include "CameraManager.h"
#include "Portal.h"
#include "ObjectEffect.h"
#include "InvenTile.h"

CPrizeItem::CPrizeItem(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNonAnim(pDevice, pContext)
{
}

CPrizeItem::CPrizeItem(const CPrizeItem & rhs)
	: CNonAnim(rhs)
{
}


HRESULT CPrizeItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPrizeItem::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_ItemDesc, pArg, sizeof(ITEMDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	//CPickingMgr::Get_Instance()->Add_PickingGroup(this);

	m_eObjectID = OBJ_KEY;

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&m_ItemDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

		if (m_ItemDesc.eInteractType == CARRYABLE)
			m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	}

	Set_Scale(_float3(3.f, 3.f, 3.f));
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_ITEM, this);

	

	return S_OK;
}

int CPrizeItem::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_ITEM, this);
		return OBJ_DEAD;
	}
	

	if ((m_ItemDesc.eType == TAIL_KEY  || m_ItemDesc.eType == RUBY_GREEN ||
		m_ItemDesc.eType == HEART_RECOVERY )&& !m_bGet)
	{
		
		_float fMinPos = m_ItemDesc.vPosition.y + 0.5f;
		_float fMaxPos = m_ItemDesc.vPosition.y + 1.f;
		
		if (!m_bTimeMax )
		{
			m_fMoveTime += 0.01f;
			if (m_fMoveTime >= 1.f)
				m_bTimeMax = true;
		}
		else
		{
			m_fMoveTime -= 0.01f;
			if (m_fMoveTime <= 0.f)
				m_bTimeMax = false;
		}

		m_fHeight = (1 - m_fMoveTime)*fMinPos + fMaxPos*m_fMoveTime;
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPosition = XMVectorSetY(vPosition, m_fHeight);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	}

	return OBJ_NOEVENT;
}

void CPrizeItem::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);

#ifdef _DEBUG
		if (m_pAABBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pAABBCom);
		if (m_pOBBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pOBBCom);
		if (m_pSPHERECom != nullptr)
			m_pRendererCom->Add_Debug(m_pSPHERECom);
#endif
	}
		

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	if (m_ItemDesc.eInteractType == PRIZE)
		LateTick_PrizeModeItem(fTimeDelta);
	else if (m_ItemDesc.eInteractType == DEFAULT)
		LateTick_DefaultModeItem(fTimeDelta);
	else if (m_ItemDesc.eInteractType == CARRYABLE)
		LateTick_CarryableModeItem(fTimeDelta);
	else if (m_ItemDesc.eInteractType == TELL)
		LateTick_TelephoneModeItem(fTimeDelta);
	
}

HRESULT CPrizeItem::Render()
{
	__super::Render();


	return S_OK;

}

void CPrizeItem::LateTick_PrizeModeItem(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);


	CBaseObj* pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	if (m_ItemDesc.eInteractType == PRIZE && m_pSPHERECom->Collision(pTarget->Get_Collider()))
	{
		SetUp_BillBoard();

		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_ITEM, this);

		if (m_ItemDesc.eInteractType == PRIZE && !m_bGet)
		{
			dynamic_cast<CPlayer*>(pTarget)->Set_AnimState(CPlayer::ITEM_GET_ST);
			CMessageBox::MSGDESC MessageDesc;
			MessageDesc.m_eMsgType = CMessageBox::GET_ITEM;
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MessageBox"), LEVEL_STATIC, TEXT("Layer_UI"), &MessageDesc);

			CUI_Manager::MSGDESC MsgDesc;
			MsgDesc.eMsgType = CUI_Manager::PASSABLE;
			MsgDesc.iTextureNum = m_ItemDesc.eType;
	
			pUI_Manager->Add_MessageDesc(MsgDesc);
			pUI_Manager->Set_Talking(true);
			pUI_Manager->Open_Message(true);
			Make_GetItemEffect();

			if(m_ItemDesc.eType == DRUM || m_ItemDesc.eType ==  HORN || m_ItemDesc.eType== BELL || m_ItemDesc.eType == CELLO)
				pGameInstance->PlaySounds(TEXT("4_Event_Siren's Instrument Fanfare.mp3"), SOUND_SYSTEM, 0.4f);
			else if (m_ItemDesc.eType == FEATHER || m_ItemDesc.eType == BOSS_KEY || m_ItemDesc.eType == TAIL_KEY)
				pGameInstance->PlaySounds(TEXT("4_Event_Get Important Item Fanfare.mp3"), SOUND_SYSTEM, 0.4f);
			else
				pGameInstance->PlaySounds(TEXT("4_Event_Get Item Fanfare.mp3"), SOUND_SYSTEM, 0.4f);


		}

		m_bGet = true;
	}


	if (m_bGet)
	{
		//m_eShaderID = SHADER_GLOWSHADER;
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		_vector pPlayerPostion = pPlayer->Get_TransformState(CTransform::STATE_POSITION);

		pPlayerPostion = XMVectorSetY(pPlayerPostion, XMVectorGetY(pPlayerPostion) + 3.f);
		m_pTransformCom->Go_PosTarget(fTimeDelta, pPlayerPostion);

		if (pPlayer->Get_AnimState() == CPlayer::ITEM_GET_ED || pPlayer->Get_AnimState() == CPlayer::S_ITEM_GET_ED)
		{
			pUI_Manager->Open_Message(false);
			Setting_Get_Item();
			m_bDead = true;

			list<CGameObject*>* pEffectList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_ObjectEffect"));
			if (pEffectList != nullptr)
			{
				for (auto& iter : *pEffectList)
				{
					if(iter != nullptr)
						dynamic_cast<CObjectEffect*>(iter)->Set_EndMode(true);
				}
					
			}

			if (m_ItemDesc.eType == TAIL_KEY)
			{
				CPortal::PORTALDESC PortalDesc;
				PortalDesc.ePortalType = CPortal::PORTAL_LEVEL;
				pPlayer->Set_2DMode(false);
				XMStoreFloat3(&PortalDesc.vInitPos , pPlayer->Get_TransformState(CTransform::STATE_POSITION));
				PortalDesc.eConnectLevel = LEVEL_GAMEPLAY;
				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_TOWER, TEXT("Layer_Portal"), &PortalDesc)))
					return;
			}
			else if (m_ItemDesc.eType == CELLO)
			{
				CPortal::PORTALDESC PortalDesc;
				PortalDesc.ePortalType = CPortal::PORTAL_LEVEL;
				pPlayer->Set_2DMode(false);
				XMStoreFloat3(&PortalDesc.vInitPos, pPlayer->Get_TransformState(CTransform::STATE_POSITION));
				PortalDesc.eConnectLevel = LEVEL_END;
				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_TAILCAVE, TEXT("Layer_Portal"), &PortalDesc)))
					return;
			}
		}

	}

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

void CPrizeItem::LateTick_DefaultModeItem(_float fTimeDelta)
{
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom))
		m_bGet = true;


	if (m_bGet)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		if (m_ItemDesc.eType == RUBY_GREEN)
			pPlayer->Set_RubyAdd();
		else if (m_ItemDesc.eType == HEART_RECOVERY)
			pPlayer->Set_RecoverHp();
		m_bDead = true;
	}
}

void CPrizeItem::LateTick_CarryableModeItem(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	CUIButton*		pButton = dynamic_cast<CUIButton*>(CUI_Manager::Get_Instance()->Get_Button());


	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom))
	{
		if (!m_bGet)
		{
			pButton->Set_TexType(CUIButton::CARRY);
			pButton->Set_Visible(true);
			_float2 fPosition = pPlayer->Get_ProjPosition();
			fPosition.y = g_iWinSizeY - fPosition.y;
			fPosition.x += 50.f;
			fPosition.y -= 30.f;
			pButton->Set_Position(fPosition);
		}
		
		if (CGameInstance::Get_Instance()->Key_Up(DIK_A))
		{
			if (!m_bGet)
			{
				m_bGet = true;
				pPlayer->Set_AnimState(CPlayer::ITEM_CARRY);
				pPlayer->Ready_Parts(m_ItemDesc.eType, CPlayer::PARTS_ITEM);
				m_bDead = true;
				pButton->Set_Visible(false);
			}
		}	
	}		
}

void CPrizeItem::LateTick_TelephoneModeItem(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	CUIButton*		pButton = dynamic_cast<CUIButton*>(CUI_Manager::Get_Instance()->Get_Button());


	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom))
	{
		if (!m_bGet)
		{
			pButton->Set_TexType(CUIButton::CARRY);
			pButton->Set_Visible(true);
			_float2 fPosition = pPlayer->Get_ProjPosition();
			fPosition.y = g_iWinSizeY - fPosition.y;
			fPosition.x += 50.f;
			fPosition.y -= 30.f;
			pButton->Set_Position(fPosition);
		}

		if (CGameInstance::Get_Instance()->Key_Up(DIK_A))
		{
			if (!m_bGet)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Button_On.wav"), SOUND_SYSTEM, g_fUIVolume);
				m_bGet = true;
			
				pPlayer->Set_AnimState(CPlayer::EV_TELL_ST);
				pPlayer->Ready_Parts(m_ItemDesc.eType, CPlayer::PARTS_ITEM);
				m_bDead = true;
				pButton->Set_Visible(false);
				Setting_TelephoneMessage();

			}
		}

	}
	else
	{
		pButton->Set_Visible(false);
	}
}

void CPrizeItem::Setting_Get_Item()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (m_ItemDesc.eType == SMALL_KEY)
		pUI_Manager->Get_Key();

	else if (m_ItemDesc.eType == COMPASS)
	{
		CInvenItem::ITEMDESC ItemDesc;
		ItemDesc.eItemType = CInvenItem::ITEM_PRIZE;
		ItemDesc.m_iTextureNum = CInvenItem::COMPASS;
		ItemDesc.vPosition = _float2(1400, 200);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CInvenItem"), LEVEL_STATIC, TEXT("Layer_Compass"), &ItemDesc)))
			int a = 0;	
	}
	else if (m_ItemDesc.eType == TAIL_KEY)
	{
		list<CGameObject*>* pQuestItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_DungeonKey"));
		for (auto& iter : *pQuestItemList)
		{
			if (dynamic_cast<CInvenItem*>(iter)->Get_TextureNum() == CInvenItem::DGN_NONE)
			{
				dynamic_cast<CInvenItem*>(iter)->Set_TextureNum(CInvenItem::DGN_TAILKEY);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == CELLO)
	{
		list<CGameObject*>* pQuestItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Collect"));
		for (auto& iter : *pQuestItemList)
		{
			if (dynamic_cast<CInvenItem*>(iter)->Get_TextureNum() == CInvenItem::COLLECT_NONE)
			{
				dynamic_cast<CInvenItem*>(iter)->Set_TextureNum(CInvenItem::COLLECT_CELLO);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == DRUM)
	{
		list<CGameObject*>* pQuestItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Collect"));
		for (auto& iter : *pQuestItemList)
		{
			if (dynamic_cast<CInvenItem*>(iter)->Get_TextureNum() == CInvenItem::COLLECT_NONE)
			{
				dynamic_cast<CInvenItem*>(iter)->Set_TextureNum(CInvenItem::COLLECT_DRUM);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == HORN)
	{
		list<CGameObject*>* pQuestItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Collect"));
		for (auto& iter : *pQuestItemList)
		{
			if (dynamic_cast<CInvenItem*>(iter)->Get_TextureNum() == CInvenItem::COLLECT_NONE)
			{
				dynamic_cast<CInvenItem*>(iter)->Set_TextureNum(CInvenItem::COLLECT_HORN);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == BELL)
	{
		list<CGameObject*>* pQuestItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Collect"));
		for (auto& iter : *pQuestItemList)
		{
			if (dynamic_cast<CInvenItem*>(iter)->Get_TextureNum() == CInvenItem::COLLECT_NONE)
			{
				dynamic_cast<CInvenItem*>(iter)->Set_TextureNum(CInvenItem::COLLECT_BELL);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == YOSHIDOLL)
	{
		list<CGameObject*>* pQuestItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_QuestItem"));
		for (auto& iter : *pQuestItemList)
		{
			if (dynamic_cast<CInvenItem*>(iter)->Get_TextureNum() == CInvenItem::QUEST_NONE)
			{
				dynamic_cast<CInvenItem*>(iter)->Set_TextureNum(CInvenItem::YOSHI);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == FEATHER)
	{
		list<CGameObject*>* pItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Ineventile"));
		for (auto& iter : *pItemList)
		{
			if (dynamic_cast<CInvenTile*>(iter)->Get_TextureNum() == CInvenItem::ITEM_NONE)
			{
				dynamic_cast<CInvenTile*>(iter)->Set_TextureNum(CInvenItem::ITEM_FEATHER);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == NECKLACE)
	{
		list<CGameObject*>* pQuestItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_QuestItem"));
		for (auto& iter : *pQuestItemList)
		{
			if (dynamic_cast<CInvenItem*>(iter)->Get_TextureNum() == CInvenItem::QUEST_NONE)
			{
				dynamic_cast<CInvenItem*>(iter)->Set_TextureNum(CInvenItem::NECKLACE);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == MAGIC_ROD)
	{
		list<CGameObject*>* pItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Ineventile"));
		for (auto& iter : *pItemList)
		{
			if (dynamic_cast<CInvenTile*>(iter)->Get_TextureNum() == CInvenItem::ITEM_NONE)
			{
				dynamic_cast<CInvenTile*>(iter)->Set_TextureNum(CInvenItem::ITEM_WAND);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == DOGFOOD)
	{
		list<CGameObject*>* pQuestItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_QuestItem"));
		for (auto& iter : *pQuestItemList)
		{
			if (dynamic_cast<CInvenItem*>(iter)->Get_TextureNum() == CInvenItem::ITEM_NONE)
			{
				dynamic_cast<CInvenItem*>(iter)->Set_TextureNum(CInvenItem::DOG_FOOD);
				break;

			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == BOW)
	{
		list<CGameObject*>* pItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Ineventile"));
		for (auto& iter : *pItemList)
		{
			if (dynamic_cast<CInvenTile*>(iter)->Get_TextureNum() == CInvenItem::ITEM_NONE)
			{
				dynamic_cast<CInvenTile*>(iter)->Set_TextureNum(CInvenItem::ITEM_BOW);
				break;
			}
			else
				continue;
		}
	}
	else if (m_ItemDesc.eType == RUBYPURPLE)
	{
		CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
		pPlayer->Set_RubyAdd(100);
	}
	else if (m_ItemDesc.eType == RUBY_GREEN)
	{
		CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
		pPlayer->Set_RubyAdd(50);
	}
	else if (m_ItemDesc.eType == HEART_CONTAINER)
	{
		CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
		pPlayer->Set_MaxHp();
		pGameInstance->Clear_Layer(LEVEL_STATIC, TEXT("Layer_Hp"));
	}
	else if (m_ItemDesc.eType == MAP)
	{
		pUI_Manager->Set_Get_DgnMap(true);
	}

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

void CPrizeItem::Setting_TelephoneMessage()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	//CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
	//dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_TALK);

	CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
	pUI_Manager->Set_Talking(true);


	CMessageBox::MSGDESC MessageDesc;
	MessageDesc.m_eMsgType = CMessageBox::TELEPHONE_TALK;

	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MessageBox"), LEVEL_STATIC, TEXT("Layer_UI"), &MessageDesc);

	CUI_Manager::MSGDESC eMsgDesc;
	eMsgDesc.eMsgType = CUI_Manager::PASSABLE;

	if (pUI_Manager->Get_TellEnd())
	{
		eMsgDesc.iTextureNum = 16;
		pUI_Manager->Add_MessageDesc(eMsgDesc);
	}
	else if(pUI_Manager->Get_Is_HaveItem(LEVEL_STATIC, TEXT("Layer_DungeonKey"), CInvenItem::DGN_TAILKEY))
	{
		for (int i = 11; i < 16; ++i)
		{
			eMsgDesc.iTextureNum = i;
			pUI_Manager->Add_MessageDesc(eMsgDesc);
		}
		pUI_Manager->Set_TellEnd(true);
	}
	else if (pUI_Manager->Get_Is_HaveItem(LEVEL_STATIC, TEXT("Layer_Collect"), CInvenItem::COLLECT_DRUM) &&
		pUI_Manager->Get_Is_HaveItem(LEVEL_STATIC, TEXT("Layer_Collect"), CInvenItem::COLLECT_HORN) &&
		pUI_Manager->Get_Is_HaveItem(LEVEL_STATIC, TEXT("Layer_Collect"), CInvenItem::COLLECT_BELL))
	{
		for (int i = 6; i < 11; ++i)
		{
			eMsgDesc.iTextureNum = i;
			pUI_Manager->Add_MessageDesc(eMsgDesc);
		}
	}
	else
	{
		for (int i = 0; i < 6; ++i)
		{
			eMsgDesc.iTextureNum = i;
			pUI_Manager->Add_MessageDesc(eMsgDesc);
		}
	}

	pUI_Manager->Open_Message(true);

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

void CPrizeItem::Make_GetItemEffect()
{
	if (m_bMakeEffect)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect::EFFECTDESC EffectDesc;
	
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);// +XMVectorSet(0.f, -0.2f, 0.f, 0.f);
	EffectDesc.pTarget = this;

	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CObjectEffect::RAINBOW_RING;
	EffectDesc.fDeadTime = 2.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.015f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);

	EffectDesc.eEffectID = CObjectEffect::GRAD_RING;
	EffectDesc.fStartTime = 1.f;
	EffectDesc.fDeadTime = EffectDesc.fStartTime + 3.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.05f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.eEffectID = CObjectEffect::RAINBOW_HALO;
	EffectDesc.fStartTime = 0.f;
	EffectDesc.fDeadTime = 4.f;
	EffectDesc.vInitScale = _float3(10.f, 10.f, 10.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.04f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.eEffectID = CObjectEffect::HORIZONTAL_GLOW;
	EffectDesc.iTextureNum = 5;
	EffectDesc.fDeadTime = 2.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.03f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.eEffectID = CObjectEffect::ITEM_GET_GLOW;
	EffectDesc.iTextureNum = 0;
	EffectDesc.fStartTime = 1.f;
	EffectDesc.fDeadTime = EffectDesc.fStartTime + 3.5f;
	EffectDesc.vInitScale = _float3(1.f, 1.f, 1.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.02f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.fStartTime = 0.f;
	EffectDesc.iTextureNum = 0;
	EffectDesc.fDeadTime = 2.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.01f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.eEffectID = CObjectEffect::ITEM_GET_FLASH;
	EffectDesc.iTextureNum = 6;
	EffectDesc.fDeadTime = 1.25f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, 0.1f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.iTextureNum = 3;
	EffectDesc.fDeadTime = 1.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, 0.12f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	RELEASE_INSTANCE(CGameInstance);

	m_bMakeEffect = true;
}

HRESULT CPrizeItem::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();

	return S_OK;
}

HRESULT CPrizeItem::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 4.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	switch (m_ItemDesc.eType)
	{
	case SMALL_KEY:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_SmallKey"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case BOSS_KEY:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_BossKey"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case COMPASS:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Compass"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case MAP:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_DgnMap"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case HEART_RECOVERY:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_HeartRecovery"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case RUBY_GREEN:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Ruby"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case FEATHER:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Feather"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case CELLO:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_FullMoonCello"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ARROW:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Arrow"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case DOGFOOD:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_DogFood"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case HEART_CONTAINER:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_HeartContainer"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case MAGIC_ROD:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_MagicRod"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case BOW:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_ItemBow"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case RUBYPURPLE:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_RubyPurple"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case MAGICPOWDER:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_MagicPowder"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case YOSHIDOLL:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_YoshiDoll"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case NECKLACE:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Necklace"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case TAIL_KEY:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_TailKey"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case DRUM:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Drum"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case HORN:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_ConchHorn"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case TELEPHONE:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_ROOM, TEXT("Prototype_Component_Model_Telephone"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case BELL:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_SeaLilysBell"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	default:
		break;
	}

	
	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_SPHERE */
		ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	if(m_ItemDesc.eInteractType == DEFAULT)
		ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	if (m_ItemDesc.eInteractType == TELL)
		ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CPrizeItem * CPrizeItem::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPrizeItem*	pInstance = new CPrizeItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPrizeItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPrizeItem::Clone(void * pArg)
{
	CPrizeItem*	pInstance = new CPrizeItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPrizeItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPrizeItem::Free()
{
	__super::Free();
}
