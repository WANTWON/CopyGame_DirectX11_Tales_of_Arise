#include "stdafx.h"
#include "..\Public\Bullet.h"
#include "Monster.h"
#include "Player.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBaseObj(pDevice, pContext)
{
}

CBullet::CBullet(const CBullet & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void * pArg)
{
	memcpy(&m_BulletDesc, pArg, sizeof(BULLETDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if(m_BulletDesc.eCollisionGroup == PLAYER)
		CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, this);
	else if (m_BulletDesc.eCollisionGroup == MONSTER)
		CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, this);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, m_BulletDesc.vInitPositon);

	return S_OK;
}

int CBullet::Tick(_float fTimeDelta)
{
	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		if (pCamera->Get_CamMode() == CCamera_Dynamic::CAM_BATTLE_CLEAR)
			return OBJ_DEAD;
	}

	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
		return OBJ_DEAD;
	return OBJ_NOEVENT;
}

void CBullet::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	if (m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		__super::Late_Tick(fTimeDelta);
	}

	m_fTime += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
	if (m_fTime >= m_BulletDesc.fDeadTime)
		m_bDeadEffect = true;

	Collision_Check();
}

HRESULT CBullet::Render()
{
	if (!m_pShaderCom || !m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	if (FAILED(SetUp_ShaderID()))
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


HRESULT CBullet::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f * m_BulletDesc.fVelocity;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet::SetUp_ShaderResources()
{
	if (!m_pShaderCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CBullet::SetUp_ShaderID()
{
	return S_OK;
}

void CBullet::Collision_Check()
{
	CBaseObj* pCollisionTarget = nullptr;

	if (m_BulletDesc.eCollisionGroup == PLAYER)
	{
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
		{
			dynamic_cast<CMonster*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
			m_bDead = true;
		}
	}
	else
	{
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionTarget))
		{
			if (!dynamic_cast<CPlayer*>(pCollisionTarget)->Get_IsJustDodge())
			{
				dynamic_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner);
				m_bDead = true;
			}
		}
	}
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);

	for (auto& iter : m_pEffects)
	{
		if(iter != nullptr)
			iter->Set_Dead(true);
	}
	m_pEffects.clear();

	for (auto& iter : m_pDeadEffects)
	{
		if (iter != nullptr)
			iter->Set_Dead(true);
	}
	m_pDeadEffects.clear();

	if (m_BulletDesc.eCollisionGroup == PLAYER)
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, this);
	else if (m_BulletDesc.eCollisionGroup == MONSTER)
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, this);

}
