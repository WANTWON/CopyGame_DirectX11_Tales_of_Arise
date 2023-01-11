#include "stdafx.h"
#include "..\Public\FightEffect.h"
#include "Weapon.h"
#include "Player.h"
#include "GameInstance.h"

CFightEffect::CFightEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CFightEffect::CFightEffect(const CFightEffect & rhs)
	: CEffect(rhs)
{
}

HRESULT CFightEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFightEffect::Initialize(void * pArg)
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
	case HITRING:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_vColorBack = XMVectorSet(255, 121, 0, 255);
		m_vColorFront = XMVectorSet(255, 230, 150, 255);
		break;
	case HITSPARK:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_vDirection = _float3((rand() % 20 - 10) * 0.1f,  0.f, (rand() % 20 - 10)* 0.1f);
		m_vColorBack = XMVectorSet(255, 121, 0, 255);
		m_vColorFront = XMVectorSet(255, 191, 95, 255);
		break;
	case HITFLASH:
		m_eShaderID = SHADERM_TWOCOLOR_PRIORITY;
		m_vColorBack = XMVectorSet(200, 95, 0, 255);
		m_vColorFront = XMVectorSet(255, 191, 95, 255);
		break;
	case HITFLASH_TEX:
		m_eShaderID = SHADER_HITFLASH_TEX2;
		m_vScale = m_EffectDesc.vInitScale;

		if (m_EffectDesc.iTextureNum == 1)
			m_eShaderID = SHADER_HITFLASH_TEX;

		if (m_EffectDesc.iTextureNum == 2)
			m_eShaderID = SHADER_HITFLASH_TEX3;
		break;
	case SMOKEBACK:
		m_eShaderID = SHADER_SMOKEBACK;
		break;
	case SMOKEFRONT:
		m_fAlpha = 0.f;
		m_eShaderID = SHADER_SMOKEFRONT;
		m_vColorBack = XMVectorSet(63, 0, 38, 255);
		m_vColorFront = XMVectorSet(114, 0, 153, 255);
		break;
	case GLOW_LARGE:
		m_fAngle = XMConvertToRadians(float(rand() % 180));
		m_eShaderID = SHADER_TWOCOLOR_PRIORITY;
		m_vColorBack = XMVectorSet(144, 2, 225, 255);
		m_vColorFront = XMVectorSet(228, 226, 228, 255);
		break;
	case GLOW_MIDDLE:
		m_fGlowMaxSize = 2.5f;
		m_fGlowAddScale = 0.1f;
		m_fSpeed = 0.5f;
		m_eShaderID = SHADER_TWOCOLOR_PRIORITY;
		m_vColorBack = XMVectorSet(76, 102, 255, 255);
		m_vColorFront = XMVectorSet(228, 226, 228, 255);
		break;
	case GLOW_SMALL:
		m_fGlowMaxSize = 1.0f;
		m_fGlowAddScale = 0.1f;
		m_fSpeed = 0.7f;
		m_eShaderID = SHADER_TWOCOLOR_PRIORITY;
		m_vColorBack = XMVectorSet(196, 0, 196, 255);
		m_vColorFront = XMVectorSet(228, 226, 228, 255);
		break;
	case GLOW_MINI:
		m_fGlowMaxSize = 0.6f;
		m_fGlowAddScale = 0.05f;
		m_fSpeed = 0.2f;
		m_eShaderID = SHADER_TWOCOLOR_PRIORITY;
		m_vColorBack = XMVectorSet(226, 0, 225, 255);
		m_vColorFront = XMVectorSet(228, 226, 228, 255);
		break;
	case GLOW_SPHERE:
		m_fAngle = XMConvertToRadians(float(rand() % 180));
		m_eShaderID = SHADER_TWOCOLOR_PRIORITY;
		m_vColorFront = m_EffectDesc.vColor;
		m_vColorBack = XMVectorSet(144, 2, 225, 255);
		break;
	case GUARD_FLASH:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_fAlpha = 0.f;
		m_vColorFront = XMVectorSet(51, 178, 255, 255);
		m_vColorBack = XMVectorSet(51, 102, 225, 255);
		break;
	case GUARD_RING:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_fAlpha = 0.f;
		m_vColorFront = XMVectorSet(228, 226, 228, 255);
		m_vColorBack = XMVectorSet(51, 102, 225, 255);
		break;
	case GUARD_RING2:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_fAlpha = 0.f;
		m_vColorFront = XMVectorSet(228, 226, 228, 255);
		m_vColorBack = XMVectorSet(51, 102, 225, 255);
		break;
	case GUARD_DUST:
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_vDirection = _float3( XMVectorGetX(m_EffectDesc.vLook)*-1.f + (rand() % 20 - 10) * 0.1f, 0.f, (rand() % 20 - 10)* 0.1f);
		m_vColorFront = XMVectorSet(51, 102, 225, 255);
		m_vColorBack = XMVectorSet(228, 226, 228, 255);
		break;
	case DEAD_FLASH:
		m_fSpeed = (rand() % 100)*0.01f + 0.3f;
		m_eShaderID = SHADERM_TWOCOLOR_PRIORITY;
		m_vColorBack = XMVectorSet(228, 0, 228, 255);
		m_vColorFront = XMVectorSet(228, 226, 228, 255);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.vInitPositon + m_EffectDesc.vDistance);
		break;
	default:
		break;
	}

	return S_OK;
}

int CFightEffect::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	__super::Tick(fTimeDelta);

	switch (m_EffectDesc.eEffectID)
	{
	case HITFLASH:
	case HITRING:
	case HITSPARK:
	case HITFLASH_TEX:
		Tick_HitFlash(fTimeDelta);
		break;
	case GLOW_SPHERE:
	case SMOKEBACK:
	case SMOKEFRONT:
	case DEAD_FLASH:
		Tick_DeadEffect(fTimeDelta);
		break;
	case GLOW_LARGE:
	case GLOW_MIDDLE:
	case GLOW_SMALL:
	case GLOW_MINI:
		Tick_GlowEffect(fTimeDelta);
		break;
	case GUARD_FLASH:
	case GUARD_RING:
	case GUARD_RING2:
		Tick_GuardEffect(fTimeDelta);
		break;
	case GUARD_DUST:
		Tick_SparkEffect(fTimeDelta);
		break;
	default:
		break;
	}

	return OBJ_NOEVENT;
}

void CFightEffect::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CFightEffect::Render()
{

	__super::Render();


	return S_OK;
}

HRESULT CFightEffect::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	__super::Ready_Components();

	switch (m_EffectDesc.eEffectID)
	{
	case HITFLASH:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_HitFlash"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case HITRING:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_HitRing"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case HITSPARK:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_HitSpark"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case HITFLASH_TEX:
	case DEAD_FLASH:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Flash"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case GLOW_LARGE:
	case GLOW_MIDDLE:
	case GLOW_SMALL:
	case GLOW_MINI:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Glow"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case GLOW_SPHERE:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Glow"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case SMOKEBACK:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeDeath"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case SMOKEFRONT:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeDeath"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;

		if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
			return E_FAIL;

		if (FAILED(__super::Add_Components(TEXT("Com_SmokeDstTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeDst"), (CComponent**)&m_pSmokeDstTexture)))
			return E_FAIL;
		break;
	case GUARD_FLASH:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardFlash"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case GUARD_RING:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardRing"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case GUARD_RING2:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardRing_2"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case GUARD_DUST:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardDust"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	default:
		break;
	}
	
	return S_OK;
}


HRESULT CFightEffect::SetUp_ShaderID()
{
	return S_OK;
}

void CFightEffect::Change_Animation(_float fTimeDelta)
{
}

void CFightEffect::Change_Texture(_float fTimeDelta)
{
	
}

HRESULT CFightEffect::SetUp_ShaderResources()
{
	__super::SetUp_ShaderResources();


	switch (m_EffectDesc.eEffectID)
	{
	case SMOKEFRONT:
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_SmokeDstTexture", m_pSmokeDstTexture->Get_SRV())))
			return E_FAIL;
		break;
	default:
		break;
	}

	return S_OK;
}


void CFightEffect::Tick_HitFlash(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;

	if (m_EffectDesc.fDeadTime < m_fDeadtime)
		m_bDead = true;

	switch (m_EffectDesc.eEffectID)
	{
	case HITRING:
		SetUp_BillBoard();
		if (m_fDeadtime > m_EffectDesc.fDeadTime*0.5f)
			m_fAlpha -= 0.1f;
		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;
		m_vScale.z += 0.05f;
		Set_Scale(m_vScale);
		break;
	case HITSPARK:
	{
		m_pTransformCom->Go_PosDir(fTimeDelta, XMLoadFloat3(&m_vDirection));
		m_pTransformCom->LookAt(m_EffectDesc.vInitPositon);

		_float4x4 ViewMatrix;

		ViewMatrix = CGameInstance::Get_Instance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW); // Get View Matrix
		_matrix matViewInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&ViewMatrix));      // Get Inverse of View Matrix (World Matrix of Camera)

		_vector vRight = Get_TransformState(CTransform::STATE_RIGHT);
		_vector vLook = (_vector)matViewInv.r[2];
		_vector vUp = XMVector3Cross(vLook, vRight);

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) *Get_Scale().x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * Get_Scale().y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) *Get_Scale().z);


		if (m_fDeadtime > m_EffectDesc.fDeadTime*0.5f)
		{
			m_fAlpha -= 0.1f;
			m_vScale.x -= 0.1f;
			m_vScale.y -= 0.1f;
			m_vScale.z -= 0.1f;
		}
		Set_Scale(m_vScale);
		break;
	}
	case HITFLASH:
		if (m_fDeadtime > m_EffectDesc.fDeadTime*0.5f)
		{
			m_fAlpha -= 0.1f;
			m_vScale.x -= 0.1f;
			m_vScale.y -= 0.1f;
			m_vScale.z -= 0.1f;
		}
		Set_Scale(m_vScale);
		break;
	case HITFLASH_TEX:
		SetUp_BillBoard();

		if (m_eShaderID != SHADER_HITFLASH_TEX3)
		{
			if (m_fDeadtime < m_EffectDesc.fDeadTime*0.5f)
			{
				m_vScale.x += 0.1f;
				m_vScale.y += 0.1f;
				m_vScale.z += 0.1f;
			}

			else
			{
				m_vScale.x -= 0.1f;
				m_vScale.y -= 0.1f;
				m_vScale.z -= 0.1f;
			}

		}
		else
		{
			m_fAlpha -= 0.1f;
			m_vScale.x -= 0.1f;
			m_vScale.y -= 0.1f;
			m_vScale.z -= 0.1f;
		}

		Set_Scale(m_vScale);
		break;
	default:
		break;
	}


}

void CFightEffect::Tick_DeadEffect(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;

	switch (m_EffectDesc.eEffectID)
	{
	case GLOW_SPHERE:
		SetUp_BillBoard();
		if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);
		//m_pTransformCom->Rotation(Get_TransformState(CTransform::STATE_LOOK), m_fAngle);
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
		break;
	case SMOKEBACK:
		SetUp_BillBoard();

		if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);

		if (m_fDeadtime < m_EffectDesc.fDeadTime*0.5f)
		{
			m_vScale.x += 0.07f;
			m_vScale.y += 0.07f;
		}
		else
		{
			m_vScale.x -= 0.01f;
			m_vScale.y -= 0.01f;
		}
		m_fAlpha -= 0.007f;
		Set_Scale(m_vScale);
		break;
	case SMOKEFRONT:
		SetUp_BillBoard();

		if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);

		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;
		Set_Scale(m_vScale);
	
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
	case DEAD_FLASH:
		if (m_fDeadtime > m_EffectDesc.fDeadTime*0.5f)
		{
			m_fAlpha -= 0.1f;
			m_vScale.x -= 0.1f;
			m_vScale.y -= 0.1f;
			m_vScale.z -= 0.1f;
		}
		SetUp_BillBoard();

		m_pTransformCom->Go_PosDir(fTimeDelta*m_fSpeed, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		break;
	default:
		break;
	}



	if (m_fAlpha <= 0.f)
		m_bDead = true;


}

void CFightEffect::Tick_GlowEffect(_float fTimeDelta)
{

	m_fDeadtime += fTimeDelta;

	SetUp_BillBoard();

	if (m_EffectDesc.eEffectID != GLOW_LARGE)
	{
		m_pTransformCom->Go_PosDir(fTimeDelta*m_fSpeed, m_EffectDesc.vLook);

		if (!m_bMax)
		{
			m_vScale.x += m_fGlowAddScale;
			m_vScale.y += m_fGlowAddScale;

			if (m_vScale.x >= m_fGlowMaxSize)
				m_bMax = true;
		}
		else if (m_bMax && m_fDeadtime > m_EffectDesc.fDeadTime*0.5f)
		{
			m_vScale.x -= m_fGlowAddScale;
			m_vScale.y -= m_fGlowAddScale;

			if (m_EffectDesc.eEffectID == GLOW_SMALL)
				Make_GlowEffect();

			if (m_vScale.x <= 0)
				m_bDead = true;

		}
		Set_Scale(m_vScale);
	}
	else
	{
		if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, m_EffectDesc.pTarget->Get_Scale().y, 0.f, 0.f));

		if (m_fDeadtime < m_EffectDesc.fDeadTime*0.3f)
		{
			m_vScale.x += 0.05f;
			m_vScale.y += 0.05f;
		}
		else if (m_fDeadtime < m_EffectDesc.fDeadTime*0.6f)
		{
			m_vScale.x += 0.3f;
			//m_fScale.y += 0.2f;
		}
		else if (m_fDeadtime < m_EffectDesc.fDeadTime)
		{
			m_vScale.x -= 0.3f;
			m_vScale.y += 0.3f;
		}
		Set_Scale(m_vScale);

		if (m_EffectDesc.fDeadTime < m_fDeadtime)
		{
			m_fAlpha -= 0.25f;
			m_vScale.x -= 0.3f;
			m_vScale.y -= 0.3f;
			Make_GlowEffect();
		}
	}


	if (m_fAlpha <= 0.f)
		m_bDead = true;

}

void CFightEffect::Tick_GuardEffect(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;

	m_pTransformCom->LookDir(m_EffectDesc.vLook);

	if (m_EffectDesc.eEffectID == GUARD_RING || m_EffectDesc.eEffectID == GUARD_RING2)
		SetUp_BillBoard();

	if ( m_fDeadtime < m_EffectDesc.fDeadTime)
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

	Set_Scale(m_vScale);

	if (m_fAlpha <= 0.f || m_vScale.x <= 0.f)
		m_bDead = true;

}

void CFightEffect::Tick_SparkEffect(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;

	m_pTransformCom->Go_PosDir(fTimeDelta, XMLoadFloat3(&m_vDirection));
	m_pTransformCom->LookAt(m_EffectDesc.vInitPositon);

	_float4x4 ViewMatrix;

	ViewMatrix = CGameInstance::Get_Instance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW); // Get View Matrix
	_matrix matViewInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&ViewMatrix));      // Get Inverse of View Matrix (World Matrix of Camera)

	_vector vRight = Get_TransformState(CTransform::STATE_RIGHT);
	_vector vLook = (_vector)matViewInv.r[2];
	_vector vUp = XMVector3Cross(vLook, vRight);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) *Get_Scale().x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * Get_Scale().y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) *Get_Scale().z);


	if (m_fDeadtime > m_EffectDesc.fDeadTime*0.5f)
	{
		m_fAlpha -= 0.1f;
		m_vScale.x -= 0.1f;
		m_vScale.y -= 0.1f;
		m_vScale.z -= 0.1f;
	}
	Set_Scale(m_vScale);


	if (m_fAlpha <= 0.f || m_vScale.x <= 0.f)
		m_bDead = true;
	
}

void CFightEffect::Make_GlowEffect()
{
	if (!m_bMakeGlow)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CEffect::EFFECTDESC EffectDesc;
		EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
		EffectDesc.iTextureNum = 1;
		EffectDesc.fDeadTime = 0.5f;


		if (m_EffectDesc.eEffectID == GLOW_SMALL)
		{
			
			for (int i = 0; i < 4; ++i)
			{
				EffectDesc.vInitScale = _float3(0.3f, 0.3f, 0.0f);
				EffectDesc.fDeadTime = 0.5f;
				EffectDesc.eEffectID = CFightEffect::GLOW_MINI;
				m_vDirection = _float3((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)* 0.1f, 0.f /*(rand() % 20 - 10)* 0.1f*/);
				if (pGameInstance->Get_CurrentLevelIndex() == LEVEL_TAILCAVE)
					EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, 0.f, (rand() % 20 - 10)* 0.1f, 0.f);
				else
					EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)* 0.1f, 0.f, 0.f);

				pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

			}
		}
		else
		{
			
			for (int i = 0; i < 5; ++i)
			{
				EffectDesc.vInitScale = _float3(1.f, 1.f, 0.0f);
				EffectDesc.fDeadTime = 0.5f;
				EffectDesc.eEffectID = CFightEffect::GLOW_MIDDLE;
				if (pGameInstance->Get_CurrentLevelIndex() == LEVEL_TAILCAVE)
					EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, 0.f, (rand() % 20 - 10)* 0.1f, 0.f);
				else
					EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)* 0.1f, 0.f, 0.f);

				EffectDesc.vLook = XMLoadFloat3(&m_vDirection);
				pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

			}


			for (int i = 0; i < 5; ++i)
			{
				EffectDesc.vInitScale = _float3(0.5f, 0.5f, 0.0f);
				EffectDesc.fDeadTime = 1.0f;
				EffectDesc.eEffectID = CFightEffect::GLOW_SMALL;
				if (pGameInstance->Get_CurrentLevelIndex() == LEVEL_TAILCAVE)
					EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, 0.f, (rand() % 20 - 10)* 0.1f, 0.f);
				else
					EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)* 0.1f, 0.f, 0.f);

				pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

			}
		}
		RELEASE_INSTANCE(CGameInstance);
		m_bMakeGlow = true;
	}
}

CFightEffect * CFightEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFightEffect*	pInstance = new CFightEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMonsterEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFightEffect::Clone(void * pArg)
{
	CFightEffect*	pInstance = new CFightEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMonsterEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFightEffect::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTexture);
	Safe_Release(m_pSmokeDstTexture);
}
