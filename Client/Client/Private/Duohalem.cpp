#include "stdafx.h"
#include "..\Public\Duohalem.h"

#include "GameInstance.h"
#include "AI_BoostAttackState.h"

CDuohalem::CDuohalem(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CPlayer(pDevice, pContext)
{
}

CDuohalem::CDuohalem(const CDuohalem & rhs)
	: CPlayer(rhs)
{
}

HRESULT CDuohalem::Initialize_Prototype(void)
{
	return S_OK;
}

HRESULT CDuohalem::Initialize(void * pArg)
{
	m_ePlayerID = DUOHALEM;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tInfo.fMaxHp = 5000;
	m_tInfo.fCurrentHp = m_tInfo.fMaxHp;

	m_tInfo.fMaxMp = 5.f;
	m_tInfo.fCurrentMp = 2.3f;

	m_tInfo.iDamage = 100;
	m_tInfo.fCurrentBoostGuage = 100.f;

	m_tInfo.iCurrentExp = 500;
	m_tInfo.iMaxExp = 1000;
	m_tInfo.iLevel = 47;








	//m_eLevel = LEVEL_END;





	/*CAIState* pAIState = new CAI_Boostatt(this, CAIState::STATE_IDLE);
	m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);*/

	//CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_PLAYER, this);

	/*m_tInfo.fCurrentOverlimitGauge = 100.f;

	m_tInfo.fMaxHp = 5000;
	m_tInfo.fCurrentHp = m_tInfo.fMaxHp;

	m_tInfo.fMaxMp = 5.f;
	m_tInfo.fCurrentMp = 2.3f;

	m_tInfo.iDamage = 100;


	m_tInfo.iCurrentExp = 500;
	m_tInfo.iMaxExp = 1000;
	m_tInfo.iLevel = 47;*/

	return S_OK;
}

_bool CDuohalem::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{

	default:
		return false;
	}
}

HRESULT CDuohalem::Ready_Parts(void)
{
	//m_Parts.resize(PARTS_END);

	return S_OK;
}

HRESULT CDuohalem::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Duohalem"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.5f, 2.5f, 2.5f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 2.5f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;


	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	if (FAILED(__super::Add_Components(TEXT("Com_BossNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_Boss_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_BattleNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_FieldNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_CityNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_City_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_RestaurantNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_Restaurant_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_RestaurantMIniGameNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_Restaurant_MIniGameNavigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_WorkToolNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_WorkTool_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_LawBattleNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_LawBattle_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);


	return S_OK;
}

HRESULT CDuohalem::SetUp_ShaderResources(void)
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CDuohalem * CDuohalem::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDuohalem* pInstance = new CDuohalem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CDuohalem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDuohalem::Clone(void * pArg)
{
	CDuohalem* pInstance = new CDuohalem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CDuohalem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDuohalem::Free(void)
{
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSPHERECom);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	for (auto& pGameObject : m_Parts)
		Safe_Release(pGameObject);

	m_Parts.clear();

	for (auto& iter : m_vecNavigations)
		Safe_Release(iter);

	m_vecNavigations.clear();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pPlayerManager);

}
