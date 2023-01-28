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

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));

	/* Set State */
	CHawkState* pState = new CBattle_IdleState(this);
	m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);

	///* Set Binary */
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//char cName[MAX_PATH];
	//ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//pData_Manager->TCtoC(TEXT("Hawk"), cName);
	//pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	//RELEASE_INSTANCE(CData_Manager);
	//RELEASE_INSTANCE(CGameInstance);

	m_tStats.m_fMaxHp = 3;
	m_tStats.m_fCurrentHp = m_tStats.m_fMaxHp;
	m_tStats.m_fAttackPower = 10;

	_vector vPosition = *(_vector*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	
	return S_OK;
}



HRESULT CHawk::Ready_Components(void * pArg)
{
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

	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	/* For.Com_Sphere */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 4.5f, 1.f);
	ColliderDesc.vPosition = _float3(0.f, 2.28f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

int CHawk::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return OBJ_NOEVENT;
	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);
	AI_Behaviour(fTimeDelta);
	Tick_State(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	return OBJ_NOEVENT;
}

void CHawk::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
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
		SYMBOL_IDLE,
		MOVE_RUN,
		ATTACK_GRAB_LOOP,
		SYMBOL_RUN;
		return true;
	
		ATTACK_FLUTTER,
		ATTACK_DASH,
		ATTACK_PECK,
		ATTACK_GRAB_END,
		ATTACK_GRAB_START;
		ATTACK_TORNADE;
		return false;
	}

	return false;
}

_float CHawk::Take_Damage(float fDamage, CBaseObj * DamageCauser)
{

		if (fDamage <= 0 || m_bDead)
		return 0;

	_float iHp = __super::Take_Damage(fDamage, DamageCauser);

	if (iHp <= 0)
	{
		m_pModelCom->Set_TimeReset();
		CHawkState* pState = new CBattle_DeadState(this);
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);
		
		
		return 0.f;
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
		m_eShaderID = SHADER_ANIM_DISSLOVE;

	return S_OK;
}

void CHawk::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	if (m_fWalkingHeight > XMVectorGetY(vPosition))
	{
		vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
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
