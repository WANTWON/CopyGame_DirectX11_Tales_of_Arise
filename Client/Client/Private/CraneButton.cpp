#include "stdafx.h"
#include "..\Public\CraneButton.h"
#include "GameInstance.h"
#include "Player.h"
#include "UIButton.h"
#include "CameraManager.h"
#include "Crane.h"

CCraneButton::CCraneButton(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

HRESULT CCraneButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCraneButton::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_eObjectID = OBJID::OBJ_BLOCK;
	Set_Scale(_float3(1.2f, 1.2f, 1.2f));

	_vector vecPostion = XMLoadFloat3((_float3*)pArg);
	vecPostion = XMVectorSetW(vecPostion, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);


	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);
	return S_OK;
}

int CCraneButton::Tick(_float fTimeDelta)
{
	if (m_bPlay)
		Play_CraneGame(fTimeDelta);

	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
		m_bSoundOnce = false; 
	}

	Change_Animation(fTimeDelta);

	return OBJ_NOEVENT;
}

void CCraneButton::Late_Tick(_float fTimeDelta)
{

	CGameInstance*  pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
	if (pGameInstance->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f) == false)
	{
		RELEASE_INSTANCE(CGameInstance);
		RELEASE_INSTANCE(CUI_Manager);
		return;
	}

	__super::Late_Tick(fTimeDelta);

	CBaseObj*		pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	SetUp_ShaderID();

	if (!m_bPlay)
	{

		CUIButton*		pButton = dynamic_cast<CUIButton*>(CUI_Manager::Get_Instance()->Get_Button());
		LEVEL			iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();


		if (m_pOBBCom->Collision(pTarget->Get_Collider()) && pUI_Manager->Get_PlayGame() == true)
		{
			if (dynamic_cast<CPlayer*>(pTarget)->Get_Stop() == false)
				pButton->Set_Visible(true);

			pButton->Set_TexType(CUIButton::START);
			_float2 fPosition = pTarget->Get_ProjPosition();
			fPosition.y = g_iWinSizeY - fPosition.y;
			fPosition.x += 50.f;
			fPosition.y -= 30.f;
			pButton->Set_Position(fPosition);
			if (pGameInstance->Key_Down(DIK_A))
			{
				pButton->Set_Visible(false);
				CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
				dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_CRANE_GAME);
				dynamic_cast<CPlayer*>(pTarget)->Set_Stop(true);
				m_eState = RIGHTUP_OFF_LP;
				CCrane* pCrane = dynamic_cast<CCrane*>(pGameInstance->Get_Object(LEVEL_ROOM, TEXT("Layer_Crane")));
				pCrane->Setting_Default();

				for (_uint i = 0; i < DIR_END; ++i)
					m_eDir[i] = 0;

				m_bPlay = true;
			}
			m_bFirst = false;
		}
		else
		{
			if (!m_bFirst)
			{
				pButton->Set_Visible(false);
				m_bFirst = true;
			}

		}

	}
	



	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);

}

HRESULT CCraneButton::Render()
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

void CCraneButton::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CCraneButton::RIGHT_ON_LP:
	case Client::CCraneButton::RIGHTUP_ON_LP:
	case Client::CCraneButton::UP_ON_LP:
	case Client::CCraneButton::RIGHTUP_OFF_LP:
		if (!m_bSoundOnce)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Crane_Button_On.wav"), SOUND_SYSTEM, 0.5f);
			m_bSoundOnce = true;
		}
	
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta * 2, m_bIsLoop);
		break;
	default:
		break;
	}
}

void CCraneButton::Play_CraneGame(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CCrane* pCrane = dynamic_cast<CCrane*>(pGameInstance->Get_Object(LEVEL_ROOM, TEXT("Layer_Crane")));

	if (m_eDir[DIR_Z] != 1)
	{
		if (pGameInstance->Key_Up(DIK_UP))
			m_eDir[DIR_Z] = 1;
		else if (pGameInstance->Key_Pressing(DIK_UP))
		{
			pCrane->Input_Dir(DIR_Z);
			m_eState = UP_ON_LP;
		}
			
	}
	
	if (m_eDir[DIR_X] != 1)
	{
		if (pGameInstance->Key_Up(DIK_RIGHT))
			m_eDir[DIR_X] = 1;
		else if (pGameInstance->Key_Pressing(DIK_RIGHT))
		{
			pCrane->Input_Dir(DIR_X);
			m_eState = RIGHTUP_ON_LP;
		}
			
	}


	if (pCrane->Get_Finished())
	{
		CBaseObj*		pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
		dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_ROOM);
		dynamic_cast<CPlayer*>(pTarget)->Set_Stop(false);
		m_bPlay = false;
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CCraneButton::Ready_Components(void * pArg)
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

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_ROOM, TEXT("Prototype_Component_Model_CraneGameButton"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;


	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(1.8f, 1.8f, 1.8f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.0f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCraneButton::SetUp_ShaderResources()
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



CCraneButton * CCraneButton::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCraneButton*	pInstance = new CCraneButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCraneButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCraneButton::Clone(void * pArg)
{
	CCraneButton*	pInstance = new CCraneButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCraneButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCraneButton::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
