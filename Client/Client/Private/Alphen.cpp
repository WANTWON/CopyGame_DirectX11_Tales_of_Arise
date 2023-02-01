#include "stdafx.h"
#include "Alphen.h"

#include "GameInstance.h"
#include "Weapon.h"


_bool CAlphen::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{
		case ANIM_IDLE:
		case ANIM_RUN:
		case ANIM_JUMP_LOOP:
			return true;
		default:
			return false;
	}
}

CAlphen::CAlphen(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CPlayer(pDevice, pContext)
{
}

CAlphen::CAlphen(const CAlphen & rhs)
	: CPlayer(rhs)
{
}

HRESULT CAlphen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlphen::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_ePlayerID = ALPHEN;

	m_tInfo.fMaxHp = 5877;
	m_tInfo.fCurrentHp = 1156.f;
	m_tInfo.fMaxMp = 5.f;
	m_tInfo.fCurrentMp = 2.3f;
	m_tInfo.iDamage = 100;

	return S_OK;
}


HRESULT CAlphen::Ready_Parts()
{
	m_Parts.resize(PARTS_END);

	/* For.Weapon */
	_uint iLevelIndex = CGameInstance::Get_Instance()->Get_CurrentLevelIndex();

	CHierarchyNode* pSocket = m_pModelCom->Get_BonePtr("pinky_03_R_end");
	if (nullptr == pSocket)
		return E_FAIL;

	CWeapon::WEAPONDESC WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	strcpy(WeaponDesc.pModeltag, "SWO1");
	Safe_AddRef(pSocket);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_Parts[PARTS_WEAPON] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Weapon"), &WeaponDesc);
	if (nullptr == m_Parts[PARTS_WEAPON])
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CAlphen::Ready_Components(void* pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Alphen"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_OBB */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 4.5f, 1.f);
	ColliderDesc.vPosition = _float3(0.f, 2.28f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;


	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	if (FAILED(__super::Add_Components(TEXT("Com_BattleNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_FieldNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigations.push_back(m_pNavigationCom);

	return S_OK;
}

void CAlphen::Change_Level(LEVEL eLevel)
{
	__super::Change_Level(eLevel);

	if (nullptr != m_Parts[PARTS_WEAPON])
	{
		CWeapon::WEAPONDESC WeaponDesc;
		ZeroMemory(&WeaponDesc, sizeof(CWeapon::WEAPONDESC));

		CHierarchyNode* pSocket = nullptr; 

		if (LEVEL_SNOWFIELD == eLevel)
		{
			pSocket = m_pModelCom->Get_BonePtr("SWG_CHR_ARI_HUM_003_COLOAR00_00_L_end");
			if (nullptr == pSocket)
			{
				ERR_MSG(TEXT("Failed to Get BonePtr"));
				return;
			}
		}
		else if (LEVEL_BATTLE == eLevel)
		{
			pSocket = m_pModelCom->Get_BonePtr("pinky_03_R_end");
			if (nullptr == pSocket)
			{
				ERR_MSG(TEXT("Failed to Get BonePtr"));
				return;
			}
		}
		
		WeaponDesc.pSocket = pSocket;
		WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
		WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
		strcpy(WeaponDesc.pModeltag, "SWO1");
		Safe_AddRef(pSocket);

		dynamic_cast<CWeapon*>(m_Parts[PARTS_WEAPON])->Set_WeaponDesc(WeaponDesc);
	}
}

HRESULT CAlphen::SetUp_ShaderResources()
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

CAlphen * CAlphen::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAlphen* pInstance = new CAlphen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CAlphen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAlphen::Clone(void * pArg)
{
	CAlphen* pInstance = new CAlphen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CAlphen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAlphen::Free()
{
	__super::Free();
}