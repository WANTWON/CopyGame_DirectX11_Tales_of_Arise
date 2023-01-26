#include "stdafx.h"
#include "..\Public\Item.h"
#include "Player.h"
#include "Level_Manager.h"


CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInteractObject(pDevice, pContext)
{
}

CItem::CItem(const CItem & rhs)
	: CInteractObject(rhs)
{

}

HRESULT CItem::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	if (pArg != nullptr)
		memcpy(&m_ItemDesc, pArg, sizeof(ITEMDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_ItemDesc.vPosition);
	Set_Scale(m_ItemDesc.fScale);
	
	return S_OK;
}

int CItem::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CItem::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
	if (!pPlayer)
		return;


	m_bCollision = m_pSPHERECom->Collision(pPlayer->Get_Collider());
	
	if (m_bCollision)
	{
		m_bIsGain = true;
		
	}

	if (m_bIsGain)
	{
		m_DissolveAlpha += fTimeDelta;

		if (1 < m_DissolveAlpha)
		{
			m_DissolveAlpha = 1.f;
			m_bDead = true;
		}
	}
	
		
}

HRESULT CItem::Render()
{
	__super::Render();

	return S_OK;
}

CItem * CItem::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CItem* pInstance = new CItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone(void * pArg)
{
	CItem* pInstance = new CItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CItem"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem::Free()
{
	__super::Free();

}

HRESULT CItem::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_SNOWFIELD, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;
	///* For.Com_SPHERE */

	CCollider::COLLIDERDESC ColliderDesc;

	switch (m_ItemDesc.etype)
	{
	case APPLE:
		ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
		ColliderDesc.vScale = _float3(0.15f, 0.40f, 0.15f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
		break;
	case JEWEL:
		ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.1f, 1.1f, 1.1f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
		break;
	case MUSHROOM:
		ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.0f, 6.0f, 1.0f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
		break;

	case LETTUCE:
		ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.0f, 0.5f, 1.0f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
			return E_FAIL;
		break;

	default:
		break;
	}


	/* For.Com_Model*/
	switch (m_ItemDesc.etype)
	{
	case APPLE:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_SNOWFIELD, TEXT("Apple"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case JEWEL:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_SNOWFIELD, TEXT("Ore_001"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case MUSHROOM:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_SNOWFIELD, TEXT("Mushroom"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case LETTUCE:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_SNOWFIELD, TEXT("Lettuce_002"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	default:
		break;
	}
	

	return S_OK;
}

_bool CItem::Is_AnimationLoop(_uint eAnimId)
{
	return false;
}

HRESULT CItem::SetUp_ShaderID()
{
	if (false == m_bIsGain)
		m_eShaderID = SHADER_NONANIMDEFAULT;

	else
		m_eShaderID = SHADER_NONANIM_DISSOLVE;

	return S_OK;
}


