#include "stdafx.h"
#include "..\Public\MonsterEffect.h"
#include "Weapon.h"
#include "Player.h"
#include "GameInstance.h"
#include "FightEffect.h"

CMonsterEffect::CMonsterEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CMonsterEffect::CMonsterEffect(const CMonsterEffect & rhs)
	: CEffect(rhs)
{
}

HRESULT CMonsterEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterEffect::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Set_Scale(m_EffectDesc.vInitScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.vInitPositon);
	m_pTransformCom->LookDir(m_EffectDesc.vLook);
	m_vScale = m_EffectDesc.vInitScale;
	m_vColorBack = m_EffectDesc.vColor;

	switch (m_EffectDesc.eEffectID)
	{
	case BULLET_SMOKE:
		m_eShaderID = SHADER_ONECOLOR_ALPHASET;
		m_vColorFront = m_EffectDesc.vColor;
		break;
	case BLAST_RING:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_vColorFront = XMVectorSet(240, 233, 223, 255);
		m_vColorBack = XMVectorSet(165, 124, 94, 255);
		break;
	case OCTOROCK_STONE:
		m_eShaderID = SHADERM_ONLY_TEXTURE;
		m_vDirection = _float3((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)*0.f, (rand() % 20 - 10)* 0.1f);
		break;
	case CLAW_SMOKE:
	case ROLLING_SMOKE:
	case ROLA_SMOKE:
		m_eShaderID = SHADER_ONECOLOR_ALPHASET;
		m_vColorFront = m_EffectDesc.vColor;
		m_fAlpha = 0.f;
		break;
	case CLAW_RING:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_vColorFront = m_EffectDesc.vColor;
		m_vColorBack = XMVectorSet(255, 255, 0, 255);
		m_fTexUV = 1.f;
		break;
	case FLAPPING_SMOKE:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_vColorFront = XMVectorSet(255, 255, 255, 255);
		m_vColorBack = XMVectorSet(138, 136, 135, 255);
		break;
	case FEATHER:
		m_eShaderID = SHADERM_ONLY_TEXTURE;
		m_fSpeed = (rand() % 500)*0.01f + 2;
		XMStoreFloat3(&m_vDirection, m_EffectDesc.vLook);
		m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(float(rand() % 180)));
		break;
	case ALBATROSSDEAD_SMOKE:
		m_fAlpha = 0.f;
		m_fSpeed = (rand() % 100)*0.01f + 0.3f;
		m_eShaderID = SHADER_SMOKEFRONT;
		m_vColorFront = rand()% 2 == 0 ? XMVectorSet(255, 63, 255, 255) : XMVectorSet(50, 11, 44, 255);
		m_vColorBack = XMVectorGetX(m_vColorFront) == 255 ?  XMVectorSet(50, 11, 44, 255) : XMVectorSet(255, 63, 255, 255);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);
		break;
	case GLOW_SPHERE:
		m_fSpeed = (rand() % 100)*0.01f + 0.3f;
		m_fAngle = XMConvertToRadians(float(rand() % 180));
		m_eShaderID = SHADER_TWOCOLOR_PRIORITY;
		m_vColorFront = m_EffectDesc.vColor;
		m_vColorBack = XMVectorSet(144, 2, 225, 255);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);
		break;
	case ROLLING_FLASH:
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vecColor.push_back(XMVectorSet(0, 255, 255, 255.f));
		m_vecColor.push_back(XMVectorSet(125, 19, 0, 255.f));
		m_vecColor.push_back(XMVectorSet(255, 0, 0, 255.f));
		m_vColorFront = XMVectorSet(255, 255, 194, 255.f);
		m_vColorBack = m_vecColor.front();
		break;
	case ROLLING_FLASH2:
		m_fSpeed = rand()%100*0.01f + 0.3f;
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vecColor.push_back(XMVectorSet(255, 0, 0, 255.f));
		m_vecColor.push_back(XMVectorSet(125, 19, 0, 255.f));
		m_vecColor.push_back(XMVectorSet(255, 0, 0, 255.f));
		m_vColorFront = XMVectorSet(255, 255, 194, 255.f);
		m_vColorBack = m_vecColor.front();
		break;
	case ROLA_PUSHFLASH:
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vColorFront = XMVectorSet(255, 255, 0, 255.f);
		m_vColorBack = XMVectorSet(236, 91, 28, 255.f);
		break;
	case ROLA_PUSHSPARK:
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vecColor.push_back(XMVectorSet(255, 60, 0, 255.f));
		m_vecColor.push_back(XMVectorSet(255, 60, 0, 255.f));
		m_vecColor.push_back(XMVectorSet(255, 202, 105, 255.f));
		m_vColorFront = XMVectorSet(255, 255, 0, 255.f);
		m_vColorBack = m_vecColor.front();
		break;
	case ROLA_PUSHRING:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_vecColor.push_back(XMVectorSet(255, 60, 0, 255.f));
		m_vecColor.push_back(XMVectorSet(255, 60, 0, 255.f));
		m_vecColor.push_back(XMVectorSet(255, 202, 105, 255.f));
		m_vColorFront = XMVectorSet(255, 255, 0, 255.f);
		m_vColorBack = m_vecColor.front();
		break;
	default:
		break;
	}


	return S_OK;
}

int CMonsterEffect::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	__super::Tick(fTimeDelta);

	m_fDeadtime += fTimeDelta;
	switch (m_EffectDesc.eEffectID)
	{
	case BULLET_SMOKE:
	case BLAST_RING:
	case OCTOROCK_STONE:
		Tick_OctoRockEffect(fTimeDelta);
		break;
	case CLAW_SMOKE:
	case CLAW_RING:
	case FLAPPING_SMOKE:
	case FEATHER:
	case ALBATROSSDEAD_SMOKE:
		Tick_AlbatossEffect(fTimeDelta);
		break;
	case GLOW_SPHERE:
		Tick_GlowSphere(fTimeDelta);
		break;
	case ROLLING_SMOKE:
	case ROLA_SMOKE:
	case ROLLING_FLASH:
	case ROLLING_FLASH2:
	case ROLA_PUSHSPARK:
	case ROLA_PUSHRING:
	case ROLA_PUSHFLASH:
		Tick_RolaEffect(fTimeDelta) ;
		break;
	default:
		break;
	}

	return OBJ_NOEVENT;
}

void CMonsterEffect::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CMonsterEffect::Render()
{

	__super::Render();


	return S_OK;
}

HRESULT CMonsterEffect::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	__super::Ready_Components();

	switch (m_EffectDesc.eEffectID)
	{
	case BULLET_SMOKE:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeDeath"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case BLAST_RING:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Octorock_Ring"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case OCTOROCK_STONE:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Octorock_Stone"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case CLAW_SMOKE:
	case ROLLING_SMOKE:
	case ROLA_SMOKE:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Smoke"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case FLAPPING_SMOKE:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_SmokeSphere"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case CLAW_RING:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardRing"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case FEATHER:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TOWER, TEXT("Albatoss_Feather"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case ALBATROSSDEAD_SMOKE:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeDeath"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

		if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
			return E_FAIL;

		if (FAILED(__super::Add_Components(TEXT("Com_SmokeDstTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeDst"), (CComponent**)&m_pSmokeDstTexture)))
			return E_FAIL;
		break;
	case GLOW_SPHERE:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Glow"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case ROLLING_FLASH:
	case ROLLING_FLASH2:
	case ROLA_PUSHFLASH:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Flash"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case ROLA_PUSHSPARK:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Blast"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case ROLA_PUSHRING:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardRing"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	default:
		break;
	}
	

	return S_OK;
}




HRESULT CMonsterEffect::SetUp_ShaderID()
{
	return S_OK;
}

HRESULT CMonsterEffect::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();


	switch (m_EffectDesc.eEffectID)
	{
	case ALBATROSSDEAD_SMOKE:
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_SmokeDstTexture", m_pSmokeDstTexture->Get_SRV())))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DissolveTexture", m_pDissolveTexture->Get_SRV())))
			return E_FAIL;
		break;
	default:
		break;
	}

	return S_OK;
}


void CMonsterEffect::Change_Animation(_float fTimeDelta)
{

}

void CMonsterEffect::Change_Texture(_float fTimeDelta)
{
	switch (m_EffectDesc.eEffectID)
	{
	case CLAW_SMOKE:
	case ROLLING_SMOKE:
		m_EffectDesc.iTextureNum++;

		if (m_EffectDesc.iTextureNum >= m_pTextureCom->Get_TextureSize())
		{
			m_EffectDesc.iTextureNum  = 0;
		}
		break;
	}
}


void CMonsterEffect::Tick_Grass(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;


	if (m_fAlpha <= 0)
		m_bDead = true;

}

void CMonsterEffect::Tick_OctoRockEffect(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;

	switch (m_EffectDesc.eEffectID)
	{
	case BULLET_SMOKE:
		SetUp_BillBoard();

		m_fAlpha -= 0.03f;
		m_vScale.x += 0.01f;
		m_vScale.y += 0.01f;
		m_vScale.z += 0.01f;
		break;
	case BLAST_RING:
		m_fAlpha -= 0.02f;
		m_vScale.x += 0.1f;
		m_vScale.y += 0.1f;
		m_vScale.z += 0.1f;
		break;
	case OCTOROCK_STONE:
		SetUp_BillBoard();
		m_pTransformCom->Go_PosDir(fTimeDelta, XMLoadFloat3(&m_vDirection));
	
		if (m_fDeadtime > m_EffectDesc.fDeadTime)
		{
			m_fAlpha -= 0.1f;
			m_vScale.x -= 0.1f;
			m_vScale.y -= 0.1f;
			m_vScale.z -= 0.1f;
		}
		break;
	default:
		break;
	}

	Set_Scale(m_vScale);

	if (m_vScale.x <= 0 || m_fAlpha <= 0)
		m_bDead = true;
}

void CMonsterEffect::Tick_AlbatossEffect(_float fTimeDelta)
{

	

	switch (m_EffectDesc.eEffectID)
	{
	case CLAW_SMOKE:
		SetUp_BillBoard();

		if (!m_bMax)
		{
			m_fAlpha += 0.05f;

			if (m_fAlpha >= 1.f)
			{
				m_fAlpha = 1.f;
				m_bMax = true;
			}
				
		}
		else
			m_fAlpha -= 0.01f;

		m_vScale.x += 0.1f;
		m_vScale.y += 0.05f;
		m_pTransformCom->Go_PosDir(fTimeDelta, m_EffectDesc.vLook);
		break;
	case FLAPPING_SMOKE:
		m_pTransformCom->LookDir(m_EffectDesc.vLook);

		if (!m_bMax)
		{
			m_fAlpha += 0.05f;

			if (m_fAlpha >= 1.f)
			{
				m_fAlpha = 1.f;
				m_bMax = true;
			}
		}
		else
			m_fAlpha -= 0.01f;

		m_fAlpha -= 0.01f;
		m_vScale.x += 0.15f;
		m_vScale.y += 0.15f;
		m_vScale.z += 0.15f;	
		m_pTransformCom->Go_PosDir(fTimeDelta, m_EffectDesc.vLook);
		break;
	case CLAW_RING:
		SetUp_BillBoard();
		m_fTexUV -= 0.05f;
		if (m_fTexUV <= 0.f)
			m_fTexUV = 0.f;

		if (m_fDeadtime < m_EffectDesc.fDeadTime)
		{
			m_vScale.x += 0.05f;
			m_vScale.y += 0.05f;
			m_vScale.z += 0.05f;
			m_fAlpha += 0.05f;

			if (m_fAlpha >= 1.f)
				m_fAlpha = 1.f;
		}
		else
		{
			m_vScale.x += 0.01f;
			m_vScale.y += 0.01f;
			m_vScale.z += 0.01f;
			m_fAlpha -= 0.05f;

		}
		break;
	case FEATHER:
		if (m_EffectDesc.fDeadTime > m_fDeadtime)
			m_pTransformCom->Go_PosDir(fTimeDelta, XMLoadFloat3(&m_vDirection));
		else
		{
			m_pTransformCom->Go_PosDir(fTimeDelta, XMLoadFloat3(&m_vDirection));
			m_fAlpha -= 0.05f;
			m_vScale.x-= 0.05f;
			m_vScale.y-= 0.05f;
			m_vScale.z-= 0.05f;
			m_vDirection.y -= 0.1f;
		}	
		break;
	case ALBATROSSDEAD_SMOKE:
		SetUp_BillBoard();

		m_pTransformCom->Go_PosDir(fTimeDelta*m_fSpeed, XMVectorSet(0.f, 1.f, 0.f, 0.f));

		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;

		if (!m_bMax)
		{
			m_fAlpha += 0.02f;
			if (m_fAlpha >= 1.f)
			{
				m_bMax = true;
			}

		}
		else
		{
			m_fAlpha -= 0.02f;
		}
		break;
	default:
		break;
	}
	

	Set_Scale(m_vScale);

	if (m_vScale.x <= 0 || m_fAlpha <= 0)
		m_bDead = true;

}

void CMonsterEffect::Tick_GlowSphere(_float fTimeDelta)
{
	SetUp_BillBoard();
	m_pTransformCom->Go_PosDir(fTimeDelta*m_fSpeed, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	if (m_fDeadtime > m_EffectDesc.fDeadTime * 0.5f)
	{
		m_vScale.x += 0.01f;
		m_vScale.y += 0.01f;
	}
	else if (m_fDeadtime < m_EffectDesc.fDeadTime)
	{
		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;
	}
	Set_Scale(m_vScale);

	if (m_EffectDesc.fDeadTime < m_fDeadtime)
	{
		m_fAlpha -= 0.15f;
		m_vScale.x -= 0.1f;
		m_vScale.y -= 0.1f;
	}

	if (m_fAlpha <= 0.f)
		m_bDead = true;
}

void CMonsterEffect::Tick_RolaEffect(_float fTimeDelta)
{

	m_fDeadtime += fTimeDelta;
	m_fColorTime += fTimeDelta;

	if (m_vecColor.size() != 0)
	{
		if (m_fColorTime > m_EffectDesc.fDeadTime*0.3f)
		{
			if (m_iColorIndex + 1 >= m_vecColor.size() - 1)
				m_iColorIndex = 0;
			else
				m_iColorIndex++;
			m_fColorTime = 0.f;
		}
		m_vColorBack = XMVectorLerp(m_vecColor[m_iColorIndex], m_vecColor[m_iColorIndex + 1], m_fColorTime / 0.3f);
	}



	switch (m_EffectDesc.eEffectID)
	{
	case ROLLING_SMOKE:
		SetUp_BillBoard();

		if (!m_bMax)
		{
			m_fAlpha += 0.05f;

			if (m_fAlpha >= 1.f)
			{
				m_fAlpha = 1.f;
				m_bMax = true;
			}
		}
		else
			m_fAlpha -= 0.05f;


		m_vScale.x += 0.02f;
		m_vScale.y += 0.02f;
	
		break;
	case ROLA_PUSHSPARK:
	case ROLA_SMOKE:
	{
		SetUp_BillBoard();

		if (!m_bMax)
		{
			m_fAlpha += 0.05f;

			if (m_fAlpha >= 1.f)
			{
				m_fAlpha = 1.f;
				m_bMax = true;
			}
		}
		else
			m_fAlpha -= 0.05f;


		m_vScale.x += 0.02f;
		m_vScale.y += 0.02f;
		break;
	}
	case ROLLING_FLASH:
	{
		SetUp_BillBoard();

		if (m_fDeadtime < m_EffectDesc.fDeadTime)
		{
			m_vScale.x += 0.1f;
			m_vScale.y += 0.03f;
		}
		else
		{
			m_fAlpha -= 0.1f;
			m_vScale.x -= 0.05f;
			m_vScale.y -= 0.03f;
		}

		if (m_fAlpha <= 0)
		{
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CMonsterEffect::ROLLING_FLASH2;
			EffectDesc.fDeadTime = 0.5f;
			EffectDesc.vLook = XMVectorSet( XMVectorGetX(m_EffectDesc.vLook), 0.1f, rand()%200*0.01f - 1.f, 0.f);
			EffectDesc.vInitScale = _float3(0.5f, 0.5f, 0.f);
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, +0.1f, rand() % 8 - 4.f, 0.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
		}

		break;
	}
	case ROLLING_FLASH2:
		SetUp_BillBoard();

		m_pTransformCom->Go_PosDir(fTimeDelta*m_fSpeed, m_EffectDesc.vLook);

		if (m_fDeadtime < m_EffectDesc.fDeadTime)
		{
			m_vScale.x += 0.05f;
			m_vScale.y += 0.03f;
		}
		else
		{
			m_fAlpha -= 0.05f;
			m_vScale.x -= 0.05f;
			m_vScale.y -= 0.03f;
		}
		break;
	case ROLA_PUSHRING:	
		m_pTransformCom->LookDir(m_EffectDesc.vLook);

		if (!m_bMax)
		{
			m_fAlpha += 0.05f;

			if (m_fAlpha >= 1.f)
			{
				m_fAlpha = 1.f;
				m_bMax = true;
			}
		}
		else
			m_fAlpha -= 0.05f;
		m_vScale.x += 0.04f;
		m_vScale.y += 0.04f;
		m_vScale.z += 0.04f;
		break;
	case ROLA_PUSHFLASH:
		SetUp_BillBoard();

		if (!m_bMax)
		{
			m_fAlpha += 0.05f;
			m_vScale.x += 0.05f;
			m_vScale.y += 0.05f;
			if (m_fAlpha >= 1.f)
			{
				m_fAlpha = 1.f;
				m_bMax = true;
			}
		}
		else
		{
			m_fAlpha -= 0.05f;

			m_vScale.x -= 0.1f;
			m_vScale.y -= 0.1f;
		}
			
		break;
	}


	if (m_fAlpha <= 0.f || m_vScale.x <= 0.f)
		m_bDead = true;
		
}



CMonsterEffect * CMonsterEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMonsterEffect*	pInstance = new CMonsterEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMonsterEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonsterEffect::Clone(void * pArg)
{
	CMonsterEffect*	pInstance = new CMonsterEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMonsterEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterEffect::Free()
{
	__super::Free();

	Safe_Release(m_pSmokeDstTexture);
	Safe_Release(m_pDissolveTexture);
}
