#include "stdafx.h"
#include "..\Public\Ice_Wolf.h"
#include "Ice_WolfIdleState.h"


using namespace Ice_Wolf;

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

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));

	/* Set State */
	CIce_WolfState* pState = new CIdleState(this);
	m_pIce_WolfState = m_pIce_WolfState->ChangeState(m_pIce_WolfState, pState);

	///* Set Binary */
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//char cName[MAX_PATH];
	//ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//pData_Manager->TCtoC(TEXT("Ice_Wolf"), cName);
	//pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	//RELEASE_INSTANCE(CData_Manager);
	//RELEASE_INSTANCE(CGameInstance);


	_vector vPosition = *(_vector*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}



HRESULT CIce_Wolf::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Ice_Wolf"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 1.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

int CIce_Wolf::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);
	TickState(fTimeDelta);

	return OBJ_NOEVENT;
}

void CIce_Wolf::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	LateTickState(fTimeDelta);
}

void CIce_Wolf::AI_Behavior()
{
	CIce_WolfState* pNewState = m_pIce_WolfState->AI_Behavior();
	if (pNewState)
		m_pIce_WolfState = m_pIce_WolfState->ChangeState(m_pIce_WolfState, pNewState);
}


void CIce_Wolf::TickState(_float fTimeDelta)
{
	CIce_WolfState* pNewState = m_pIce_WolfState->Tick(fTimeDelta);
	if (pNewState)
		m_pIce_WolfState = m_pIce_WolfState->ChangeState(m_pIce_WolfState, pNewState);
}

void CIce_Wolf::LateTickState(_float fTimeDelta)
{
	CIce_WolfState* pNewState = m_pIce_WolfState->LateTick(fTimeDelta);
	if (pNewState)
		m_pIce_WolfState = m_pIce_WolfState->ChangeState(m_pIce_WolfState, pNewState);
}


_bool CIce_Wolf::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{
	case ANIM_MOVE_IDLE:
		return true;
	}
}

_float CIce_Wolf::Take_Damage(float fDamage, CBaseObj * DamageCauser)
{
	if (fDamage > 0.f)
	{
			if (m_tStats.m_fCurrentHp - fDamage <= 0.f)
			{
				m_tStats.m_fCurrentHp = 0.f;

				m_pModelCom->Set_TimeReset();
			//	CIce_WolfState* pState = new CDieState(DamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION));
			//	m_pIce_WolfState = m_pIce_WolfState->ChangeState(m_pIce_WolfState, pState);
			}
			else
			{
				m_tStats.m_fCurrentHp -= fDamage;

				m_pModelCom->Set_TimeReset();
			//	CIce_WolfState* pState = new CHitState(DamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION));
			//	m_pIce_WolfState = m_pIce_WolfState->ChangeState(m_pIce_WolfState, pState);
			}
	}

	return fDamage;
}

void CIce_Wolf::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	if (m_fWalkingHeight > XMVectorGetY(vPosition))
	{
		vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
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

	Safe_Delete(m_pIce_WolfState);
}
