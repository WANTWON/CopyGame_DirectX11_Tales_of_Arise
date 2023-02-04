#include "stdafx.h"

#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Effect.h"
#include "ParticleSystem.h"

IMPLEMENT_SINGLETON(CEffect_Manager)

CEffect_Manager::CEffect_Manager()
{
}

void CEffect_Manager::Add_Effect(CEffect* pEffect)
{
	m_InstancedEffects.push_back(pEffect);
}

void CEffect_Manager::Remove_Effect(CEffect* pEffect)
{
	for (auto& iter = m_InstancedEffects.begin(); iter != m_InstancedEffects.end(); iter++)
	{
		if (!wcscmp((*iter)->Get_EffectName(), pEffect->Get_EffectName()))
		{
			pEffect->Set_Dead(true);
			m_InstancedEffects.erase(iter);

			return;
		}
	}
}

void CEffect_Manager::Free()
{
}