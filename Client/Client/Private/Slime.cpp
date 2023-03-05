#include "stdafx.h"
#include "Slime.h"
#include "SlimeState.h"
#include "SlimeIdleState.h"
#include "SlimeBattle_RunState.h"
#include "SlimeBattle_TakeDamage_State.h"
#include "SlimeBattle_DeadState.h"
#include "SlimeBattle_IdleState.h"

using namespace Slime;

CSlime::CSlime(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CMonster(pDevice, pContext)
{
}

CSlime::CSlime(const CSlime & rhs)
	: CMonster(rhs)
{
}


HRESULT CSlime::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSlime::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));

	/* Set State */
	CSlimeState* pState = new CIdleState(this, CSlimeState::FIELD_STATE_ID::FIELD_STATE_IDLE);
	m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pState);


	m_tInfo.fMaxHp = 3;
	m_tInfo.fCurrentHp = m_tInfo.fMaxHp;

	m_tInfo.iDamage = 10;


	_vector vPosition = *(_vector*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 2.0f);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, 2.0f);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, 2.0f);

	//생성 시작부터 트리거 박스 세팅하기 , 만약 배틀존일때는 트리거 박스가 없어서 nullptr임
	Check_NearTrigger();

	return S_OK;
}



HRESULT CSlime::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Slime"), (CComponent**)&m_pModelCom)))
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
	ColliderDesc.vScale = _float3(1.1f, 1.1f, 1.1f);
	ColliderDesc.vPosition = _float3(0.f, 0.3f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(7.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

int CSlime::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	if (m_bDead)
		return OBJ_DEAD;

	if (true == m_bBattleMode && false == m_bDoneChangeState)
	{
		CSlimeState* pState = new CBattle_IdleState(this);
		m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pState);
		m_bDoneChangeState = true;
	}

	__super::Tick(fTimeDelta);
	AI_Behaviour(fTimeDelta);
	Tick_State(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CSlime::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;
	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom && m_bGlowUp)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	LateTick_State(fTimeDelta);
}

HRESULT CSlime::Render_Glow()
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

void CSlime::Set_BattleMode(_bool type)
{
}

//HRESULT CSlime::Render()
//{
//	if (nullptr == m_pShaderCom ||
//		nullptr == m_pModelCom)
//		return E_FAIL;
//
//	if (FAILED(SetUp_ShaderResources()))
//		return E_FAIL;
//
//	if (FAILED(SetUp_ShaderID()))
//		return E_FAIL;
//
//	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
//	//
//	_bool bGlow = true;
//	if (FAILED(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool))))
//		return E_FAIL;
//	
//	for (_uint i = 0; i < iNumMeshes; ++i)
//	{
//		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
//			return E_FAIL;
//
//		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
//			return E_FAIL;
//
//		//
//		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
//			return E_FAIL;
//		//
//
//		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
//			return E_FAIL;
//	}
//
//	
//	bGlow = false;
//	if (FAILED(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool))))
//		return E_FAIL;
//	
//
//	return S_OK;
//}

void CSlime::AI_Behavior(_float fTimeDelta)
{
	CSlimeState* pNewState = m_pSlimeState->AI_Behaviour(fTimeDelta);
	if (pNewState)
		m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pNewState);
}


void CSlime::Tick_State(_float fTimeDelta)
{
	CSlimeState* pNewState = m_pSlimeState->Tick(fTimeDelta);
	if (pNewState)
		m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pNewState);
	
}

void CSlime::LateTick_State(_float fTimeDelta)
{
	CSlimeState* pNewState = m_pSlimeState->LateTick(fTimeDelta);
	if (pNewState)
		m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pNewState);
}


_bool CSlime::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{
	case SYMBOL_IDLE:
	case SYMBOL_DETECT_IDLE:
	case MOVE_WALK_F:
	case MOVE_RUN:
	case MOVE_IDLE:
		return true;
	
	case ATTACK_TACKLE:
	case DOWN_F:
	case ARISE_F:
	case ATTACK_POISON_SHOT:
	case ATTACK_POISON_RAIN:
	case DAMAGE_LARGE_B:
	case DEAD:
	case TURN_R:
	case TURN_L:
	case MAGIC_EMIT:
	case ADVENT:
		return false;

	}

	return false;
}

_int CSlime::Take_Damage(int fDamage, CBaseObj * DamageCauser, HITLAGDESC HitDesc)
{
	if (fDamage <= 0 || m_bDead)
		return 0;

	_int iHp = __super::Take_Damage(fDamage, DamageCauser, HitDesc);

	if (iHp <= 0)
	{
		m_pModelCom->Set_TimeReset();
		CSlimeState* pState = new CBattle_DeadState(this);
		m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pState);

		return 0;
	}

	else
	{
		m_pModelCom->Set_TimeReset();
		CSlimeState* pState = new CBattle_TakeDamage_State(this);
		m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pState);
	}

	return iHp;
}

HRESULT CSlime::SetUp_ShaderID()
{
	if (false == m_bDissolve)
		m_eShaderID = SHADER_ANIMDEFAULT;

	else
		m_eShaderID = SHADER_ANIM_DISSOLVE;

	return S_OK;
}

void CSlime::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

CSlime * CSlime::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSlime* pInstance = new CSlime(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSlime::Clone(void * pArg)
{
	CSlime* pInstance = new CSlime(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSlime"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSlime::Free()
{
	__super::Free();

	Safe_Delete(m_pSlimeState);
}
