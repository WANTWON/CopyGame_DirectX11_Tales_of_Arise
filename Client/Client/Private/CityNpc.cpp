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
			m_eState = 2;
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



	if (CUI_Manager::Get_Instance()->Get_StopTick() || Check_IsinFrustum(2.f) == false)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);
	Tick_State(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	return OBJ_NOEVENT;
}

void CCityNpc::Late_Tick(_float fTimeDelta)
{
	_vector vPosition = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION);
	_float fDistance = XMVectorGetX(XMVector3Length(vPosition - Get_TransformState(CTransform::STATE_TRANSLATION)));
	if (fDistance > 50.f)
		return;

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

	if (m_NpcDesc.eNpcType == NPC_NFY_FIA_000)
	{
		switch ((_uint)eAnimId)
		{
		case NPC_NFY_FIA_000_ANIM::FIA_Idle:
			return true;
		}
	}


	switch ((_uint)eAnimId)
	{
	case NPC_NFC_SLV_000_2th_ANIM::Run_2th:
	case NPC_NFC_SLV_000_2th_ANIM::Cry_Loop_2th:
		return true;

	
	case NPC_NFC_SLV_000_ANIM::qHappyLoop:
	case NPC_NFC_SLV_000_ANIM::qHappyStart:
	case NPC_NFC_SLV_000_ANIM::qHappyEnd:
	case NPC_NFC_SLV_000_ANIM::ImHugry:
	case NPC_NFC_SLV_000_2th_ANIM::Whispe_Loop_2th:
	case NPC_NFY_FIA_000_ANIM::FIA_Laugh:
		return false;
	}

	return false;
}

void CCityNpc::Tick_State(_float fTimeDelta)
{
	/*if(m_NpcDesc.eNpcType != NPC_NFC_SLV_000)
		m_eState = 0;*/


	switch (m_NpcDesc.eNpcType)
	{
	case NPC_NFC_SLV_000:
		
		if (m_bChaseNFMStart == false)
		{
			m_bListenFinish = false;
			Find_NFM();
			if (m_fNFM_Distance >= 4.f)
			{
				m_eState = NPC_NFC_SLV_000_ANIM::Run;
				_vector vNFM_Position = m_pNFM->Get_TransformState(CTransform::STATE_TRANSLATION);
				m_pTransformCom->LookAt(vNFM_Position);
				m_pTransformCom->Sliding_Straight(fTimeDelta *1.5f, m_pNavigationCom);
			}
			else
			{
				if (m_fNFM_Distance < 4)
				{
					m_eState = NPC_NFC_SLV_000_ANIM::Squat;
				}

				if (m_bIsAnimationFinished)
				{
					m_bChaseNFMStart = true;
					m_bchaseGLDFinish = true;
				}
			}
		}

		if (m_bchaseGLDFinish == true)
		{
			Find_MAN_GLD();
			if (m_fGLD_Distance >= 5.f)
			{
				m_eState = NPC_NFC_SLV_000_ANIM::Run;
				_vector vGldPosition = m_pGld->Get_TransformState(CTransform::STATE_TRANSLATION);
				m_pTransformCom->LookAt(vGldPosition);
				m_pTransformCom->Sliding_Straight(fTimeDelta *1.5f, m_pNavigationCom);
			}

			else
			{
				if (m_fGLD_Distance < 5)
				{
					m_eState = NPC_NFC_SLV_000_ANIM::Idle;
				}

				//m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta, Is_AnimationLoop(m_pModelCom->Get_CurrentAnimIndex()), "ABone");
				if (m_bIsAnimationFinished)
				{
					m_bchaseGLDFinish = false;
					m_bchaseNMM_SLV_Start = true;
				}
			}
		}

		else if (m_bchaseNMM_SLV_Start == true)
		{	m_bTurn = false;

			Find_NMM_SLV();
			if (m_fNMM_SLV_Distance >= 4.f)
			{
				m_eState = NPC_NFC_SLV_000_ANIM::Run;
				_vector vNMM_SLV_Position = m_pNMM_SLV->Get_TransformState(CTransform::STATE_TRANSLATION);
				m_pTransformCom->LookAt(vNMM_SLV_Position);
				m_pTransformCom->Sliding_Straight(fTimeDelta *1.5f, m_pNavigationCom);
			}

			else
			{
				if (m_fNMM_SLV_Distance < 4)
				{
					m_eState = NPC_NFC_SLV_000_ANIM::ImHugry;
				}

				if (m_bIsAnimationFinished)
				{
					m_bchaseNMM_SLV_Start = false;
					m_bChaseFriend_Start = true;
				}
			}
		}

		else if (m_bChaseFriend_Start == true)
		{
			Find_Friend();
			if (m_bListenFinish == false)
			{
				if (m_fFriendDistance >= 1.6f)
				{
					m_eState = NPC_NFC_SLV_000_ANIM::Run;
					_vector vFriendPosition = m_pChildFriend->Get_TransformState(CTransform::STATE_TRANSLATION);
					m_pTransformCom->LookAt(vFriendPosition);
					m_pTransformCom->Sliding_Straight(fTimeDelta *1.5f, m_pNavigationCom);
				}

				else
				{
					m_eState = NPC_NFC_SLV_000_ANIM::Idle;
					
					if (m_bTurn == false)
					{
						m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 45.f);
						m_bTurn = true;
					}

					if (m_bIsAnimationFinished)
					{
						m_eState = NPC_NFC_SLV_000_ANIM::qHappyLoop;
						m_bListenFinish = true;
					}
				}

			
			}

			else if (m_bListenFinish == true)
			{
				
				if (m_bIsAnimationFinished)
				{
					m_bChaseFriend_Start = false;
					m_bChaseNFMStart = false;
				}
			}
		}


		break;


	case NPC_NFC_SLV_000_2th:
		Find_Friend();

			if (m_fFriendDistance >= 1.6f)
			{
				m_bWhispe_Finish = false;
				m_eState = NPC_NFC_SLV_000_2th_ANIM::Cry_Loop_2th;
				m_bFriendBye = false;
			}
			else
			{	
				if (m_bWhispe_Finish == false)
				{
					m_eState = NPC_NFC_SLV_000_2th_ANIM::Whispe_Loop_2th;
					_vector vFriendPos = m_pChildFriend->Get_TransformState(CTransform::STATE_TRANSLATION);
					m_pTransformCom->LookAt(vFriendPos);
					if (m_bIsAnimationFinished)
					{
						m_eState = NPC_NFC_SLV_000_ANIM::qHappyStart;
						m_bWhispe_Finish = true;
						m_bHappyStart = true;
					}
				}
			}
			if (m_bHappyStart == true)
			{
				if (m_bIsAnimationFinished)
				{
					m_eState = NPC_NFC_SLV_000_ANIM::qHappyLoop;
					m_bHappyStart = false;
					m_bHappyLoop = true;
				}
			}

			else if (m_bHappyLoop == true)
			{
				if (m_bIsAnimationFinished)
				{
					m_eState = NPC_NFC_SLV_000_ANIM::qHappyEnd;
					m_bHappyLoop = false;
					m_bHappyEnd = true;
				}
			}

			else if (m_bHappyEnd == true)
			{
				if (m_bIsAnimationFinished)
				{
					m_eState = NPC_NFC_SLV_000_ANIM::qHappyEnd;

					m_bHappyEnd = false;
				}
			}

		break;

	default:
		m_eState = 0;
		break;
	}

		if (m_eState != m_ePreState)
		{
			m_pModelCom->Set_CurrentAnimIndex(m_eState);
			m_ePreState = m_eState;
		}
	

	//if(m_eState != NPC_NFC_SLV_000)
	//	m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta, Is_AnimationLoop(m_pModelCom->Get_CurrentAnimIndex()), "ABone");
	//else
	//{	
	//	if(m_eState = NPC_NFC_SLV_000_ANIM::Idle)
	//		m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta * 2.0f, Is_AnimationLoop(m_pModelCom->Get_CurrentAnimIndex()), "ABone");

	//	else
		m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta , Is_AnimationLoop(m_pModelCom->Get_CurrentAnimIndex()), "ABone");
	//}
	//
	//if (m_eState != NPC_NFY_FIA_000)
	//	if (m_bIsAnimationFinished)
	//		m_eState = 0;

}

void CCityNpc::LateTick_State(_float fTimeDelta)
{
}

void CCityNpc::Talk_with_Npc()
{

		switch (m_NpcDesc.eNpcType)
		{
		case NPC_NMM_MHB_000:
			break;

		case MAN_GLD:
			switch (CUI_Manager::Get_Instance()->Get_Dialogue_section())
			{
			case 7:
				dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(7);
				CUI_Manager::Get_Instance()->Set_Dialogue_section(8);
			}
			break;

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

_float CCityNpc::Find_Friend()
{

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_CITY, TEXT("Layer_Child"), 0);
		if (pGameObject == this)
			pGameObject = pGameInstance->Get_Object(LEVEL_CITY, TEXT("Layer_Child"), 1);

		CCityNpc* pCityNpc = dynamic_cast<CCityNpc*>(pGameObject);
		if (!pCityNpc)
			return 0;

		m_pChildFriend = pCityNpc;

		_vector vChasePosition = pCityNpc->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_vector vPosition = Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vChasePosition - vPosition));

		m_fFriendDistance = fDistance;



	

		return m_fFriendDistance;
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

	m_fGLD_Distance = fDistance;

	return m_fGLD_Distance;
}

_float CCityNpc::Find_NMM_SLV()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_CITY, TEXT("Layer_NMM_SLV"), 0);

	CCityNpc* pCityNpc = dynamic_cast<CCityNpc*>(pGameObject);
	if (!pCityNpc)
		return 0;

	m_pNMM_SLV = pCityNpc;

	_vector vNMM_SLVPosition = pCityNpc->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

	_vector vPosition = Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vNMM_SLVPosition - vPosition));

	m_fNMM_SLV_Distance = fDistance;

	return m_fNMM_SLV_Distance;
}

_float CCityNpc::Find_NFM()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_CITY, TEXT("Layer_NFM"), 0);

	CCityNpc* pCityNpc = dynamic_cast<CCityNpc*>(pGameObject);
	if (!pCityNpc)
		return 0;

	m_pNFM = pCityNpc;

	_vector vNfmPosition = pCityNpc->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

	_vector vPosition = Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

	_float fDistance = XMVectorGetX(XMVector3Length(vNfmPosition - vPosition));

	m_fNFM_Distance = fDistance;

	return m_fNFM_Distance;
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
