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
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerWalkState.h"
#include "BerserkerBattle_TurnState.h"
#include "BerserkerBattle_FireBallState.h"
#include "BerserkerBattle_Multiple_FireState.h"
#include "BerserkerBattle_RunState.h"


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
		CBerserkerState* pBattleState = new CBattle_IdleState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pBattleState);
	}
	else
	{
		/* Set State */
		CBerserkerState* pState = new CWalkState(this, CBerserkerState::FIELD_STATE_ID::FIELD_STATE_END);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	
	m_tStats.m_fMaxHp = 200;
	m_tStats.m_fCurrentHp = m_tStats.m_fMaxHp;
	m_tStats.m_fAttackPower = 10;
	m_eMonsterID = BERSERKER;

	NONANIMDESC ModelDesc;
	if (pArg != nullptr)
		memcpy(&ModelDesc, pArg, sizeof(NONANIMDESC));

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(ModelDesc.vScale);

		if (ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMLoadFloat3(&ModelDesc.vRotation), XMConvertToRadians(ModelDesc.m_fAngle));
	}
	
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 2.0f);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, 2.0f);
	//m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, 2.0f);

	//생성 시작부터 트리거 박스 세팅하기 , 만약 배틀존일때는 트리거 박스가 없어서 nullptr임
	Check_NearTrigger();

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));
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


	/////* For.Com_Obb*/
	//CCollider::COLLIDERDESC ObbColliderDesc;
	//ZeroMemory(&ObbColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//ObbColliderDesc.vScale = _float3(7.f, 3.5f, 3.f);
	//ObbColliderDesc.vPosition = _float3(0.f, 2.28f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ObbColliderDesc)))
	//	return E_FAIL;


	/* For.Com_Navigation */
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

	//이전코드
	//CNavigation::NAVIDESC NaviDesc;
	//ZeroMemory(&NaviDesc, sizeof NaviDesc);
	//if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
	//	return E_FAIL;

	return S_OK;
}

int CBerserker::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();
	AI_Behavior(fTimeDelta);
	Tick_State(fTimeDelta);


	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	

	return OBJ_NOEVENT;
}

void CBerserker::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return;
	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return;
	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	LateTick_State(fTimeDelta);
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
		return false;
	}

	return false;
}

_int CBerserker::Take_Damage(int fDamage, CBaseObj * DamageCauser)
{
	if (fDamage <= 0 || m_bDead)
		return 0;
	
	m_iBeDamaged_Cnt++;

	_int iHp = __super::Take_Damage(fDamage, DamageCauser);

			if (iHp <= 0)
			{
				m_pModelCom->Set_TimeReset(); 
				CBerserkerState* pState = new CBattle_DeadState(this);
				m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);

				return 0;
			}
			else
			{
				++m_iBeDamaged_Cnt;

				if (m_bOnGoingHowLing == false)
				{
					if (iHp <= 160)
					{
						_bool pAngry = true;

						m_pModelCom->Set_TimeReset();
						CBerserkerState* pState = new CBattle_Damage_LargeB_State(this, pAngry, m_bBerserkerMode);
						m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);


						if (m_iBeDamaged_Cnt >= 3)
						{
							m_pModelCom->Set_TimeReset();
							CBerserkerState* pState = new CBattle_Damage_LargeB_State(this, true, m_bBerserkerMode, true);
							m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
							m_iBeDamaged_Cnt = 0;
						}
					}
					
					else
					{
						m_pModelCom->Set_TimeReset();
						CBerserkerState* pState = new CBattle_Damage_LargeB_State(this, false, m_bBerserkerMode);
						m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
					}



				}

				else
					return iHp;
			
			}

			return iHp;
}

HRESULT CBerserker::SetUp_ShaderID()
{
	if (false == m_bDissolve)
		m_eShaderID = SHADER_ANIMDEFAULT;

	else
		m_eShaderID = SHADER_ANIM_DISSLOVE;

	return S_OK;
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
