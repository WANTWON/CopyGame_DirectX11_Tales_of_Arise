#include "stdafx.h"
#include "..\Public\TarinNpc.h"
#include "Player.h"
#include "Level_Manager.h"
#include "CameraManager.h"
#include "UIButton.h"
#include "MessageBox.h"
#include "InvenTile.h"
#include "InvenItem.h"

CTarinNpc::CTarinNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc(pDevice, pContext)
{
}

CTarinNpc::CTarinNpc(const CTarinNpc & rhs)
	: CNpc(rhs)
{
}

HRESULT CTarinNpc::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTarinNpc::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
		memcpy(&m_NpcDesc, pArg, sizeof(NPCDESC));

	_vector vPosition = XMVectorSetW(XMLoadFloat3(&m_NpcDesc.vInitPos), 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	Set_Scale(_float3(1.2f, 1.2f, 1.2f));

	return S_OK;
}

int CTarinNpc::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Find_Target();
	if (m_pTarget != nullptr)
		m_pTransformCom->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_POSITION));


	m_pModelCom->Set_NextAnimIndex(m_eState);
	Change_Animation(fTimeDelta);

	return OBJ_NOEVENT;
}

void CTarinNpc::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (!Check_IsinFrustum())
		return;


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);


	CUIButton*		pButton = dynamic_cast<CUIButton*>(CUI_Manager::Get_Instance()->Get_Button());

	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom))
	{
		if (CUI_Manager::Get_Instance()->Get_Talking() == false)
		{
			pButton->Set_TexType(CUIButton::TALK);
			pButton->Set_Visible(true);
		}


		_float2 fPosition = pPlayer->Get_ProjPosition();
		fPosition.y = g_iWinSizeY - fPosition.y;
		fPosition.x += 50.f;
		fPosition.y -= 30.f;
		pButton->Set_Position(fPosition);

		if (CGameInstance::Get_Instance()->Key_Up(DIK_A) && CUI_Manager::Get_Instance()->Get_TalkingNpc() == nullptr && m_eState != TALK)
		{
			_tchar	sz_FullPath[MAX_PATH];
			_int iNum = rand() % 2 + 1;
			wcscpy_s(sz_FullPath, TEXT("8_Npc_Tarin (%d).wav"));
			wsprintf(sz_FullPath, sz_FullPath, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_FullPath, SOUND_OBJECT, 0.5f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Button_On.wav"), SOUND_SYSTEM, g_fUIVolume);

			CUI_Manager::Get_Instance()->Add_TalkingNpc(this);
			CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
			dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_TALK);
			m_pTransformCom->LookAt(pPlayer->Get_TransformState(CTransform::STATE_POSITION));
			pButton->Set_Visible(false);
			pPlayer->Set_Stop(true);
			m_eState = TALK;
			Change_Message();
		}
		m_bFirst = false;
	}
	else if (!m_bFirst)
	{
		pButton->Set_Visible(false);
		m_bFirst = true;
		m_eState = IDLE;
	}

	RELEASE_INSTANCE(CGameInstance);

}

HRESULT CTarinNpc::Render()
{
	__super::Render();

	return S_OK;
}




void CTarinNpc::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom == nullptr)
		return;
}

void CTarinNpc::Send_Answer_toNPC(_uint iTextureNum)
{
	
}

HRESULT CTarinNpc::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_ROOM, TEXT("Prototype_Component_Model_Tarin"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_SPHERE */
	ColliderDesc.vScale = _float3(2.5f, 2.5f, 2.5f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTarinNpc::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTarinNpc::SetUp_ShaderID()
{
	return S_OK;
}

void CTarinNpc::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CTarinNpc::TALK:
	case Client::CTarinNpc::IDLE:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	}
}

void CTarinNpc::Change_Message()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
	pUI_Manager->Set_Talking(true);

	CMessageBox::MSGDESC MessageDesc;
	MessageDesc.m_eMsgType = CMessageBox::TARIN_TALK;
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MessageBox"), LEVEL_STATIC, TEXT("Layer_UI"), &MessageDesc);

	CUI_Manager::MSGDESC eMsgDesc;
	eMsgDesc.eMsgType = CUI_Manager::PASSABLE;

	if (pUI_Manager->Get_Is_HaveItem(LEVEL_STATIC, TEXT("Layer_QuestItem"), CInvenItem::MARIN))
	{
		eMsgDesc.iTextureNum = THANK_YOU;
		pUI_Manager->Add_MessageDesc(eMsgDesc);

		eMsgDesc.iTextureNum = THISISFORYOU;
		pUI_Manager->Add_MessageDesc(eMsgDesc);

		m_bGiveItem = true;
		m_iGiveItemTexNum = CPrizeItem::BELL;
	}
	else
	{
		eMsgDesc.iTextureNum = DEFAULT;
		pUI_Manager->Add_MessageDesc(eMsgDesc);
	}
	
	
	
	pUI_Manager->Open_Message(true);

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}


void CTarinNpc::GiveItemMode()
{
	//if (m_bGiveItem && CUI_Manager::Get_Instance()->Get_Talking() == false &&
	//	CUI_Manager::Get_Instance()->Get_OpenMessage() == false && CUI_Manager::Get_Instance()->Get_MessageSize() == 0)

	CPrizeItem::ITEMDESC ItemDesc;
	ItemDesc.eType = (CPrizeItem::TYPE)m_iGiveItemTexNum;
	ItemDesc.eInteractType = CPrizeItem::PRIZE;
	_vector vPlayerPos = dynamic_cast<CBaseObj*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")))->Get_TransformState(CTransform::STATE_POSITION);
	_vector vDir = XMVector3Normalize(vPlayerPos - Get_TransformState(CTransform::STATE_POSITION));

	XMStoreFloat3(&ItemDesc.vPosition, Get_TransformState(CTransform::STATE_POSITION) + vDir);

	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, TEXT("Layer_Item"), &ItemDesc)))
		return;
	m_bGiveItem = false;

}



void CTarinNpc::Free()
{
	__super::Free();


	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSPHERECom);

}

CTarinNpc * CTarinNpc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTarinNpc*	pInstance = new CTarinNpc(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTarinNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTarinNpc::Clone(void * pArg)
{
	CTarinNpc*	pInstance = new CTarinNpc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTarinNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
