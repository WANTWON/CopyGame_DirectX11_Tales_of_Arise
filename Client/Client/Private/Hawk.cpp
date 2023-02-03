#include "stdafx.h"
#include "Hawk.h"
#include "HawkState.h"
#include "HawkIdleState.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_RevolveState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_Damage_LargeB_State.h"
#include "HawkBattle_DeadState.h"
#include "HawkSitOnState.h"
#include "HawkBattle_DashState.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_TornadeState.h"
#include "HawkBattle_PeckState.h"

using namespace Hawk;

CHawk::CHawk(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CMonster(pDevice, pContext)
{
}

CHawk::CHawk(const CHawk & rhs)
	: CMonster(rhs)
{
}


HRESULT CHawk::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHawk::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();


	if (m_bBattleMode)
	{
		/*Set_Battle State*/
		CHawkState* pState = new CBattle_IdleState(this, CHawkState::STATE_ID::START_BATTLEMODE);
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);
	}
	else
	{
		//테스트코드///
		/* Set State */
		CHawkState* pState = new CIdleState(this, CHawkState::FIELD_STATE_ID::FIELD_STATE_END);
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);


		//원본코드 //
		///* Set State */
		//CHawkState* pState = new CSitOnState(this);
		//m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);
	}

	m_eMonsterID = HAWK;


	m_tStats.m_fMaxHp = 200;
	m_tStats.m_fCurrentHp = m_tStats.m_fMaxHp;
	m_tStats.m_fAttackPower = 10;

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
	
	//생성 시작부터 트리거 박스 세팅하기 , 만약 배틀존일때는 트리거 박스가 없어서 nullptr임
	Check_NearTrigger();

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));
	return S_OK;
}



HRESULT CHawk::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 6.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Hawk"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	///* For.Com_Navigation */
	//CNavigation::NAVIDESC NaviDesc;
	//ZeroMemory(&NaviDesc, sizeof NaviDesc);
	//if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
	//	return E_FAIL;

	/* For.Com_Sphere */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	//ColliderDesc.vScale = _float3(3.f, 3.f, 3.f); 지금 콜라이더 사이즈 6은 너무 큰데, 3으로 하면 피격이안됨
	ColliderDesc.vPosition = _float3(0.f, 2.28f, 0.f);
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


	/////* For.Com_Obb*/
	//CCollider::COLLIDERDESC ObbColliderDesc;
	//ZeroMemory(&ObbColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//ObbColliderDesc.vScale = _float3(7.f, 3.5f, 3.f);
	//ObbColliderDesc.vPosition = _float3(0.f, 2.28f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ObbColliderDesc)))
	//	return E_FAIL;



	return S_OK;
}

int CHawk::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();

	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	if(!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return OBJ_NOEVENT;
	

	__super::Tick(fTimeDelta);

	
	AI_Behaviour(fTimeDelta);
	Tick_State(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());


	return OBJ_NOEVENT;
}

void CHawk::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;
	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return;

	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	LateTick_State(fTimeDelta);
}

void CHawk::AI_Behavior(_float fTimeDelta)
{
	CHawkState* pNewState = m_pHawkState->AI_Behaviour(fTimeDelta);
	if (pNewState)
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pNewState);
}


void CHawk::Tick_State(_float fTimeDelta)
{
	CHawkState* pNewState = m_pHawkState->Tick(fTimeDelta);
	if (pNewState)
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pNewState);
	
}

void CHawk::LateTick_State(_float fTimeDelta)
{
	CHawkState* pNewState = m_pHawkState->LateTick(fTimeDelta);
	if (pNewState)
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pNewState);
}


_bool CHawk::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{
	case SYMBOL_IDLE:
	case MOVE_RUN:
	case ATTACK_GRAB_LOOP:
	case SYMBOL_RUN:
	case MOVE_WALK_F:
	case SYMBOL_STOP:
		return true;

	case ATTACK_FLUTTER:
	case ATTACK_DASH:
	case ATTACK_PECK:
	case ATTACK_GRAB_END:
	case ATTACK_GRAB_START:
	case ATTACK_TORNADE:
	case TURN_R:
	case ATTACK_BRAVE:
	case SYMBOL_DETECT_STOP:
	case ATTACK_CHARGE:
		return false;
	}

	return false;
}

_int CHawk::Take_Damage(int fDamage, CBaseObj * DamageCauser)
{

		if (fDamage <= 0 || m_bDead)
		return 0;

		_int iHp = __super::Take_Damage(fDamage, DamageCauser);

	if (iHp <= 0)
	{
		m_pModelCom->Set_TimeReset();
		CHawkState* pState = new CBattle_DeadState(this);
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);
		
		
		return 0;
	}
	else
	{
		m_pModelCom->Set_TimeReset();
		CHawkState* pState = new CBattle_Damage_LargeB_State(this);
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);
	}

	return iHp;

	
}

HRESULT CHawk::SetUp_ShaderID()
{
	if (false == m_bDissolve)
		m_eShaderID = SHADER_ANIMDEFAULT;

	else
		m_eShaderID = SHADER_ANIM_DISSOLVE;

	return S_OK;
}

void CHawk::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

CHawk * CHawk::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHawk* pInstance = new CHawk(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHawk"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHawk::Clone(void * pArg)
{
	CHawk* pInstance = new CHawk(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHawk"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHawk::Free()
{
	__super::Free();

	Safe_Delete(m_pHawkState);
}
