#include "stdafx.h"
#include "Ice_Wolf.h"
#include "IceWolfState.h"
#include "IceWolfIdleState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_Damage_LargeB_State.h"
#include "IceWolfBattle_DeadState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfBattle_HowLingState.h"
#include "IceWolfBattle_RunState.h"


using namespace IceWolf;

CIce_Wolf::CIce_Wolf(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CMonster(pDevice, pContext)
{
}

CIce_Wolf::CIce_Wolf(const CIce_Wolf & rhs)
	: CMonster(rhs)
{
}


HRESULT CIce_Wolf::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIce_Wolf::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();

	if (m_bBattleMode)
	{
		/*Set_Battle State*/
		CIceWolfState* pBattleState = new CBattle_IdleState(this, CIceWolfState::STATE_ID::START_BATTLE);
		m_pState = m_pState->ChangeState(m_pState, pBattleState);
	}
	else
	{
		/* Set State */
		CIceWolfState* pState = new CIdleState(this, CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}


	m_eMonsterID = ICE_WOLF;


	m_tStats.m_fMaxHp = 200.f;
	m_tStats.m_fCurrentHp = m_tStats.m_fMaxHp;
	m_tStats.m_fAttackPower = 10.f;
	m_tStats.m_fWalkSpeed = 0.05f;
	m_tStats.m_fRunSpeed = 5.f;

	m_fRadius = 1.f;


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

	//���� ���ۺ��� Ʈ���� �ڽ� �����ϱ� , ���� ��Ʋ���϶��� Ʈ���� �ڽ��� ��� nullptr��
	Check_NearTrigger();


	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), 90.f);
	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));

	m_fTimeDletaAcc = 0;
	m_fCntChanceTime = ((rand() % 1000) *0.001f)*((rand() % 100) * 0.01f);
	m_bDone_HitAnimState = false;
	return S_OK;
}



HRESULT CIce_Wolf::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Ice_Wolf"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	///* For.Com_OBB */
	//CCollider::COLLIDERDESC ColliderDesc;
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//ColliderDesc.vScale = _float3(7.f, 3.f, 3.f);
	//ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	//ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
	//	return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
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

int CIce_Wolf::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
		

	if (CUI_Manager::Get_Instance()->Get_StopTick() || !Check_IsinFrustum(2.f))
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	m_bBattleMode =  CBattleManager::Get_Instance()->Get_IsBattleMode();

	
	AI_Behaviour(fTimeDelta);
	Tick_State(fTimeDelta);

	m_fTimeDletaAcc += fTimeDelta;

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	//m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	if (m_fTimeDletaAcc > m_fCntChanceTime)
		m_iRand = rand() % 3;




	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_I))
	{
		CIceWolfState* pState = new CAttack_Elemental_Charge(this, CIceWolfState::STATE_ID::STATE_END);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_O))
	{
		CIceWolfState* pState = new CAttackBiteState(this);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_H))
	{
		CIceWolfState* pState = new CBattle_Damage_LargeB_State(this, false);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_J))
	{
		CIceWolfState* pState = new CAttackNormalState(this);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_K))
	{
		CIceWolfState* pState = new CBattle_SomerSaultState(this);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_L))
	{
		CIceWolfState* pState = new CBattle_SomerSaultState(this);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_N))
	{
		CIceWolfState* pState = new CBattle_RunState(this, CIceWolfState::STATE_ID::STATE_END);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}*/



	return OBJ_NOEVENT;
}

void CIce_Wolf::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || !Check_IsinFrustum(2.f))
		return;

	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	/*if (m_bBattleMode)
	{
		LateTick_BattleState(fTimeDelta);
	}
	else*/
//	{
		LateTick_State(fTimeDelta);
//	}

}

HRESULT CIce_Wolf::Render_Glow()
{
	if (!m_pShaderCom || !m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (!m_bDead)
	{
		_bool bGlow = true;
		if (FAILED(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool))))
			return E_FAIL;
	}

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (!m_bDead)
		{
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
				return E_FAIL;
		}

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))
			return E_FAIL;
	}

	if (!m_bDead)
	{
		_bool bGlow = false;
		if (FAILED(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool))))
			return E_FAIL;
	}

	return S_OK;
}

void CIce_Wolf::AI_Behavior(_float fTimeDelta)
{
	CIceWolfState* pNewState = m_pState->AI_Behaviour(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);
}


void CIce_Wolf::Tick_State(_float fTimeDelta)
{
	
	CIceWolfState* pNewState = m_pState->Tick(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);
	
}

void CIce_Wolf::LateTick_State(_float fTimeDelta)
{
	CIceWolfState* pNewState = m_pState->LateTick(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);
}


_bool CIce_Wolf::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{
	case ANIM_MOVE_IDLE:
	case ANIM_MOVE_RUN:
	case ANIM_MOVE_WALK_F:
	
		return true;

	case ANIM_TURN_L:
	case ANIM_ATTACK_HOWLING:
	case ANIM_TURN_R:
	case ANIM_ATTACK_NORMAL:
	case ANIM_ATTACK_ELEMENTAL_CHARGE_START:
	case ANIM_ATTACK_ELEMENTAL_CHARGE_LOOP:
	case ANIM_ATTACK_ELEMENTAL_CHARGE_END:
	case ANIM_SYMBOL_DETECT_IDLE:
	case ANIM_DAMAGE_AIR_LARGE_B:
	case ANIM_DEAD:
	case ANIM_ATTACK_STEP_BACK:
	case ANIM_ATTACK_SOMERSAULT_END:
	case ANIM_ATTACK_BITE:
		return false;
	}

	return false;
}

_int CIce_Wolf::Take_Damage(int fDamage, CBaseObj * DamageCauser)
{

	if (fDamage <= 0 || m_bDead || m_bDissolve || m_tStats.m_fCurrentHp <= 0.f)
		return 0; 

	_int iHp = __super::Take_Damage(fDamage, DamageCauser);

	if (iHp <= 0)
	{
		m_pModelCom->Set_TimeReset();
		CIceWolfState* pState = new CBattle_DeadState(this);
		m_pState = m_pState->ChangeState(m_pState, pState);
		
		return 0;
	}

	else
	{

		m_iBeDamaged_Cnt++;

		if (m_bOnGoing_Bite == false)
		{
			if (m_bSomeSauling == false)
			{
				//m_pModelCom->Set_TimeReset();
				CIceWolfState* pState = new CBattle_Damage_LargeB_State(this);
				m_pState = m_pState->ChangeState(m_pState, pState);


			}
			if (m_iBeDamaged_Cnt >= 3)
			{

				m_pModelCom->Set_TimeReset();
				CIceWolfState* pState = new CBattle_Damage_LargeB_State(this, true);
				m_pState = m_pState->ChangeState(m_pState, pState);
				m_iBeDamaged_Cnt = 0;
				m_bSomeSauling = true;

			}
		}
		else
			return iHp;

	}

	return iHp;
}

HRESULT CIce_Wolf::SetUp_ShaderID()
{
	if (false == m_bDissolve)
		m_eShaderID = SHADER_ANIMDEFAULT;

	else
		m_eShaderID = SHADER_ANIM_DISSLOVE;

	return S_OK;
}

void CIce_Wolf::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

		vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	
}

CIce_Wolf * CIce_Wolf::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CIce_Wolf* pInstance = new CIce_Wolf(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CIce_Wolf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CIce_Wolf::Clone(void * pArg)
{
	CIce_Wolf* pInstance = new CIce_Wolf(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CIce_Wolf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIce_Wolf::Free()
{
	__super::Free();

	Safe_Delete(m_pState);
}
