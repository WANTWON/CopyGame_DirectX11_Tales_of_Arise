#include "stdafx.h"
#include "Slime.h"
#include "SlimeState.h"
#include "SlimeIdleState.h"
#include "SlimeBattle_RunState.h"

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
	CSlimeState* pState = new CIdleState(this);
	m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pState);

	///* Set Binary */
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//char cName[MAX_PATH];
	//ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//pData_Manager->TCtoC(TEXT("Hawk"), cName);
	//pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	//RELEASE_INSTANCE(CData_Manager);
	//RELEASE_INSTANCE(CGameInstance);

	m_tInfo.iMaxHp = 3;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;
	m_tInfo.iDamage = 10;


	_vector vPosition = *(_vector*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 2.0f);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, 2.0f);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, 2.0f);

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

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_BATTLE, TEXT("Slime"), (CComponent**)&m_pModelCom)))
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
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_BATTLE, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

int CSlime::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);
	AI_Behaviour(fTimeDelta);
	TickState(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CSlime::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	LateTickState(fTimeDelta);
}

HRESULT CSlime::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	//
	/*_bool bGlow = true;
	if (FAILED(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool))))
		return E_FAIL;*/
	//
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		//
		/*if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
			return E_FAIL;*/
		//

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}

	//
	/*bGlow = false;
	if (FAILED(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool))))
		return E_FAIL;*/
	//

	return S_OK;
}

void CSlime::AI_Behavior(_float fTimeDelta)
{
	CSlimeState* pNewState = m_pSlimeState->AI_Behaviour(fTimeDelta);
	if (pNewState)
		m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pNewState);
}


void CSlime::TickState(_float fTimeDelta)
{
	CSlimeState* pNewState = m_pSlimeState->Tick(fTimeDelta);
	if (pNewState)
		m_pSlimeState = m_pSlimeState->ChangeState(m_pSlimeState, pNewState);
	
}

void CSlime::LateTickState(_float fTimeDelta)
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
		return false;

	}

	return false;
}

_float CSlime::Take_Damage(float fDamage, CBaseObj * DamageCauser)
{
	//if (fDamage > 0.f)
	//{
	//		if (m_tStats.m_fCurrentHp - fDamage <= 0.f)
	//		{
	//			m_tStats.m_fCurrentHp = 0.f;

	//			m_pModelCom->Set_TimeReset();
	//			CHawkState* pState = new CBattle_DeadState(this);
	//			m_pSlimeState = m_pHawkState->ChangeState(m_pHawkState, pState);
	//		}
	//		else
	//		{
	//			m_tStats.m_fCurrentHp -= fDamage;

	//			m_pModelCom->Set_TimeReset();
	//			CHawkState* pState = new CBattle_Damage_LargeB_State(this);
	//			m_pSlimeState = m_pHawkState->ChangeState(m_pHawkState, pState);
	//		}

	////else
	////{
	////	m_tStats.m_fCurrentHp -= fDamage;

	////	m_pModelCom->Set_TimeReset();
	////	CHawkState* pState = new CBattle_Damage_LargeB_State(DamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION));
	////	m_pHawkState = m_pHawkState->ChangeState(m_pHawkState, pState);
	////}
	//}

	return fDamage;
}

void CSlime::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	if (m_fWalkingHeight > XMVectorGetY(vPosition))
	{
		vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
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
