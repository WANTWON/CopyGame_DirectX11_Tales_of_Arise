#include "stdafx.h"
#include "..\Public\AI_Rinwell_Fascinated.h"
#include "GameInstance.h"
#include "Rinwell.h"
#include "AICheckState.h"
#include "Effect.h"

using namespace AIPlayer;

CAI_Rinwell_Fascinated::CAI_Rinwell_Fascinated(CPlayer* pPlayer)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
}

CAIState * CAI_Rinwell_Fascinated::Tick(_float fTimeDelta)
{

	 m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	/*if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
	}
	else
		m_pOwner->Check_Navigation();
*/

	return nullptr;
}

CAIState * CAI_Rinwell_Fascinated::LateTick(_float fTimeDelta)
{
	for (auto& pEffect : m_pEffects)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;
	}

	/*if (m_bIsAnimationFinished)
	{
		return new CAICheckState(m_pOwner, STATE_END);
	}*/


	return nullptr;
}

void CAI_Rinwell_Fascinated::Enter()
{
	_vector vOffset = { 0.f,3.f,0.f,0.f };
	_matrix mWorldMatrix =m_pOwner->Get_Transform()->Get_WorldMatrix();
	m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Heart.dat"), mWorldMatrix);

 m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_LOOP;
 m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);

}









//RinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 1.f)


void CAI_Rinwell_Fascinated::Exit()
{
	__super::Exit();
	
	for (auto& pEffect : m_pEffects)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;
	}

	for (auto& pEffect : m_pEffects)
	{
		if (!pEffect)
			continue;

		pEffect->Set_Dead(true);
		pEffect = nullptr;
	}
}