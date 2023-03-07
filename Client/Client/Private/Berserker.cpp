#include "stdafx.h"
#include "Berserker.h"
#include "BerserkerState.h"
#include "BerserkerIdleState.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_HowLingState.h"
#include "BerserkerBattle_Damage_LargeB_State.h"
#include "BerserkerBattle_DeadState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_PouncingState.h"
#include "BerserkerBattle_Quadruple_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerWalkState.h"
#include "BerserkerBattle_TurnState.h"
#include "BerserkerBattle_FireBallState.h"
#include "BerserkerBattle_Multiple_FireState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerTurnR_State.h"

using namespace Berserker;

CBerserker::CBerserker(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CMonster(pDevice, pContext)
{
}

CBerserker::CBerserker(const CBerserker & rhs)
	: CMonster(rhs)
{
}


HRESULT CBerserker::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBerserker::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();

	if (m_bBattleMode)
	{
		/*Set_Battle State*/
		CBerserkerState* pBattleState = new CBattle_IdleState(this, CBerserkerState::START_BATTLE);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pBattleState);
	}
	else
	{
		/* Set State */
		CBerserkerState* pState = new CTurnR_State(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	m_tStats.m_fMaxHp = 15000;
	m_tStats.m_fCurrentHp = m_tStats.m_fMaxHp;
	m_tStats.m_fAttackPower = 10;
	m_eMonsterID = BERSERKER;

	return S_OK;
}



HRESULT CBerserker::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	if(false == m_bBattleMode)
		TransformDesc.fSpeedPerSec = 2.5f;
	else
		TransformDesc.fSpeedPerSec = 8.5f;

	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Berserker"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 1.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;


	if (FAILED(__super::Add_Components(TEXT("Com_FieldNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_vecNavigation[LEVEL_SNOWFIELD])))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_BattleNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_vecNavigation[LEVEL_BATTLE])))
		return E_FAIL;

	m_pNavigationCom = m_vecNavigation[LEVEL_SNOWFIELD];

	return S_OK;
}

int CBerserker::Tick(_float fTimeDelta)
{
	_int iSuperTick = __super::Tick(fTimeDelta);
	if (iSuperTick == OBJ_DEAD)
		return OBJ_DEAD;

	if (iSuperTick == OBJ_NOSHOW)
		return OBJ_NOSHOW;

	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return OBJ_NOSHOW;

	AI_Behavior(fTimeDelta);
	Tick_State(fTimeDelta);

	
	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	if (m_pBerserkerState->Get_StateId() == CBerserkerState::STATE_ID::STATE_DOWN)
		m_fFresnelTimer += fTimeDelta * 6;
	else
		m_fFresnelTimer = 0.f;

	return OBJ_NOEVENT;
}

void CBerserker::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || m_eLevel == LEVEL_LOADING || m_eLevel == LEVEL_LOGO)
		return;
	
	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return;

	if (ExceptingActionCamHanding() == false)
		return;

	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	if (ExceptionHanding() == false)
		return;

	LateTick_State(fTimeDelta);
}

HRESULT CBerserker::Render()
{
	if (m_pBerserkerState->Get_StateId() == CBerserkerState::STATE_ID::STATE_DOWN)
	{
		_bool bDownState = true;
		m_pShaderCom->Set_RawValue("g_bRimLight", &bDownState, sizeof(_bool));
	}

	__super::Render();

	return S_OK;
}

HRESULT CBerserker::Render_Glow()
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

void CBerserker::AI_Behavior(_float fTimeDelta)
{
	CBerserkerState* pNewState = m_pBerserkerState->AI_Behaviour(fTimeDelta);
	if (pNewState)
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pNewState);
}


void CBerserker::Tick_State(_float fTimeDelta)
{
	CBerserkerState* pNewState = m_pBerserkerState->Tick(fTimeDelta);
	if (pNewState)
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pNewState);
	
}

void CBerserker::LateTick_State(_float fTimeDelta)
{
	CBerserkerState* pNewState = m_pBerserkerState->LateTick(fTimeDelta);
	if (pNewState)
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pNewState);
}

void CBerserker::Set_BattleMode(_bool type)
{
	m_bBattleMode = type;
	if (m_bBattleMode)
	{
		/*Set_Battle State*/
		CBerserkerState* pBattleState = new CBattle_IdleState(this, CBerserkerState::START_BATTLE);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pBattleState);
	}
	else
	{
		Check_NearTrigger();
		/* Set State */
		CBerserkerState* pState = new CWalkState(this, CBerserkerState::FIELD_STATE_ID::FIELD_STATE_END);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);

}


_bool CBerserker::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{
	case MOVE_IDLE:
	case MOVE_WALK_F:
	case MOVE_RUN:
		return true;
	case ATTACK_DOUBLE_CLAW:
	case ATTACK_DOUBLE_CROW:
	case ATTACK_SHOCK_WAVE:
	case DAMAGE_LARGE_B:
	case ATTACK_DASH_CLAW_START:
	case ATTACK_DASH_CLAW_LOOP:
	case ATTACK_DASH_CLAW_END:
	case DEAD:
	case ATTACK_HOWLING:
	case ATTACK_QUADRUPLE_CLAW:
	case ATTACK_POUNCING:
	case TURN_R:
	case TURN_L:
	case DAMAGE_LARGE_F:
	case DAMAGE_LARGE_L:
	case DAMAGE_LARGE_R:
	case DAMAGE_SMALL_F:
	case DAMAGE_SMALL_L:
	case DAMAGE_SMALL_R:
	case SYMBOL_TURN_LEFT:
	case DOWN_F:
	case ARISE_F:
		return false;
	}

	return false;
}

_int CBerserker::Take_Damage(int fDamage, CBaseObj* DamageCauser, HITLAGDESC HitDesc)
{
	if (fDamage <= 0 || m_bDead || m_bDissolve || m_bTakeDamage || m_pBerserkerState->Get_StateId() == CBerserkerState::STATE_DEAD)
		return 0;
	
	m_iBeDamaged_Cnt++;

	_int iHp = __super::Take_Damage(fDamage, DamageCauser, HitDesc);

	if (m_bOnGoingDown == false)
	{
		if (iHp <= 0)
		{
			m_tStats.m_fCurrentHp = 0;
			CBattleManager::Get_Instance()->Update_LockOn();
			Check_AmILastMoster();

			CBerserkerState* pState = new CBattle_DeadState(this);
			m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
			return 0;
		}
		else
		{
			++m_iBeDamaged_Cnt;
			if (m_bOnGoingHowLing == false)
			{
				if (m_bDownState == false)
				{
					if (iHp <= 8000)
					{
						_bool pAngry = true;
						
						if (m_bBedamageAnim_Delay == false)
						{
							if (m_bBedamageAnim == true)
							{
								//m_pModelCom->Set_TimeReset();
								CBerserkerState* pState = new CBattle_Damage_LargeB_State(this, true, m_bBerserkerMode, true, CBerserkerState::STATE_ID::STATE_TAKE_DAMAGE);
								m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
								
							}
							else if (m_bBedamageAnim == false)
							{
								return iHp;
							}
						}

						else if (m_bBedamageAnim_Delay == true)
						{
							return iHp;
						}
					}

					else
					{
						if (m_bBedamageAnim_Delay == false)
						{
							if (m_bBedamageAnim == true)
							{
								//m_pModelCom->Set_TimeReset();
								CBerserkerState* pState = new CBattle_Damage_LargeB_State(this, false, m_bBerserkerMode, false, CBerserkerState::STATE_ID::STATE_TAKE_DAMAGE);
								m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
							}
						}
						else
							return iHp;
					}

				}

				else
				{
					CBerserkerState* pState = new CBattle_Damage_LargeB_State(this, false, false, false, CBerserkerState::STATE_ID::STATE_DOWN);
					m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
				}
			}

			else
				return iHp;
		}
	}
	

	return iHp;
}

HRESULT CBerserker::SetUp_ShaderID()
{
	if (false == m_bDissolve)
		m_eShaderID = SHADER_ANIMDEFAULT;

	else
		m_eShaderID = SHADER_ANIM_DISSOLVE;

	return S_OK;
}

void CBerserker::Set_HitState()
{
	CBerserkerState* pState = new CBattle_Damage_LargeB_State(this, true, m_bBerserkerMode, true, CBerserkerState::STATE_ID::STATE_TAKE_DAMAGE);
	m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);

}

void CBerserker::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

CBerserker * CBerserker::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBerserker* pInstance = new CBerserker(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CBerserker"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBerserker::Clone(void * pArg)
{
	CBerserker* pInstance = new CBerserker(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CBerserker"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBerserker::Free()
{
	__super::Free();

	Safe_Delete(m_pBerserkerState);
}
