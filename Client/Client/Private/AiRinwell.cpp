#include "stdafx.h"
#include "AiRinwell.h"
#include "RinwellMoveState.h"
#include "RinwellState.h"
#include "RinwellPoseState.h"
#include "RinwellDamageState.h"
#include "CameraManager.h"
using namespace AiRinwell;

CAiRinwell::CAiRinwell(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CMonster(pDevice, pContext)
{
}

CAiRinwell::CAiRinwell(const CAiRinwell & rhs)
	: CMonster(rhs)
{
}


HRESULT CAiRinwell::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAiRinwell::Initialize(void * pArg)
{
	NONANIMDESC ModelDesc;

	if (pArg != nullptr)
		memcpy(&ModelDesc, pArg, sizeof(NONANIMDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_tStats.m_fMaxHp = 100000.f;

	m_tStats.m_fCurrentHp = m_tStats.m_fMaxHp;
	m_tStats.m_fAttackPower = 10.f;
	m_tStats.m_fWalkSpeed = 0.05f;
	m_tStats.m_fRunSpeed = 5.f;

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(ModelDesc.vScale);

		if (ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMLoadFloat3(&ModelDesc.vRotation), XMConvertToRadians(ModelDesc.m_fAngle));
	}


	if (m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode())
	{
		/* Set State */
		CRinwellState* pState = new AiRinwell::CPoseState(this, CRinwellState::STATE_BATTLESTART);
		m_pState = m_pState->ChangeState(m_pState, pState);
		m_pTransformCom->LookAt(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION));
		CBattleManager::Get_Instance()->Set_BossMonster(this);
	}
	else
	{
		CRinwellState* pState = new AiRinwell::CPoseState(this, CRinwellState::STATE_IDLE);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}



	m_eMonsterID = RINWELL;
	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));
	return S_OK;
}



HRESULT CAiRinwell::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 6.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("AIRinwell"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 2.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;


	if (FAILED(__super::Add_Components(TEXT("Com_FieldNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_vecNavigation[LEVEL_SNOWFIELD])))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_BattleNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_vecNavigation[LEVEL_BATTLE])))
		return E_FAIL;

	m_pNavigationCom = m_vecNavigation[LEVEL_SNOWFIELD];

	return S_OK;
}

int CAiRinwell::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		CBattleManager::Get_Instance()->Set_BossMonster(nullptr);
		CBattleManager::Get_Instance()->Out_Monster(this);
		return OBJ_DEAD;
	}

	m_eLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
	if (CUI_Manager::Get_Instance()->Get_StopTick() || m_eLevel == LEVEL_LOADING || m_eLevel == LEVEL_LOGO)
		return OBJ_NOEVENT;
	if (m_pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(m_pCameraManager->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		return OBJ_NOEVENT;

	if (m_eLevel == LEVEL_SNOWFIELD && m_bBattleMode)
		return OBJ_NOEVENT;

	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();

	AI_Behavior(fTimeDelta);
	Tick_State(fTimeDelta);

	return OBJ_NOEVENT;
}

void CAiRinwell::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || m_eLevel == LEVEL_LOADING || m_eLevel == LEVEL_LOGO)
		return;

	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return;

	__super::Late_Tick(fTimeDelta);

	if (m_eLevel == LEVEL_SNOWFIELD && m_bBattleMode)
		return;

	if (m_pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(m_pCameraManager->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		return;

	if (m_pRendererCom && m_bGlowUp)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	if (!m_bDissolve)
	{
		LateTick_State(fTimeDelta);
	}

}

HRESULT CAiRinwell::Render_Glow()
{
	if (!m_pShaderCom || !m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_bool bUseDiffuseColor = true;
	if (FAILED(m_pShaderCom->Set_RawValue("g_bUseDiffuseColor", &bUseDiffuseColor, sizeof(_bool))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_bDissolve ? SHADER_ANIM_GLOW_DISSOLVE : SHADER_ANIM_GLOW)))
			return E_FAIL;
	}

	return S_OK;
}

void CAiRinwell::AI_Behavior(_float fTimeDelta)
{
	CRinwellState* pNewState = m_pState->AI_Behaviour(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);
}


void CAiRinwell::Tick_State(_float fTimeDelta)
{
	CRinwellState* pNewState = m_pState->Tick(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);

}

void CAiRinwell::LateTick_State(_float fTimeDelta)
{
	CRinwellState* pNewState = m_pState->LateTick(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);
}


void CAiRinwell::Battle_Animation(_float fTimeDelta)
{
	_bool isAnimationFinised = m_pModelCom->Play_Animation(fTimeDelta, Is_AnimationLoop(m_eAnim), "TransN");

	if (isAnimationFinised)
	{
		m_bMotion_Finished = true;
		m_pModelCom->Set_CurrentAnimIndex(DASH);
	}
}


void CAiRinwell::Set_BattleMode(_bool type)
{
	m_bBattleMode = type;
	if (m_bBattleMode)
	{
		/* Set State */
		CRinwellState* pState = new AiRinwell::CPoseState(this, CRinwellState::STATE_BATTLESTART);
		m_pState = m_pState->ChangeState(m_pState, pState);
		m_pTransformCom->LookAt(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION));
		CBattleManager::Get_Instance()->Set_BossMonster(this);
	}
	else
	{
		Check_NearTrigger();
		CRinwellState* pState = new AiRinwell::CPoseState(this, CRinwellState::STATE_IDLE);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
}

_bool CAiRinwell::Is_AnimationLoop(_uint eAnimId)
{
	switch (eAnimId)
	{
	case Client::CAiRinwell::BTL_ADVENT:
	case Client::CAiRinwell::DASH_BRAKE_000:
	case Client::CAiRinwell::DASH_BRAKE_001:
	case Client::CAiRinwell::BTL_ATTACK_NORMAL_0:
	case Client::CAiRinwell::BTL_ATTACK_NORMAL_1:
	case Client::CAiRinwell::BTL_ATTACK_NORMAL_2:
	case Client::CAiRinwell::BTL_ATTACK_NORMAL_3:
	case Client::CAiRinwell::BTL_ATTACK_NORMAL_AIR_0:
	case Client::CAiRinwell::BTL_ATTACK_NORMAL_AIR_1:
	case Client::CAiRinwell::BTL_ATTACK_NORMAL_AIR_2:
	case Client::CAiRinwell::BTL_DAMAGE_AIR_LARGE_B:
	case Client::CAiRinwell::BTL_DAMAGE_AIR_LARGE_F:
	case Client::CAiRinwell::BTL_DAMAGE_AIR_LARGE_L:
	case Client::CAiRinwell::BTL_DAMAGE_AIR_LARGE_R:
	case Client::CAiRinwell::BTL_DAMAGE_AIR_LOOP:
	case Client::CAiRinwell::BTL_DAMAGE_AIR_SMALL_B:
	case Client::CAiRinwell::BTL_DAMAGE_AIR_SMALL_F:
	case Client::CAiRinwell::BTL_DAMAGE_AIR_SMALL_L:
	case Client::CAiRinwell::BTL_DAMAGE_AIR_SMALL_R:
	case Client::CAiRinwell::BTL_DAMAGE_LARGE_B:
	case Client::CAiRinwell::BTL_DAMAGE_LARGE_F:
	case Client::CAiRinwell::BTL_DAMAGE_LARGE_L:
	case Client::CAiRinwell::BTL_DAMAGE_LARGE_R:
	case Client::CAiRinwell::BTL_DAMAGE_LOOP:
	case Client::CAiRinwell::BTL_DAMAGE_SMALL_B:
	case Client::CAiRinwell::BTL_DAMAGE_SMALL_F:
	case Client::CAiRinwell::BTL_DAMAGE_SMALL_L:
	case Client::CAiRinwell::BTL_DAMAGE_SMALL_R:
	case Client::CAiRinwell::BTL_DEAD:
	case Client::CAiRinwell::TOUCH_STAND:
	case Client::CAiRinwell::TREASURE_OPEN:
	case Client::CAiRinwell::WALK_TRANS_IDLE_000:
	case Client::CAiRinwell::WALK_TRANS_IDLE_001:
	case Client::CAiRinwell::RUN_BRAKE_000:
	case Client::CAiRinwell::RUN_BRAKE_001:
	case Client::CAiRinwell::BTL_ATTACK_BRAVE:
	case Client::CAiRinwell::BTL_ATTACK_DENGEKISYOUHEKI:
	case Client::CAiRinwell::BTL_ATTACK_DENZIHOU:
	case Client::CAiRinwell::BTL_ATTACK_PHOTONFLASH:
	case Client::CAiRinwell::BTL_ATTACK_GALEFORCE:
	case Client::CAiRinwell::BTL_ATTACK_HIEN:
	case Client::CAiRinwell::BTL_ATTACK_HOUDEN:
	case Client::CAiRinwell::BTL_ATTACK_HYOUROU:
	case Client::CAiRinwell::BTL_ATTACK_MIZUTAMARI_END:
	case Client::CAiRinwell::BTL_STEP_LAND_BACK:
	case Client::CAiRinwell::BTL_STEP_LAND:
	case Client::CAiRinwell::BTL_ATTACK_MAGIC_EMIT_AIR:
		return false;
	}

	return true;
}

_int CAiRinwell::Take_Damage(int fDamage, CBaseObj * DamageCauser)
{
	if (fDamage <= 0 || m_bDead || m_bTakeDamage)
		return 0;

	_int iHp = __super::Take_Damage(fDamage, DamageCauser);




	if (iHp <= 0)
	{
		m_bTakeDamage = true;
		CRinwellState* pState = new CDamageState(this, m_eDmg_Direction, CRinwellState::STATE_DEAD);
		m_pState = m_pState->ChangeState(m_pState, pState);
		return 0;
	}
	else
	{
		m_pTarget = DamageCauser;
		m_eDmg_Direction = Calculate_DmgDirection();
		m_bTakeDamage = true;

		if (fDamage > 100)
		{
			CRinwellState* pState = new CDamageState(this, m_eDmg_Direction, CRinwellState::STATE_DAMAGE);
			m_pState = m_pState->ChangeState(m_pState, pState);
		}

	}

	return iHp;
}

HRESULT CAiRinwell::SetUp_ShaderID()
{
	if (false == m_bDissolve)
		m_eShaderID = SHADER_ANIMDEFAULT;
	else
		m_eShaderID = SHADER_ANIM_DISSOLVE;

	return S_OK;
}

void CAiRinwell::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

CAiRinwell * CAiRinwell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAiRinwell* pInstance = new CAiRinwell(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CAiRinwell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAiRinwell::Clone(void * pArg)
{
	CAiRinwell* pInstance = new CAiRinwell(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CAiRinwell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAiRinwell::Free()
{
	__super::Free();

	Safe_Delete(m_pState);

}
