#include "stdafx.h"
#include "..\Public\UI_Monster_HPbar.h"

#include "GameInstance.h"

#include "Monster.h"

CUI_Monster_HPbar::CUI_Monster_HPbar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Monster_HPbar::CUI_Monster_HPbar(const CUI_Monster_HPbar & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Monster_HPbar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Monster_HPbar::Initialize(void * pArg)
{

	/*if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;*/
	//memcpy(&m_ScreenEffectdesc, pArg, sizeof(SCREENFADE_DESC));
	/*if (m_iIndex == CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1)
		m_bforMainPlayer = true;*/

	m_fSize.x = 200.0f;
	m_fSize.y = 22.0f;
	m_fPosition.x = 100.f;
	m_fPosition.y = 100.f;
	m_fNext = 11.f;


	m_eShaderID = UI_HPBAR;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Monster_HPbar::Tick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
		return OBJ_NOEVENT;
	m_iMonstername = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_MonsterID();

	for (_uint i = 0; i < 6; ++i)
		m_fbrightpos_hp[i] += 0.015f;

	for (_uint i = 0; i < 6; ++i)
	{
		if (m_fbrightpos_hp[i] >= 3.f)
			m_fbrightpos_hp[i] = 0.f;
	}

		
	m_fcurrenthp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fCurrentHp;
	m_fmaxhp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fMaxHp;

	
		
		


		
		


	


		

	m_fSize.x = 200.0f;
	m_fSize.y = 22.0f;
	m_fPosition.x = 100.f;
	m_fPosition.y = 30.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));



	

	return OBJ_NOEVENT;
}

void CUI_Monster_HPbar::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CUI_Monster_HPbar::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	
	
	
	

	

	
	if (dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
	{
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();

		
		m_fSize.x = 200.0f;
		m_fSize.y = 22.0f;
		m_fPosition.x = 100.f;
		m_fPosition.y = 50.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
			return E_FAIL;
		m_eShaderID = UI_HPBAR;
		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render();


		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();

		/////////////////////////////////////////////
		m_eShaderID = UI_BRIGHT;
		m_fSize.x = 16.0f;
		m_fSize.y = 16.0f;
		m_fPosition.x = 54.f;
		m_fPosition.y = 65.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

		RenderCurrenthpfont();


		m_fPosition.x += m_fNext;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(10))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();

		//return S_OK;

		RenderMaxhpfont();


		/////////////////////////////////////////////
	}

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;
	m_eShaderID = UI_HPBAR;
	if (CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr);
	return S_OK;
	Compute_CamDistance(dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_TransformState(CTransform::STATE_TRANSLATION));
	m_fPosition.x = CBattleManager::Get_Instance()->Get_LackonMonster()->Get_ProjPosition().x;
	m_fPosition.y = CBattleManager::Get_Instance()->Get_LackonMonster()->Get_ProjPosition().y + 20.f - (m_fCamDistance / 5.f);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));



	if (m_fCamDistance > 20.f)
	{
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 200.f / m_fCamDistance * 20.f);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, 22.f / m_fCamDistance * 20.f);
	}
	else
	{
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 200.0f);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y = 22.0f);
	}

	if(FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();
	
	return S_OK;
	
}

HRESULT CUI_Monster_HPbar::RenderCurrenthpfont()
{
	if (m_fcurrenthp >= 100000.f)
	{
		m_itexnum = (_uint)(m_fcurrenthp / 100000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 10000.f)
	{
		m_itexnum = (((_uint)m_fcurrenthp % 100000) / 10000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 1000.f)
	{
		m_itexnum = (((_uint)m_fcurrenthp % 10000) / 1000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 100.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fcurrenthp % 1000) / 100);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 10.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[4], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fcurrenthp % 100) / 10);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fcurrenthp >= 0)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[5], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (_uint)m_fcurrenthp % 10;

		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Monster_HPbar::RenderMaxhpfont()
{
	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 100000.f)
	{
		m_itexnum = (_uint)(m_fmaxhp / 100000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 10000.f)
	{
		m_itexnum = (((_uint)m_fmaxhp % 100000) / 10000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 1000.f)
	{
		m_itexnum = (((_uint)m_fmaxhp % 10000) / 1000);
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
			return E_FAIL;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 100.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fmaxhp % 1000) / 100);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}
	m_fPosition.x += m_fNext;
	if (m_fmaxhp >= 10.f)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[4], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (((_uint)m_fmaxhp % 100) / 10);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);
		m_pVIBufferCom->Render();
	}


	if (m_fmaxhp >= 0)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[5], sizeof(_float))))
			return E_FAIL;
		m_itexnum = (_uint)m_fmaxhp % 10;

		m_fPosition.x += m_fNext;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Monster_HPbar::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPbar"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monstername"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_HPbar::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(m_iMonstername))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrenthp, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fmaxhp, sizeof(_float))))
		return E_FAIL;



	return S_OK;
}

CUI_Monster_HPbar * CUI_Monster_HPbar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Monster_HPbar*	pInstance = new CUI_Monster_HPbar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Monster_HPbar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_Monster_HPbar::Clone(void * pArg)
{
	CUI_Monster_HPbar*	pInstance = new CUI_Monster_HPbar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Monster_HPbar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_HPbar::Free()
{

	//Safe_Release(m_pTextureCom1);
	__super::Free();
	Safe_Release(m_pTextureCom1);
    Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
}
