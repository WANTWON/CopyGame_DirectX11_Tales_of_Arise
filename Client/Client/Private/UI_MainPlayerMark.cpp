#include "stdafx.h"
#include "..\Public\UI_MainPlayerMark.h"
#include "GameInstance.h"
#include "Player.h"
#include "BattleManager.h"
CUI_MainPlayerMark::CUI_MainPlayerMark(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_MainPlayerMark::CUI_MainPlayerMark(const CUI_MainPlayerMark & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_MainPlayerMark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_MainPlayerMark::Initialize(void * pArg)
{

	if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;
	//memcpy(&m_ScreenEffectdesc, pArg, sizeof(SCREENFADE_DESC));
	/*if (m_iIndex == CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1)
		m_bforMainPlayer = true;*/

	m_fSize.x = 200.0f;
	m_fSize.y = 22.0f;
	m_fPosition.x = 1130.f;
	m_fPosition.y = 310.f + (m_iIndex * 60.f);



	m_eShaderID = UI_HPBAR;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_MainPlayerMark::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return OBJ_NOEVENT;
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return OBJ_NOEVENT;

	
		/*m_fcurrenthp = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Info().fCurrentHp;
		m_fmaxhp = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Info().fMaxHp;*/
		/*CGameObject* pGameObject = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CTransform*	pPlayerTransform = (CTransform*)CGameInstance::Get_Instance()->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));*/

	m_fWorldsizeX = 0.8f;
	m_fWorldsizeY = 1.2f;
	CGameObject* pGameObject = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CTransform*	pPlayerTransform = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Transform();//(CTransform*)CGameInstance::Get_Instance()->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION));
	//dynamic_cast<CPlayer*>(pGameObject)->Get_ProjPosition();
	Compute_CamDistance(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	SetUp_BillBoard();

//	_float3 pos = { 0.f,0.f,0.f };
	_vector vpos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	//XMStoreFloat3(&pos, vpos);
	vpos.m128_f32[1] += 4.5f;
	//pos.y += 0.7f;
//	vpos = XMLoadFloat3(&pos);
	//vpos = XMVectorSetW(vpos, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vpos);

	if (m_fCamDistance < 14.6579256f)
		Set_Scale({ m_fWorldsizeX*m_fCamDistance / 13.f, m_fWorldsizeY*m_fCamDistance / 13.f, 1.f });
	else
		Set_Scale({ m_fWorldsizeX, m_fWorldsizeY, 1.f });



		
	/*	m_fPosition.x = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_ProjPosition().x;
		m_fPosition.y = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_ProjPosition().y - 150.f - (m_fCamDistance / 5.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));



		if (m_fCamDistance > 20.f)
		{
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 25.f / m_fCamDistance * 20.f);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, 39.f/ m_fCamDistance * 20.f);
		}
		else
		{
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 25.f);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y = 39.f);
		}*/

	


	/*if (CGameInstance::Get_Instance()->Key_Pressing(DIK_K))
	--m_fcurrenthp;

	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_J))
	++m_fcurrenthp;*/

	return OBJ_NOEVENT;
}

void CUI_MainPlayerMark::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return ;
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return;
	__super::Late_Tick(fTimeDelta);

}

HRESULT CUI_MainPlayerMark::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
	return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();*/

	return S_OK;


}

HRESULT CUI_MainPlayerMark::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minigame"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	///* For.Com_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
	//	return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MainPlayerMark::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;


	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;



	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &CGameInstance::Get_Instance()->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CGameInstance::Get_Instance()->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(6))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;*/


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrenthp, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fmaxhp, sizeof(_float))))
		return E_FAIL;*/



	return S_OK;
}

CUI_MainPlayerMark * CUI_MainPlayerMark::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_MainPlayerMark*	pInstance = new CUI_MainPlayerMark(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_MainPlayerMark"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_MainPlayerMark::Clone(void * pArg)
{
	CUI_MainPlayerMark*	pInstance = new CUI_MainPlayerMark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_MainPlayerMark"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MainPlayerMark::Free()
{

	//Safe_Release(m_pTextureCom1);
	__super::Free();
//	Safe_Release(m_pTextureCom1);
	//Safe_Release(m_pTextureCom2);
}
