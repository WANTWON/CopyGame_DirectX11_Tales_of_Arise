#include "stdafx.h"
#include "..\Public\UI_LOCKON.h"

#include "GameInstance.h"
#include "Player.h"
#include "BattleManager.h"
#include "Monster.h"
CUI_LOCKON::CUI_LOCKON(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_LOCKON::CUI_LOCKON(const CUI_LOCKON & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_LOCKON::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_LOCKON::Initialize(void * pArg)
{


	//Set_Scale({ 0.35f, 0.35f, 1.f });
	m_fSize.x = 32.f;
	m_fSize.y = 32.f;
	m_fPosition.x = 600.f;
	m_fPosition.y = 600.f;
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

	CUI_Manager::Get_Instance()->Set_LockOn(this);

	return S_OK;
}

int CUI_LOCKON::Tick(_float fTimeDelta)
{
	
	if (m_bDead || CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
	{
		CUI_Manager::Get_Instance()->Set_LockOn(nullptr);
		return OBJ_DEAD;
	}
		


	/*CGameObject* pGameObject = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CTransform*	pPlayerTransform = (CTransform*)CGameInstance::Get_Instance()->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	Compute_CamDistance(pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION));
	m_fPosition.x = dynamic_cast<CPlayer*>(pGameObject)->Get_ProjPosition().x - 85.f;
	m_fPosition.y = dynamic_cast<CPlayer*>(pGameObject)->Get_ProjPosition().y + (m_fCamDistance / 5.f);*/


	//if (m_fCamDistance > 20.f)
	//{
	//	m_fNext = 13.f / m_fCamDistance * 20;
	//	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 16.f / m_fCamDistance * 18.f);
	//	m_pTransformCom->Set_Scale(CTransform::STATE_UP, 16.f / m_fCamDistance * 18.f);
	//	m_fPosition.x += m_fCamDistance *2.f - 35.f;//(m_fNext*3);
	//}
	//else
	//{
	//	m_fNext = 13.f;
	//	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 16.f);
	//	m_pTransformCom->Set_Scale(CTransform::STATE_UP, 16.f);
	//}

//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));




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
	if (CGameInstance::Get_Instance()->Key_Up(DIK_5))
	{
		m_fcurrentmp += 0.1f;
	}
	if (CGameInstance::Get_Instance()->Key_Up(DIK_6))
	{
		m_fcurrentmp -= 0.1f;
	}
	//dynamic_cast<CPlayer*>(pGameObject)->Get_ProjPosition().x - 85.f;

	m_fcurrentmp = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fLockonSmashGuage;

	//m_pTransformCom->Turn({ 0.f,1.f,0.f,1.f }, 45.f);

	_float2 lockonPos = CBattleManager::Get_Instance()->Get_LackonMonster()->Get_ProjPosition();

	m_fPosition.x = lockonPos.x;//550.f;
	m_fPosition.y = lockonPos.y; //600.f;
	m_fSize.x = 48.f;
	m_fSize.y = 48.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	return OBJ_NOEVENT;
}

void CUI_LOCKON::Late_Tick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
		return;

	m_fcurrent_render_slot_mp = m_fcurrentmp - (_uint)m_fcurrentmp;

	__super::Late_Tick(fTimeDelta);



}

HRESULT CUI_LOCKON::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;
	//0번


	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

//m_pShaderCo	m->Begin(0);
//
//	m_pVIBufferCom->Render();
	
	//m_fPosition.x -= 9.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(2))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();
	m_fSize.x = 80.f;
	m_fSize.y = 80.f;
//	m_fPosition.x -= 3.f;
	
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_Rotation({ 0.f,0.f,0.f });
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fSize.x = 36.f;
	m_fSize.y = 4.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(3))))
		return E_FAIL;

	m_fPosition.x += 13.f;
	m_fPosition.y -= 13.f;
	m_pTransformCom->Set_Rotation({ 0.f,0.f,-45.f });
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	return E_FAIL;

	m_eShaderID = UI_MP_GUAGE;
	if (m_fcurrentmp >= 1.f)
	m_eShaderID = UI_POTRAIT_ALLBLUE;
	

	m_pShaderCom->Begin(UI_COLOR_BLACK);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();
	/////////////////////////첫번째마나
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(5))))
		return E_FAIL;
	m_fPosition.y += 25.f;
	m_pTransformCom->Set_Rotation({ 0.f,0.f,45.f });
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	if (m_fcurrentmp<1.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 2.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_LOCKON_REVERSE;


	m_pShaderCom->Begin(UI_COLOR_BLACK);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	//////////////////////////두번째마나
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(3))))
		return E_FAIL;
	m_fPosition.x -= 25.f;
	m_pTransformCom->Set_Rotation({ 0.f,0.f,-225.f });
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp<2.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 3.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(UI_COLOR_BLACK);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	
	//////////////////////세번째마나
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(3))))
		return E_FAIL;


	m_fPosition.y -= 25.f;
	m_pTransformCom->Set_Rotation({ 0.f,0.f,-315.f });
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_fcurrentmp<3.f)
		m_eShaderID = UI_COLOR_BLACK;
	else if (m_fcurrentmp > 4.f)
		m_eShaderID = UI_POTRAIT_ALLBLUE;
	else
		m_eShaderID = UI_MP_GUAGE;
	m_pShaderCom->Begin(UI_COLOR_BLACK);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	
	////////////////////네번째마나

	




	return S_OK;
}

HRESULT CUI_LOCKON::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Lockon"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	//m_pVIBufferCom1
	return S_OK;
}

HRESULT CUI_LOCKON::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;





	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrent_render_slot_mp, sizeof(_float))))
		return E_FAIL;

	

	return S_OK;
}

CUI_LOCKON * CUI_LOCKON::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_LOCKON*	pInstance = new CUI_LOCKON(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_LOCKON"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_LOCKON::Clone(void * pArg)
{
	CUI_LOCKON*	pInstance = new CUI_LOCKON(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_LOCKON"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_LOCKON::Free()
{
	//Safe_Release(m_pVIBufferCom1);
	__super::Free();
}
