#include "stdafx.h"
#include "..\Public\FemaleYoung.h"
#include "GameInstance.h"
#include "Player.h"
//#include "Level_Manager.h"
#include "PlayerManager.h"
#include "UI_Manager.h"
#include "UI_Dialogue.h"
#include "UI_InterectMsg.h"

CFemaleYoung::CFemaleYoung(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNpc(pDevice, pContext)
{
}

CFemaleYoung::CFemaleYoung(const CFemaleYoung & rhs)
	: CNpc(rhs)
{
}

HRESULT CFemaleYoung::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CFemaleYoung::Initialize(void * pArg)
{
	__super::Initialize(pArg);
	m_pModelCom->Set_CurrentAnimIndex(m_eState);

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));
	Check_Navigation();

	return S_OK;
}


int CFemaleYoung::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || Check_IsinFrustum(2.f) == false)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);
	Tick_State(fTimeDelta);

	m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	return OBJ_NOEVENT;
}

void CFemaleYoung::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
   LateTick_State(fTimeDelta);

   CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
   if (!pPlayer)
	   return;

   m_bCollision = m_pOBBCom->Collision(pPlayer->Get_Collider());

   if (m_bCollision)
   {
	   dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Open_sysmsg(2);
	   m_bIsFirst_conversation = true;

	   if (CGameInstance::Get_Instance()->Key_Up(DIK_E))
	   {
		   switch (CUI_Manager::Get_Instance()->Get_Dialogue_section())
		   {
		   case 0:
			   dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(0);
			   break;
		   case 1:
			   dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(1);
			   break;
		   }
	   }
		   
   }

   if (m_bIsFirst_conversation && !m_bCollision)
   {
	   m_bIsFirst_conversation = false;
	   dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();
   }
}


_bool CFemaleYoung::Is_AnimationLoop(_uint eAnimId)
{
	switch (m_eState)
	{
	case Client::CFemaleYoung::AVOID_ED:
	case Client::CFemaleYoung::AVOID_LP:
	case Client::CFemaleYoung::COLD:
	case Client::CFemaleYoung::CROSS1_ED:
	case Client::CFemaleYoung::AVOID_ST:
	case Client::CFemaleYoung::CROSS2_ED:
	case Client::CFemaleYoung::CROSS2_ST:
	case Client::CFemaleYoung::CROSS1_ST:
	case Client::CFemaleYoung::CROSS_ANS_TALK:
	case Client::CFemaleYoung::CRY:
	case Client::CFemaleYoung::DOWN:
	case Client::CFemaleYoung::GREET:
	case Client::CFemaleYoung::LAUGH:
	case Client::CFemaleYoung::LOOK_AROUND:
	case Client::CFemaleYoung::TURNBACK:
	case Client::CFemaleYoung::TURNLEFT:
	case Client::CFemaleYoung::TURNRIGJT:
	case Client::CFemaleYoung::LOOK_AROUND2:
	case Client::CFemaleYoung::WALK:
	case Client::CFemaleYoung::WALK2:
		return false;
	case Client::CFemaleYoung::CROSS1_LP:
	case Client::CFemaleYoung::CROSS2_LP:
	case Client::CFemaleYoung::GET_DOWN_KNESS:
	case Client::CFemaleYoung::HELP:
	case Client::CFemaleYoung::HI:
	case Client::CFemaleYoung::HURT:
	case Client::CFemaleYoung::PLEASE:
	case Client::CFemaleYoung::POSE_STAND:
	case Client::CFemaleYoung::RUN:
	case Client::CFemaleYoung::SIT:
	case Client::CFemaleYoung::STAND:
	case Client::CFemaleYoung::SCARED:
		return true;
	}
	return false;
}

void CFemaleYoung::Tick_State(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta, true);
}

void CFemaleYoung::LateTick_State(_float fTimeDelta)
{
}


HRESULT CFemaleYoung::Ready_Components(void * pArg)
{

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 2.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("NpcFemaleYoung"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	/* For.Com_OBB */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 4.5f, 1.f);
	ColliderDesc.vPosition = _float3(0.f, 2.28f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}


CFemaleYoung * CFemaleYoung::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFemaleYoung* pInstance = new CFemaleYoung(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CFemaleYoung"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFemaleYoung::Clone(void * pArg)
{
	CFemaleYoung* pInstance = new CFemaleYoung(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CFemaleYoung"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFemaleYoung::Free()
{
	__super::Free();
}
