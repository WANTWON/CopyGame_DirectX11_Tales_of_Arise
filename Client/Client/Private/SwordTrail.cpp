#include "stdafx.h"
#include "..\Public\SwordTrail.h"
#include "Weapon.h"
#include "Player.h"
#include "GameInstance.h"

CSwordTrail::CSwordTrail(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CSwordTrail::CSwordTrail(const CSwordTrail & rhs)
	: CEffect(rhs)
{
}

HRESULT CSwordTrail::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSwordTrail::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_eShaderID = SHADER_TRAIL;
	return S_OK;
}

int CSwordTrail::Tick(_float fTimeDelta)
{
	CVIBuffer_SwordTrail::TRAILPOINT TrailPointDesc;

	TrailPointDesc.vPointUp = m_vPoint_Up;
	TrailPointDesc.vPointDown = m_vPointDown;

	TrailPointDesc.vTrailMatrix = m_WeaponMatrix;

	m_pVIBufferCom->Update(TrailPointDesc);

	return OBJ_NOEVENT;
}

void CSwordTrail::Late_Tick(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	if (pPlayer->Get_AnimState() == CPlayer::SLASH_HOLD_ED )
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

		if (pPlayer->Get_RightHandItem() == CPlayer::MESH_SWORD)
		{
			m_vColorFront = { 102, 255, 255, 255 };
			m_vColorBack = { 12, 76, 255, 255 };
			m_EffectDesc.iTextureNum = 2;

		}	
		else if (pPlayer->Get_RightHandItem() == CPlayer::MESH_ROD)
		{
			m_vColorFront = { 143, 105, 255, 255 };
			m_vColorBack = { 143, 105, 255, 255 };
			m_EffectDesc.iTextureNum = 1;

		}

	}
	else if (pPlayer->Get_AnimState() == CPlayer::SLASH)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);

		if (pPlayer->Get_RightHandItem() == CPlayer::MESH_SWORD)
		{
			m_vColorFront = { 255, 244, 132, 255 };
			m_vColorBack = { 248, 100, 0, 255 };
			m_EffectDesc.iTextureNum = 2;

		}
		else if (pPlayer->Get_RightHandItem() == CPlayer::MESH_ROD)
		{
			m_vColorFront = { 143, 105, 255, 255 };
			m_vColorBack = { 143, 105, 255, 255 };
			m_EffectDesc.iTextureNum = 1;

		}
	}

}

HRESULT CSwordTrail::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	return S_OK;
		
}

HRESULT CSwordTrail::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SwordTrail"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_SwordTrail"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}


HRESULT CSwordTrail::SetUp_ShaderID()
{
	return S_OK;
}

HRESULT CSwordTrail::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_EffectDesc.iTextureNum))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_ColorBack", &m_vColorBack, sizeof(_vector))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ColorFront", &m_vColorFront, sizeof(_vector))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CSwordTrail::Update_Trail_Point(_float3 vPointDown, _float3 vPointUp, _fmatrix vWeaponMatrix)
{
	m_vPointDown = vPointDown;
	m_vPoint_Up = vPointUp;

	XMStoreFloat4x4(&m_WeaponMatrix, vWeaponMatrix);

}


CSwordTrail * CSwordTrail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSwordTrail*	pInstance = new CSwordTrail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CSwordTrail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSwordTrail::Clone(void * pArg)
{
	CSwordTrail*	pInstance = new CSwordTrail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CSwordTrail"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSwordTrail::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
