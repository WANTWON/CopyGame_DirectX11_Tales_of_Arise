#include "stdafx.h"
#include "..\Public\DamageFont.h"

#include "GameInstance.h"
#include "UI_RuneEffect.h"
#include "Player.h"
#include "PlayerManager.h"
#include "CriticalFont.h"
CDamageFont::CDamageFont(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CDamageFont::CDamageFont(const CDamageFont & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CDamageFont::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CDamageFont::Initialize(void * pArg)
{

	if (pArg != nullptr)
		memcpy(&m_damagedesc, pArg, sizeof(DMGDESC));

	//m_fTargetPos = m_damagedesc.fposition;
	m_fTargetPos = m_damagedesc.pPointer->Get_ProjPosition();
	m_iCurrentDamage = m_damagedesc.iDamage;
	
	m_fRandomOffset = { _float(rand() % 80)  , _float(rand() % 55) };

	_uint random = rand() % 2;

	if (random == 0)
		m_bplusminus = true;
	else
		m_bplusminus = false;
	
		

	if (m_damagedesc.bisNormal)
	{
		m_eShaderID = UI_BRIGHT;
		m_fAlpha = 1.f;


		m_fSize.x = 30.f * m_fScaler;
		m_fSize.y = 30.f * m_fScaler;
		m_fNext = 23.f;
	}
	else
	{
		m_fAlpha = 1.f;


		m_fSize.x = 30.f * m_fScaler;
		m_fSize.y = 30.f * m_fScaler;





		if (m_damagedesc.itype >= 4)
		{
			m_bfontmaker = false;
		}
		else
			m_bfontmaker = true;

		m_eShaderID = UI_BRIGHT;

		if (m_damagedesc.itype == 1)
		{

			m_fNext = 38.f;
		}
		else if (m_damagedesc.itype == 2)
		{

			m_fNext = 23.f;
		}
		else if (m_damagedesc.itype == 3)
		{

			m_fNext = 23.f;
		}


		else if (m_damagedesc.itype == 4)
		{

			m_fNext = 23.f;
		}
		else if (m_damagedesc.itype == 5)
		{

			m_fNext = 23.f;

		}
	}
	
	






	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	return S_OK;
}

int CDamageFont::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return OBJ_NOEVENT;

	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	//CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID();

	if (m_damagedesc.pPointer == nullptr)
	{
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Object(LEVEL_STATIC, TEXT("Layer_Damage"), this);
		return OBJ_POOL;
	}
		

	for (_uint i = 0; i < 4; ++i)
		m_fbrightpos_hp[i] += 0.015f;

	for (_uint i = 0; i < 4; ++i)
	{
		if (m_fbrightpos_hp[i] >= 2.f)
			m_fbrightpos_hp[i] = 0.f;
	}

	


	m_fStart_timer += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));;


	if (m_damagedesc.bisNormal)
	{
		if (m_bfadein)
		{

			m_fScaler -= 0.34f;
			if (m_fScaler <= 1.f)
			{
				m_fScaler = 1.f;
				m_bfadein = false;
			}

		}
		if (m_fStart_timer > 1.f)
			m_bfadeout = true;

		if (m_bfadeout)
		{
			m_fAlpha -= 0.25f;
			m_fYFadeout += 2.f;

			if (m_fAlpha <= 0.f)
			{
				CObject_Pool_Manager::Get_Instance()->Add_Pooling_Object(LEVEL_STATIC, TEXT("Layer_Damage"), this);
				return OBJ_POOL;
			}

		}



		m_fSize.x = 30.f * m_fScaler;
		m_fSize.y = 30.f * m_fScaler;
	}
	else
	{
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
			{
				CObject_Pool_Manager::Get_Instance()->Add_Pooling_Object(LEVEL_STATIC, TEXT("Layer_DamageCritical"), this);
				return OBJ_POOL;
			}
		}


		if (m_damagedesc.itype == 1)
		{
			m_fSize.x = 60.f * m_fScaler;
			m_fSize.y = 60.f * m_fScaler;
		}
		else
		{
			m_fSize.x = 30.f * m_fScaler;
			m_fSize.y = 30.f * m_fScaler;
		}

		if (m_balphaup)
		{
			if (m_bfontmaker)
			{
				CCriticalFont::DMGDESC fontdesc;
				ZeroMemory(&fontdesc, sizeof(CCriticalFont::DMGDESC));
				fontdesc.itype = m_damagedesc.itype;
				fontdesc.position = m_fPosition;
				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Critical"), LEVEL_STATIC, TEXT("dddd"), &fontdesc)))
					return OBJ_NOEVENT;
				m_bfontmaker = false;
			}

			m_fAlpha += 0.25f;

		}
	}
	
	
	if (m_bplusminus)
	{
		m_fPosition.x = m_fTargetPos.x + m_fRandomOffset.x;
		m_fPosition.y = m_fTargetPos.y - m_fRandomOffset.y - m_fYFadeout - 40.f;

	}
	else
	{
		m_fPosition.x = m_fTargetPos.x - m_fRandomOffset.x;
		m_fPosition.y = m_fTargetPos.y - m_fRandomOffset.y - m_fYFadeout - 40.f;


	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);



		
	

	return OBJ_NOEVENT;
}


void CDamageFont::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return ;
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

	if (m_bnorend)
	{
		m_bnorend = false;
		return;
	}

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);


}

HRESULT CDamageFont::Render()
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

		if (m_damagedesc.bisNormal)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
				return E_FAIL;
		}
		else
		{
			if (m_damagedesc.itype == 1)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else if (m_damagedesc.itype == 2)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else if (m_damagedesc.itype == 4)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else if (m_damagedesc.itype == 3 || m_damagedesc.itype == 5)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
		}
		
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

		if (m_damagedesc.bisNormal)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
				return E_FAIL;
		}
		else
		{
			if (m_damagedesc.itype == 1)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else if (m_damagedesc.itype == 2)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else if (m_damagedesc.itype == 4)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else if (m_damagedesc.itype == 3 || m_damagedesc.itype == 5)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
		}

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

		if (m_damagedesc.bisNormal)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
				return E_FAIL;
		}
		else
		{
			if (m_damagedesc.itype == 1)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else if (m_damagedesc.itype == 2)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else if (m_damagedesc.itype == 4)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else if (m_damagedesc.itype == 3 || m_damagedesc.itype == 5)
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
					return E_FAIL;
			}
		}
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

void CDamageFont::ReUse_Setting(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_damagedesc, pArg, sizeof(DMGDESC));

	//m_fTargetPos = m_damagedesc.fposition;
	if (m_damagedesc.bisNormal)
	{
		m_fSize.x = 30.f * m_fScaler;
		m_fSize.y = 30.f * m_fScaler;

		m_fNext = 23.f;	
		
	}
	else
	{	
	
		m_balphaup = false;

		

		m_fSize.x = 30.f * m_fScaler;
		m_fSize.y = 30.f * m_fScaler;
		m_fNext = 38.f;


		if (m_damagedesc.itype >= 4)
		{
			m_bfontmaker = false;
		}
		else
			m_bfontmaker = true;



		if (m_damagedesc.itype == 1)
		{
			//		m_eShaderID = UI_CRITICALDAMAGEFONT;
			m_fNext = 42.f;
		}
		else if (m_damagedesc.itype == 2)
		{
			//		m_eShaderID = UI_RESISTDAMAGEFONT;
			m_fNext = 23.f;
		}
		else if (m_damagedesc.itype == 3)
		{
			//		m_eShaderID = UI_PlayerHitfont;
			m_fNext = 23.f;
		}


		else if (m_damagedesc.itype == 4)
		{
			//		m_eShaderID = UI_RECOVERFONT;
			m_fNext = 23.f;
		}
		else if (m_damagedesc.itype == 5)
		{
			//		m_eShaderID = UI_PlayerHitfont;
			m_fNext = 23.f;

		}
	}
	m_fStart_timer = 0.f;
	m_fTargetPos = m_damagedesc.pPointer->Get_ProjPosition();
	m_iCurrentDamage = m_damagedesc.iDamage;

	m_fRandomOffset = { _float(rand() % 80)  , _float(rand() % 55) };

	_uint random = rand() % 2;

	if (random == 0)
		m_bplusminus = true;
	else
		m_bplusminus = false;

	m_fbrightpos_hp[0] = 0.f;
	m_fbrightpos_hp[1] = -0.5f;
	m_fbrightpos_hp[2] = -1.f;
	m_fbrightpos_hp[3] = -1.5f;
	m_eShaderID = UI_BRIGHT;//UI_BRIGHT;
	m_fAlpha = 1.f;
	m_fYFadeout = 0.f;
	m_fScaler = 2.f;
	m_bfadeout = false;
	m_bfadein = true;
	m_bnorend = true;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));


	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
}

HRESULT CDamageFont::Ready_Components(void * pArg)
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

	
	//	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_normalfont"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_bluefont"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_redfont"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_yellowfont"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture4"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_greenfont"), (CComponent**)&m_pTextureCom4)))
		return E_FAIL;

	

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamageFont::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (m_damagedesc.bisNormal)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
	}
	else
	{
		if (m_damagedesc.itype == 1)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_itexnum))))
				return E_FAIL;
		}
		else if (m_damagedesc.itype == 2)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum))))
				return E_FAIL;
		}
		else if (m_damagedesc.itype == 4)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(m_itexnum))))
				return E_FAIL;
		}
		else if (m_damagedesc.itype == 3 || m_damagedesc.itype == 5)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
				return E_FAIL;
		}
	}


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CDamageFont * CDamageFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CDamageFont*	pInstance = new CDamageFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CDamageFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CDamageFont::Clone(void * pArg)
{
	CDamageFont*	pInstance = new CDamageFont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CDamageFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageFont::Free()
{

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	Safe_Release(m_pTextureCom4);
	__super::Free();
}
