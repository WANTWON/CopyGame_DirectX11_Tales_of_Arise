#include "stdafx.h"
#include "..\Public\UI_BattleResult.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerManager.h"



CUI_BattleResult::CUI_BattleResult(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_BattleResult::CUI_BattleResult(const CUI_BattleResult & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_BattleResult::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_BattleResult::Initialize(void * pArg)
{
	/*if (pArg != nullptr)
		memcpy(&m_popupdesc, pArg, sizeof(POPUPDESC));*/

	CUI_Manager::Get_Instance()->Set_SkillMsg(this);
	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fAlpha = 0;
	m_btick = true;
	//if(m_popupdesc.iIndex == 0)
	/*switch (m_popupdesc.iIndex)
	{
	case 0:
	m_fIndexOffsetY = 40.f;
	break;

	case 1:
	m_fIndexOffsetY = 80.f;
	break;

	case 2:
	m_fIndexOffsetY = 120.f;
	break;

	case 3:
	m_fIndexOffsetY = 160.f;
	break;

	case 4:
	m_fIndexOffsetY = 160.f;
	break;
	case 5:
	m_fIndexOffsetY = 200.f;
	break;
	}*/


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_BattleResult::Tick(_float fTimeDelta)
{
	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_3))
	{
	m_bfadein = true;
	m_fAlpha = 0.f;
	m_fFadeX = 200.f;
	m_iNum = rand() % 6;
	timer = 0.f;


	}*/
	m_fbrightpos += 0.015f;
	m_fbrightpos_hpbar += 0.015f;

	for (_uint i = 0; i < 9; ++i)
		m_fbrightpos_hpfont[i] += 0.015f;

	if (m_fbrightpos >= 3.f)
		m_fbrightpos = 0.f;

	if (m_fbrightpos_hpbar >= 5.f)
		m_fbrightpos_hpbar = 0.f;



	for (_uint i = 0; i < 9; ++i)
	{
		if (m_fbrightpos_hpfont[i] >= 3.f)
			m_fbrightpos_hpfont[i] = 0.f;
	}
	_uint index = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1;
	for (_uint i = 0; i < index; ++i)
	{
		m_PlayersCurrentHP[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fCurrentHp;
		m_PlayersMaxHP[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fMaxHp;

		m_PlayersCurrentExp[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().iCurrentExp;
		m_PlayersMaxExp[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().iMaxExp;

		m_PlayersLevel[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().iLevel;
	}

	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_4))
	{
	m_bfadeout = true;
	}*/

	/*if (!m_btick)
		return OBJ_NOEVENT;*/

	//if (m_bfirstarrived)
	timer += fTimeDelta;

	/*if (timer > 4.f)
	{
		m_bfadeout = true;


	}*/


	if (m_bfadein)
	{
		m_fAlpha += 0.1f; //»ý±æ¶§
		m_fFadeX -= 18.f;
		if (m_fFadeX <= -20.f)
		{
			m_bfadein = false;
			m_fAlpha = 1.f;
			m_fFadeX = -20.f;
		}

		//m_bfirstarrived = true;
	}


	else if (m_bfadeout)
	{
		m_fAlpha -= 0.1f;
		m_fAlpha1 -= 0.1f;
		//	m_fFade1X -= 2.f;
		m_fFadeX -= 18.f;

		if (m_fAlpha <= 0.f)
		{
			m_bfadeout = false;
			/*m_fAlpha = 0.f;
			m_fFadeX = 180.f;*/
			m_btick = false;
		}

	}






	/*if (m_fAlpha >= 1.f && m_bfadein)
	{
	m_fAlpha = 1.f;
	m_bfadein = false;

	}*/



	/*if (m_fAlpha <= 0.f && m_bfadeout)
	{
	m_fAlpha = 0.f;
	m_bfadeout = false;
	m_fAlpha1 = 0;
	m_btick = false;

	return OBJ_DEAD;
	}*/
	return OBJ_NOEVENT;
}






void CUI_BattleResult::Late_Tick(_float fTimeDelta)
{

	if (m_btick)
	{


		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	}



}

HRESULT CUI_BattleResult::Render()
{
	if (!m_btick)
		return S_OK;


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	_float alpha = m_fAlpha * 0.8f;

	m_fSize.x = 430.f;
	m_fSize.y = 520.f;
	m_fPosition.x = 50.f - m_fFadeX;
	m_fPosition.y = 330.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_DIALOGUEBOX);

	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 300.f;
	m_fSize.y = 4.f;
	m_fPosition.x = 100.f - m_fFadeX;
	m_fPosition.y = 540.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();

	m_fPosition.y = 115.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();

	//m_fSize.x = 28.f;
	//m_fSize.y = 28.f;
	//m_fPosition.x = 600.f - m_fFadeX;
	//m_fPosition.y = 120.f;// + m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(5))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_BRIGHT);

	//m_pVIBufferCom->Render();

	//m_fSize.x = 240.f;
	//m_fSize.y = 26.f;
	//m_fPosition.x = 700.f - m_fFadeX;
	//m_fPosition.y = 120.f;// + m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_iNum))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_BRIGHT);

	//m_pVIBufferCom->Render();



	Render_Player1();









	return S_OK;
}

HRESULT CUI_BattleResult::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillmsg"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Exp"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPbar"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture4"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_LIGHTEFFECT"), (CComponent**)&m_pTextureCom4)))
		return E_FAIL;
	
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture5"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"), (CComponent**)&m_pTextureCom5)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture6"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"), (CComponent**)&m_pTextureCom6)))
		return E_FAIL;


	///* For.Com_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_Texture26"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Exp"), (CComponent**)&m_pTextureCom26)))
	//	return E_FAIL;

	///* For.Com_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_name"), (CComponent**)&m_pTextureCom2)))
	//	return E_FAIL;



	return S_OK;
}

HRESULT CUI_BattleResult::SetUp_ShaderResources()
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

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
	return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fFlowCurrent, sizeof(_float))))
	return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fFlowMAX, sizeof(_float))))
	return E_FAIL;*/


	return S_OK;
}

HRESULT CUI_BattleResult::Render_Player1()
{
	m_fSize.x = 48.f;
	m_fSize.y = 48.f;
	m_fPosition.x = 78.f;
	m_fPosition.y = 124.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	_float thrower = (_float)m_PlayersMaxExp[0] - (_float)m_PlayersCurrentExp[0];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &thrower, sizeof(_float))))
		return E_FAIL;
	thrower = (_float)m_PlayersMaxExp[0];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &thrower, sizeof(_float))))
		return E_FAIL;
	_float2 garara = { 1.f,1.f };
	if (FAILED(m_pShaderCom->Set_RawValue("g_WinXY", &garara, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_EXPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 20.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 70.f;
	m_fPosition.y = 125.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_PlayersLevel[0] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersLevel[0] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersLevel[0] % 10);
	if (m_PlayersLevel[0] >= 10)
		m_fPosition.x += 14.f;
	else
		m_fPosition.x += 6.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();



	m_fSize.x = 44.f;
	m_fSize.y = 28.f;
	m_fPosition.x = 130.f;
	m_fPosition.y = 110.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
	return E_FAIL;*/


	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();




	m_fSize.x = 200.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpbar, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 15.f;
	m_fPosition.y += 18.f;

	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[0] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[0] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[0] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[0] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[0] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[0] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[2], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersCurrentHP[0] % 10);

	m_fPosition.x += 12.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 16.f;
	m_fSize.y = 20.f;
	m_fPosition.x += 12.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(10))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[4], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 12.f;
	m_fSize.y = 16.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_fPosition.x += 9.f;
	if ((_uint)m_PlayersMaxHP[0] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[0] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[5], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[0] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[0] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[6], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[0] >= 10.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[0] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[7], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersMaxHP[0] % 10);

	m_fPosition.x += 9.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[8], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_BattleResult::Render_Player2()
{
	return E_NOTIMPL;
}

HRESULT CUI_BattleResult::Render_Player3()
{
	return E_NOTIMPL;
}

HRESULT CUI_BattleResult::Render_Player4()
{
	return E_NOTIMPL;
}




void CUI_BattleResult::Skillmsg_on(_uint index)//, _uint index1)
{

	m_btick = true;

	m_bfadein = true;
	m_fAlpha = 0.f;
	m_fFadeX = 200.f;
	m_iNum = index;
	timer = 0.f;
}




CUI_BattleResult * CUI_BattleResult::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_BattleResult*	pInstance = new CUI_BattleResult(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_BattleResult"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_BattleResult::Clone(void * pArg)
{
	CUI_BattleResult*	pInstance = new CUI_BattleResult(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_BattleResult"));
		Safe_Release(pInstance);
	}

	/*CUI_Manager::Get_Instance()->Get_Itempopup_list()->push_back(pInstance);*/

	return pInstance;
}

void CUI_BattleResult::Free()
{

	//CUI_Manager::Get_Instance()->Erase_Itempopup_list(this);

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	Safe_Release(m_pTextureCom4);
	Safe_Release(m_pTextureCom5);
	Safe_Release(m_pTextureCom6);
	
	//Safe_Release(m_pTextureCom2);
	__super::Free();
}


