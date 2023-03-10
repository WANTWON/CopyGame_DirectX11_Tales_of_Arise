#include "stdafx.h"
#include "..\Public\AstralDoubt_Battle_Hit_AndDead.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"
#include "AstralDoubt_Battle_IdleState.h"
#include "Effect.h"

using namespace Astral_Doubt;

CBattle_Hit_AndDead::CBattle_Hit_AndDead(CAstralDoubt* pAstralDoubt,  STATE_ID StateId, _bool bThirdHit)
{
	m_pOwner = pAstralDoubt;
	m_fTimeDeltaAcc = 0;
	m_fCntChanceTime = ((rand() % 1000) *0.001f)*((rand() % 100) * 0.01f);
	m_bThirdHit = bThirdHit;
	m_eStateId = StateId;
}

CAstralDoubt_State * CBattle_Hit_AndDead::Tick(_float fTimeDelta)
{
	m_pOwner->Set_Done_HitAnimState();

	switch (m_eStateId)
	{
	case Client::CAstralDoubt_State::STATE_DEAD:
		if(!m_bIsAnimationFinished)
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		
		m_pOwner->Check_Navigation();
		break;
	case Client::CAstralDoubt_State::STATE_DOWN:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
		break;
	case Client::CAstralDoubt_State::STATE_SMASHHIT:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		break;
	default:
		break;
	}
		

	if(m_eStateId != CAstralDoubt_State::STATE_SMASHHIT)
		m_pOwner->Check_Navigation();
	return nullptr;
}

CAstralDoubt_State * CBattle_Hit_AndDead::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	
	if(m_fTimeDeltaAcc > m_fCntChanceTime)
	m_iRand = rand() % 1;
	

		if (m_bIsAnimationFinished)
		{
			switch (m_eStateId)
			{
			case Client::CAstralDoubt_State::STATE_DEAD:
				m_bDeadAnimFinish = false;
				if (m_bDeadAnimFinish == false)
				{
					CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
					m_pAtkColliderCom = nullptr;

					pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);

					m_bDeadAnimFinish = true;
					m_pOwner->Set_GlowUp();
				}
				break;
			case Client::CAstralDoubt_State::STATE_DOWN:
			{
				CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
				pCamera->Set_Zoom(false, -8.f, 4.f, 6.f, 10.f);
				m_pOwner->Set_OnGoingDown();
				return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_DOWN);
			}
			case Client::CAstralDoubt_State::STATE_SMASHHIT:
			{
				if(CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
					return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_DOWN);
			}
			}
		}
	

	return nullptr;
}

void CBattle_Hit_AndDead::Enter()
{
	m_iRand = rand() % 9;

	
	switch (m_eStateId)
	{
	case Client::CAstralDoubt_State::STATE_DEAD:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::DEAD);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAus_Dead.wav"), SOUND_OBJECT, 0.8f);
		m_pOwner->Check_Navigation();
		break;

	case Client::CAstralDoubt_State::STATE_DOWN:
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_Zoom(true, -8.f, 4.f, 6.f, 10.f);
		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 3.f, 0.5f, true);

		_vector vOffset = XMVectorSet(0.f, 5.f, 0.f, 0.f);
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] += vOffset;
		CEffect::PlayEffectAtLocation(TEXT("BossDown.dat"), mWorldMatrix);


		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::DOWN_UNIQUE);
		m_pOwner->Set_OnGoingDown();
		CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Hit.wav"), SOUND_OBJECT, 0.5f);
		break;
	}
	case Client::CAstralDoubt_State::STATE_SMASHHIT:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::DOWN_UNIQUE);
		m_pOwner->Get_Model()->Reset();
		m_pOwner->Set_OnGoingDown();
		CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Hit.wav"), SOUND_OBJECT, 0.5f);
		break;
	}


}

void CBattle_Hit_AndDead::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);

	Safe_Release(m_pAtkColliderCom);

}
