#include "stdafx.h"
#include "..\Public\Fascinate.h"
#include "CameraManager.h"
#include "UI_Dialogue_Caption.h"
#include "Monster_LawIdleState.h"
#include "Effect.h"

using namespace MonsterLaw;

CFascinate::CFascinate(CMonsterLaw* pRinwell)
{
	m_pOwner = pRinwell;
	
}


CMonsterLawState * CFascinate::Tick(_float fTimeDelta)
{
	//if (!m_bFinised)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.f,false);




	return nullptr;
}

CMonsterLawState * CFascinate::LateTick(_float fTimeDelta)
{
	/*if (m_bIsAnimationFinished)
	{
	

		return new CMonster_LawIdleState(m_pOwner);
	}*/
	if (m_pOwner->Get_EventFinish())
	{
		return new CMonster_LawIdleState(m_pOwner);
	}
	return nullptr;
}

void CFascinate::Enter()
{
	
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CMonsterLaw::BTL_ARISE_B);
	//m_pOwner->Get_Model()->Reset();


	_vector vOffset = { 0.f,3.f,0.f,0.f };
	_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Heart.dat"), mWorldMatrix);

}

void CFascinate::Exit()
{
	if (!m_pEffects.empty())
	{
		for (auto& iter : m_pEffects)
		{
			if (iter != nullptr)
			{
				iter->Set_Dead(true);
				iter = nullptr;
			}

		}
	}
}

