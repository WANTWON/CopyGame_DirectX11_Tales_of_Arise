#include "stdafx.h"
#include "..\Public\UI_JustDodgeEffect.h"

#include "GameInstance.h"
#include "Player.h"
#include "PlayerManager.h"
CUI_JustDodgeEffect::CUI_JustDodgeEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_JustDodgeEffect::CUI_JustDodgeEffect(const CUI_JustDodgeEffect & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_JustDodgeEffect::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CUI_JustDodgeEffect::Initialize(void * pArg)
{

	if (pArg != nullptr)
		memcpy(&pPointer, pArg, sizeof(CBaseObj*));

	//m_fTargetPos = m_damagedesc.fposition;
	m_fTargetPos = pPointer->Get_ProjPosition();


	//m_eShaderID = UI_BRIGHT;
	m_fAlpha = 0.5f;

	m_fScaler = 0.1f;
	m_fSize.x = 512.f * m_fScaler;
	m_fSize.y = 512.f * m_fScaler;
	//m_fNext = 23.f;
	






	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	return S_OK;
}

int CUI_JustDodgeEffect::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return OBJ_NOEVENT;
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;




	m_fStart_timer += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));




//	if (m_bfadein)
//	{

		m_fScaler += 0.4f;
//		if (m_fScaler <= 1.f)
//		{
//			m_fScaler = 1.f;
//			m_bfadein = false;
//		}

//	}
	if (m_fStart_timer > 10.f)
		return OBJ_DEAD;
		//m_bfadeout = true;

	/*if (m_bfadeout)
	{
		m_fAlpha -= 0.02f;

		

	}*/


	m_fPosition = pPointer->Get_ProjPosition();
	m_fSize.x = 512.f * m_fScaler;
	m_fSize.y = 512.f * m_fScaler;
	
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	

	return OBJ_NOEVENT;
}


void CUI_JustDodgeEffect::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return;
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;
	

	
	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);


}

HRESULT CUI_JustDodgeEffect::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;


	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

//if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
//			return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
		m_pShaderCom->Begin(UI_JUSTDODGE);

		m_pVIBufferCom->Render();





	return S_OK;
}

//void CUI_JustDodgeEffect::ReUse_Setting(void * pArg)
//{
//	if (pArg != nullptr)
//		memcpy(&m_damagedesc, pArg, sizeof(DMGDESC));
//
//	//m_fTargetPos = m_damagedesc.fposition;
//	m_fTargetPos = m_damagedesc.pPointer->Get_ProjPosition();
//	m_iCurrentDamage = m_damagedesc.iDamage;
//
//	m_fRandomOffset = { _float(rand() % 80)  , _float(rand() % 55) };
//
//	_uint random = rand() % 2;
//
//	if (random == 0)
//		m_bplusminus = true;
//	else
//		m_bplusminus = false;
//
//	m_fbrightpos_hp[0] = 0.f;
//	m_fbrightpos_hp[1] = -0.5f;
//	m_fbrightpos_hp[2] = -1.f;
//	m_fbrightpos_hp[3] = -1.5f;
//
//	m_eShaderID = UI_BRIGHT;
//	m_fAlpha = 1.f;
//	m_fYFadeout = 0.f;
//	m_fScaler = 2.f;
//
//	m_fSize.x = 30.f * m_fScaler;
//	m_fSize.y = 30.f * m_fScaler;
//
//	m_fNext = 23.f;
//	m_fStart_timer = 0.f;
//	m_bfadeout = false;
//	m_bfadein = true;
//
//	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
//	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f)));
//
//
//	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
//	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
//
//	m_bnorend = true;
//}

HRESULT CUI_JustDodgeEffect::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_JustDodge"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_JustDodgeEffect::SetUp_ShaderResources()
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

CUI_JustDodgeEffect * CUI_JustDodgeEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_JustDodgeEffect*	pInstance = new CUI_JustDodgeEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_JustDodgeEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_JustDodgeEffect::Clone(void * pArg)
{
	CUI_JustDodgeEffect*	pInstance = new CUI_JustDodgeEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_JustDodgeEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_JustDodgeEffect::Free()
{


	__super::Free();
}
