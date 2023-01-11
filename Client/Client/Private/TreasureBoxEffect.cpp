#include "stdafx.h"
#include "..\Public\TreasureBoxEffect.h"
#include "Weapon.h"
#include "Player.h"
#include "GameInstance.h"
#include "TreasureBox.h"
#include "FightEffect.h"
#include "CameraManager.h"
#include "ObjectEffect.h"

CTreasureBoxEffect::CTreasureBoxEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CEffect(pDevice, pContext)
{
}

CTreasureBoxEffect::CTreasureBoxEffect(const CTreasureBoxEffect & rhs)
	: CEffect(rhs)
{
}

HRESULT CTreasureBoxEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreasureBoxEffect::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Set_Scale(m_EffectDesc.vInitScale);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.vInitPositon);
	m_pTransformCom->LookDir(m_EffectDesc.vLook);
	m_vScale = m_EffectDesc.vInitScale;


	switch (m_EffectDesc.eEffectID)
	{
	case TREASURE_BEAM:
		m_eShaderID = SHADERM_TWOCOLOR_NOTALPHASET;
		m_vColorFront = m_EffectDesc.vColor;
		m_vColorBack = m_EffectDesc.vColor;
		break;
	case TREASURE_HALO:
		m_fSpeed = 10.f;
		m_eShaderID = SHADERM_TWOCOLOR_DEFAULT;
		m_vColorFront = m_EffectDesc.vColor;
		m_vColorBack = m_EffectDesc.vColor;
		m_fAlpha = 0.f;
		break;
	case TREASURE_GLOW:
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vColorFront = XMVectorSet(228.f, 226.f, 228.f, 255.f);
		m_vColorBack = m_EffectDesc.vColor;
		break;
	case TREASURE_CROSS:
		m_vMaxScale = _float3(5.f, 5.f, 5.f);
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vecColor.push_back(XMVectorSet(198, 133, 0, 255.f));
		m_vecColor.push_back(XMVectorSet(255.f, 255, 0.f, 255.f));
		m_vColorFront = XMVectorSet(255, 255, 174, 255.f);
		m_vColorBack = m_vecColor.front();
		break;
	case TREASURE_FLASH:
	case TRASURE_ENTRANCEBOX:
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vecColor.push_back(XMVectorSet(192, 72, 0, 255.f));
		m_vecColor.push_back(XMVectorSet(188, 62, 28, 255.f));
		m_vColorFront = XMVectorSet(188, 95, 28, 255.f);
		m_vColorBack = m_vecColor.front();
		break;
	case TREASURE_GLITTER:
		m_fSpeed = 0.5f;
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vColorFront = XMVectorSet(247, 255, 139, 255.f);
		m_vColorBack = XMVectorSet(255, 204, 127, 255.f);
		break;
	default:
		break;
	}


	return S_OK;
}

int CTreasureBoxEffect::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	__super::Tick(fTimeDelta);


	if (m_bEndMode)
	{
		m_vScale.x -= 0.05f;
		m_vScale.y -= 0.05f;
		m_vScale.z -= 0.05f;
		m_fAlpha -= 0.05f;

		if (m_fAlpha < 0.f || m_vScale.x <= 0.f)
			m_bDead = true;

		return OBJ_NOEVENT;
	}


	switch (m_EffectDesc.eEffectID)
	{
	case TREASURE_GLOW:
		Tick_GlowEffect(fTimeDelta);
		break;
	case TREASURE_FLASH:
		Tick_FlashEffect(fTimeDelta);
		break;
	case TREASURE_HALO:
		Tick_HaloEffect(fTimeDelta);
		break;
	case TREASURE_GLITTER:
		Tick_GlitterEffect(fTimeDelta);
		break;
	case TREASURE_BEAM:
	case TRASURE_ENTRANCEBOX:
		Tick_TreausureBox(fTimeDelta);
		break;
	case TREASURE_CROSS:
		Tick_TreasureCross(fTimeDelta);
		break;
	default:
		break;
	}

	return OBJ_NOEVENT;
}

void CTreasureBoxEffect::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CTreasureBoxEffect::Render()
{

	__super::Render();


	return S_OK;
}

HRESULT CTreasureBoxEffect::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	__super::Ready_Components();

	switch (m_EffectDesc.eEffectID)
	{
	case TREASURE_FLASH:
	case TREASURE_CROSS:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Flash"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case TREASURE_GLOW:
	case TREASURE_GLITTER:
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Glow"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	case TREASURE_BEAM:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_TreasureBox_Effect"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case TREASURE_HALO:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_TreasureBoxHalo"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case TRASURE_ENTRANCEBOX:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Treasure_EntranceBox"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	default:
		break;
	}


	return S_OK;
}


HRESULT CTreasureBoxEffect::SetUp_ShaderID()
{
	return S_OK;
}

void CTreasureBoxEffect::Change_Animation(_float fTimeDelta)
{
}

void CTreasureBoxEffect::Tick_GlowEffect(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;
	m_fColorTime += fTimeDelta;

	SetUp_BillBoard();

	if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);

	if (m_vecColor.size() != 0)
	{
		if (m_fColorTime > m_EffectDesc.fDeadTime*0.3f)
		{
			if (m_iColorIndex + 1 >= m_vecColor.size() - 1)
				m_iColorIndex = 0/*m_vecColor.size() - 2*/;
			else
				m_iColorIndex++;
				m_fColorTime = 0.f;
		}


		m_vColorBack = XMVectorLerp(m_vecColor[m_iColorIndex], m_vecColor[m_iColorIndex + 1], m_fColorTime / 0.3f);

	}

	if (m_fDeadtime < m_EffectDesc.fDeadTime*0.3f && m_fDeadtime >= m_EffectDesc.fStartTime)
	{
		m_vScale.x += 0.04f;
		m_vScale.y += 0.04f;
	}
	else if (m_fDeadtime < m_EffectDesc.fDeadTime*0.6f)
	{
		m_vScale.x += 0.01f;
		m_vScale.y += 0.01f;
	}
	Set_Scale(m_vScale);

}

void CTreasureBoxEffect::Tick_FlashEffect(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;
	m_fColorTime += fTimeDelta;

	SetUp_BillBoard();

	if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);

	if (m_EffectDesc.fStartTime == 0)
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


	if (m_fDeadtime < m_EffectDesc.fDeadTime &&  m_EffectDesc.fStartTime <= m_fDeadtime)
	{
		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;
	}
	else if (m_fDeadtime > m_EffectDesc.fDeadTime)
	{
		m_vScale.x -= 0.2f;
		m_vScale.y -= 0.2f;
		m_fAlpha -= 0.05f;

		if (m_fAlpha <= 0.f || m_vScale.x <= 0.f)
		{
			CEffect::EFFECTDESC EffectDesc;

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);// +XMVectorSet(0.f, Get_Scale().y - 0.4f, 0.f, 0.f);
			EffectDesc.pTarget = m_EffectDesc.pTarget;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			for (int i = 0; i < 10; ++i)
			{
				
				EffectDesc.eEffectID = CObjectEffect::GLITTER;
				_float fRandNum = (rand() % 5 + 1) * 0.1f;
				EffectDesc.vInitScale = _float3(fRandNum, fRandNum, 0.0f);
				EffectDesc.fDeadTime = 1.5f;
				EffectDesc.iTextureNum = 1;
				EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)*0.1f, 0.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);
			}

			m_bDead = true;
		}
			
	}



	

	Set_Scale(m_vScale);
}

void CTreasureBoxEffect::Tick_HorizontalGlowEffect(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;
	m_fColorTime += fTimeDelta;

	SetUp_BillBoard();

	if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);

	
		if (m_fColorTime > m_EffectDesc.fDeadTime*0.3f)
		{
			if (m_iColorIndex + 1 >= m_vecColor.size() - 1)
				m_iColorIndex = 0;
			else
				m_iColorIndex++;

			m_fColorTime = 0.f;
		}


		m_vColorBack = XMVectorLerp(m_vecColor[m_iColorIndex], m_vecColor[m_iColorIndex + 1], m_fColorTime / 0.3f);

	

	if (!m_bMax)
	{
		m_fAlpha += 0.1f;
		m_vScale.x += 0.3f;
		m_vScale.y += 0.1f;

		if (m_vScale.x >= m_vMaxScale.x)
		{
			m_vScale.x = m_vMaxScale.x;
			m_bMax = true;
		}	
		if (m_vScale.y >= m_vMaxScale.y)
			m_vScale.y = m_vMaxScale.y;

		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;
	}
	else
	{
		m_vScale.y -= 0.02f;
		m_fAlpha -= 0.05f;

		if (m_fAlpha < 0.f || m_vScale.x < 0.f)
			m_bDead = true;
	}

	Set_Scale(m_vScale);
}

void CTreasureBoxEffect::Tick_RingEffect(_float fTimeDelta)
{
	SetUp_BillBoard();

	m_fDeadtime += fTimeDelta;
	m_fColorTime += fTimeDelta;

	if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);


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


	if (m_fDeadtime < m_EffectDesc.fDeadTime*0.5  && m_fDeadtime > m_EffectDesc.fStartTime)
	{
		m_fAlpha += 0.1f;
		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;
		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;
		m_vScale.z += 0.05f;
	}
	else if (m_fDeadtime < m_EffectDesc.fDeadTime)
	{
		m_vScale.x += 0.025f;
		m_vScale.y += 0.025f;
		m_vScale.z += 0.025f;

	}
	else
	{
		m_vScale.x += 0.01f;
		m_vScale.y += 0.01f;
		m_vScale.z += 0.01f;
		m_fAlpha -= 0.025f;
	}

	Set_Scale(m_vScale);

	if (m_fAlpha <= 0)
		m_bDead = true;
}

void CTreasureBoxEffect::Tick_HaloEffect(_float fTimeDelta)
{
	SetUp_BillBoard();

	m_fDeadtime += fTimeDelta;
	m_fColorTime += fTimeDelta;

	if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);


	if (!m_bMax)
	{
		m_fAlpha += 0.002f*m_fSpeed;
		if (m_fAlpha >= 0.5f)
		{
			m_fAlpha = 0.5f;
			m_bMax = true;
		}
			

	}
	else if(m_fDeadtime > m_EffectDesc.fDeadTime)
	{
		m_fAlpha -= 0.002f*m_fSpeed;

		if (m_fAlpha <= 0)
			m_bDead = true;
	}


	
}

void CTreasureBoxEffect::Tick_GlitterEffect(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;

	SetUp_BillBoard();

	m_pTransformCom->Go_PosDir(fTimeDelta*m_fSpeed, m_EffectDesc.vLook);

	if (m_fDeadtime < m_EffectDesc.fDeadTime*0.2f)
	{
		m_vScale.x += 0.05f;
		m_vScale.y += 0.05f;

		if (m_vScale.x >= m_vMaxScale.x)
		{
			m_vScale = m_vMaxScale;
		}
	}
	else if (m_fDeadtime < m_EffectDesc.fDeadTime*0.4f)
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

void CTreasureBoxEffect::Tick_Ray(_float fTimeDelta)
{
	if (m_EffectDesc.pTarget != nullptr && m_EffectDesc.pTarget->Get_Dead() == false)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);
		m_pTransformCom->LookAt(m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION));

		_float4x4 ViewMatrix;

		ViewMatrix = CGameInstance::Get_Instance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW); // Get View Matrix
		_matrix matViewInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&ViewMatrix));      // Get Inverse of View Matrix (World Matrix of Camera)

		_vector vRight = Get_TransformState(CTransform::STATE_RIGHT);
		_vector vLook = (_vector)matViewInv.r[2];
		_vector vUp = XMVector3Cross(vLook, vRight);

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) *Get_Scale().x);
		m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * Get_Scale().y);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) *Get_Scale().z);

	}

	if (m_fDeadtime < m_EffectDesc.fDeadTime)
	{
		m_vScale.y += 0.01f;
		m_fAlpha += 0.01f;

		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;
	}
	else
	{
		m_vScale.y += 0.01f;
		m_fAlpha -= 0.01f;
	}

	Set_Scale(m_vScale);

	if (m_fAlpha <= 0.f)
		m_bDead = true;
	
}

void CTreasureBoxEffect::Tick_TreausureBox(_float fTimeDelta)
{
	SetUp_BillBoard();

	if (m_EffectDesc.eEffectID == TREASURE_BEAM)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION) + m_EffectDesc.vDistance);

		if (!m_bMax)
		{
			m_fAlpha += 0.1f;

			if (m_fAlpha >= 1.f)
			{
				m_bMax = true;
				m_fAlpha = 1.f;
			}
		}
	}
	else if (m_EffectDesc.eEffectID == TRASURE_ENTRANCEBOX)
	{
		m_fDeadtime += fTimeDelta;

		if (!m_bMax)
		{
			m_fAlpha += 0.1f;

			if (m_fAlpha >= 1.f)
			{
				m_bMax = true;
				m_fAlpha = 1.f;
			}
		}

		if (m_fDeadtime > m_EffectDesc.fDeadTime && m_bMax)
		{
			m_fAlpha -= 0.05f;

			if (m_fAlpha <= 0.f)
				m_bDead = true;
		}

	}

	Set_Scale(m_vScale);
}

void CTreasureBoxEffect::Tick_TreasureCross(_float fTimeDelta)
{
	CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_ZoomValue(m_fZoomValue);

	SetUp_BillBoard();
	Set_Scale(m_vScale);

	if (!m_bMax)
	{
		m_fZoomValue -= 0.1f;
		if (m_fZoomValue <= -1.f)
			m_fZoomValue = -1.f;

		m_fDeadtime += fTimeDelta;
		if (m_fDeadtime < m_EffectDesc.fDeadTime*0.5f)
		{
			m_vScale.x += 0.02f;
			m_vScale.y += 0.02f;
		}
		else if (m_fDeadtime < m_EffectDesc.fDeadTime)
		{
			m_vScale.x -= 0.02f;
			m_vScale.y -= 0.02f;
		}
		else
		{
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);// +XMVectorSet(0.f, Get_Scale().y - 0.4f, 0.f, 0.f);
			EffectDesc.pTarget = nullptr;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CTreasureBoxEffect::TREASURE_GLITTER;
			EffectDesc.vInitScale = _float3(0.4f, 0.4f, 0.0f);
			EffectDesc.fDeadTime = 1.5f;
			EffectDesc.iTextureNum = 1;
			EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, -1.f, (rand() % 20 - 10) * 0.1f, 0.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_TreasureBoxEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);

			m_fDeadtime = 0.f;
		}

		
		if (XMVectorGetY(m_EffectDesc.pTarget->Get_TransformState(CTransform::STATE_POSITION)) + 1.f >= XMVectorGetY(Get_TransformState(CTransform::STATE_POSITION)))
		{
			m_vScale.x += 0.1f;
			m_vScale.y += 0.1f;

			if (m_vScale.x >= m_vMaxScale.x)
			{
				CEffect::EFFECTDESC EffectDesc;
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
				EffectDesc.pTarget = this;
				EffectDesc.vDistance = XMVectorSet(0.f, 0.5f, 0.f, 0.f);
				EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
				EffectDesc.eEffectID = CTreasureBoxEffect::TREASURE_FLASH;
				EffectDesc.vInitScale = _float3(0.4f, 0.4f, 0.0f);
				EffectDesc.fDeadTime = 1.0f;
				EffectDesc.iTextureNum = 3;
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_TreasureBoxEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
				EffectDesc.pTarget = this;
				EffectDesc.vDistance = XMVectorSet(0.f, 0.5f, 0.f, 0.f);
				EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
				EffectDesc.eEffectID = CTreasureBoxEffect::TREASURE_FLASH;
				EffectDesc.vInitScale = _float3(0.4f, 0.4f, 0.0f);
				EffectDesc.fDeadTime = 1.5f;
				EffectDesc.iTextureNum = 4;
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_TreasureBoxEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);


				for (int i = 0; i < 10; ++i)
				{
					EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
					EffectDesc.pTarget = nullptr;
					EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
					EffectDesc.eEffectID = CTreasureBoxEffect::TREASURE_GLITTER;
					EffectDesc.vInitScale = _float3(0.5f, 0.5f, 0.5f);
					EffectDesc.fDeadTime = 1.5f;
					EffectDesc.iTextureNum = 1;
					EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, 1.f, (rand() % 20 - 10) * 0.1f, 0.f);
					CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_TreasureBoxEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);
				}
		

				CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_TreasureBox_Appear.wav"), SOUND_OBJECT, 0.4f);
				dynamic_cast<CTreasureBox*>(m_EffectDesc.pTarget)->Set_Visible(true);
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
				pPlayer->Set_Stop(false);
				m_bMax = true;
			}
				

		}
		else
		{
			m_pTransformCom->Go_PosDir(fTimeDelta*0.4f, m_EffectDesc.vLook);
		}
	}
	else
	{
		m_vScale.x -= 0.1f;
		m_vScale.y -= 0.1f;
		m_fAlpha -= 0.05f;

		m_fZoomValue += 0.1f;
		if (m_fZoomValue >= 0.f)
			m_fZoomValue = 0.f;
			
	}

	if (m_fAlpha <= 0.f)
	{
		m_bDead = true;
		CGameInstance::Get_Instance()->PlaySounds(TEXT("1-40 Puzzle Solved Jingle.mp3"), SOUND_SYSTEM, 0.5f);
	}
		
}


CTreasureBoxEffect * CTreasureBoxEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTreasureBoxEffect*	pInstance = new CTreasureBoxEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTreasureBoxEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTreasureBoxEffect::Clone(void * pArg)
{
	CTreasureBoxEffect*	pInstance = new CTreasureBoxEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTreasureBoxEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTreasureBoxEffect::Free()
{
	__super::Free();
}
