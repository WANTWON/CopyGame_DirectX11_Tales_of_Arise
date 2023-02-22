#include "stdafx.h"
#include "..\Public\Anim.h"

#include "GameInstance.h"
#include "Weapon.h"
#include "Imgui_Manager.h"

CAnim::CAnim(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CAnim::CAnim(const CAnim & rhs)
	: CBaseObj(rhs)
{
}

void CAnim::Set_AnimIndex(_int iIndex)
{
	if (nullptr != m_pModelCom)
		m_pModelCom->Set_CurrentAnimIndex(iIndex);
}

void CAnim::PlayAnimation(void)
{
	if (nullptr != m_pModelCom)
		m_pModelCom->Play_Animation(0.f);
}

HRESULT CAnim::Ready_Parts(const char * pBoneName, const _tchar * pModelTag)
{
	/* For.Handle */
	CHierarchyNode* pSocket = m_pModelCom->Get_BonePtr(pBoneName);
	if (nullptr == pSocket)
		return E_FAIL;

	Safe_AddRef(pSocket);

	CWeapon::HANDLEDESC WeaponDesc;
	ZeroMemory(&WeaponDesc, sizeof(CWeapon::HANDLEDESC));

	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	WeaponDesc.pModelTag = pModelTag;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_Parts[PARTS_WEAPON] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_PlayerHandle"), &WeaponDesc);
	if (nullptr == m_Parts[PARTS_WEAPON])
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CAnim::Change_Bone(const char * pBoneName)
{
	if (nullptr != m_Parts[PARTS_WEAPON])
	{
		/* For.Handle */
		CHierarchyNode* pSocket = m_pModelCom->Get_BonePtr(pBoneName);
		if (nullptr == pSocket)
			return E_FAIL;

		Safe_AddRef(pSocket);

		CWeapon::HANDLEDESC WeaponDesc;
		ZeroMemory(&WeaponDesc, sizeof(CWeapon::HANDLEDESC));

		WeaponDesc.pSocket = pSocket;
		WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
		WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
		WeaponDesc.pModelTag = ((CWeapon*)m_Parts[PARTS_WEAPON])->Get_HandleDesc().pModelTag;

		((CWeapon*)m_Parts[PARTS_WEAPON])->Change_HandleDesc(WeaponDesc);
	}

	return S_OK;
}

HRESULT CAnim::Initialize_Prototype(void)
{
	return S_OK;
}

HRESULT CAnim::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_ModelDesc, pArg, sizeof(PLAYERDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_Parts.resize(PARTS_END);
	/*if (FAILED(Ready_Parts()))
	return E_FAIL;*/

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_ModelDesc.vScale.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_ModelDesc.vScale.y);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, m_ModelDesc.vScale.z);

	m_pTransformCom->Set_Rotation(m_ModelDesc.vRotation);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&m_ModelDesc.vPosition), 1.f));

	m_pModelCom->Set_CurrentAnimIndex(0);
	m_eShaderID = 0;
	m_pModelCom->Play_Animation(0.1f);

	return S_OK;
}

int CAnim::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;
	
	
	if (m_isPlayAnim)
		m_bisFinshed = m_pModelCom->Play_Animation(fTimeDelta, true);



	for (auto& pParts : m_Parts)
	{
		if (nullptr != pParts)
			pParts->Tick(fTimeDelta);
	}

	return OBJ_NOEVENT;
}

void CAnim::Late_Tick(_float fTimeDelta)
{
	for (auto& pParts : m_Parts)
	{
		if (nullptr != pParts)
			pParts->Late_Tick(fTimeDelta);
	}

	if (nullptr != m_pRendererCom && !CImgui_Manager::Get_Instance()->Get_ShowOnlyNavi())
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		if (nullptr != m_Parts[PARTS_WEAPON])
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_Parts[PARTS_WEAPON]);
	}
}

HRESULT CAnim::Render(void)
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;
		
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CAnim::Ready_Components(void * pArg)
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
	switch (m_ModelDesc.eModel)
	{
	case MODEL_ALPHEN:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Alphen"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case MODEL_ICEWOLF:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Ice_Wolf"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case MODEL_SION:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Sion"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_BERSERKER:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Berserker"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_HAWK:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Hawk"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_ASTRAL_DOUBT:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Astral_Doubt"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_RINWELL:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Rinwell"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;

	case MODEL_LAW:
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Law"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CAnim::SetUp_ShaderResources(void)
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

HRESULT CAnim::SetUp_ShaderID(void)
{
	if (m_bDead)
		m_eShaderID = SHADER_PICKED;
	else
		m_eShaderID = SHADER_DEFAULT;

	return S_OK;
}

CAnim * CAnim::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAnim* pInstance = new CAnim(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CAnim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAnim::Clone(void * pArg)
{
	CAnim* pInstance = new CAnim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CAnim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnim::Free(void)
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
