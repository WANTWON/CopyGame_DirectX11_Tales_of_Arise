#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
#include "PlayerHandle.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	ZeroMemory(&m_tPivot, sizeof(PIVOT));
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
	, m_tPivot(rhs.m_tPivot)
{
}

void CPlayer::Set_AnimIndex(_int iIndex)
{
	if (nullptr != m_pModelCom)
		m_pModelCom->Set_AnimIndex(iIndex);
}

void CPlayer::PlayAnimation(void)
{
	m_pModelCom->Play_Animation(0.f);
}

HRESULT CPlayer::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_tPivot, pArg, sizeof(PLAYERDESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_Parts.resize(PARTS_END);
	/*if (FAILED(Ready_Parts()))
		return E_FAIL;*/

	m_pTransformCom->Set_Scale(m_tPivot.tPivot.vScale);
	m_pTransformCom->Set_Rotation(m_tPivot.tPivot.vRotation);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_tPivot.tPivot.vPosition);

	m_pModelCom->Set_CurrentAnimIndex(0);

	m_pModelCom->Play_Animation(0.1f);
	
	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	vector<CAnimation::EVENTTYPE> pEvents;

	if (m_isPlayAnim)
		m_pModelCom->Play_Animation(fTimeDelta, &pEvents);	

	for (auto& pParts : m_Parts)
	{
		if (nullptr != pParts)
			pParts->Tick(fTimeDelta);
	}

	/*for (auto& pEvent : pEvents)
	{
		if (CAnimation::EVENT_COLLIDER == pEvent)
			((CPlayerHandle*)m_Parts[PARTS_WEAPON])->Collision();
	}*/
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	for (auto& pParts : m_Parts)
	{
		if (nullptr != pParts)
			pParts->Late_Tick(fTimeDelta);
	}

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		if (nullptr != m_Parts[PARTS_WEAPON])
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_Parts[PARTS_WEAPON]);
	}
}

HRESULT CPlayer::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (nullptr == m_pModelCom)
		return E_FAIL;
	
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	
	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer::Ready_Parts(const char* pBoneName, const _tchar* pModelTag)
{
	/* For.Handle */
	CHierarchyNode* pSocket = m_pModelCom->Get_BonePtr(pBoneName);
	if (nullptr == pSocket)
		return E_FAIL;

	Safe_AddRef(pSocket);

	CPlayerHandle::HANDLEDESC HandleDesc;
	ZeroMemory(&HandleDesc, sizeof(CPlayerHandle::HANDLEDESC));

	HandleDesc.pSocket = pSocket;
	HandleDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	HandleDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	HandleDesc.pModelTag = pModelTag;
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_Parts[PARTS_WEAPON] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_PlayerHandle"), &HandleDesc);
	if (nullptr == m_Parts[PARTS_WEAPON])
		return E_FAIL;	
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Change_Bone(const char * pBoneName)
{
	if (nullptr != m_Parts[PARTS_WEAPON])
	{
		/* For.Handle */
		CHierarchyNode* pSocket = m_pModelCom->Get_BonePtr(pBoneName);
		if (nullptr == pSocket)
			return E_FAIL;

		Safe_AddRef(pSocket);

		CPlayerHandle::HANDLEDESC HandleDesc;
		ZeroMemory(&HandleDesc, sizeof(CPlayerHandle::HANDLEDESC));

		HandleDesc.pSocket = pSocket;
		HandleDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
		HandleDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
		HandleDesc.pModelTag = ((CPlayerHandle*)m_Parts[PARTS_WEAPON])->Get_HandleDesc().pModelTag;

		((CPlayerHandle*)m_Parts[PARTS_WEAPON])->Change_HandleDesc(HandleDesc);
	}

	return S_OK;
}

HRESULT CPlayer::Ready_Components(void)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	switch (m_tPivot.eModel)
	{
	case MODEL_ALPHEN:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Alphen"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case MODEL_ICEWOLF:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_IceWolf"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case MODEL_SION:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Sion"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_BERSERKER:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Berserker"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_HAWK:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Hawk"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_ASTRAL_DOUBT:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Astral_Doubt"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_RINWELL:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rinwell"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_LAW:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Law"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_ASU_005:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ASU_005"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_CURRENT_ASU:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Current_Astral_Doubt"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	}



	
	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResources(void)
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free(void)
{
	__super::Free();
	
	for (auto& iter : m_Parts)
		Safe_Release(iter);
	m_Parts.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
