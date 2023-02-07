#include "stdafx.h"
#include "..\Public\SnowFieldNpc.h"
#include "GameInstance.h"
#include "Player.h"
//#include "Level_Manager.h"
#include "PlayerManager.h"
#include "UI_Manager.h"
#include "UI_Dialogue.h"
#include "UI_InterectMsg.h"

CSnowFieldNpc::CSnowFieldNpc(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNpc(pDevice, pContext)
{
}

CSnowFieldNpc::CSnowFieldNpc(const CSnowFieldNpc & rhs)
	: CNpc(rhs)
{
}

HRESULT CSnowFieldNpc::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CSnowFieldNpc::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	switch (m_NpcDesc.eNpcType)
	{
	case FEMALE_YOUNG:
		m_eState = HURT;
		break;
	case MAN_OLD:
		m_eState = ANIM_OLD_MAN::CROSSONESARMS_LP;
		break;
	case MAN_GLD:
		m_eState = ANIM_MAN_GLD::TALKING;
		break;
	case MAN_PLC:
		m_eState = ANIM_MAN_PLC::IDLE;
		break;
	}
	
	m_pModelCom->Set_CurrentAnimIndex(m_eState);

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));
	Check_Navigation();

	return S_OK;
}


int CSnowFieldNpc::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || Check_IsinFrustum(2.f) == false)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);
	Tick_State(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	return OBJ_NOEVENT;
}

void CSnowFieldNpc::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
   LateTick_State(fTimeDelta);

   CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
   if (!pPlayer)
	   return;

   m_bCollision = m_pSPHERECom->Collision(pPlayer->Get_Collider());

   if (m_bCollision)
   {
	   dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Open_sysmsg(2);
	   m_bIsFirst_conversation = true;

	   if (CGameInstance::Get_Instance()->Key_Up(DIK_E))
			Talk_with_Npc(); 
		   
   }

   if (m_bIsFirst_conversation && !m_bCollision)
   {
	   m_bIsFirst_conversation = false;
	   dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();
   }
}


_bool CSnowFieldNpc::Is_AnimationLoop(_uint eAnimId)
{

	switch (m_NpcDesc.eNpcType)
	{
	case FEMALE_YOUNG:
		switch (m_eState)
		{
		case Client::CSnowFieldNpc::CROSS1_LP:
		case Client::CSnowFieldNpc::CROSS2_LP:
		case Client::CSnowFieldNpc::GET_DOWN_KNESS:
		case Client::CSnowFieldNpc::HELP:
		case Client::CSnowFieldNpc::HI:
		case Client::CSnowFieldNpc::HURT:
		case Client::CSnowFieldNpc::PLEASE:
		case Client::CSnowFieldNpc::POSE_STAND:
		case Client::CSnowFieldNpc::RUN:
		case Client::CSnowFieldNpc::SIT:
		case Client::CSnowFieldNpc::STAND:
		case Client::CSnowFieldNpc::SCARED:
			return true;
		default:
			return false;
		}
	case MAN_OLD:
		break;
	case MAN_GLD:
		break;
	case MAN_PLC:
		break;
	}

	return true;
}

void CSnowFieldNpc::Tick_State(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta, true);
}

void CSnowFieldNpc::LateTick_State(_float fTimeDelta)
{
}

void CSnowFieldNpc::Talk_with_Npc()
{

	switch (m_NpcDesc.eNpcType)
	{
	case FEMALE_YOUNG:

		switch (CUI_Manager::Get_Instance()->Get_Dialogue_section())
		{
		case 0:
			dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(0);
			break;
		case 1:
			dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(1);
			break;
		}
		break;
	case MAN_OLD:
		break;
	case MAN_GLD:
		break;
	case MAN_PLC:
		break;
	default:
		break;
	}
}


HRESULT CSnowFieldNpc::Ready_Components(void * pArg)
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

	_tchar			szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_NpcDesc.Modeldesc.pModeltag, (int)strlen(m_NpcDesc.Modeldesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 3.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}


CSnowFieldNpc * CSnowFieldNpc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSnowFieldNpc* pInstance = new CSnowFieldNpc(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSnowFieldNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSnowFieldNpc::Clone(void * pArg)
{
	CSnowFieldNpc* pInstance = new CSnowFieldNpc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSnowFieldNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnowFieldNpc::Free()
{
	__super::Free();
}
