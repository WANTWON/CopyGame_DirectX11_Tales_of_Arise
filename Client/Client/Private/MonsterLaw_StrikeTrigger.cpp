#include "stdafx.h"
#include "..\Public\MonsterLaw_StrikeTrigger.h"
#include "Law.h"
#include "GameInstance.h"
#include "AICheckState.h"
#include "Monster_LawIdleState.h"
#include "MonsterLaw_Strike1.h"
#include "MonsterLaw_Strike2.h"



using namespace MonsterLaw;

CMonsterLaw_StrikeTrigger::CMonsterLaw_StrikeTrigger(CMonsterLaw * pPlayer)
{
	//m_bbackstep = back;
	m_pOwner = pPlayer;
	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());
	

}



CMonsterLawState * CMonsterLaw_StrikeTrigger::Tick(_float fTimeDelta)
{

	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.025f), fRotationRadian, m_pOwner->Get_Navigation());


		m_pOwner->Check_Navigation();

	}


	

	

	return nullptr;
}

CMonsterLawState * CMonsterLaw_StrikeTrigger::LateTick(_float ftimeDelta)
{



	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();


	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				CBaseObj* pCollisionTarget = nullptr;
				if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pOwner->Get_SPHERECollider(), &pCollisionTarget))
				{
					Update_StrikePosition(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Strike_Position_LawAttack1.dat"), 3);
					m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, m_vStrikePosition[LOCKON]);
					m_pTarget->Set_IsActionMode(true);
					m_pTarget->Set_HitState();
					
					m_pOwner->Set_IsActionMode(true);
					m_pOwner->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, m_vStrikePosition[ACTIVE2]);
					m_pOwner->Get_Transform()->LookDir(XMVectorSet(0.f, 0.f, 1.f, 0.f));


					CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Object"), 1.f);
					if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
						dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_Zoom(false);

					CCameraManager* pCameraManager = CCameraManager::Get_Instance();
					pCameraManager->Set_CamState(CCameraManager::CAM_ACTION);
			

					CBattleManager::Get_Instance()->Set_IsStrike(true);

					if (m_pOwner->Get_doStrike1())
					{
						m_pOwner->Set_doStrike1(false);
						pCameraManager->Play_ActionCamera(TEXT("WithLawStrike2.dat"), XMMatrixIdentity());
						return new CMonsterLaw_Strike1(m_pOwner, pCollisionTarget);
					}

					else if (m_pOwner->Get_doStrike2())
					{
						m_pOwner->Set_doStrike2(false);
						pCameraManager->Play_ActionCamera(TEXT("withlaw3.dat"), XMMatrixIdentity());
						return new CMonsterLaw_Strike2(m_pOwner, pCollisionTarget);
					}


					/*switch (rand() % 2)
					{
					case 0:
					
						break;

					case 1:
						
						break;
					}*/
					
				//		return new CMonsterLaw_Strike2(m_pOwner, pCollisionTarget);
						
				}


				
				// returnÇÏ±â m_pOwner->SmashAttack(CPlayer::ALPHEN_LAW);

			}


		}


	}

	if (m_bIsAnimationFinished)
	{

		return new CMonster_LawIdleState(m_pOwner);
	}

	return nullptr;
}

void CMonsterLaw_StrikeTrigger::Enter()
{

	//__super::Enter();

	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());

	if (m_pTarget == nullptr)
		return;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_MUEISYOU);




}

void CMonsterLaw_StrikeTrigger::Exit(void)
{
	//__super::Exit();

	//Safe_Release(m_pDodgeCollider);
}


void CMonsterLaw_StrikeTrigger::Update_StrikePosition(_tchar * FilePath, _int iMonsterPosNum)
{
	m_vStrikeLockOnPos.clear();

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC Active1;
	NONANIMDESC Active2;
	NONANIMDESC AIplayer1;
	NONANIMDESC AIplayer2;
	NONANIMDESC Monster;
	_uint iNum = 0;

	hFile = CreateFile(FilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;

	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	ReadFile(hFile, &(Active1), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(Active2), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(AIplayer1), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(AIplayer2), sizeof(NONANIMDESC), &dwByte, nullptr);

	for (int i = 0; i < iMonsterPosNum; ++i)
	{
		ReadFile(hFile, &(Monster), sizeof(NONANIMDESC), &dwByte, nullptr);
		m_vStrikeLockOnPos.push_back(XMVectorSetW(XMLoadFloat3(&Monster.vPosition), 1.f));
	}


	CloseHandle(hFile);


	m_vStrikePosition[ACTIVE1] = XMVectorSetW(XMLoadFloat3(&Active1.vPosition), 1.f);
	m_vStrikePosition[ACTIVE2] = XMVectorSetW(XMLoadFloat3(&Active2.vPosition), 1.f);
	m_vStrikePosition[AIPLAYER1] = XMVectorSetW(XMLoadFloat3(&AIplayer1.vPosition), 1.f);
	m_vStrikePosition[AIPLAYER2] = XMVectorSetW(XMLoadFloat3(&AIplayer2.vPosition), 1.f);
	m_vStrikePosition[LOCKON] = m_vStrikeLockOnPos[0];


	CPlayerManager::Get_Instance()->Set_StrikeLockOnPos(m_vStrikeLockOnPos);
	
}