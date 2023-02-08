#include "stdafx.h"
#include "..\Public\UI_MPguage.h"

#include "GameInstance.h"
#include "Player.h"


CMP_Guage::CMP_Guage(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CMP_Guage::CMP_Guage(const CMP_Guage & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CMP_Guage::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMP_Guage::Initialize(void * pArg)
{
	
	
	//Set_Scale({ 0.35f, 0.35f, 1.f });
	m_fSize.x = 16.f;
	m_fSize.y = 16.f;
	//m_fPosition.x = 600.f;
	//m_fPosition.y = 600.f;
	///*m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f;
	//m_fPosition.y = g_iWinSizeY - m_fSize.y * 0.5f;


	/*if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;*/
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(1.f, 0.f, 0.f, 1.f));

	//m_vRight = _float4(0.5f, 0.f, 0.f, 0.f);
	//m_vUp = _float4(0.f, 0.5f, 0.f, 0.f);
	/*pVertices->vLook = _float4(0.f, 0.f, 0.5f, 0.f);*/

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CMP_Guage::Tick(_float fTimeDelta)
{
	
		

		



	m_fmaxmp = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Info().fMaxMp;
	m_fcurrentmp = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Info().fCurrentMp;
	

	/*CGameObject* pGameObject = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CTransform*	pPlayerTransform = (CTransform*)CGameInstance::Get_Instance()->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));*/
	Compute_CamDistance(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fPosition.x = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_ProjPosition().x - 85.f;
	m_fPosition.y = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_ProjPosition().y + (m_fCamDistance / 5.f);
	

	if (m_fCamDistance > 20.f)
	{
		m_fNext = 13.f / m_fCamDistance * 20;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 16.f / m_fCamDistance * 18.f);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, 16.f / m_fCamDistance * 18.f);
		m_fPosition.x += m_fCamDistance *2.f - 35.f ;//(m_fNext*3);
	}
	else
	{
		m_fNext = 13.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 16.f);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, 16.f);
	}

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));




	//Set_Scale({ 0.2f,0.2f,1.f });
	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_V))
	{*/
//		m_pTransformCom->Change_Speed(1.f);
//	
//	m_pTransformCom->Go_Left(1.5f);
////	}
	

	

	

	//_float3 pos = { 0.f,0.f,0.f };
	////_vector vpos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	//XMStoreFloat3(&pos, vplayerpos);
	//pos.x -= 1.6f;
	//vplayerpos = XMLoadFloat3(&pos);
	//vplayerpos = XMVectorSetW(vplayerpos, 1.f);
	//SetUp_BillBoard();
	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_5))
	{
		m_fcurrentmp += 0.1f;
	}
	if (CGameInstance::Get_Instance()->Key_Up(DIK_6))
	{
		m_fcurrentmp -= 0.1f;
	}*/


	

	
	/*m_fPosition.x = 600.f;
	m_fPosition.y = 600.f;
	m_fSize.x = 1000.f;
	m_fSize.y = 1000.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));*/

	return OBJ_NOEVENT;
}

void CMP_Guage::Late_Tick(_float fTimeDelta)
{

	m_fcurrent_render_slot_mp = m_fcurrentmp - (_uint)m_fcurrentmp;

	__super::Late_Tick(fTimeDelta);



}

HRESULT CMP_Guage::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;
	//0번

	
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	
	
	m_eShaderID = UI_MP_GUAGE;
	if(m_fcurrentmp > 1.f)
	m_eShaderID = UI_POTRAIT_ALLBLUE;

	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();
	/////////////////////////첫번째마나

	m_fPosition.x += m_fNext;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	if (m_fcurrentmp<1.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 2.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	
	
	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	////////////////////////두번째마나
	m_fPosition.x += m_fNext;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp<2.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 3.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	if (m_fmaxmp == 3)
		return S_OK;
	//////////////////////세번째마나
	

	m_fPosition.x += m_fNext;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp<3.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 4.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	if (m_fmaxmp == 4)
		return S_OK;
	//////////////////////네번째마나

	m_fPosition.x += m_fNext;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp<4.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 5.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	if (m_fmaxmp == 5)
		return S_OK;
	//////////////////////오번째마나

	m_fPosition.x += m_fNext;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp<5.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 6.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	if (m_fmaxmp == 6)
		return S_OK;
	//////////////////////욱번째마나
	m_fPosition.x += m_fNext;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp<6.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 7.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	if (m_fmaxmp == 7)
		return S_OK;
	//////////////////////칠번째마나

	m_fPosition.x += m_fNext;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp<7.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 8.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	if (m_fmaxmp == 8)
		return S_OK;
	//////////////////////팔번째마나

	m_fPosition.x += m_fNext;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp<8.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 9.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	if (m_fmaxmp == 9)
		return S_OK;
	//////////////////////구번째마나

	m_fPosition.x += m_fNext;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp< 9.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 10.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(12);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	//////////////////////10번째마나

	
	

	return S_OK;
}

HRESULT CMP_Guage::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MPguage"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	//m_pVIBufferCom1
	return S_OK;
}

HRESULT CMP_Guage::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;*/



	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrent_render_slot_mp, sizeof(_float))))
		return E_FAIL;
	 
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxMp", &m_fmaxmp, sizeof(_float))))
		return E_FAIL;*/

	

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fUVx", &m_fmaxmp, sizeof(_float))))
		return E_FAIL;*/

	

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_vCamPosition", &CGameInstance::Get_Instance()->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;*/

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;*/


	return S_OK;
}

CMP_Guage * CMP_Guage::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMP_Guage*	pInstance = new CMP_Guage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMP_Guage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CMP_Guage::Clone(void * pArg)
{
	CMP_Guage*	pInstance = new CMP_Guage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMP_Guage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMP_Guage::Free()
{
	//Safe_Release(m_pVIBufferCom1);
	__super::Free();
}
