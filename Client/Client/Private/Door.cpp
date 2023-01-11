#include "stdafx.h"
#include "..\Public\Door.h"
#include "GameInstance.h"
#include "Player.h"
#include "TreasureBox.h"
#include "Level_TailCave.h"
#include "UIButton.h"
#include "FootSwitch.h"
#include "ObjectEffect.h"
#include "CameraManager.h"
#include "UIIcon.h"

CDoor::CDoor(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

HRESULT CDoor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDoor::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_DoorDesc, pArg, sizeof(DOORDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_eObjectID = OBJID::OBJ_BLOCK;
	Set_Scale(_float3(1.2f, 1.2f, 1.2f));

	_vector vecPostion = XMLoadFloat3(&m_DoorDesc.InitPosition);
	vecPostion = XMVectorSetW(vecPostion, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_DoorDesc.fAngle));

	CUIIcon::ICONDESC IconDesc;
	switch (m_DoorDesc.eType)
	{
	case DOOR_CLOSED:
		m_eState = CLOSE_CD;

		
		IconDesc.iTexureNum = CUIIcon::ICON_CLOSE_DOOR;
		IconDesc.pTarget = this;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_TAILCAVE, TEXT("UI_ICON"), &IconDesc)))
			return E_FAIL;
		break;
	case DOOR_KEY:
		m_eState = CLOSE_LD;

		IconDesc.iTexureNum = CUIIcon::ICON_LOCK_DOOR;
		IconDesc.pTarget = this;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_TAILCAVE, TEXT("UI_ICON"), &IconDesc)))
			return E_FAIL;
		break;
	case DOOR_BOSS:
		m_eState = REMOVE_KEY;

		IconDesc.iTexureNum = CUIIcon::ICON_LOCK_BOX;
		IconDesc.pTarget = this;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_TAILCAVE, TEXT("UI_ICON"), &IconDesc)))
			return E_FAIL;
		break;
	case DOOR_TAIL:
		m_eState = CLOSE_TAIL;
		break;
	default:
		break;
	}


	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_BLOCK, this);
	
	
	return S_OK;
}

int CDoor::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_BLOCK, this);
		return OBJ_DEAD;
	}

	switch (m_DoorDesc.eType)
	{
	case DOOR_CLOSED:
		Tick_ClosedDoor(fTimeDelta);
		break;
	case DOOR_KEY:
		Tick_LockDoor(fTimeDelta);
		break;
	case DOOR_BOSS:
		Tick_BossDoor(fTimeDelta);
		break;
	case DOOR_TAIL:
		Tick_TailDoor(fTimeDelta);
		break;
	default:
		break;
	}

	
	return OBJ_NOEVENT;
}

void CDoor::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
	{
		__super::Late_Tick(fTimeDelta);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}
	SetUp_ShaderID();
}

HRESULT CDoor::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}



	return S_OK;
}

void CDoor::Change_Animation(_float fTimeDelta)
{
	switch (m_DoorDesc.eType)
	{
	case DOOR_CLOSED:
		Change_Animation_ClosedDoor(fTimeDelta);
		break;
	case DOOR_KEY:
		Change_Animation_LockDDoor(fTimeDelta);
		break;
	case DOOR_BOSS:
		Change_Animation_BossDoor(fTimeDelta);
		break;
	case DOOR_TAIL:
		Change_Animation_TailDoor(fTimeDelta);
		break;
	}
}

HRESULT CDoor::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;

	switch (m_DoorDesc.eType)
	{
	case DOOR_CLOSED:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_ClosedDoor"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		/* For.Com_OBB*/
		ColliderDesc.vScale = _float3(3.f, 2.f, 0.7f);
		ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
			return E_FAIL;
		break;
	case DOOR_KEY:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_LockDoor"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		/* For.Com_OBB*/
		ColliderDesc.vScale = _float3(3.f, 2.f, 0.7f);
		ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
			return E_FAIL;
		break;
	case DOOR_BOSS:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_BossDoor"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		/* For.Com_OBB*/
		ColliderDesc.vScale = _float3(3.f, 2.f, 0.7f);
		ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
			return E_FAIL;
		break;
	case DOOR_TAIL:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TailCaveShutter"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		/* For.Com_OBB*/
		ColliderDesc.vScale = _float3(3.f, 2.f, 1.5f);
		ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
			return E_FAIL;
		break;
	default:
		break;
	}

	

	
	


	return S_OK;
}

HRESULT CDoor::SetUp_ShaderResources()
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

void CDoor::Tick_ClosedDoor(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if ( Check_IsinFrustum() == true)
	{
		m_bOpen = Check_Open();

		if (m_bOpen == false)
		{
			list<CGameObject*>* pDoorList = pGameInstance->Get_ObjectList(LEVEL_TAILCAVE, TEXT("Layer_ColsedDoor"));
			if (pDoorList != nullptr)
			{
				for (auto& iter : *pDoorList)
				{
					dynamic_cast<CDoor*>(iter)->Set_OpenDoor(false);
				}
			}
		}
		else
		{
			list<CGameObject*>* pDoorList = pGameInstance->Get_ObjectList(LEVEL_TAILCAVE, TEXT("Layer_ColsedDoor"));
			if (pDoorList != nullptr)
			{
				for (auto& iter : *pDoorList)
				{
					dynamic_cast<CDoor*>(iter)->Set_OpenDoor(true);
				}
			}
		}

			
	}

	if (m_bOpen)
	{
		if (m_eState != OPEN_WAIT_CD)
		{
			m_eState = OPEN_CD;
			m_bPlay = true;
		}
	}
	else
		m_eState = CLOSE_CD;


	if (m_bPlay)
	{
		if (m_eState != m_ePreState)
		{
			m_pModelCom->Set_NextAnimIndex(m_eState);
			m_ePreState = m_eState;
		}

		Change_Animation(fTimeDelta);
	}
		

	RELEASE_INSTANCE(CGameInstance);
}

void CDoor::Tick_LockDoor(_float fTimeDelta)
{
	if (!Check_IsinFrustum())
		return;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);

	_vector vTargetPos = pPlayer->Get_TransformState(CTransform::STATE_POSITION);
	_float fDistanceToTarget = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_POSITION) - vTargetPos));
	CUIButton*		pButton = dynamic_cast<CUIButton*>(CUI_Manager::Get_Instance()->Get_Button());
	
	if (fDistanceToTarget < 2.5f)
	{
		if (m_eState != OPEN_LD)
		{
			pButton->Set_TexType(CUIButton::OPEN);
			pButton->Set_Visible(true);
		}
			
		_float2 fPosition = pPlayer->Get_ProjPosition();
		fPosition.y = g_iWinSizeY - fPosition.y;
		fPosition.x += 50.f;
		fPosition.y -= 30.f;
		pButton->Set_Position(fPosition);

		if (CGameInstance::Get_Instance()->Key_Up(DIK_A)) 
		{
			if (CUI_Manager::Get_Instance()->Get_KeySize() != 0)
			{
				pButton->Set_Visible(false);
				dynamic_cast<CPlayer*>(pPlayer)->Set_AnimState(CPlayer::KEY_OPEN);
				m_eState = OPEN_LD;
				CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Door_Slide.wav"), SOUND_OBJECT, 0.3f);

			}
		}
		
	}
	else
	{
		m_eState = CLOSE_LD;
		pButton->Set_Visible(false);
	}
		
		


	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
	}

	
	Change_Animation(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);
}

void CDoor::Tick_BossDoor(_float fTimeDelta)
{
	if (!Check_IsinFrustum())
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);

	_vector vTargetPos = pPlayer->Get_TransformState(CTransform::STATE_POSITION);
	_float fDistanceToTarget = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_POSITION) - vTargetPos));
	CUIButton*		pButton = dynamic_cast<CUIButton*>(CUI_Manager::Get_Instance()->Get_Button());

	if (fDistanceToTarget < 2.5f)
	{
		if (m_bOpen == false)
		{
			pButton->Set_TexType(CUIButton::OPEN);
			pButton->Set_Visible(true);
		}
			
		_float2 fPosition = pPlayer->Get_ProjPosition();
		fPosition.y = g_iWinSizeY - fPosition.y;
		fPosition.x += 50.f;
		fPosition.y -= 30.f;
		pButton->Set_Position(fPosition);

		if (CGameInstance::Get_Instance()->Key_Up(DIK_A))
		{
			if (CUI_Manager::Get_Instance()->Get_KeySize() != 0)
			{
				pButton->Set_Visible(false);
				dynamic_cast<CPlayer*>(pPlayer)->Set_AnimState(CPlayer::KEY_OPEN);
				m_eState = REMOVE_KEY;
				CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Door Open.wav"), SOUND_OBJECT, 0.5f);
				CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_TailKeyUse.wav"), SOUND_ACTOR, 0.5f);
				m_bOpen = true;
				m_bPlay = true;
			}
		}
	}
	else
		pButton->Set_Visible(false);


	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
	}

	if (m_bPlay)
		Change_Animation(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);
}

void CDoor::Tick_TailDoor(_float fTimeDelta)
{
	if (!Check_IsinFrustum())
		return;

	if (m_bOpen && m_eState != OPEN_WAIT_TAIL)
	{
		m_eState = OPEN_TAIL;
	}
		

	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
	}

	Change_Animation(fTimeDelta);
}

void CDoor::Change_Animation_ClosedDoor(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CDoor::CLOSE_CD:
	case Client::CDoor::CLOSE2_CD:
		if (!m_bFirst)
		{
			if(Check_IsinFrustum())
				CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Door_Slide.wav"), SOUND_OBJECT, 0.3f);
			m_bFirst = true;
		}

		m_bIsLoop = false;
		m_fEffectTime += fTimeDelta;

		if (m_fEffectTime > 0.5f && !m_bMakeEffect)
		{
			if (Check_IsinFrustum())
				CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Door_Close.wav"), SOUND_OBJECT, 0.3f);
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CObjectEffect::SMOKE;
			EffectDesc.fDeadTime = 0.5f;
			EffectDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
			EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
			EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook + XMVectorSet(0.f, 0.4f, 0.5f, 0.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook +  XMVectorSet(0.f, 0.4f, -0.5f, 0.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

			EffectDesc.vInitScale = _float3(3.f, 3.f, 3.f);
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook * 0.3f + XMVectorSet(0.f, 0.4f, 0.5f, 0.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook * 0.3f + XMVectorSet(0.f, 0.4f, -0.5f, 0.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
			
			m_bMakeEffect = true;
		}

		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
		{
			if (m_bPlay)
				CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_BLOCK, this);
			m_bPlay = false;
			m_bMakeEffect = false;
			m_fEffectTime = 0.f;
			m_bFirst = false;
		}
		break;
	case Client::CDoor::OPEN_CD:
	case Client::CDoor::OPEN2_CD:
		if (m_bOpen)
		{
			if (!m_bFirst)
			{
				if (Check_IsinFrustum())
					CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Door_Slide.wav"), SOUND_OBJECT, 0.3f);
				m_bFirst = true;
			}

			m_bIsLoop = false;
			if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
			{
				CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_BLOCK, this);
				m_eState = OPEN_WAIT_CD;
				if (Check_IsinFrustum())
					CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Door_Open.wav"), SOUND_OBJECT, 0.3f);
				m_bFirst = false;
			}

		}
		break;
	case Client::CDoor::OPEN_WAIT_CD:
	case Client::CDoor::OPEN_WAIT2_CD:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);
		break;
	default:
		break;
	}
}

void CDoor::Change_Animation_LockDDoor(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CDoor::CLOSE_LD:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);
		break;
	case Client::CDoor::OPEN_LD:
	case Client::CDoor::OPEN2_LD:
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Door_Open.wav"), SOUND_OBJECT, 0.3f);
			CUI_Manager::Get_Instance()->Use_Key();
			m_bDead = true;		
		}	
		break;
	default:
		break;
	}
}

void CDoor::Change_Animation_BossDoor(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CDoor::REMOVE_KEY:
		if (m_bOpen)
		{
			m_bIsLoop = false;
			if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Door_Slide.wav"), SOUND_OBJECT, 0.3f);
				CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_BLOCK, this);
				m_eState = OPEN_BOSS;
			}

		}
		break;
	case Client::CDoor::OPEN_BOSS:
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Door_Open.wav"), SOUND_OBJECT, 0.3f);
			m_bDead = true;
		}
		break;
	default:
		break;
	}
}

void CDoor::Change_Animation_TailDoor(_float fTimeDelta)
{
	CGameObject* pTarget = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);

	switch (m_eState)
	{
	case Client::CDoor::CLOSE_TAIL:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);
		break;
	case Client::CDoor::OPEN_WAIT_TAIL:
		g_fBGMVolume += 0.01f;
		if (g_fBGMVolume >= 0.2f)
			g_fBGMVolume = 0.2f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);
		break;
	case Client::CDoor::OPEN_TAIL:
		pPlayer->Set_Stop(true);

		g_fBGMVolume -= 0.01f;
		if (g_fBGMVolume <= 0.05f)
			g_fBGMVolume = 0.05f;

		dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ZoomValue(2.f);
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
		{
			pPlayer->Set_Stop(false);
			m_eState = OPEN_WAIT_TAIL;
			CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_BLOCK, this);
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ZoomValue(0.f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("1-40 Puzzle Solved Jingle.mp3"), SOUND_SYSTEM, 0.5f);
		}
		break;
	default:
		break;
	}
}

void CDoor::Check_Close()
{
	
}

_bool CDoor::Check_Open()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CBaseObj* pPlayer = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerPos = pPlayer->Get_TransformState(CTransform::STATE_POSITION);
	_vector vDir = vPlayerPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (XMVectorGetX(XMVector3Length(vDir)) < 3.f && m_bOpen)
	{
		RELEASE_INSTANCE(CGameInstance);
		return true;
	}


	//if (0 < XMVectorGetX(XMVector3Dot(vDir, vLook)))
	//{
		// 조건 1 스위치가 없거나 스위치가 눌려 있을 경우
		list<CGameObject*>* pSwitches = pGameInstance->Get_ObjectList(LEVEL_TAILCAVE, TEXT("Layer_FootSwitch"));
		for (auto& iter : *pSwitches)
		{
			if (dynamic_cast<CBaseObj*>(iter)->Check_IsinFrustum() == false)
				continue;

			if (dynamic_cast<CFootSwitch*>(iter)->Get_IsBoxMade() == false)
				goto RETURN_CLOSEDOOR;

		}

		// 조건 2 .화면 내에 있는 몬스터들이 없거나 모두 죽었다면 
		list<CGameObject*>* pMonsters = pGameInstance->Get_ObjectList(LEVEL_TAILCAVE, TEXT("Layer_Monster"));

		for (auto& iter : *pMonsters)
		{
			if (iter == nullptr)
				continue;

			if (dynamic_cast<CBaseObj*>(iter)->Check_IsinFrustum() == false)
				continue;

			if (XMVectorGetX(XMVector3Length(dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_POSITION) - Get_TransformState(CTransform::STATE_POSITION))) >= 15.f)
				continue;

			if (iter->Get_Dead() == false)
				goto RETURN_CLOSEDOOR;
		}
			

		RELEASE_INSTANCE(CGameInstance);
		return true;
//	}


RETURN_CLOSEDOOR:
	RELEASE_INSTANCE(CGameInstance);
	return false;

}

CDoor * CDoor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDoor*	pInstance = new CDoor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CDoor::Clone(void * pArg)
{
	CDoor*	pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_BLOCK, this);
	Safe_Release(m_pModelCom);
}
