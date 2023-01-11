#include "stdafx.h"
#include "..\Public\PlayerEffect.h"
#include "Weapon.h"
#include "Player.h"
#include "GameInstance.h"

CPlayerEffect::CPlayerEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CPlayerEffect::CPlayerEffect(const CPlayerEffect & rhs)
	: CEffect(rhs)
{
}

HRESULT CPlayerEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerEffect::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Set_Scale(m_EffectDesc.vInitScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.vInitPositon);
	m_pTransformCom->LookDir(m_EffectDesc.vLook);
	m_vScale = m_EffectDesc.vInitScale;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	switch (m_EffectDesc.eEffectID)
	{
	case ARC:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.vInitPositon);
		m_pTransformCom->LookDir(m_EffectDesc.vLook);
		m_eShaderID = SHADERM_SLASH;
		m_fTexUV = 1.f;
		m_fAlpha = 1.f;
	case SWISH:
		m_eShaderID = SHADERM_ROLLCUT;
		m_fTexUV = 1.f;
		m_fAlpha = 1.f;
		if (pPlayer->Get_AnimState() == CPlayer::SLASH)
		{
			m_vColorFront = XMVectorSet(255, 255, 0, 255.f);
			m_vColorBack = XMVectorSet(255, 76, 0, 255.f);
		}
		else
		{
			m_vColorFront = { 102, 255, 255, 255 };
			m_vColorBack = { 12, 76, 255, 255 };
		}
		break;
	case SLASH:
		Set_Scale(_float3(3.f, 3.f, 3.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.vInitPositon);
		m_pTransformCom->LookDir(m_EffectDesc.vLook);
		m_eShaderID = SHADERM_SLASH;
		m_fTexUV = 1.f;
		m_fAlpha = 1.f;
		if (pPlayer->Get_AnimState() == CPlayer::SLASH)
		{
			m_vColorFront = XMVectorSet(255, 255, 125, 255.f);
		}
		else
		{
			m_vColorFront = { 102, 255, 255, 255 };
		}
		break;
	case CROSS:
	{
		m_eShaderID = SHADERM_TWOCOLOR_PRIORITY;
		m_vColorFront = XMVectorSet(228.f, 226.f, 228.f, 255.f);
		_int iRandNum = rand() % 5 + 1;
		if (iRandNum == 1) m_vColorBack = XMVectorSet(255.f, 63.f, 63.f, 255.f);
		else if (iRandNum == 2) m_vColorBack = XMVectorSet(255.f, 127.f, 0.f, 255.f);
		else if (iRandNum == 3)	m_vColorBack = XMVectorSet(0.f, 255.f, 0.f, 255.f);
		else if (iRandNum == 4)	m_vColorBack = XMVectorSet(63.f, 127.f, 255.f, 255.f);
		else  m_vColorBack = XMVectorSet(255.f, 63.f, 63.f, 255.f);
		m_fSpeed = 0.1f;
		m_vMaxScale = m_EffectDesc.vInitScale;
		break;
	}
	case RIPPLE:
		m_vScale = _float3(0.f, 0.f, 0.f);
		m_vMaxScale = m_EffectDesc.vInitScale;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.vInitPositon);
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vColorFront = XMVectorSet(200, 255, 255, 255.f);
		m_vColorBack = { 102, 255, 255, 255 };
		m_fAlpha = 0.f;
		break;
	default:
		break;
	}



	if (dynamic_cast<CPlayer*>(m_EffectDesc.pTarget)->Get_2DMode() && ( m_EffectDesc.eEffectID == SWISH || m_EffectDesc.eEffectID == SLASH) )
	{
		
		if (XMVectorGetX(m_EffectDesc.vLook) == 1.f)
		{
			m_pTransformCom->Rotation(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180.f);
			m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), -20.f);
		}	
		else
		{
			m_pTransformCom->Rotation(XMVectorSet(-1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
			m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), 20.f);
		}
	}
		

	
	return S_OK;
}

int CPlayerEffect::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}
		
	__super::Tick(fTimeDelta);

	switch (m_EffectDesc.eEffectID)
	{
	case ARC:
	case SLASH:
		Tick_Slash(fTimeDelta);
		break;
	case SWISH:
		Tick_RollCut(fTimeDelta);
		break;
	case CROSS:
		Tick_Cross(fTimeDelta);
		break;
	case RIPPLE:
		Tick_Ripple(fTimeDelta);
		break;
	default:
		break;
	}

	return OBJ_NOEVENT;
}

void CPlayerEffect::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CPlayerEffect::Render()
{

	__super::Render();
		

	return S_OK;
}

HRESULT CPlayerEffect::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	__super::Ready_Components();

	switch (m_EffectDesc.eEffectID)
	{
	case SWISH:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordSwish0"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		if (FAILED(__super::Add_Components(TEXT("Com_GlowTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Glow"), (CComponent**)&m_pGlowTexture)))
			return E_FAIL;
		break;
	case SLASH:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_RollCut"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		if (FAILED(__super::Add_Components(TEXT("Com_GlowTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Glow"), (CComponent**)&m_pGlowTexture)))
			return E_FAIL;
		break;
	case CROSS:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Glow"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case RIPPLE:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Ripple"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case ARC:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordArc"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		if (FAILED(__super::Add_Components(TEXT("Com_GlowTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Glow"), (CComponent**)&m_pGlowTexture)))
			return E_FAIL;
		break;
	default:
		break;
	}


	return S_OK;
}


HRESULT CPlayerEffect::SetUp_ShaderID()
{
	return S_OK;
}

HRESULT CPlayerEffect::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();

	if (m_pGlowTexture != nullptr)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GlowTexture", m_pGlowTexture->Get_SRV(0))))
			return E_FAIL;
	}


	return S_OK;
}

void CPlayerEffect::Change_Animation(_float fTimeDelta)
{
}

void CPlayerEffect::Change_Texture(_float fTimeDelta)
{
	
}


void CPlayerEffect::Tick_RollCut(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;
	m_fTexUV -= 0.07f;
	if (m_fTexUV <= 0.f)
		m_fTexUV = 0.f;

	if (m_EffectDesc.fDeadTime*0.2f > m_fDeadtime)
	{
		m_fAlpha = 0.f;
	}
	else if (m_EffectDesc.fDeadTime*0.2f < m_fDeadtime && m_EffectDesc.fDeadTime > m_fDeadtime)
	{
		m_fAlpha += 0.1f;
		if (m_fAlpha >= 1)
			m_fAlpha = 1.f;
	}
	else if (m_EffectDesc.fDeadTime < m_fDeadtime)
	{
		m_fAlpha -= 0.1f;
		if(m_fAlpha <= 0)
			m_bDead = true;
	}
		

}

void CPlayerEffect::Tick_Slash(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;
	m_fTexUV -= 0.07f;
	if (m_fTexUV <= 0.f)
		m_fTexUV = 0.f;

	if (m_EffectDesc.fDeadTime < m_fDeadtime)
	{
		m_fAlpha -= 0.1f;
		if (m_fAlpha <= 0)
			m_bDead = true;
	}

}

void CPlayerEffect::Tick_Cross(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;

	SetUp_BillBoard();

	if (m_fDeadtime < m_EffectDesc.fDeadTime*0.2f)
	{
		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;

		if (m_vScale.x >= m_vMaxScale.x)
		{
			m_vScale = m_vMaxScale;
		}
	}
	else if (m_fDeadtime < m_EffectDesc.fDeadTime*g_fUIVolume)
	{
		m_vScale.x -= 0.05f;
		m_vScale.y -= 0.05f;
	}
	else if (m_fDeadtime < m_EffectDesc.fDeadTime*0.6f)
	{
		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;

		if (m_vScale.x >= m_vMaxScale.x)
		{
			m_vScale = m_vMaxScale;
		}
	}
	else if (m_fDeadtime < m_EffectDesc.fDeadTime*0.8f)
	{
		m_vScale.x -= 0.05f;
		m_vScale.y -= 0.05f;
	}
	else if (m_fDeadtime < m_EffectDesc.fDeadTime)
	{
		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;

		if (m_vScale.x >= m_vMaxScale.x)
		{
			m_vScale = m_vMaxScale;
		}
	}
	else
	{
		m_vScale.x -= 0.05f;
		m_vScale.y -= 0.05f;
		m_fAlpha -= 0.25f;
	}

	Set_Scale(m_vScale);

	if (m_fAlpha <= 0.f)
		m_bDead = true;
}

void CPlayerEffect::Tick_Ripple(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;

	if (m_fDeadtime <= m_EffectDesc.fStartTime)
		return;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f,0.5f,0.f,0.f));
	SetUp_BillBoard();
	m_pTransformCom->Turn(XMVectorSet(-1.f, 0.f, 0.f, 0.f), -40.f);
	m_pTransformCom->LookDir(XMVectorSet(0.f, 90.f, 1.f, 0.f));


	if (m_vScale.x < m_vMaxScale.x)
	{
		m_fAlpha += 0.1f;
		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;

		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;
	}
	else
	{
		m_vScale.x += 0.01f;
		m_vScale.y += 0.01f;
		m_fAlpha -= 0.02f;
	}

	Set_Scale(m_vScale);

	if (m_fAlpha <= 0.f)
		m_bDead = true;

	
}

CPlayerEffect * CPlayerEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayerEffect*	pInstance = new CPlayerEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayerEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayerEffect::Clone(void * pArg)
{
	CPlayerEffect*	pInstance = new CPlayerEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPlayerEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerEffect::Free()
{
	__super::Free();

	Safe_Release(m_pGlowTexture);
}
