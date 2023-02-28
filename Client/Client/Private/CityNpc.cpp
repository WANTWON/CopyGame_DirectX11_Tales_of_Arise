#include "stdafx.h"
#include "..\Public\CityNpc.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerManager.h"
#include "UI_Manager.h"
#include "UI_Dialogue.h"
#include "UI_InterectMsg.h"
#include "Rinwell.h"
#include "Level.h"

CCityNpc::CCityNpc(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNpc(pDevice, pContext)
{
}

CCityNpc::CCityNpc(const CCityNpc & rhs)
	: CNpc(rhs)
{
}

HRESULT CCityNpc::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CCityNpc::Initialize(void * pArg)
{
	__super::Initialize(pArg);


	

		switch (m_NpcDesc.eNpcType)
		{
		case NPC_NFC_SLV_000_2th:
			m_eState = NPC_NFC_SLV_000_2th_ANIM::Run_2th;
			
			break;

		case NPC_NFM_SLV_000:
			m_eState = NPC_NFC_SLV_000_ANIM::Run;
			break;

		case NPC_NMM_SLV_000:
			m_eState = 1;

		case NPC_NFY_FIA_000:
			m_eState = 0;

		default:
			m_eState = 0;
			break;
		}
	



	m_pModelCom->Set_CurrentAnimIndex(m_eState);

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));
	Check_Navigation();

	return S_OK;
}


int CCityNpc::Tick(_float fTimeDelta)
{

		m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta, Is_AnimationLoop(m_pModelCom->Get_CurrentAnimIndex()), "ABone");
	



	if (m_NpcDesc.eNpcType == NPC_NFY_FIA_000)
	{
		if (m_bIsAnimationFinished)
			m_pModelCom->Set_CurrentAnimIndex(NPC_NFY_FIA_000_ANIM::FIA_Idle);
	}

	switch (m_NpcDesc.eNpcType)
	{
		
	case NPC_NFC_SLV_000:
		Find_Target();

		if (m_chaseGldFinish = false)
		{
		//	m_fChild_TimeDleta += fTimeDelta;
			_vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_pTransformCom->Sliding_Straight(fTimeDelta *0.8f, m_pNavigationCom);
			m_pTransformCom->LookAt(vTargetPos);
		}
		else
		{
			m_pModelCom->Set_CurrentAnimIndex(NPC_NFC_SLV_000_ANIM::qHappyLoop);
		}

		//if (m_fChild_TimeDleta <= 5.f)
		//{
		//	m_pModelCom->Set_CurrentAnimIndex(NPC_NFC_SLV_000_ANIM::Run);
		//	m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta, true);
		//	//_vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		//	m_pTransformCom->Sliding_Straight(fTimeDelta *0.2f, m_pNavigationCom);
		//	//m_pTransformCom->LookAt(vTargetPos);
		//}
		//
		//else
		//{
		//	m_pModelCom->Set_CurrentAnimIndex(NPC_NFC_SLV_000_ANIM::qHappyLoop);
		//	m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta, false);
		//	if (m_bIsAnimationFinished)
		//	{
		//		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180.f);
		//		m_fChild_TimeDleta = 0.f;
		//	}
		//}
		break;


	case NPC_NFC_SLV_000_2th:
		Find_MAN_GLD();
		
		if (m_fDistance >= 6.f)
		{
			m_fChildSpeed = 0.8f;
			_vector vGldPosition = m_pGld->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_pTransformCom->LookAt(vGldPosition);
			m_pTransformCom->Sliding_Straight(fTimeDelta *0.9, m_pNavigationCom);
							
		}

		else
		{
			if (m_fDistance < 6)
				m_pModelCom->Set_CurrentAnimIndex(NPC_NFC_SLV_000_2th_ANIM::Cry_Loop_2th);
			m_chaseGldFinish = true;
		}

		//Find_Target();
		//m_fChild_2thTimeDleta += fTimeDelta;
		//if (m_fChild_2thTimeDleta <= 5.f)
		//{
		//	m_pModelCom->Set_CurrentAnimIndex(NPC_NFC_SLV_000_2th_ANIM::Run_2th);
		//	m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta, true);
		//	//vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		//	m_pTransformCom->Sliding_Straight(fTimeDelta *0.2f, m_pNavigationCom);
		//	//m_pTransformCom->LookAt(vTargetPos);
		//}

		//else
		//{
		//	m_pModelCom->Set_CurrentAnimIndex(NPC_NFC_SLV_000_2th_ANIM::Cry_Loop_2th);
		//	m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta, false);
		//	if (m_bIsAnimationFinished)
		//	{
		//		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180.f);
		//		m_fChild_2thTimeDleta = 0.f;
		//	}

		//}
		break;
		//////회전 코드 
		//_vector		vPos = Get_TransformState(CTransform::STATE_TRANSLATION);
		//_vector		vLook = XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK));

		//_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		//_vector		vRight = XMVector3Cross(vAxisY, vLook);


		//_vector vDir = XMVector3Normalize(vPos - vRight);


		//vLook = XMVectorSetY(vLook, 0.f);
		//vDir = XMVectorSetY(vDir, 0.f);

		//_float fDot = XMVectorGetX(XMVector3Dot(vDir, vLook));

		//if (fDot < 0.95f)
		//	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.07f);

		////m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
		//m_pTransformCom->Sliding_Straight(fTimeDelta *0.3f, m_pNavigationCom);
		

	/*case NPC_NMM_SLV_000:
		m_pTransformCom->Go_Straight(fTimeDelta * 0.001f, m_pNavigationCom);
		break;*/

	case NPC_NFY_FIA_000:
		

	default:
		break;
	}

	if (CUI_Manager::Get_Instance()->Get_StopTick() || Check_IsinFrustum(2.f) == false)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);
	Tick_State(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	return OBJ_NOEVENT;
}

void CCityNpc::Late_Tick(_float fTimeDelta)
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
}


_bool CCityNpc::Is_AnimationLoop(_uint eAnimId)
{
	switch ((_uint)eAnimId)
	{
	//case NPC_NFC_SLV_000_2th_ANIM::Cry_Loop_2th:
	case NPC_NFC_SLV_000_ANIM::qHappyLoop:
	case NPC_NFY_FIA_000_ANIM::FIA_Laugh:
	
		return false;
	}

	return true;
}

void CCityNpc::Tick_State(_float fTimeDelta)
{
	
	//m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta, true);


}

void CCityNpc::LateTick_State(_float fTimeDelta)
{
}

void CCityNpc::Talk_with_Npc()
{




		switch (m_NpcDesc.eNpcType)
		{
		case NPC_NMM_DIM_000:
			m_pModelCom->Set_CurrentAnimIndex(1);
			break;

		case NPC_NFY_FIA_000:
			m_pModelCom->Set_CurrentAnimIndex(1);
			break;
		default:
			break;
		}
	


		
}

_float CCityNpc::Find_Target()
{
	if (m_NpcDesc.eNpcType == NPC_NFC_SLV_000)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_CITY, TEXT("Layer_Child"), 0);
		if (pGameObject == this)
			pGameObject = pGameInstance->Get_Object(LEVEL_CITY, TEXT("Layer_Child"), 1);

		CCityNpc* pCityNpc = dynamic_cast<CCityNpc*>(pGameObject);
		if (!pCityNpc)
			return 0;

		m_pTarget = pCityNpc;

		_vector vChasePosition = pCityNpc->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_vector vPosition = Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vChasePosition - vPosition));

		m_fDistance = fDistance;
	}

	else if (m_NpcDesc.eNpcType == NPC_NFC_SLV_000_2th)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_CITY, TEXT("Layer_Child"), 1);
		if (pGameObject == this)
			pGameObject = pGameInstance->Get_Object(LEVEL_CITY, TEXT("Layer_Child"), 0);

		CCityNpc* pCityNpc = dynamic_cast<CCityNpc*>(pGameObject);
		if (!pCityNpc)
			return 0;

		m_pTarget = pCityNpc;

		_vector vChasePosition = pCityNpc->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_vector vPosition = Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vChasePosition - vPosition));

		m_fDistance = fDistance;
	}

	return m_fDistance;

}

_float CCityNpc::Find_MAN_GLD()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_CITY, TEXT("Layer_Gld"), 2);

	CCityNpc* pCityNpc = dynamic_cast<CCityNpc*>(pGameObject);
	if (!pCityNpc)
		return 0;

	m_pGld = pCityNpc;

	_vector vGldPosition = pCityNpc->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

	_vector vPosition = Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vGldPosition - vPosition));

	m_fDistance = fDistance;

	return m_fDistance;
}


HRESULT CCityNpc::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_City_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 3.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}


CCityNpc * CCityNpc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCityNpc* pInstance = new CCityNpc(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCityNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCityNpc::Clone(void * pArg)
{
	CCityNpc* pInstance = new CCityNpc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCityNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCityNpc::Free()
{
	__super::Free();
}
