#include "stdafx.h"
#include "InvenItem.h"
#include "GameInstance.h"
#include "UIEffect.h"

CInvenItem::CInvenItem(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CObj_UI(pDevice, pContext)
{
}

CInvenItem::CInvenItem(const CInvenItem & rhs)
	: CObj_UI(rhs)
{
}

HRESULT CInvenItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInvenItem::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_ItemDesc, pArg, sizeof(ITEMDESC));


	m_fSize.x = 95;
	m_fSize.y = 95;
	m_fPosition = m_ItemDesc.vPosition;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_eShaderID = UI_ALPHABLEND; 


	switch (m_ItemDesc.eItemType)
	{
	case ITEM_PRIZE:
		m_fSize = _float2(90, 90);
		break;
	case ITEM_EQUIP:
	case ITEM_QUEST:
		m_fSize = _float2(70, 70);
		break;
	case ITEM_DGNKEY:
		m_fSize = _float2(70, 70);
		break;
	case ITEM_USABLE:
		m_fSize = _float2(95, 95);
		break;
	default:
		break;
	}
		

	return S_OK;
}

int CInvenItem::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	return OBJ_NOEVENT;
}

void CInvenItem::Late_Tick(_float fTimeDelta)
{
	//__super::Late_Tick(fTimeDelta);

	if (CUI_Manager::Get_Instance()->Get_NextLevel() == true)
		return;


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_FRONT, this);

	if (m_ItemDesc.eItemType != ITEM_USABLE && m_ItemDesc.eItemType != ITEM_PRIZE)
		m_bShow = CUI_Manager::Get_Instance()->Get_UI_OpenType() == CUI_Manager::UI_INVEN ? true : false;


	if (m_ItemDesc.eItemType == ITEM_PRIZE && m_ItemDesc.m_iTextureNum == COMPASS)
	{

		if (!m_bCompass)
		{
			m_bFirst = false;
			m_falpha += 0.025f;
			if (m_falpha >= 1.f)
			{
				m_falpha = 1.f;
			}
				
		}
		else
		{
			if (!m_bFirst)
			{
				m_bFirst = true;
			}
			


			m_falpha -= 0.025f;
			if (m_falpha <= 0.f)
				m_falpha = 0.f;

			m_fEffectEndTime = 1.5f;
			m_fEffectTime += fTimeDelta;
			if (m_fEffectTime > m_fEffectEndTime)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Composs_On.wav"), SOUND_SYSTEM, 0.6f);
				CUIEffect::EFFECTDESC EffectDesc;
				EffectDesc.eEffectID = CUIEffect::COMPOSS_RING;
				EffectDesc.vInitPositon = m_fPosition;
				EffectDesc.fDeadTime = 1.f;
				EffectDesc.vColor = XMVectorSet(255, 255, 255, 255);
				EffectDesc.vInitScale = _float2(30, 30);
				EffectDesc.iTextureNum = 2;
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);
				m_fEffectTime = 0.f;
			}

		}
			m_fPosition.x  = (1 - m_falpha) * 1200 + m_falpha *1400;
	}

}

HRESULT CInvenItem::Render()
{
	if (!m_bShow)
		return E_FAIL;

	__super::Render();

	return S_OK;
}

HRESULT CInvenItem::Ready_Components(void * pArg)
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

	switch (m_ItemDesc.eItemType)
	{
	case ITEM_EQUIP:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_EquipItem"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case ITEM_USABLE:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_UsableItem"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case ITEM_COLLECT:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CollectItem"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case ITEM_DGNKEY:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_DungeonKey"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case ITEM_QUEST:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuestItem"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case ITEM_PRIZE:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_PrizeItem"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInvenItem::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_ItemDesc.m_iTextureNum))))
		return E_FAIL;

	return S_OK;
}


CInvenItem * CInvenItem::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CInvenItem*	pInstance = new CInvenItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CInvenItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CInvenItem::Clone(void * pArg)
{
	CInvenItem*	pInstance = new CInvenItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CInvenItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInvenItem::Free()
{
	__super::Free();
}
