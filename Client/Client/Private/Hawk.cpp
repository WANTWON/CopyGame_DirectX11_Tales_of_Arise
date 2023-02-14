#include "stdafx.h"
#include "Hawk.h"
#include "HawkState.h"
#include "HawkIdleState.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
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
		CHawkState* pState = new CIdleState(this, CHawkState::FIELD_STATE_ID::FIELD_STATE_END);
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);

	}

	m_eMonsterID = HAWK;
	m_tStats.m_fMaxHp = 20000;
	m_tStats.m_fCurrentHp = m_tStats.m_fMaxHp;
	m_tStats.m_fAttackPower = 10;

	Set_Scale(_float3(0.7f,0.7f,0.7f));

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

	/* For.Com_Sphere */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	//ColliderDesc.vScale = _float3(3.f, 3.f, 3.f); ���� �ݶ��̴� ������ 6�� �ʹ� ū��, 3���� �ϸ� �ǰ��̾ȵ�
	ColliderDesc.vPosition = _float3(0.f, 2.28f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;


	if (FAILED(__super::Add_Components(TEXT("Com_FieldNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_vecNavigation[LEVEL_SNOWFIELD])))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_BattleNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_vecNavigation[LEVEL_BATTLE])))
		return E_FAIL;

	m_pNavigationCom = m_vecNavigation[iLevel];


	return S_OK;
}

int CHawk::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();

	m_eLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
	if (CUI_Manager::Get_Instance()->Get_StopTick() || m_eLevel == LEVEL_LOADING)
		return OBJ_NOEVENT;
	if(!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return OBJ_NOEVENT;
	
	if (m_pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(m_pCameraManager->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	
	AI_Behaviour(fTimeDelta);
	Tick_State(fTimeDelta);

	//RELEASE_INSTANCE(CCollision_Manager);
	
	if (CGameInstance::Get_Instance()->Key_Up(DIK_L))
	{
		CHawkState* pState = new CBattle_TornadeState(this);
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);
	}

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());


	return OBJ_NOEVENT;
}

void CHawk::Late_Tick(_float fTimeDelta)
{
	m_eLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
	if (CUI_Manager::Get_Instance()->Get_StopTick() || m_eLevel == LEVEL_LOADING)

		return;
	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return;

	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom && m_bGlowUp)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	if (m_pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(m_pCameraManager->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		return;

	LateTick_State(fTimeDelta);
}

HRESULT CHawk::Render_Glow()
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

		if (fDamage <= 0 || m_bDead || m_bTakeDamage)
		return 0;

		_int iHp = __super::Take_Damage(fDamage, DamageCauser);

		if (m_bDead == false)
		{
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
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 2.f);
	if (m_pHawkState->Get_StateId() == CHawkState::STATE_DEAD)
		m_fWalkingHeight -= 2.f;
	

	vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CHawk::Set_BattleMode(_bool type)
{
	m_bBattleMode = type;
	if (m_bBattleMode)
	{
		/*Set_Battle State*/
		CHawkState* pState = new CBattle_IdleState(this, CHawkState::STATE_ID::START_BATTLEMODE);
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);
	}
	else
	{
		Check_NearTrigger();
		CHawkState* pState = new CIdleState(this, CHawkState::FIELD_STATE_ID::FIELD_STATE_END);
		m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);

	}
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);

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
