#include "stdafx.h"
#include "..\Public\AI_Rinwell_Fascinated.h"
#include "GameInstance.h"
#include "Rinwell.h"
#include "AICheckState.h"
#include "Effect.h"

using namespace AIPlayer;

CAI_Rinwell_Fascinated::CAI_Rinwell_Fascinated(CPlayer* pPlayer)
{
	m_pOwner = pPlayer;
}

CAIState * CAI_Rinwell_Fascinated::Tick(_float fTimeDelta)
{
	 m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	return nullptr;
}

CAIState * CAI_Rinwell_Fascinated::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CAI_Rinwell_Fascinated::Enter()
{
	_matrix mWorldMatrix =m_pOwner->Get_Transform()->Get_WorldMatrix();
	CEffect::PlayEffectAtLocation(TEXT("Heart.dat"), mWorldMatrix);

	m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_LOOP;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
}

void CAI_Rinwell_Fascinated::Exit()
{
	__super::Exit();
}