#include "stdafx.h"
#include "..\Public\BladeTrap.h"
#include "GameInstance.h"
#include "Player.h"
#include "Cell.h"

CBladeTrap::CBladeTrap(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

HRESULT CBladeTrap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBladeTrap::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_eObjectID = OBJID::OBJ_BLOCK;
	//Set_Scale(_float3(0.5, 0.5, 0.5));

	_vector vecPostion = XMLoadFloat3((_float3*)pArg);
	vecPostion = XMVectorSetW(vecPostion, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);

	m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_TRAP, this);
	return S_OK;
}

int CBladeTrap::Tick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_vector vPosition = Get_TransformState(CTransform::STATE_POSITION);

	if (pGameInstance->isIn_WorldFrustum(vPosition) == false)
	{
		RELEASE_INSTANCE(CGameInstance);
		return OBJ_NOEVENT;
	}

	if (!m_bStart)
	{
		CBaseObj* pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		_vector vDir = pTarget->Get_TransformState(CTransform::STATE_POSITION)- vPosition;
		m_vDir = XMVectorSetX(m_vDir, XMVectorGetX(vDir));
		_float fDistance = XMVectorGetX(XMVector3Length(vDir));

		if (fDistance < 10.f)
			m_bStart = true;
	}
	else
	{
		if (m_iMoveCount % 3 == 0)
		{
			m_dwAttackTime = GetTickCount();
			m_iMoveCount++;
			m_bMove = false;
		}
			

		if (m_dwAttackTime + 2000 < GetTickCount() )
		{
			m_eState = TURN;
			
		}
			
	}

	
	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
	}

	Change_Animation(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);
	return OBJ_NOEVENT;
}

void CBladeTrap::Late_Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->isIn_WorldFrustum(Get_TransformState(CTransform::STATE_POSITION)) == false)
		return;

	if (nullptr != m_pRendererCom)
	{
		__super::Late_Tick(fTimeDelta);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}
	SetUp_ShaderID();

	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::DROP ||
		CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_TRAP, m_pSPHERECom))
	{
		m_vDir *= -1.f;
		m_iMoveCount++;

		_uint iNum = rand() % 3 + 1;
		_tchar	sz_Sound[MAX_PATH];
		_float fVolume = 0.4f;
		wcscpy_s(sz_Sound, TEXT("5_Obj_Trap_Collision (%d).wav"));
		wsprintf(sz_Sound, sz_Sound, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_OBJECT, fVolume);

	}
}

HRESULT CBladeTrap::Render()
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

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
			return E_FAIL;


		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}



	return S_OK;
}

void CBladeTrap::Change_Animation(_float fTimeDelta)
{

	switch (m_eState)
	{
	case Client::CBladeTrap::TURN:
		if (!m_bMove)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Trap_MoveStart.wav"), SOUND_OBJECT, 0.4f);
			m_bMove = true;
		}
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta * 2, m_bIsLoop);
		if (m_pTransformCom->Go_PosDir(fTimeDelta, m_vDir, m_pNavigationCom) == false)
		{
			m_eState = IDLE;
			m_vDir *= -1.f;
			m_iMoveCount++;

			_uint iNum = rand() % 3 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.4f;
			wcscpy_s(sz_Sound, TEXT("5_Obj_Trap_Collision (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_OBJECT, fVolume);
		}
		break;
	case Client::CBladeTrap::IDLE:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta * 2, m_bIsLoop);
		break;
	default:
		break;
	}
}

HRESULT CBladeTrap::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 4.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_BladeTrap"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_SHPERE */
	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iCurrentCellIndex = 0;
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_TailCave"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TailCave"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBladeTrap::SetUp_ShaderResources()
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



CBladeTrap * CBladeTrap::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBladeTrap*	pInstance = new CBladeTrap(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CBladeTrap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBladeTrap::Clone(void * pArg)
{
	CBladeTrap*	pInstance = new CBladeTrap(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CBladeTrap"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBladeTrap::Free()
{
	__super::Free();


	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);
}
