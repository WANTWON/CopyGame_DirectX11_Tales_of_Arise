#include "stdafx.h"
#include "..\Public\HPbar_World.h"

#include "GameInstance.h"

CHPbar_World::CHPbar_World(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CHPbar_World::CHPbar_World(const CHPbar_World & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CHPbar_World::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHPbar_World::Initialize(void * pArg)
{

	if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;
	//memcpy(&m_ScreenEffectdesc, pArg, sizeof(SCREENFADE_DESC));
	//m_fSize.x = 200.0f;
	//m_fSize.y = 22.0f;
	//m_fPosition.x = 1130.f;
	//m_fPosition.y = 360.f + (m_iIndex * 60.f);

	
	
	//if (FAILED(__super::Initialize(pArg)))
	//	return E_FAIL;
		if (FAILED(Ready_Components(pArg)))
			return E_FAIL;

		m_pTransformCom->Change_Speed(10.f);
		m_eShaderID = UI_HPBAR;
	Set_Scale({ 3.f, 1.f, 1.f });
	
	/*_float3 pos = { 0.f,0.f,0.f };
	_vector vpos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	XMStoreFloat3(&pos, vpos);
	pos.y -= 0.6f;
	vpos = XMLoadFloat3(&pos);
	vpos = XMVectorSetW(vpos, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vpos);*/

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 3.f);
	return S_OK;
}

int CHPbar_World::Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Up(DIK_K))
	{
		--m_fcurrenthp;
		m_pTransformCom->Go_Right(0.0167);
	}
		

	if (CGameInstance::Get_Instance()->Key_Up(DIK_J))
		++m_fcurrenthp;

	CGameObject* pGameObject = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CTransform*	pPlayerTransform = (CTransform*)CGameInstance::Get_Instance()->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION));

	SetUp_BillBoard();

	_float3 pos = { 0.f,0.f,0.f };
	_vector vpos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	XMStoreFloat3(&pos, vpos);
	pos.y -= 0.7f;
	vpos = XMLoadFloat3(&pos);
	vpos = XMVectorSetW(vpos, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vpos);

	Set_Scale({ 4.f, 0.3f, 1.f });

	return OBJ_NOEVENT;
}

void CHPbar_World::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CHPbar_World::Render()
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


	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
	return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();*/

	return S_OK;


}

HRESULT CHPbar_World::Ready_Components(void * pArg)
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

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHPbar_World::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &CGameInstance::Get_Instance()->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &CGameInstance::Get_Instance()->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
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

CHPbar_World * CHPbar_World::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHPbar_World*	pInstance = new CHPbar_World(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHPbar_World"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CHPbar_World::Clone(void * pArg)
{
	CHPbar_World*	pInstance = new CHPbar_World(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHPbar_World"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHPbar_World::Free()
{

	//Safe_Release(m_pTextureCom1);
	__super::Free();
	Safe_Release(m_pTextureCom1);
	//Safe_Release(m_pTextureCom2);
}
