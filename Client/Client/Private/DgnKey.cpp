#include "stdafx.h"
#include "..\Public\DgnKey.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_Manager.h"
#include "MessageBox.h"
#include "PrizeItem.h"
#include "ObjectEffect.h"
#include "CameraManager.h"

CDgnKey::CDgnKey(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNonAnim(pDevice, pContext)
{
}

CDgnKey::CDgnKey(const CDgnKey & rhs)
	: CNonAnim(rhs)
{
}


HRESULT CDgnKey::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDgnKey::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_eKeyDesc, pArg, sizeof(DGNKEYDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	//CPickingMgr::Get_Instance()->Add_PickingGroup(this);

	m_eObjectID = OBJ_KEY;
	m_eShaderID = SHADER_GLOWSHADER;

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&m_eKeyDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}

	Set_Scale(_float3(3.f, 3.f, 3.f));
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_ITEM, this);

	return S_OK;
}

int CDgnKey::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_ITEM, this);
		return OBJ_DEAD;
	}
		

	if (m_bGet)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		_vector pPlayerPostion = pPlayer->Get_TransformState(CTransform::STATE_POSITION);
		pPlayerPostion = XMVectorSetY(pPlayerPostion, XMVectorGetY(pPlayerPostion) + 3.f );
		m_pTransformCom->Go_PosTarget(fTimeDelta, pPlayerPostion);


		if (pPlayer->Get_AnimState() == CPlayer::ITEM_GET_ED)
		{
			m_bDead = true;
			CUI_Manager::Get_Instance()->Open_Message(false);
			CUI_Manager::Get_Instance()->Get_Key();
		}
			
	}


	return OBJ_NOEVENT;
}

void CDgnKey::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
	{
#ifdef _DEBUG
		if (m_pAABBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pAABBCom);
		if (m_pOBBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pOBBCom);
		if (m_pSPHERECom != nullptr)
			m_pRendererCom->Add_Debug(m_pSPHERECom);
#endif
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}
		

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	CBaseObj* pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	if (m_pOBBCom != nullptr && m_pOBBCom->Collision(pTarget->Get_Collider()))
	{
		if (!m_bFirst)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Switch_OnOff.mp3"), SOUND_OBJECT, 0.4f);
			CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
			dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_ZoomValue(-1.f);
			m_bFirst = true;
		}
		dynamic_cast<CPlayer*>(pTarget)->Set_Stop(true);
		m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, -1.f, 0.f, 0.f));

		_vector vPostion = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		if (XMVectorGetY(vPostion) < 0.5f)
		{
			vPostion = XMVectorSetY(vPostion, 0.5f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPostion);
			Safe_Release(m_pOBBCom);
			m_pOBBCom = nullptr;
			pGameInstance->PlaySounds(TEXT("5_Obj_Key_Sound.wav"), SOUND_OBJECT, 0.4f);
			dynamic_cast<CPlayer*>(pTarget)->Set_Stop(false);
			CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
			dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_ZoomValue(0.f);
		
		}
			
	}

	if ( m_pSPHERECom->Collision(pTarget->Get_Collider()))
	{
		SetUp_BillBoard();

		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_ITEM, this);

		if (!m_bGet)
		{
			dynamic_cast<CPlayer*>(pTarget)->Set_AnimState(CPlayer::ITEM_GET_ST);
			CMessageBox::MSGDESC MessageDesc;
			MessageDesc.m_eMsgType = CMessageBox::GET_ITEM;
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MessageBox"), LEVEL_STATIC, TEXT("Layer_UI"), &MessageDesc);
			pGameInstance->PlaySounds(TEXT("4_Event_Get Item Fanfare.mp3"), SOUND_SYSTEM, 0.4f);

			CUI_Manager::MSGDESC MsgDesc;
			MsgDesc.eMsgType = CUI_Manager::PASSABLE;
			MsgDesc.iTextureNum = CPrizeItem::MSG_SMALL_KEY;

			pUI_Manager->Add_MessageDesc(MsgDesc);
			pUI_Manager->Set_Talking(true);
			pUI_Manager->Open_Message(true);
			Make_GetItemEffect();


		}

		m_bGet = true;
	}


	if (m_bGet)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		_vector pPlayerPostion = pPlayer->Get_TransformState(CTransform::STATE_POSITION);

		pPlayerPostion = XMVectorSetY(pPlayerPostion, XMVectorGetY(pPlayerPostion) + 3.f);
		m_pTransformCom->Go_PosTarget(fTimeDelta, pPlayerPostion);

		if (pPlayer->Get_AnimState() == CPlayer::ITEM_GET_ED || pPlayer->Get_AnimState() == CPlayer::S_ITEM_GET_ED)
		{
			pUI_Manager->Open_Message(false);
			m_bDead = true;

			list<CGameObject*>* pEffectList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_ObjectEffect"));
			if (pEffectList != nullptr)
			{
				for (auto& iter : *pEffectList)
				{
					if (iter != nullptr)
						dynamic_cast<CObjectEffect*>(iter)->Set_EndMode(true);
				}
			}
		}
	}

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CDgnKey::Render()
{
	__super::Render();


	return S_OK;

}

void CDgnKey::Make_GetItemEffect()
{
	if (m_bMakeEffect)
		return;


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect::EFFECTDESC EffectDesc;

	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);// +XMVectorSet(0.f, -0.2f, 0.f, 0.f);
	EffectDesc.pTarget = this;

	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CObjectEffect::RAINBOW_RING;
	EffectDesc.fDeadTime = 2.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.015f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);

	EffectDesc.eEffectID = CObjectEffect::GRAD_RING;
	EffectDesc.fStartTime = 1.f;
	EffectDesc.fDeadTime = EffectDesc.fStartTime + 3.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.05f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.eEffectID = CObjectEffect::RAINBOW_HALO;
	EffectDesc.fStartTime = 0.f;
	EffectDesc.fDeadTime = 4.f;
	EffectDesc.vInitScale = _float3(10.f, 10.f, 10.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.04f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.eEffectID = CObjectEffect::HORIZONTAL_GLOW;
	EffectDesc.iTextureNum = 5;
	EffectDesc.fDeadTime = 2.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.03f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.eEffectID = CObjectEffect::ITEM_GET_GLOW;
	EffectDesc.iTextureNum = 0;
	EffectDesc.fStartTime = 1.f;
	EffectDesc.fDeadTime = EffectDesc.fStartTime + 3.5f;
	EffectDesc.vInitScale = _float3(1.f, 1.f, 1.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.02f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.fStartTime = 0.f;
	EffectDesc.iTextureNum = 0;
	EffectDesc.fDeadTime = 2.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, -0.01f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.eEffectID = CObjectEffect::ITEM_GET_FLASH;
	EffectDesc.iTextureNum = 6;
	EffectDesc.fDeadTime = 1.25f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, 0.1f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	EffectDesc.iTextureNum = 3;
	EffectDesc.fDeadTime = 1.f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, 0.12f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


	RELEASE_INSTANCE(CGameInstance);


	m_bMakeEffect = true;
}

HRESULT CDgnKey::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();

	return S_OK;
}

HRESULT CDgnKey::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 4.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_SmallKey"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;

	///* For.Com_AABB */
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	//ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	//ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
	//	return E_FAIL;

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(0.5f, 20.f, 0.5f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, -0.3f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ColliderDesc.vScale = _float3(0.1f, 0.1f, 0.1f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CDgnKey * CDgnKey::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDgnKey*	pInstance = new CDgnKey(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CDgnKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDgnKey::Clone(void * pArg)
{
	CDgnKey*	pInstance = new CDgnKey(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CDgnKey"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDgnKey::Free()
{
	__super::Free();

}
