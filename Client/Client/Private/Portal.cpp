#include "stdafx.h"
#include "..\Public\Portal.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_Manager.h"
#include "UIIcon.h"

CPortal::CPortal(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNonAnim(pDevice, pContext)
{
}

CPortal::CPortal(const CPortal & rhs)
	: CNonAnim(rhs)
{
}

HRESULT CPortal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortal::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_PortalDesc, pArg, sizeof(PORTALDESC));
	
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	m_eObjectID = OBJ_KEY;

	if (pArg != nullptr)
	{
		_vector vecPostion = XMLoadFloat3(&m_PortalDesc.vInitPos);
		vecPostion = XMVectorSetW(vecPostion, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);

		CUIIcon::ICONDESC IconDesc;
		IconDesc.pTarget = this;
	

		switch (m_PortalDesc.eRoomType)
		{
		case MARINHOUSE:
			IconDesc.iTexureNum = CUIIcon::ICON_HOUSE;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_GAMEPLAY, TEXT("UI_ICON"), &IconDesc)))
				return E_FAIL;
			break;
		case SHOP:
			IconDesc.iTexureNum = CUIIcon::ICON_SHOP;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_GAMEPLAY, TEXT("UI_ICON"), &IconDesc)))
				return E_FAIL;
			break;
		case CRANE_GAME:
			IconDesc.iTexureNum = CUIIcon::ICON_HOUSE;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_GAMEPLAY, TEXT("UI_ICON"), &IconDesc)))
				return E_FAIL;
			break;
		case TELEPHONE:
			IconDesc.iTexureNum = CUIIcon::ICON_TELL;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_GAMEPLAY, TEXT("UI_ICON"), &IconDesc)))
				return E_FAIL;
			break;
		default:
			break;
		}

		switch (m_PortalDesc.eConnectLevel)
		{
		case LEVEL_TAILCAVE:
		case LEVEL_TOWER:
			IconDesc.iTexureNum = CUIIcon::ICON_DGN;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_GAMEPLAY, TEXT("UI_ICON"), &IconDesc)))
				return E_FAIL;
			break;
		default:
			break;
		}
		
	}

	Set_Scale(_float3(3.f, 3.f, 3.f));
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);

	if (m_PortalDesc.ePortalType == PORTAL_POSITION)
	{
		CUIIcon::ICONDESC IconDesc;
		IconDesc.iTexureNum = CUIIcon::ICON_STAIR;
		IconDesc.pTarget = this;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_TAILCAVE, TEXT("UI_ICON"), &IconDesc)))
			return E_FAIL;
	}

	return S_OK;
}

int CPortal::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);
		return OBJ_DEAD;
	}
	

	return OBJ_NOEVENT;
}

void CPortal::Late_Tick(_float fTimeDelta)
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

	switch (m_PortalDesc.ePortalType)
	{
	case PORTAL_POSITION:
		Portal_Position_Tick(fTimeDelta);
		break;
	case PORTAL_LEVEL:
		Portal_Level_Tick(fTimeDelta);
		break;
	default:
		break;
	}
	
	
}

HRESULT CPortal::Render()
{
	//__super::Render();

	return S_OK;

}

void CPortal::Portal_Position_Tick(_float fTimeDelta)
{
	CBaseObj* pTarget = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pOBBCom, &pTarget) == true)
	{
		if (CGameInstance::Get_Instance()->Key_Up(DIK_A))
		{

			if (m_PortalDesc.bConnectPortal2D == false)
				dynamic_cast<CPlayer*>(pTarget)->Set_AnimState(CPlayer::LADDER_UP_ED);
			else
				dynamic_cast<CPlayer*>(pTarget)->Set_AnimState(CPlayer::STAIR_DOWN);
			dynamic_cast<CPlayer*>(pTarget)->Set_NextPortal(m_PortalDesc.vConnectPos, m_PortalDesc.bConnectPortal2D);
		}
	}
}

void CPortal::Portal_Level_Tick(_float fTimeDelta)
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	CBaseObj* pTarget = nullptr;
	if (pUI_Manager->Get_NextLevel() == false && CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pOBBCom, &pTarget) == true)
	{
		if (dynamic_cast<CPlayer*>(pTarget)->Get_AnimState() == CPlayer::WARP_LP)
		{
			RELEASE_INSTANCE(CUI_Manager)
			return;
		}
		pUI_Manager->Set_NextLevelIndex(m_PortalDesc.eConnectLevel);
		pUI_Manager->Set_NextLevel(true);
		pUI_Manager->Set_RoomType((CUI_Manager::ROOMTYPE)m_PortalDesc.eRoomType);
	}
	RELEASE_INSTANCE(CUI_Manager)
}

HRESULT CPortal::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Boulder"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(0.5f, 0.2f, 0.5f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}


CPortal * CPortal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPortal*	pInstance = new CPortal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPortal::Clone(void * pArg)
{
	CPortal*	pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortal::Free()
{
	__super::Free();

}
