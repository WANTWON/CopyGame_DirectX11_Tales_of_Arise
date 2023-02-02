#include "stdafx.h"
#include "..\Public\Monster.h"
#include "Player.h"
#include "Level_Manager.h"
#include "CameraManager.h"
#include "BattleManager.h"
#include "Effect.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBaseObj(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
	return S_OK;
}

int CMonster::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	if (m_bTakeDamage)
		m_fTime_TakeDamageDeltaAcc += fTimeDelta;

	if (3 <= m_fTime_TakeDamageDeltaAcc)
	{
		m_bTakeDamage = false;
		m_fTime_TakeDamageDeltaAcc = 0.f;
	}

	m_fTimeDletaAcc += fTimeDelta;



	return OBJ_NOEVENT;
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;
	__super::Late_Tick(fTimeDelta);

	if (Check_IsinFrustum(2.f) == false)
		return;

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_B) && false == m_bTakeDamage)
	{
		Take_Damage(20, m_pTarget);
		m_bTakeDamage = true;
	}

	if (m_bDissolve)
	{
		m_DissolveAlpha += fTimeDelta*m_fDissolveOffset;

		if (1 < m_DissolveAlpha)
		{
			m_DissolveAlpha = 1.f;
			m_bDead = true;
		}
	}


	m_eCurLevel = CGameInstance::Get_Instance()->Get_CurrentLevelIndex();

	if (LEVEL_BATTLE == m_eCurLevel)
		m_bBattleMode = true;

	if (LEVEL_SNOWFIELD == m_eCurLevel)
	{
		CCollider* pPlayerCollider = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Collider();
		if (m_pSPHERECom->Collision(pPlayerCollider))
		{
			CBattleManager::Get_Instance()->Set_BattleMode(true, m_eMonsterID);
		}
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_9))
		m_bDead = true;


	CBaseObj* pCollisionMonster = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionMonster))
	{

		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - pCollisionMonster->Get_TransformState(CTransform::STATE_TRANSLATION);
		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
		m_pTransformCom->Go_PosDir(fTimeDelta, vDirection, m_pNavigationCom);
	}

}

HRESULT CMonster::Render()
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

HRESULT CMonster::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_ShadowLightView(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, SHADER_ANIMSHADOW)))
			return S_OK;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


CMonster::DMG_DIR CMonster::Calculate_DmgDirection()
{
	if (m_pTarget == nullptr)
		return FRONT;

	// New Logic
	_matrix matWorld = m_pTransformCom->Get_WorldMatrix();
	_matrix matTargetWorld = dynamic_cast<CPlayer*>(m_pTarget)->Get_Transform()->Get_WorldMatrix();


	_vector fLookDot = XMVector3Dot(matWorld.r[2], matTargetWorld.r[2]);
	_vector fRightDot = XMVector3Dot(matWorld.r[0], matTargetWorld.r[0]);
	_vector fLookRightDot = XMVector3Dot(matWorld.r[2], matTargetWorld.r[0]);

	cout << XMVectorGetX(fLookDot) << endl;
	if (XMVectorGetX(fLookDot) >= cos(45))
	{
		cout << "BACK" << endl;
		
		m_eDmg_Direction = BACK;
	}
	else if (XMVectorGetX(fLookDot) < cos(135))
	{
		cout << "FRONT" << endl;
		m_eDmg_Direction = FRONT;
	}
	else if (XMVectorGetX(fLookRightDot) >= cos(45))
	{
		cout << "RIGHT" << endl;
		m_eDmg_Direction = RIGHT;
	}
	else //if (XMVectorGetX(fLookRightDot) < cos(135))
	{
		cout << "LEFT" << endl;
		m_eDmg_Direction = LEFT;
	}
		
	


	return m_eDmg_Direction;
}

_vector CMonster::Calculate_DirectionByPos()
{
	if (m_pTarget == nullptr)
		return _vector();

	// New Logic
	_vector vMyPos = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	_vector vTargetPos = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pTarget)->Get_TransformState(CTransform::STATE_TRANSLATION));

	return vTargetPos - vMyPos;
}

_bool CMonster::Check_AmILastMoster()
{
	list<CGameObject*>* pMonsterList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_BATTLE, TEXT("Layer_Monster"));

	if (pMonsterList->size() == 1)
	{
		if (pMonsterList->front() == this && m_bDissolve)
		{
			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_CamMode(CCamera_Dynamic::CAM_BATTLE_CLEAR);
			pCamera->Set_TargetPosition(Get_TransformState(CTransform::STATE_TRANSLATION));
			m_fDissolveOffset = 0.1f;
			return true;
		}
			
	}
	return false;
}

void CMonster::Find_Target()
{
	if (!m_bDead)
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();

		if (pPlayer)
		{
			if (pPlayer->Get_Dead())
			{
				m_pTarget = nullptr;
				m_bAggro = false;
				return;
			}

			_vector vTargetPos = pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fDistanceToTarget = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - vTargetPos));
			m_pTarget = pPlayer;
		}
	}
}

CBaseObj* CMonster::Find_MinDistance_Target()
{
	list<CGameObject*>* pPlayerList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Player"));

	for (auto& iter : *pPlayerList)
	{
		_float fDistance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_TRANSLATION)));
		if (m_fMinLengh > fDistance)
		{
			m_fMinLengh = fDistance;
			m_pTarget = dynamic_cast<CBaseObj*>(iter);
		}
	}
	
	return m_pTarget;
}

_float  CMonster::Target_Distance(CBaseObj* pTarget)
{

	_float fDistance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - pTarget->Get_TransformState(CTransform::STATE_TRANSLATION)));

	return fDistance;
}

HRESULT CMonster::Drop_Items()
{
	int iRadomItem = rand() % 3;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	//Setting Item Struct
	//And Add GameObject Item

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CMonster::Make_GetAttacked_Effect(CBaseObj* DamageCauser)
{
	/*if (m_bMakeEffect)
		return;*/

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CBaseObj* pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	_vector vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
	_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
	CEffect::PlayEffectAtLocation(TEXT("Monster_Hit.dat"), vLocation);

	/*m_bMakeEffect = true;*/

	RELEASE_INSTANCE(CGameInstance);
}

void CMonster::Make_DeadEffect(CBaseObj * Target)
{
	if (m_bMakeEffect)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//Setting Effect Struct
	//And Add GameObject Effect

	RELEASE_INSTANCE(CGameInstance);

	m_bMakeEffect = true;
}


_int CMonster::Take_Damage(int fDamage, CBaseObj * DamageCauser)
{
	if (fDamage <= 0 || m_bDead)
		return 0;

	m_DamageCauser = DamageCauser;

	m_tStats.m_fCurrentHp-= (int)fDamage;

	if (m_tStats.m_fCurrentHp <= 0)
	{
		m_tStats.m_fCurrentHp = 0;
		return _int(m_tStats.m_fCurrentHp);

	}

	CBattleManager::Get_Instance()->Set_LackonMonster(this);
	m_bHit = true;
	m_dwHitTime = GetTickCount();

	Make_GetAttacked_Effect(DamageCauser);

	return _int(m_tStats.m_fCurrentHp);

}

void CMonster::Compute_CurrentIndex()
{
	m_pNavigationCom->Compute_CurrentIndex_byXZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
}

void CMonster::Check_NearTrigger()
{
	list<CGameObject*>* TriggerLIst = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_SNOWFIELD, TEXT("Layer_Trigger"));
	if (TriggerLIst == nullptr)
		return;

	for (auto& iter : *TriggerLIst)
	{
		_vector vTriggerPos = dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_TRANSLATION);
		_float fDIstance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_TRANSLATION) - vTriggerPos));

		if (m_fMinLengh > fDIstance)
		{
			m_fMinLengh = fDIstance;
			m_pTrigger = dynamic_cast<CBaseObj*>(iter);
		}
	}
}


HRESULT CMonster::SetUp_ShaderResources()
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

	if (m_pDissolveTexture != nullptr)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DissolveTexture", m_pDissolveTexture->Get_SRV(0))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fDissolveAlpha", &m_DissolveAlpha, sizeof(_float))))
			return E_FAIL;

	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster::SetUp_ShaderID()
{

	return S_OK;
}


void CMonster::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);

	Safe_Release(m_pDissolveTexture);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pModelCom);


}