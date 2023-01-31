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

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));

	/* Set State */
	CBerserkerState* pState = new CIdleState(this, CBerserkerState::FIELD_STATE_ID::FIELD_STATE_IDLE);
	m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);

	m_eMonsterID = BERSERKER;


	m_tInfo.fMaxHp = 3;
	m_tInfo.fCurrentHp = m_tInfo.fMaxHp;
	m_tInfo.iDamage = 10;


	_vector vPosition = *(_vector*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return S_OK;
}



HRESULT CBerserker::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	if(false == m_bOnAngry)
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
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 1.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;


	/* For.Com_Obb*/
	CCollider::COLLIDERDESC ObbColliderDesc;
	ZeroMemory(&ObbColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ObbColliderDesc.vScale = _float3(7.f, 3.5f, 3.f);
	ObbColliderDesc.vPosition = _float3(0.f, 2.28f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ObbColliderDesc)))
		return E_FAIL;


	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

int CBerserker::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || !Check_IsinFrustum(2.f))
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;

	AI_Behavior(fTimeDelta);

	if (true == m_bBattleMode && false == m_bDoneChangeState)
	{
		CBerserkerState* pState = new CBattle_IdleState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
		m_bDoneChangeState = true;
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_K))
	{
		CBerserkerState* pState = new CBattle_Double_CrowState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_L))
	{
		CBerserkerState* pState = new CBattle_Double_ClawState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_H))
	{
		CBerserkerState* pState = new CBattle_Double_CrowState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_J))
	{
		CBerserkerState* pState = new CBattle_Double_ClawState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_H))
	{
		CBerserkerState* pState = new CBattle_Quadruple_ClawState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_J))
	{
		CBerserkerState* pState = new CBattle_DashStartState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_P))
	{
		CBerserkerState* pState = new CBattle_PouncingState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_O))
	{
		CBerserkerState* pState = new CBattle_Shock_WaveState(this);
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
	}

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	__super::Tick(fTimeDelta);
	Tick_State(fTimeDelta);

	return OBJ_NOEVENT;
}

void CBerserker::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
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
		return false;
	}

	return false;
}

_int CBerserker::Take_Damage(int fDamage, CBaseObj * DamageCauser)
{
	if (fDamage > 0.f)
	{
			if (m_tStats.m_fCurrentHp - fDamage <= 0.f)
			{
				m_tStats.m_fCurrentHp = 0.f;

				m_pModelCom->Set_TimeReset(); 
				CBerserkerState* pState = new CBattle_DeadState(this);
				m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
			}
			else
			{
				if (false == m_bOnGoingHowLing || false == m_bOnGoingAnim)
				{
					m_tStats.m_fCurrentHp -= fDamage;

					m_pModelCom->Set_TimeReset();
					CBerserkerState* pState = new CBattle_Damage_LargeB_State(this, m_bOnAngry);
					m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
				}
			}
	}

	return fDamage;
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

	if (m_fWalkingHeight > XMVectorGetY(vPosition))
	{
		vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
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
