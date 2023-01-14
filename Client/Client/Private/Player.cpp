#include "stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"
#include "Weapon.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

//	if (FAILED(Ready_Parts()))
//		return E_FAIL;

	m_eState = 0;
	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));


	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//	char cName[MAX_PATH];
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("Alphen"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	//	RELEASE_INSTANCE(CData_Manager);

	//RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

int CPlayer::Tick(_float fTimeDelta)
{

	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_RIGHT))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	}
	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LEFT))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f,1.f,0.f,0.f), -fTimeDelta);
	}
	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_DOWN))
	{
		m_pTransformCom->Sliding_Backward(fTimeDelta, m_pNavigationCom, 0.f);
	}

	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_UP))
	{
		m_pTransformCom->Sliding_Straight(fTimeDelta, m_pNavigationCom, 0.f);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_3))
	{
		m_eState++;
		if (m_eState >= 114)
			m_eState = 0;
	}
	if (CGameInstance::Get_Instance()->Key_Up(DIK_4))
	{
		m_eState--;
		if (m_eState <= 0)
			m_eState = 114;
	}
		
		

	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
	}

	m_pModelCom->Play_Animation(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

//	for (auto& pParts : m_Parts)
//		pParts->Tick(fTimeDelta);

	return OBJ_NOEVENT;
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
//	for (auto& pParts : m_Parts)
//		pParts->Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_Parts[PARTS_WEAPON]);
		if (m_pNavigationCom != nullptr)
			m_pRendererCom->Add_Debug(m_pNavigationCom);
		__super::Late_Tick(fTimeDelta);
		
	}

	Check_Navigation();
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CPlayer::Ready_Parts()
{
	m_Parts.resize(PARTS_END);

	/* For.Weapon */
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("SWORD");
	if (nullptr == pSocket)
		return E_FAIL;

	CWeapon::WEAPONDESC		WeaponDesc;
	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_Parts[PARTS_WEAPON] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Weapon"), &WeaponDesc);
	if (nullptr == m_Parts[PARTS_WEAPON])
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayer::Ready_Components(void* pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Alphen"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_SPHERE */
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigaitions.push_back(m_pNavigationCom);

	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResources()
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

	return S_OK;
}

void CPlayer::Change_Navigation(LEVEL eLevel)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (eLevel)
	{
	case Client::LEVEL_GAMEPLAY:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Navigation")));
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);


		if (m_fWalkingHeight > XMVectorGetY(vPosition))
		{
			vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
}

void CPlayer::Compute_CurrentIndex(LEVEL eLevel)
{
	m_pNavigationCom->Compute_CurrentIndex_byXZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer*	pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_Parts)
		Safe_Release(pGameObject);

	m_Parts.clear();

	for (auto& iter : m_vecNavigaitions)
		Safe_Release(iter);
	m_vecNavigaitions.clear();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
}
