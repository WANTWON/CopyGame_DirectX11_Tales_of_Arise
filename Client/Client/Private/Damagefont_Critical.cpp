#include "stdafx.h"
#include "..\Public\Damagefont_Critical.h"

#include "GameInstance.h"
#include "UI_RuneEffect.h"
#include "Player.h"
#include "PlayerManager.h"
CDamagefont_Critical::CDamagefont_Critical(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CDamagefont_Critical::CDamagefont_Critical(const CDamagefont_Critical & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CDamagefont_Critical::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CDamagefont_Critical::Initialize(void * pArg)
{

	if (pArg != nullptr)
		memcpy(&m_damagedesc, pArg, sizeof(DMGDESC));

	m_fTargetPos = m_damagedesc.pPointer->Get_ProjPosition();
	m_iCurrentDamage = m_damagedesc.iDamage;

	m_fRandomOffset = { _float(rand() % 80)  , _float(rand() % 55) };

	_uint random = rand() % 2;

	if (random == 0)
		m_bplusminus = true;
	else
		m_bplusminus = false;

	//m_fFirstPos = m_fPosition = { m_fTargetPos.x -= rand() % 150  , m_fTargetPos.y -= rand() % 150 };



	/*if (m_fPosition.x > 1280.f)
	m_fPosition.x = 1270.f;
	else if (m_fPosition.x < 0.f)
	m_fPosition.x = 10.f;

	if (m_fPosition.y > 720)
	m_fPosition.y = 710.f;
	else if (m_fPosition.y < 0.f)
	m_fPosition.y = 10.f;*/
	/*if (m_iIndex == CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1)
	m_bforMainPlayer = true;*/

	m_eShaderID = UI_BRIGHT;
	m_fAlpha = 1.f;


	m_fSize.x = 30.f * m_fScaler;
	m_fSize.y = 30.f * m_fScaler;
	m_fNext = 34.f;
	//m_fPosition.y -= m_fYFadeout;



	//desc.position.x = 1130.f;
	//m_fPosition.x = 1180.f;
	//if (m_iIndex == 0)
	//{
	//	//		m_fPosition.x = 1180.f + (m_iIndex * 14);


	//	m_fnumberY = desc.position.y = m_fPosition.y = 375.f;


	//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
	//		return E_FAIL;

	//	desc.position.x = 1060.f;
	//	desc.position.y = m_fnumberY - 34.f;
	//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
	//		return E_FAIL;

	//}
	//if (m_iIndex == 1)
	//{

	//	m_fnumberY = desc.position.y = m_fPosition.y = 435.f;


	//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
	//		return E_FAIL;
	//	desc.position.x = 1060.f;
	//	desc.position.y = m_fnumberY - 34.f;
	//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
	//		return E_FAIL;
	//}
	//if (m_iIndex == 2)
	//{

	//	m_fnumberY = desc.position.y = m_fPosition.y = 495.f;


	//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
	//		return E_FAIL;
	//	desc.position.x = 1060.f;
	//	desc.position.y = m_fnumberY - 34.f;
	//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
	//		return E_FAIL;

	//}
	//	if (m_iIndex == 3)
	//	{
	////		m_fPosition.x = 1180.f + (m_iIndex * 14);
	//		m_fnumberY = desc.position.y = m_fPosition.y = 555;
	//	}







	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	return S_OK;
}

int CDamagefont_Critical::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	//CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID();

	if (m_damagedesc.pPointer == nullptr)
		return OBJ_DEAD;

	for (_uint i = 0; i < 4; ++i)
		m_fbrightpos_hp[i] += 0.015f;

	for (_uint i = 0; i < 4; ++i)
	{
		if (m_fbrightpos_hp[i] >= 2.f)
			m_fbrightpos_hp[i] = 0.f;
	}




	m_fStart_timer += fTimeDelta;




	if (m_bfadein)
	{

		m_fScaler -= 0.34f;
		m_fAlpha -= 0.33f;
		if (m_fScaler <= 1.f)
		{
			m_fScaler = 1.f;
			m_fAlpha = 0.f;
			m_balphaup = true;
			m_bfadein = false;
		}

	}

	

	if (m_fStart_timer > 1.f)
	{
		m_fAlpha = 1.f;
		m_fStart_timer = 0.f;
		m_balphaup = false;
		m_bfadeout = true;
	}
		

	if (m_bfadeout)
	{
		m_fAlpha -= 0.25f;
		m_fYFadeout += 2.f;

		if (m_fAlpha <= 0.f)
			return OBJ_DEAD;

	}



	m_fSize.x = 60.f * m_fScaler;
	m_fSize.y = 60.f * m_fScaler;
	if (m_bplusminus)
	{
	m_fPosition.x = m_fTargetPos.x + m_fRandomOffset.x;
	m_fPosition.y = m_fTargetPos.y - m_fRandomOffset.y - m_fYFadeout - 40.f;
	//	m_fPosition.x = m_damagedesc.pPointer->Get_ProjPosition().x + m_fRandomOffset.x;
	//	m_fPosition.y = m_damagedesc.pPointer->Get_ProjPosition().y - m_fRandomOffset.y - m_fYFadeout - 40.f;
	}
	else
	{
		m_fPosition.x = m_fTargetPos.x - m_fRandomOffset.x;
		m_fPosition.y = m_fTargetPos.y - m_fRandomOffset.y - m_fYFadeout - 40.f;

//		m_fPosition.x = m_damagedesc.pPointer->Get_ProjPosition().x - m_fRandomOffset.x;
//		m_fPosition.y = m_damagedesc.pPointer->Get_ProjPosition().y - m_fRandomOffset.y - m_fYFadeout - 40.f;
		//	m_fPosition.x = m_damagedesc.pPointer->Get_ProjPosition().y - m_fRandomOffset.y - m_fYFadeout - 40.f;;
	}

	if (m_balphaup)
	{
		if (m_bfontmaker)
		{
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Critical"), LEVEL_STATIC, TEXT("dddd"), &m_fPosition)))
				return OBJ_NOEVENT;
			m_bfontmaker = false;
		}

		m_fAlpha += 0.25f;

	}
	//m_fPosition.x = m_damagedesc.pPointer->Get_ProjPosition().x;
	//m_fPosition.y = m_fPosition.y - m_fYFadeout - 40.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//	m_fAlpha += 0.01f;


	//if (CGameInstance::Get_Instance()->Key_Up(DIK_9))
	//{
	//	CUI_RuneEffect::RUNEDESC desc;
	//	desc.position.x = 1130.f;
	//	desc.position.y = 375.f;
	////	desc.m_etype = 1;
	//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, TEXT("test"), &desc)))
	//		return E_FAIL;
	//	desc.position.y = 435.f;
	//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, TEXT("test"), &desc)))
	//		return E_FAIL;
	//	desc.position.y = 495.f;
	//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, TEXT("test"), &desc)))
	//		return E_FAIL;
	//}

	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_9))
	{
	CUI_RuneEffect::RUNEDESC desc;
	desc.position.x = 1130.f;
	desc.position.y = 375.f;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
	return E_FAIL;
	desc.position.y = 435.f;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
	return E_FAIL;
	desc.position.y = 495.f;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
	return E_FAIL;
	}*/


	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_L))
	{

	CUI_Get_item_Popup::POPUPDESC testdesc;
	ZeroMemory(&testdesc, sizeof(CUI_Get_item_Popup::POPUPDESC));
	testdesc.iIndex = 0;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"), LEVEL_STATIC, TEXT("TETE"), &testdesc)))
	return E_FAIL;
	testdesc.iIndex = 1;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"), LEVEL_STATIC, TEXT("TETE"), &testdesc)))
	return E_FAIL;
	testdesc.iIndex = 2;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"), LEVEL_STATIC, TEXT("TETE"), &testdesc)))
	return E_FAIL;
	testdesc.iIndex = 3;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"), LEVEL_STATIC, TEXT("TETE"), &testdesc)))
	return E_FAIL;

	}*/


	/*if (CGameInstance::Get_Instance()->Key_Pressing(DIK_K))
	--m_iCurrenthp;

	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_J))
	++m_iCurrenthp;*/

	//if (!m_bforMainPlayer)
	//{
	//	m_fSize.x = 16.f;
	//	m_fSize.y = 20.f;
	//	m_fPosition.x = 1180.f;
	//	m_fPosition.y = m_fnumberY;
	//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	/*m_fSize.x = 40.f;
	m_fSize.y = 20.f;*/


	//}
	//else
	//{
	//	/*CGameObject* pGameObject = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	//	CTransform*	pPlayerTransform = (CTransform*)CGameInstance::Get_Instance()->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));*/
	//	Compute_CamDistance(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));//Get_State(CTransform::STATE_TRANSLATION));
	//	m_fPosition.x = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_ProjPosition().x + 44.f;
	//	m_fPosition.y = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_ProjPosition().y + 31.f - (m_fCamDistance / 5.f);

	//	if (m_fCamDistance > 20.f)
	//	{
	//		m_fNext = 14.f / m_fCamDistance * 20;
	//		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 16.f / m_fCamDistance * 18.f);
	//		m_pTransformCom->Set_Scale(CTransform::STATE_UP, 16.f / m_fCamDistance * 18.f);
	//		m_fPosition.x += m_fCamDistance *0.1f - 10.f;//(m_fNext*3);
	//	}
	//	else
	//	{
	//		m_fNext = 14.f;
	//		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 16.f);
	//		m_pTransformCom->Set_Scale(CTransform::STATE_UP, 16.f);
	//	}

	//}

	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));


	return OBJ_NOEVENT;
}


void CDamagefont_Critical::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return ;

	/*if (m_fStart_timer > 0.8f)
	{
	if (m_fAlpha >= 1.f)
	{
	m_fAlpha = 1.f;
	m_bfadein = false;
	}


	}*/
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);


}

HRESULT CDamagefont_Critical::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_itexnum = m_iCurrentDamage / 1000;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	if (m_iCurrentDamage >= 1000)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	if (m_iCurrentDamage >= 100)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
			return E_FAIL;
		m_itexnum = ((m_iCurrentDamage % 1000) / 100);
		m_fPosition.x = m_fPosition.x + m_fNext;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	if (m_iCurrentDamage >= 10)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
			return E_FAIL;
		m_itexnum = ((m_iCurrentDamage % 100) / 10);
		m_fPosition.x = m_fPosition.x + m_fNext;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}


	if (m_iCurrentDamage >= 1)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
			return E_FAIL;
		m_itexnum = m_iCurrentDamage % 10;
		m_fPosition.x = m_fPosition.x + m_fNext;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();


	}

	/*if (!m_bforMainPlayer)
	{
	m_fPosition.x = 1060.f;
	m_fPosition.y = m_fnumberY - 34.f;
	m_fSize.x = 44.f;
	m_fSize.y = 32.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_iCharactername))))
	return E_FAIL;
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();
	}*/


	return S_OK;
}

HRESULT CDamagefont_Critical::Ready_Components(void * pArg)
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamagefont_Critical::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CDamagefont_Critical * CDamagefont_Critical::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDamagefont_Critical*	pInstance = new CDamagefont_Critical(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CDamagefont_Critical"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CDamagefont_Critical::Clone(void * pArg)
{
	CDamagefont_Critical*	pInstance = new CDamagefont_Critical(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CDamagefont_Critical"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamagefont_Critical::Free()
{

	Safe_Release(m_pTextureCom1);
	__super::Free();
}
