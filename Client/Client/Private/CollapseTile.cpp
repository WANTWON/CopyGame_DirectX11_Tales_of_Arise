#include "stdafx.h"
#include "..\Public\CollapseTile.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_Manager.h"

CCollapseTile::CCollapseTile(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNonAnim(pDevice, pContext)
{
}

CCollapseTile::CCollapseTile(const CCollapseTile & rhs)
	: CNonAnim(rhs)
{
}

HRESULT CCollapseTile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollapseTile::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_TileDesc, pArg, sizeof(TILEDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	m_eObjectID = OBJ_KEY;


	if (pArg != nullptr)
	{
		_vector vecPostion = XMLoadFloat3((_float3*)pArg);
		vecPostion = XMVectorSetW(vecPostion, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	}

	Set_Scale(_float3(3.f, 3.f, 3.f));
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLITION_TILE, this);

	return S_OK;
}

int CCollapseTile::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);
		return OBJ_DEAD;
	}
	

	return OBJ_NOEVENT;
}

void CCollapseTile::Late_Tick(_float fTimeDelta)
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

	switch (m_TileDesc.eTileType)
	{
	case COLLAPSE:
		m_eShaderID = SHADER_ALPHABLEND;
		CollapseTile_Tick(fTimeDelta);
		break;
	case CRANE_TILE:
		CraneTile_Tick(fTimeDelta);
		break;
	default:
		break;
	}

}

HRESULT CCollapseTile::Render()
{
	__super::Render();

	return S_OK;

}

HRESULT CCollapseTile::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	switch (m_TileDesc.eTileType)
	{
	case COLLAPSE:

		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_CollapseTile"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case CRANE_TILE:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_ROOM, TEXT("Prototype_Component_Model_MovingFloorCrane"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		/* For.Com_Navigation */
		if (FAILED(__super::Add_Components(TEXT("Com_Navigation_CraneGame"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_CraneGame"), (CComponent**)&m_pNavigationCom)))
			return E_FAIL;
		break;
	default:
		break;
	}


	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(0.5f, 0.2f, 0.5f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;


	return S_OK;
}

void CCollapseTile::CollapseTile_Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CBaseObj* pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	if (m_pOBBCom != nullptr && m_pOBBCom->Collision(pTarget->Get_Collider()))
	{
		if (!m_bFirst)
		{
			_uint iNum = rand() % 3 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.4f;
			wcscpy_s(sz_Sound, TEXT("5_Obj_Tile_Mineral_Break (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_OBJECT, fVolume);
			m_bFirst = true;
		}
		m_fAlpha -= 0.05f;

		if (m_fAlpha <= 0.f)
			m_bDead = true;
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CCollapseTile::CraneTile_Tick(_float fTimeDelta)
{

	if (m_pTransformCom->Go_PosDir(fTimeDelta, m_vDir, m_pNavigationCom) == false)
	{

		m_vDir *= -1.f;
	}

	if (!m_bCheck &&  CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_INTERACT, m_pOBBCom, &m_pCollisionObj))
	{
		m_bCheck = true;
		
	}

	if (!m_bCheck && m_pCollisionObj == nullptr && CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_ITEM, m_pOBBCom, &m_pCollisionObj))
	{
		if (m_pCollisionObj != nullptr)
		{
			_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPosition = XMVectorSetY(vPosition, 1.f);
			m_pCollisionObj->Set_State(CTransform::STATE_POSITION, vPosition);
		}
	}

	if (!m_bCheck && m_pCollisionObj != nullptr)
	{
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPosition = XMVectorSetY(vPosition, 1.f);
		m_pCollisionObj->Set_State(CTransform::STATE_POSITION, vPosition);
	}
}


CCollapseTile * CCollapseTile::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCollapseTile*	pInstance = new CCollapseTile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCollapseTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCollapseTile::Clone(void * pArg)
{
	CCollapseTile*	pInstance = new CCollapseTile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCollapseTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollapseTile::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLITION_TILE, this);

	Safe_Release(m_pNavigationCom);
}
