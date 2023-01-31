#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
	ZeroMemory(&m_tPivot, sizeof(PIVOT));
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
	, m_tPivot(rhs.m_tPivot)
{
}

HRESULT CMonster::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMonster::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
		memcpy(&m_tPivot, pArg, sizeof(PIVOT));
	
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(m_tPivot.vScale);
	m_pTransformCom->Set_Rotation(m_tPivot.vRotation);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_tPivot.vPosition);
	
	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform* pPlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	RELEASE_INSTANCE(CGameInstance);

	if (nullptr != pPlayerTransform)
	{
		_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) -
			vPlayerPos));

		if (5.f > fDistance)
		{
			m_pTransformCom->LookAt(vPlayerPos);

			if (m_isAttack)
				m_pModelCom->Set_CurrentAnimIndex(3);
			else
			{
				m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
				m_pModelCom->Set_CurrentAnimIndex(1);
			}
		}
		else
		{
			m_pModelCom->Set_CurrentAnimIndex(0);
		}
	}
	else
		m_pModelCom->Set_CurrentAnimIndex(0);
	

	m_pModelCom->Play_Animation(fTimeDelta);

	m_pAABBCom->Tick(m_pTransformCom->Get_WorldMatrix());
	m_pSphereCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCollider* pTargetCollider = (CCollider*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_AABB"));
	if (nullptr != pTargetCollider)
	{
		m_pAABBCom->Collision(pTargetCollider);
		if (m_pSphereCom->Collision(pTargetCollider))
		{
			m_isAttack = true;
			if (m_pModelCom->Get_Finished(3))
				m_pModelCom->Reset_NonLoop(3);
		}
		else
		{
			if (m_pModelCom->Get_Finished(3))
			{
				m_isAttack = false;
				m_pModelCom->Reset_NonLoop(3);
			}
		}
	}
		
	RELEASE_INSTANCE(CGameInstance);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CMonster::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
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

#ifdef _DEBUG
	m_pAABBCom->Render();
	m_pSphereCom->Render();

	m_pNavigationCom->Render_Navigation();
#endif // _DEBUG

	return S_OK;
}

HRESULT CMonster::Ready_Component(void)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	CModel::ANIMDESC AnimDesc;
	ZeroMemory(&AnimDesc, sizeof(CModel::ANIMDESC));

	AnimDesc.iArraySize = 1;
	AnimDesc.pNonLoopArray = new _int[AnimDesc.iArraySize];
	AnimDesc.pNonLoopArray[0] = 3;
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LeeBongGoo"), (CComponent**)&m_pModelCom, &AnimDesc)))
		return E_FAIL;

	/* For.Com_AABB */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	ColliderDesc.vScale = _float3(0.5f, 1.4f, 0.5f);
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Sphere */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_Sphere"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC NaviDesc;
	ZeroMemory(&NaviDesc, sizeof(CNavigation::NAVIDESC));

	NaviDesc.iCurrentCellIndex = 0;

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::SetUp_ShaderResources(void)
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

CMonster * CMonster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMonster* pInstance = new CMonster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonster::Clone(void * pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free(void)
{
	__super::Free();

	Safe_Release(m_pSphereCom);
	Safe_Release(m_pAABBCom);

	Safe_Release(m_pNavigationCom);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
