#include "stdafx.h"
#include "..\Public\AI_Item_Use_State.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "Law.h"
#include "AICheckState.h"
#include "AI_LAW_NomalAttack_State.h"
#include "UI_Skillmessage.h"
#include "Damagefont_Critical.h"

using namespace AIPlayer;

CAI_Item_Use_State::CAI_Item_Use_State(CPlayer* pPlayer, ITEM_NAME itemname)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eItemname = itemname;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}

CAIState * CAI_Item_Use_State::Tick(_float fTimeDelta)
{

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
		}
		else
			m_pOwner->Check_Navigation();
	

		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				

				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						Use_Item(m_eItemname);
						m_fEventStart = pEvent.fStartTime;
					}

				}

				
			}
		}

		
			
			

	return nullptr;
}

CAIState * CAI_Item_Use_State::LateTick(_float fTimeDelta)
{
	

	if (m_bIsAnimationFinished)
	{
		return new CAICheckState(m_pOwner, STATE_END);
	}






	return nullptr;
}

void CAI_Item_Use_State::Enter()
{
	//m_eStateId = STATE_ID::STATE_RUN;

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_USE_ITEM;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;

	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_ITEM;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;

	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ITEM;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;

	case CPlayer::LAW:
		m_iCurrentAnimIndex = CLaw::ANIM::BTL_ITEM;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;



	}

	m_pOwner->Set_Manarecover(true);


}









//RinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 1.f)


void CAI_Item_Use_State::Exit()
{
	__super::Exit();
}

void CAI_Item_Use_State::Use_Item(ITEM_NAME item)
{

	_float recoverpower = 0.f;
	_bool bfindtarget = true;
	switch (item)
	{
	case ITEMNAME_LEMONJELLY:
		recoverpower = 2000.f;

		while (bfindtarget)
		{
			_int random = rand() % (CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1);
			if (CPlayerManager::Get_Instance()->Get_EnumPlayer(random)->Get_Info().fCurrentHp > 0)
			{
				CPlayerManager::Get_Instance()->Get_EnumPlayer(random)->RecoverHP(recoverpower);
				

				CDamagefont_Critical::DMGDESC testdesc;
				ZeroMemory(&testdesc, sizeof(CDamagefont_Critical::DMGDESC));
				testdesc.iDamage = (_int)recoverpower;
				testdesc.pPointer = CPlayerManager::Get_Instance()->Get_EnumPlayer(random);
				testdesc.itype = 4;

				if (false == (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Object(LEVEL_STATIC, TEXT("Layer_DamageCritical"), &testdesc)))
				{

					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont_Critical"), LEVEL_STATIC, TEXT("Layer_DamageCritical"), &testdesc)))
						return;
				}
				break;

				bfindtarget = false;
			}

		}

		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(1, true);
		break;
	case ITEMNAME_GRAPEJELLY:
		recoverpower = 2800.f;

		while (bfindtarget)
		{
			_int random = rand() % (CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1);
			if (CPlayerManager::Get_Instance()->Get_EnumPlayer(random)->Get_Info().fCurrentHp > 0)
			{
				CPlayerManager::Get_Instance()->Get_EnumPlayer(random)->RecoverHP(recoverpower);
				CDamagefont_Critical::DMGDESC testdesc;
				ZeroMemory(&testdesc, sizeof(CDamagefont_Critical::DMGDESC));
				testdesc.iDamage =(_int)recoverpower;
				testdesc.pPointer = CPlayerManager::Get_Instance()->Get_EnumPlayer(random);
				testdesc.itype = 4;

				if (false == (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Object(LEVEL_STATIC, TEXT("Layer_DamageCritical"), &testdesc)))
				{

					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont_Critical"), LEVEL_STATIC, TEXT("Layer_DamageCritical"), &testdesc)))
						return;
				}
				break;
				bfindtarget = false;
			}

		}

		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(2, true);
		break;

	case ITEMNAME_LIFEBOTTLE:
		recoverpower = 800.f;
		_int random = rand() % (CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1);

		if (CPlayerManager::Get_Instance()->Get_EnumPlayer(random)->Get_Info().fCurrentHp > 0)
			CPlayerManager::Get_Instance()->Get_EnumPlayer(random)->Revive();

		CPlayerManager::Get_Instance()->Get_EnumPlayer(random)->RecoverHP(recoverpower);

		CDamagefont_Critical::DMGDESC testdesc;
		ZeroMemory(&testdesc, sizeof(CDamagefont_Critical::DMGDESC));
		testdesc.iDamage = (_int)recoverpower;
		testdesc.pPointer = CPlayerManager::Get_Instance()->Get_EnumPlayer(random);
		testdesc.itype = 4;

		if (false == (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Object(LEVEL_STATIC, TEXT("Layer_DamageCritical"), &testdesc)))
		{

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont_Critical"), LEVEL_STATIC, TEXT("Layer_DamageCritical"), &testdesc)))
				return;
		}
		break;

		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(0, true);

		break;


	}

		
		
		
		

		
		


	
}


