#include "stdafx.h"
#include "..\Public\UI_Skillbutton.h"

#include "GameInstance.h"
#include "PlayerManager.h"
#include "Player.h" 
#include "UI_Skillmessage.h"
CUI_Skillbutton::CUI_Skillbutton(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Skillbutton::CUI_Skillbutton(const CUI_Skillbutton & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Skillbutton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Skillbutton::Initialize(void * pArg)
{
	m_eShaderID = 0;

	m_fSize.x = 32;
	m_fSize.y = 32.f;
	m_fPosition.x = 1200.f;
	m_fPosition.y = 500.f;

	m_fAlpha = 1;

	m_bfadein = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Skillbutton::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	switch (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID())
	{
	case CPlayer::PLAYERID::ALPHEN:
		m_bSkill1non = true;
		m_bSkill2non = true;
		m_bSkill3non = true;

		m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_CHOOSAWOO;
		m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_BEEYEONIN;
		m_iSkill1name3 = CUI_Skillmessage::SKILLNAME::SKILLNAME_BOONGSUPGEEAKJIN;
		if (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_IsFly())
		{
			m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_SUMGONGSANGEULPA;
			m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_YOUSEONGJIN;
			m_iSkill1name3 = CUI_Skillmessage::SKILLNAME::SKILLNAME_ONEWALL;
		}

		
		break;

	case CPlayer::PLAYERID::SION:

		m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_GRAVITY;
		m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_MAGNARAY;
		m_iSkill1name3 = CUI_Skillmessage::SKILLNAME::SKILLNAME_EXPLODE;
		if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL))
		{
			m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_AQUARUINA;
			m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_GLACIA;
			m_bSkill3non = false;
		}
		else
			m_bSkill3non = true;

		if (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_IsFly())
		{
			//	m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_SUMGONGSANGEULPA;
			m_bSkill1non = false;
			m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_TRESVENTUS;
			m_bSkill3non = false;
			//m_iSkill1name3 = CUI_Skillmessage::SKILLNAME::SKILLNAME_ONEWALL;
		}
		else
		{
			m_bSkill1non = true;
			m_bSkill3non = true;
		}

		/*if (!CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL))
		{

			m_bSkill3non = false;
		}
		else
			m_bSkill3non = true;*/
			


		break;

	case CPlayer::PLAYERID::RINWELL:
		m_bSkill1non = true;
		m_bSkill2non = true;
		m_bSkill3non = true;
		m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_THUNDERFIELD;
		m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_GALEFORCE;
		m_iSkill1name3 = CUI_Skillmessage::SKILLNAME::SKILLNAME_METEORSWARM;
		if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL))
		{
			m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_DIVINESABER;
			m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_BANGJEON;
			m_iSkill1name3 = CUI_Skillmessage::SKILLNAME::SKILLNAME_HOLYRANCE;
			//m_bSkill3non = false;
		}
		//else
		//	m_bSkill3non = true;

		//if (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_IsFly())
		//{
		//	//	m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_SUMGONGSANGEULPA;
		//	m_bSkill1non = false;
		//	m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_TRESVENTUS;
		//	m_bSkill3non = false;
		//	//m_iSkill1name3 = CUI_Skillmessage::SKILLNAME::SKILLNAME_ONEWALL;
		//}
		//else
		//{
		//	m_bSkill1non = true;
		//	m_bSkill3non = true;
		//}
		break;

	case CPlayer::PLAYERID::LAW:
		m_bSkill1non = true;
		m_bSkill2non = true;
		m_bSkill3non = true;

		m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_WARYUNGGONGPA;
		m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_YOONMUSUNPOONG;
		m_iSkill1name3 = CUI_Skillmessage::SKILLNAME::SKILLNAME_CHOYUNATAN;
		if (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_IsFly())
		{
			m_iSkill1name1 = CUI_Skillmessage::SKILLNAME::SKILLNAME_SANHWAMANGSUP;
			m_iSkill1name2 = CUI_Skillmessage::SKILLNAME::SKILLNAME_YUENSOKWAN;
			m_iSkill1name3 = CUI_Skillmessage::SKILLNAME::SKILLNAME_YUMPAEFUCKSAY;
		}


		break;


	}

	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_3))
	{
		m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f - 45 + 160;
		m_bmoveleft = true;
		m_bfadein = true;
		m_fAlpha = 1;
	}
*/
	if (m_bfadein)
		m_fAlpha -= 0.04f; //생길때
	else if (m_bfadeout)
		m_fAlpha += 0.0483f;


	if (CGameInstance::Get_Instance()->Key_Up(DIK_4)) // 사라질때
	{
		m_bfadeout = true;
	}




    m_fSize.x = 32.f;
	m_fSize.y = 32.f;
	m_fPosition.x = 1170.f;
	m_fPosition.y = 570.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	return OBJ_NOEVENT;
}

void CUI_Skillbutton::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return ;


	if (m_fAlpha <= 0)
	{
		m_fAlpha = 0;
		m_bfadein = false;
	}

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CUI_Skillbutton::Render()
{

	

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();



	m_fPosition.x = 1140.f;
	m_fPosition.y = 610.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

		m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


	m_fPosition.x = 1170.f;
	m_fPosition.y = 650.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(2))))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


		
	_float alpha = m_fAlpha * 0.5f;

	//m_fSize.x = 80.f;
	//m_fSize.y = 40.f;
	//m_fPosition.x = 1100.f;
	//m_fPosition.y = 600.f;// +m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
	//	return E_FAIL;
	//m_pShaderCom->Begin(UI_DIALOGUEBOX);

	//m_pVIBufferCom->Render();
	alpha = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 150.f;
	m_fSize.y = 2.5f;
	m_fPosition.x = 1090.f;
	m_fPosition.y = 580.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();

	
	m_fSize.x = 200.f;
	m_fSize.y = 24.f;
	m_fPosition.x = 1135.f;
	m_fPosition.y = 567.f;// + m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_iSkill1name1))))
		return E_FAIL;

	if (m_bSkill1non)
	{
		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();
	}

	

	//_float alpha = m_fAlpha * 0.5f;

	//m_fSize.x = 80.f;
	//m_fSize.y = 40.f;
	//m_fPosition.x = 1070.f;
	//m_fPosition.y = 640.f;// +m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
	//	return E_FAIL;
	//m_pShaderCom->Begin(UI_DIALOGUEBOX);

	//m_pVIBufferCom->Render();
	//alpha = 1.f;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
	//	return E_FAIL;

	m_fSize.x = 150.f;
	m_fSize.y = 2.5f;
	m_fPosition.x = 1060.f;
	m_fPosition.y = 620.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();


	m_fSize.x = 200.f;
	m_fSize.y = 24.f;
	m_fPosition.x = 1095.f;
	m_fPosition.y = 607.f;// + m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_iSkill1name2))))
		return E_FAIL;

	if (m_bSkill2non)
	{
		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();
	}

	

	////m_fSize.x = 80.f;
	////m_fSize.y = 40.f;
	////m_fPosition.x = 1100.f;
	////m_fPosition.y = 680.f;// +m_fFadeY + m_fIndexOffsetY;
	////m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	////m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	////m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	////if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	////	return E_FAIL;
	////if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
	////	return E_FAIL;
	////if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
	////	return E_FAIL;
	////m_pShaderCom->Begin(UI_DIALOGUEBOX);

	////m_pVIBufferCom->Render();
	alpha = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 150.f;
	m_fSize.y = 2.5f;
	m_fPosition.x = 1090.f;
	m_fPosition.y = 660.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();


	m_fSize.x = 200.f;
	m_fSize.y = 24.f;
	m_fPosition.x = 1130.f;
	m_fPosition.y = 647.f;// + m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_iSkill1name3))))
		return E_FAIL;

	if (m_bSkill3non)
	{
		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();
	}
	


	

	return S_OK;
}

HRESULT CUI_Skillbutton::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillbutton"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillmsg"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillname"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;


	

	return S_OK;
}

HRESULT CUI_Skillbutton::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CUI_Skillbutton * CUI_Skillbutton::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Skillbutton*	pInstance = new CUI_Skillbutton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Skillbutton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_Skillbutton::Clone(void * pArg)
{
	CUI_Skillbutton*	pInstance = new CUI_Skillbutton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Skillbutton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skillbutton::Free()
{

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	
	__super::Free();
}
