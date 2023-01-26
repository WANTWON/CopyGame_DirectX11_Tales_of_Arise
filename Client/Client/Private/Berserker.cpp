#include "stdafx.h"
#include "Berserker.h"
#include "BerserkerState.h"
#include "BerserkerIdleState.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_HowLingState.h"
#include "BerserkerBattle_Damage_LargeB_State.h"
#include "BerserkerBattle_DeadState.h"

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
	CBerserkerState* pState = new CBattle_HowLingState(this);
	m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);

	/* Set Binary */
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//char cName[MAX_PATH];
	//ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//pData_Manager->TCtoC(TEXT("Berserker"), cName);
	//pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	//RELEASE_INSTANCE(CData_Manager);
	//RELEASE_INSTANCE(CGameInstance);


	m_tInfo.iMaxHp = 3;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;
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

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_BATTLE, TEXT("Berserker"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 1.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_BATTLE, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
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
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);
	TickState(fTimeDelta);

	return OBJ_NOEVENT;
}

void CBerserker::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return;
	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	LateTickState(fTimeDelta);
}

void CBerserker::AI_Behavior(_float fTimeDelta)
{
	CBerserkerState* pNewState = m_pBerserkerState->AI_Behaviour(fTimeDelta);
	if (pNewState)
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pNewState);
}


void CBerserker::TickState(_float fTimeDelta)
{
	CBerserkerState* pNewState = m_pBerserkerState->Tick(fTimeDelta);
	if (pNewState)
		m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pNewState);
	
}

void CBerserker::LateTickState(_float fTimeDelta)
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
	case MOVE_WALK:
		
		return true;
	

	case DASH_START:
	case DASH_LOOP:
	case HOWING_AND_BACK:
	case HOWING:
	case FLYING_ATTACK2:
	case ATTACK_DOUBLE_CLAW:
	case ATTACK_DOUBLE_CROW:
	case ATTACK_SHOCK_WAVE:
	case DAMAGE_LARGE_B:
		return false;
	}

	return false;
}

_float CBerserker::Take_Damage(float fDamage, CBaseObj * DamageCauser)
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
				m_tStats.m_fCurrentHp -= fDamage;

				m_pModelCom->Set_TimeReset();
				CBerserkerState* pState = new CBattle_Damage_LargeB_State(this);
				m_pBerserkerState = m_pBerserkerState->ChangeState(m_pBerserkerState, pState);
			}
	}

	return fDamage;
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
