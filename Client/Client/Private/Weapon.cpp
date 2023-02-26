#include "stdafx.h"
#include "..\Public\Weapon.h"

#include "GameInstance.h"
#include "Object_Pool_Manager.h"
#include "Collision_Manger.h"
#include "Monster.h"
#include "Player.h"
#include "PlayerState.h"

CWeapon::CWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CWeapon::CWeapon(const CWeapon & rhs)
	: CBaseObj(rhs)
{
}

void CWeapon::Set_WeaponDesc(WEAPONDESC tWeaponDesc)
{
	Safe_Release(m_WeaponDesc.pSocket);
	m_WeaponDesc.pSocket = nullptr;

	memcpy(&m_WeaponDesc, &tWeaponDesc, sizeof(WEAPONDESC));
}

void CWeapon::Collision_Immediate()
{

}

HRESULT CWeapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_WeaponDesc, pArg, sizeof(WEAPONDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, .1f);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, .1f);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, .1f);

	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));

	return S_OK;
}

int CWeapon::Tick(_float fTimeDelta)
{
	_matrix		SocketMatrix = XMLoadFloat4x4(&m_WeaponDesc.RotationCorrectionMatrix) *
		m_WeaponDesc.pSocket->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_WeaponDesc.TranslationCorrectionMatrix) *
		XMLoadFloat4x4(&m_WeaponDesc.SocketPivotMatrix) * XMLoadFloat4x4(m_WeaponDesc.pParentWorldMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_CombinedWorldMatrix, SocketMatrix);

	if (m_isCollider)
	{
		if (nullptr == m_pSPHERECom)
		{
			CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

			CCollider::COLLIDERDESC		ColliderDesc;

			ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
			ColliderDesc.vPosition = _float3(0.f, 0.f, -3.f);

			m_pSPHERECom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
			m_pSPHERECom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));
			pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, this);

			//if (!m_bSoundStart)
			//{
			//	CGameInstance::Get_Instance()->PlaySounds(TEXT("StrikeSound.wav"), SOUND_EFFECT, 0.2f);
			//	m_bSoundStart = true;
			//}
		}
		else
			m_pSPHERECom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));
	}
	else if (nullptr != m_pSPHERECom && !m_isCollider)
	{
		CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

		pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pSPHERECom);
		m_pSPHERECom = nullptr;
	}
	//m_bSoundStart = false;


	return OBJ_NOEVENT;
}

void CWeapon::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pSPHERECom)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);

		CPlayerState* pPlayerState = pPlayer->Get_State();
		CPlayerState::STATE_ID ePlayerState = pPlayerState->Get_StateId();

		CBaseObj* pCollisionTarget = nullptr;

		m_fSoundStopTimeDelta += fTimeDelta;

		//if ((m_fSoundStopTimeDelta > 1.5f))
		//{
		//	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
		//	m_fSoundStopTimeDelta = 0.f;
		//}

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionTarget))
		{

			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
			{
				pCollided->Take_Damage(rand() % 100, m_WeaponDesc.pOwner);
			}

			//if (!m_bSoundStart)
			//{
			//	CGameInstance::Get_Instance()->PlaySounds(TEXT("StrikeSound.wav"), SOUND_EFFECT, 0.4f);
			//	m_bSoundStart = true;
			//}

			////SkillSound 
			//if (ePlayerState == CPlayerState::STATE_ID::STATE_SKILL_ATTACK_E)
			//{
			//	m_fTimeDeltaAcc += 0.4f;
			//	_bool bSoundStart = false;
			//	if (!bSoundStart)
			//	{
			//		CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_E.wav"), SOUND_EFFECT, 0.2f);
			//		bSoundStart = true;
			//	}
			//}
		}

		/*else
		{
			if (ePlayerState == CPlayerState::STATE_ID::STATE_SKILL_ATTACK_E)
			{
				m_fTimeDeltaAcc += 0.4f;
				_bool bSoundStart = false;
				if (!bSoundStart)
				{
					if (m_fTimeDeltaAcc > 0.04f)
					{
						CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
						m_fTimeDeltaAcc = 0.f;
					}
					CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_E.wav"), SOUND_EFFECT, 0.2f);
					bSoundStart = true;
				}
			}
		}*/
	}

	else
		m_bSoundStart = false;

	if (nullptr != m_pSPHERECom && !m_isCollider)
	{
		CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

		pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pSPHERECom);
		m_bSoundStart = false;
		m_pSPHERECom = nullptr;
		pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, this);
	}

#ifdef _DEBUG
	if (nullptr != m_pSPHERECom)
		m_pRendererCom->Add_Debug(m_pSPHERECom);
#endif

}

HRESULT CWeapon::Render()
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

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CWeapon::Render_EdgeDetection()
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

HRESULT CWeapon::Ready_Components(void* pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar			szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_WeaponDesc.pModeltag, (int)strlen(m_WeaponDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;



	return S_OK;
}

HRESULT CWeapon::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_CombinedWorldMatrix)));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &WorldMatrix, sizeof(_float4x4))))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CWeapon * CWeapon::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CWeapon*	pInstance = new CWeapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CWeapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CWeapon::Clone(void * pArg)
{
	CWeapon*	pInstance = new CWeapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CWeapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon::Free()
{
	__super::Free();


	Safe_Release(m_WeaponDesc.pSocket);

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSPHERECom);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
