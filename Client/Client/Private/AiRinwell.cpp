#include "stdafx.h"
#include "AiRinwell.h"
#include "RinwellMoveState.h"
#include "RinwellState.h"
#include "RinwellDamageState.h"

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

	m_tStats.m_fMaxHp = 100.f;
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
		CRinwellState* pState = new AiRinwell::CMoveState(this, STATETYPE_MAIN, 0);
		m_pState = m_pState->ChangeState(m_pState, pState);
		m_pModelCom->Set_CurrentAnimIndex(BTL_ADVENT);
		m_eAnim = BTL_ADVENT;

	}
	else
	{
		m_pModelCom->Set_CurrentAnimIndex(IDLE_CHARA);
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Rinwell"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 3.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;


	switch (iLevel)
	{
	case Client::LEVEL_SNOWFIELD:
		/* For.Com_Navigation */
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
		break;
	case Client::LEVEL_BATTLE:
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
		break;
	default:
		break;
	}
	return S_OK;
}

int CAiRinwell::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || !Check_IsinFrustum(2.f))
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);

	if (m_bDissolve)
	{
		m_pModelCom->Play_Animation(fTimeDelta, Is_AnimationLoop(m_eAnim), "TransN");
		return OBJ_NOEVENT;
	}


	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();

	if (m_bBattleMode && !m_bTakeDamage)
	{
		if (m_bMotion_Finished)
		{
			AI_Behavior(fTimeDelta);
			Tick_State(fTimeDelta);
		}
		else
			Battle_Animation(fTimeDelta);
	}
	else
		Field_Animation(fTimeDelta);

	return OBJ_NOEVENT;
}

void CAiRinwell::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || !Check_IsinFrustum(2.f))
		return;
	
	__super::Late_Tick(fTimeDelta);

	if (m_bBattleMode && m_bMotion_Finished && !m_bDissolve)
	{
		LateTick_State(fTimeDelta);
	}

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

void CAiRinwell::Field_Animation(_float fTimeDelta)
{
	Find_Target();

	if (m_fDistanceToTarget < 10.f && !m_bAggro)
	{
		m_eAnim = BTL_ADVENT;
		m_bAggro = true;
	}

	_bool isAnimationFinised = m_pModelCom->Play_Animation(fTimeDelta, Is_AnimationLoop(m_eAnim), "TransN");

	if (isAnimationFinised)
	{
		m_eAnim = RUN;
	}

	if (m_eAnim == RUN)
	{
		m_pTransformCom->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		Check_Navigation();
	}

	if (m_eAnim != m_ePreAnim)
	{
		m_pModelCom->Set_CurrentAnimIndex(m_eAnim);
		m_ePreAnim = m_eAnim;
	}

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
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
		return false;
	case Client::CAiRinwell::BTL_ARISE_B:
		break;
	case Client::CAiRinwell::BTL_ARISE_F:
		break;
	case Client::CAiRinwell::BTL_ATTACK_BRAVE:
		break;
	case Client::CAiRinwell::BTL_ATTACK_DENGEKISYOUHEKI:
		break;
	case Client::CAiRinwell::BTL_ATTACK_DENZIHOU:
		break;
	case Client::CAiRinwell::BTL_ATTACK_FLASH:
		break;
	case Client::CAiRinwell::BTL_ATTACK_FUATU:
		break;
	case Client::CAiRinwell::BTL_ATTACK_FUZIN:
		break;
	case Client::CAiRinwell::BTL_ATTACK_HIEN:
		break;
	case Client::CAiRinwell::BTL_ATTACK_HOUDEN:
		break;
	case Client::CAiRinwell::BTL_ATTACK_HYOUROU:
		break;
	case Client::CAiRinwell::BTL_ATTACK_MIZUTAMARI_END:
		break;
	case Client::CAiRinwell::BTL_ATTACK_MIZUTAMARI_LOOP:
		break;
	case Client::CAiRinwell::BTL_ATTACK_MIZUTAMARI_START:
		break;
	case Client::CAiRinwell::BTL_ATTACK_SENKOU:
		break;
	case Client::CAiRinwell::BTL_ATTACK_STRIKE:
		break;
	case Client::CAiRinwell::BTL_ATTACK_STRIKE_AIR:
		break;
	case Client::CAiRinwell::BTL_ATTACK_SUIHOU:
		break;
	case Client::CAiRinwell::BTL_ATTACK_TUMUZIKAZE:
		break;
	case Client::CAiRinwell::BTL_DOWN_B:
		break;
	case Client::CAiRinwell::BTL_DOWN_F:
		break;
	case Client::CAiRinwell::BTL_FALL:
		break;
	case Client::CAiRinwell::BTL_GUARD_HIT:
		break;
	case Client::CAiRinwell::BTL_GUARD_LOOP:
		break;
	case Client::CAiRinwell::BTL_GUARD_START:
		break;
	case Client::CAiRinwell::BTL_ITEM:
		break;
	case Client::CAiRinwell::BTL_JUMP:
		break;
	case Client::CAiRinwell::BTL_GUARD_WALK_B:
	case Client::CAiRinwell::BTL_GUARD_WALK_F:
	case Client::CAiRinwell::BTL_GUARD_WALK_L:
	case Client::CAiRinwell::BTL_GUARD_WALK_R:
	case Client::CAiRinwell::DASH:
	case Client::CAiRinwell::IDLE:
	case Client::CAiRinwell::IDLE_CHARA:
	case Client::CAiRinwell::RUN:
		return true;
	case Client::CAiRinwell::IDLE_TRANS_WALK:
		break;
	case Client::CAiRinwell::JUMP_LANDING:
		break;
	case Client::CAiRinwell::JUMP_LANDING_HIGH:
		break;
	case Client::CAiRinwell::JUMP_LOOP:
		break;
	case Client::CAiRinwell::JUMP_RUN_END:
		break;
	case Client::CAiRinwell::JUMP_RUN_LANDING:
		break;
	case Client::CAiRinwell::JUMP_RUN_LOOP:
		break;
	case Client::CAiRinwell::JUMP_RUN_START:
		break;
	case Client::CAiRinwell::JUMP_START:
		break;
	case Client::CAiRinwell::LADDER_DOWN_END:
		break;
	case Client::CAiRinwell::LADDER_DOWN_LOOP:
		break;
	case Client::CAiRinwell::LADDER_DOWN_START:
		break;
	case Client::CAiRinwell::LADDER_DOWN_TRANS_LADDER_IDLE:
		break;
	case Client::CAiRinwell::LADDER_IDLE:
		break;
	case Client::CAiRinwell::LADDER_TRANS_JUMP_RUN_LOOP:
		break;
	case Client::CAiRinwell::LADDER_UP_END:
		break;
	case Client::CAiRinwell::LADDER_UP_LOOP:
		break;
	case Client::CAiRinwell::LADDER_UP_START:
		break;
	case Client::CAiRinwell::LADDER_UP_TRANS_LADDER_IDLE:
		break;
	case Client::CAiRinwell::RUN_BRAKE_000:
		break;
	case Client::CAiRinwell::RUN_BRAKE_001:
		break;
	case Client::CAiRinwell::TOUCH_STAND:
		break;
	case Client::CAiRinwell::TREASURE_OPEN:
		break;
	case Client::CAiRinwell::WALK:
		break;
	case Client::CAiRinwell::WALK_TRANS_IDLE_000:
		break;
	case Client::CAiRinwell::WALK_TRANS_IDLE_001:
		break;
	default:
		break;
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
		m_eAnim = BTL_DEAD;
		m_bTakeDamage = true;
		m_bDissolve = true;
		return 0;
	}
	else
	{
		m_pTarget = DamageCauser;
		m_eDmg_Direction =  Calculate_DmgDirection();
		m_bTakeDamage = true;
		CRinwellState* pState = new CDamageState(this, m_eDmg_Direction);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}

	return iHp;
}

HRESULT CAiRinwell::SetUp_ShaderID()
{
	if (false == m_bDissolve)
		m_eShaderID = SHADER_ANIMDEFAULT;
	else
		m_eShaderID = SHADER_ANIM_DISSLOVE;

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
