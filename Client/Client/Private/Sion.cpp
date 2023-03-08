#include "stdafx.h"
#include "Sion.h"

#include "GameInstance.h"
#include "Weapon.h"

_bool CSion::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{
	case ANIM::IDLE_CHARA:
	case ANIM::JUMP_LOOP:
	case ANIM::JUMP_RUN_LOOP:
	case ANIM::BTL_MOVE_IDLE:
	case ANIM::BTL_MOVE_RUN:
	case ANIM::SYS_RUN:
	case ANIM::DASH:
		return true;
	default:
		return false;
	}
}

CSion::CSion(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CPlayer(pDevice, pContext)
{
}

CSion::CSion(const CSion & rhs)
	: CPlayer(rhs)
{
}

HRESULT CSion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSion::Initialize(void * pArg)
{
	m_ePlayerID = SION;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tInfo.fMaxHp = 5000.f;
	m_tInfo.fCurrentHp = 5000.f;

	m_tInfo.fMaxMp = 5.f;
	m_tInfo.fCurrentMp = 2.3f;
	m_tInfo.iDamage = 100;
	m_tInfo.fCurrentBoostGuage = 20.f;
	m_tInfo.iCurrentExp = 500;
	m_tInfo.iMaxExp = 1000;
	m_tInfo.iLevel = 8;

	return S_OK;
}


HRESULT CSion::Ready_Parts()
{
	m_Parts.resize(PARTS_END);

	/* For.Weapon */
	CHierarchyNode* pSocket = m_pModelCom->Get_BonePtr("pinky_03_R");
	if (nullptr == pSocket)
		return E_FAIL;

	CWeapon::WEAPONDESC WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	WeaponDesc.pOwner = this;
	strcpy(WeaponDesc.pModeltag, "SIOW(00)");
	Safe_AddRef(pSocket);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_Parts[PARTS_WEAPON] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Weapon"), &WeaponDesc);
	if (nullptr == m_Parts[PARTS_WEAPON])
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CSion::Ready_Components(void* pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Sion"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_OBB */
	CCollider::COLLIDERDESC ColliderDesc;
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//ColliderDesc.vScale = _float3(1.f, 4.5f, 1.f);
	//ColliderDesc.vPosition = _float3(0.f, 2.28f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
	//	return E_FAIL;

	/* For.Com_SPHERE */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
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

void CSion::Change_Level(LEVEL eLevel)
{
	__super::Change_Level(eLevel);

	if (nullptr != m_Parts[PARTS_WEAPON])
	{
		CWeapon::WEAPONDESC WeaponDesc;
		ZeroMemory(&WeaponDesc, sizeof(CWeapon::WEAPONDESC));

		CHierarchyNode* pSocket = nullptr;

		if (LEVEL_LAWBATTLE == eLevel)
		{
			if (CBattleManager::Get_Instance()->Get_IsBattleMode() == true)
			{

				pSocket = m_pModelCom->Get_BonePtr("pinky_03_R");
				if (nullptr == pSocket)
				{
					ERR_MSG(TEXT("Failed to Get BonePtr"));
					return;
				}

				XMStoreFloat4x4(&WeaponDesc.RotationCorrectionMatrix, XMMatrixIdentity());
				XMStoreFloat4x4(&WeaponDesc.TranslationCorrectionMatrix, XMMatrixIdentity());
			}
			else
			{
				pSocket = m_pModelCom->Get_BonePtr("pinky_03_R");
				if (nullptr == pSocket)
				{
					ERR_MSG(TEXT("Failed to Get BonePtr"));
					return;
				}

				XMStoreFloat4x4(&WeaponDesc.RotationCorrectionMatrix, XMMatrixRotationX(XMConvertToRadians(180.f)));
				XMStoreFloat4x4(&WeaponDesc.TranslationCorrectionMatrix, XMMatrixTranslation(-40.f, 50.f, 50.f));
			}
		}
		else if (LEVEL_SNOWFIELD == eLevel || LEVEL_CITY == eLevel || LEVEL_RESTAURANT == eLevel)
		{
			pSocket = m_pModelCom->Get_BonePtr("pinky_03_R");
			if (nullptr == pSocket)
			{
				ERR_MSG(TEXT("Failed to Get BonePtr"));
				return;
			}

			XMStoreFloat4x4(&WeaponDesc.RotationCorrectionMatrix, XMMatrixRotationX(XMConvertToRadians(180.f)));
			XMStoreFloat4x4(&WeaponDesc.TranslationCorrectionMatrix, XMMatrixTranslation(-40.f, 50.f, 50.f));

		}
		else if (LEVEL_BATTLE == eLevel || LEVEL_BOSS == eLevel || LEVEL_WORKTOOL == eLevel)
		{
			pSocket = m_pModelCom->Get_BonePtr("pinky_03_R");
			if (nullptr == pSocket)
			{
				ERR_MSG(TEXT("Failed to Get BonePtr"));
				return;
			}

			_matrix RotationMatrix = XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationZ(XMConvertToRadians(180.f)) * XMMatrixRotationY(XMConvertToRadians(90.f));
			XMStoreFloat4x4(&WeaponDesc.RotationCorrectionMatrix, RotationMatrix);
			XMStoreFloat4x4(&WeaponDesc.TranslationCorrectionMatrix, XMMatrixIdentity());
		}

		WeaponDesc.pSocket = pSocket;
		WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
		WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
		WeaponDesc.pOwner = this;
		strcpy(WeaponDesc.pModeltag, "SIOW(00)");
		Safe_AddRef(pSocket);

		dynamic_cast<CWeapon*>(m_Parts[PARTS_WEAPON])->Set_WeaponDesc(WeaponDesc);
	}
}

HRESULT CSion::SetUp_ShaderResources()
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


CSion * CSion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSion* pInstance = new CSion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSion::Clone(void * pArg)
{
	CSion* pInstance = new CSion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSion"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSion::Free()
{
	__super::Free();
}