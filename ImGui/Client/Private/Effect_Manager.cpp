#include "stdafx.h"

#include "Effect_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CEffect_Manager)

CEffect_Manager::CEffect_Manager()
{
}

HRESULT CEffect_Manager::Create_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect::EFFECTDESC tEffectDesc;
	memcpy(&tEffectDesc, (CEffect::EFFECTDESC*)pArg, sizeof(CEffect::EFFECTDESC));

	CEffect* pEffect = nullptr;
	if (FAILED(pGameInstance->Add_GameObject_Out(TEXT("Prototype_GameObject_Effect"), LEVEL_GAMEPLAY, TEXT("Layer_Effect"), (CGameObject*&)pEffect, &tEffectDesc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	m_InstancedEffects.push_back(pEffect);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CEffect_Manager::Free()
{
}