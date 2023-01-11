#include "stdafx.h"
#include "..\Public\InvenTile.h"

#include "GameInstance.h"
#include "InvenItem.h"

CInvenTile::CInvenTile(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CObj_UI(pDevice, pContext)
{
}

CInvenTile::CInvenTile(const CInvenTile & rhs)
	: CObj_UI(rhs)
	, m_InvenDesc(rhs.m_InvenDesc)
{
}

HRESULT CInvenTile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInvenTile::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_InvenDesc, pArg, sizeof(INVENDESC));

	m_fSize.x = 110;
	m_fSize.y = 110;
	m_fPosition = m_InvenDesc.vPosition;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_eShaderID = UI_ALPHABLEND;

	if (m_InvenDesc.eTileType == INVEN_TILE)
		CUI_Manager::Get_Instance()->Add_InvenGroup(this);

	CInvenItem::ITEMDESC ItemDesc;
	ItemDesc.eItemType = CInvenItem::ITEM_USABLE;
	ItemDesc.m_iTextureNum = CInvenItem::ITEM_NONE;
	ItemDesc.vPosition = m_fPosition;

	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_CInvenItem"), LEVEL_STATIC, TEXT("Layer_InvenItem"), &ItemDesc)))
		return E_FAIL;

	_uint iTextureNum = 0;
	list<CGameObject*>* pItemList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_InvenItem"));

	if (pItemList != nullptr) //ø÷ Release Mode¿œ ∂ß æ»µ ?
	{
		m_pItem = dynamic_cast<CInvenItem*>(pItemList->back());
		iTextureNum = 0;
	}
	

	if (m_InvenDesc.eTileType == EQUIP_TILE)
	{
		dynamic_cast<CInvenItem*>(m_pItem)->Set_TextureNum(0);
		dynamic_cast<CInvenItem*>(m_pItem)->Set_ItemSize(_float2(85, 85));
	}
	else
		dynamic_cast<CInvenItem*>(m_pItem)->Set_TextureNum(iTextureNum);

	return S_OK;
}

int CInvenTile::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	return OBJ_NOEVENT;
}

void CInvenTile::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_NextLevel() == true)
		return;

	__super::Late_Tick(fTimeDelta);

	if (CUI_Manager::Get_Instance()->Get_UI_OpenType() != CUI_Manager::UI_INVEN && m_InvenDesc.eTileType != EQUIP_TILE)
		dynamic_cast<CInvenItem*>(m_pItem)->Set_bShow(false);
	else
		dynamic_cast<CInvenItem*>(m_pItem)->Set_bShow(true);


	CObj_UI* pItemObj = nullptr;

	if (m_InvenDesc.eTileType == EQUIP_TILE)
	{
		switch (m_InvenDesc.eEquipKey)
		{
		case EQUIP_X:
			pItemObj = CUI_Manager::Get_Instance()->Get_EquipItem(CUI_Manager::EQUIP_X);
			break;
		case EQUIP_Y:
			pItemObj = CUI_Manager::Get_Instance()->Get_EquipItem(CUI_Manager::EQUIP_Y);
			break;
		}
		if (pItemObj == nullptr)
			return;

		_uint itextureNum = dynamic_cast<CInvenTile*>(pItemObj)->Get_TextureNum();
		dynamic_cast<CInvenItem*>(m_pItem)->Set_TextureNum(itextureNum);
	}

}

HRESULT CInvenTile::Render()
{
	if (CUI_Manager::Get_Instance()->Get_UI_OpenType() != CUI_Manager::UI_INVEN && m_InvenDesc.eTileType != EQUIP_TILE)
		return E_FAIL;
		

	__super::Render();

	return S_OK;
}

void CInvenTile::Set_TextureNum(_uint iNum)
{
	if (m_pItem == nullptr)
		return;

	return m_pItem->Set_TextureNum(iNum);
}

_uint CInvenTile::Get_TextureNum()
{
	if (m_pItem == nullptr)
		return 0;

	return m_pItem->Get_TextureNum();
}

HRESULT CInvenTile::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	switch (m_InvenDesc.eTileType)
	{
	case INVEN_TILE:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_InvenTile"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case EQUIP_TILE:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_EquipTile"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInvenTile::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;
	
	if (m_InvenDesc.eTileType == EQUIP_TILE)
	{
		if (CUI_Manager::Get_Instance()->Get_UI_OpenType() == CUI_Manager::UI_INVEN)
			m_InvenDesc.eState = STATE_EQUIP;
		else
			m_InvenDesc.eState = STATE_DEFAULT;
	}

	_float m_fAlpha = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_InvenDesc.eState))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInvenTile::SetUp_ShaderID()
{
	if (m_bSelected)
		m_eShaderID = UI_PICKED;
	else
		m_eShaderID = UI_ALPHABLEND;

	return S_OK;
}

CInvenTile * CInvenTile::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CInvenTile*	pInstance = new CInvenTile(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CInvenTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CInvenTile::Clone(void * pArg)
{
	CInvenTile*	pInstance = new CInvenTile(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CInvenTile"));
		Safe_Release(pInstance);
	}

	


	return pInstance;
}

void CInvenTile::Free()
{
	__super::Free();
}
