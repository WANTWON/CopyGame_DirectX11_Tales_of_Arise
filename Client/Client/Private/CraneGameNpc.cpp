#include "stdafx.h"
#include "..\Public\CraneGameNpc.h"
#include "Player.h"
#include "Level_Manager.h"
#include "CameraManager.h"
#include "PrizeItem.h"
#include "UIButton.h"
#include "MessageBox.h"
#include "Weapon.h"
#include "UIButton.h"
#include "InvenTile.h"
#include "InvenItem.h"

CCraneGameNpc::CCraneGameNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc(pDevice, pContext)
{
}

CCraneGameNpc::CCraneGameNpc(const CCraneGameNpc & rhs)
	: CNpc(rhs)
{
}

HRESULT CCraneGameNpc::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCraneGameNpc::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
		memcpy(&m_NpcDesc, pArg, sizeof(NPCDESC));

	_vector vPosition = XMVectorSetW(XMLoadFloat3(&m_NpcDesc.vInitPos), 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));


	return S_OK;
}

int CCraneGameNpc::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom->Set_NextAnimIndex(m_eState);
	Change_Animation(fTimeDelta);


	return OBJ_NOEVENT;
}

void CCraneGameNpc::Late_Tick(_float fTimeDelta)
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
		if (m_eState == IDLE)
		{
			pButton->Set_TexType(CUIButton::TALK);
			pButton->Set_Visible(true);
		}

		_float2 fPosition = pPlayer->Get_ProjPosition();
		fPosition.y = g_iWinSizeY - fPosition.y;
		fPosition.x += 50.f;
		fPosition.y -= 30.f;
		pButton->Set_Position(fPosition);

		if (CGameInstance::Get_Instance()->Key_Up(DIK_A) && m_eState != TALK)
		{
			_tchar	sz_FullPath[MAX_PATH];
			_int iNum = rand() % 2 + 1;
			wcscpy_s(sz_FullPath, TEXT("8_Npc_CraneGame (%d).wav"));
			wsprintf(sz_FullPath, sz_FullPath, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_FullPath, SOUND_ACTOR, 0.5f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Button_On.wav"), SOUND_SYSTEM, g_fUIVolume);

			CUI_Manager::Get_Instance()->Add_TalkingNpc(this);
			CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
			dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_TALK);
			pButton->Set_Visible(false);
			pPlayer->Set_Stop(true);
			m_eState = TALK;
			Change_Message();
		}
		m_bFirst = false;
	}
	else if(!m_bFirst)
	{
		pButton->Set_Visible(false);
		m_eState = IDLE;
		m_bFirst = true;
	}

	RELEASE_INSTANCE(CGameInstance);

}

HRESULT CCraneGameNpc::Render()
{
	__super::Render();

	return S_OK;
}


void CCraneGameNpc::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom == nullptr)
		return;
}

void CCraneGameNpc::Send_Answer_toNPC(_uint iTextureNum)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
	CWeapon::TYPE eItemType = (CWeapon::TYPE)pPlayer->Get_PartsItemType();

	pUI_Manager->Clear_ChoiceButton();

	if (iTextureNum == CUIButton::DO)
	{
		if (pPlayer->Set_RubyUse(10))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Event_Buy.wav"), SOUND_PEFFECT, 0.5f);
			CUI_Manager::MSGDESC eMsgDesc;
			eMsgDesc.eMsgType = CUI_Manager::PASSABLE;
			eMsgDesc.iTextureNum = HOW_TO_PLAY;
			pUI_Manager->Add_MessageDesc(eMsgDesc);


			eMsgDesc.eMsgType = CUI_Manager::PASSABLE;
			eMsgDesc.iTextureNum = LETS_GO;
			pUI_Manager->Add_MessageDesc(eMsgDesc);
			m_bGameStart = true;
		}
		else
		{
			CUI_Manager::MSGDESC eMsgDesc;
			eMsgDesc.eMsgType = CUI_Manager::PASSABLE;
			eMsgDesc.iTextureNum = SORRY;
			pUI_Manager->Add_MessageDesc(eMsgDesc);
		}
	}

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CCraneGameNpc::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_ROOM, TEXT("Prototype_Component_Model_GameShopOwner"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_SPHERE */
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCraneGameNpc::SetUp_ShaderResources()
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

HRESULT CCraneGameNpc::SetUp_ShaderID()
{
	return S_OK;
}

void CCraneGameNpc::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CCraneGameNpc::TALK:
	case Client::CCraneGameNpc::IDLE:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	default:
		break;
	}
}

void CCraneGameNpc::Change_Message()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
	pUI_Manager->Set_Talking(true);


	/*if (CUI_Manager::Get_Instance()->Get_PlayGame() == false)
	{
	m_bGameStart = false;
	}*/

	CMessageBox::MSGDESC MessageDesc;
	MessageDesc.m_eMsgType = CMessageBox::CRANEGAME_TALK;
	MessageDesc.fPosition.x -= 150.f;
	
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MessageBox"), LEVEL_STATIC, TEXT("Layer_UI"), &MessageDesc);

	
	CUI_Manager::MSGDESC eMsgDesc;
	eMsgDesc.eMsgType = CUI_Manager::MUST_CHOICE;
	eMsgDesc.iTextureNum = TALK_DEFAULT;
	eMsgDesc.eChoiceType = CUI_Manager::DO_UNDO;
	pUI_Manager->Add_MessageDesc(eMsgDesc);
	
	

	pUI_Manager->Open_Message(true);

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}



void CCraneGameNpc::Free()
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

CCraneGameNpc * CCraneGameNpc::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCraneGameNpc*	pInstance = new CCraneGameNpc(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CFieldNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCraneGameNpc::Clone(void * pArg)
{
	CCraneGameNpc*	pInstance = new CCraneGameNpc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CFieldNpc"));
		Safe_Release(pInstance);
	}

	return pInstance;
}
