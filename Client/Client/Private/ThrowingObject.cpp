#include "stdafx.h"
#include "..\Public\ThrowingObject.h"
#include "PlayerManager.h"
#include "Player.h"

CThrowingObject::CThrowingObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CInteractObject(pDevice, pContext)
{
	ZeroMemory(&m_tThrowDesc, sizeof(THROWDESC));
}

CThrowingObject::CThrowingObject(const CThrowingObject & rhs)
	: CInteractObject(rhs)
	, m_tThrowDesc(rhs.m_tThrowDesc)
{
}

HRESULT CThrowingObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CThrowingObject::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_tThrowDesc, pArg, sizeof(THROWDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_tThrowDesc.tNonDesc.vScale.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_tThrowDesc.tNonDesc.vScale.y);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, m_tThrowDesc.tNonDesc.vScale.z);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&m_tThrowDesc.tNonDesc.vPosition), 1.f));

	m_vStartPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float fPlayerY = XMVectorGetY((CPlayerManager::Get_Instance()->Get_ActivePlayer())->Get_TransformState(CTransform::STATE_TRANSLATION));
	m_vTargetPos = XMVectorSet(_float(rand() % 23) + 6.f, fPlayerY, _float(rand() % 14) + 25.f, 1.f);
	
	m_iTurnAxis = rand() % 3;

	return S_OK;
}

int CThrowingObject::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;

	__super::Tick(fTimeDelta);

	switch (m_iTurnAxis)
	{
	case 0:
		m_vTurnAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);
		break;
	case 1:
		m_vTurnAxis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		break;
	case 2:
		m_vTurnAxis = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		break;
	}

	m_pTransformCom->Turn(m_vTurnAxis, fTimeDelta * 5.f);

	_float fMaxTime = 1.f;
	_float fEndHeight = XMVectorGetY(m_vTargetPos - m_vStartPos);
	_float fHeight = 7.f - XMVectorGetY(m_vStartPos);
	_float fGravity = 2.f * fHeight / (fMaxTime * fMaxTime);

	_float fVelocityY = sqrtf(2.f * fGravity * fHeight);

	_float fEndTime = ((2.f * fVelocityY) + sqrtf(4.f * fVelocityY * fVelocityY - 8.f * fGravity * fEndHeight)) / (2 * fGravity);

	_float fVelocityX = (XMVectorGetX(m_vStartPos - m_vTargetPos) * (-1.f)) / fEndTime;
	_float fVelocityZ = (XMVectorGetZ(m_vStartPos - m_vTargetPos) * (-1.f)) / fEndTime;

	m_fTime += fTimeDelta * m_tThrowDesc.fTime;

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(XMVectorGetX(m_vStartPos) + (fVelocityX * m_fTime), XMVectorGetY(m_vStartPos) + (fVelocityY * m_fTime) - (0.5f * fGravity * m_fTime * m_fTime), XMVectorGetZ(m_vStartPos) + (fVelocityZ * m_fTime), 1.f));
	
	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CThrowingObject::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	__super::Late_Tick(fTimeDelta);

	if (m_bDown && (XMVectorGetY(m_vTargetPos) > XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION))))
		Set_Dead(true);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EDGE_DETECTION, this);
}

HRESULT CThrowingObject::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CThrowingObject::Render_EdgeDetection()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 7)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CThrowingObject::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_tThrowDesc.tNonDesc.pModeltag, (int)strlen(m_tThrowDesc.tNonDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

_bool CThrowingObject::Is_AnimationLoop(_uint eAnimId)
{
	return false;
}

HRESULT CThrowingObject::SetUp_ShaderID()
{
	//if (false == m_bIsGain)
		m_eShaderID = SHADER_NONANIMDEFAULT;
	/*else
		m_eShaderID = SHADER_NONANIM_DISSOLVE;*/

	return S_OK;
}

CThrowingObject * CThrowingObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CThrowingObject* pInstance = new CThrowingObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CThrowingObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CThrowingObject::Clone(void * pArg)
{
	CThrowingObject* pInstance = new CThrowingObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CThrowingObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CThrowingObject::Free(void)
{
	__super::Free();
}
