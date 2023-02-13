#include "stdafx.h"

#include "Monster.h"
#include "Level_Manager.h"
#include "CameraManager.h"
#include "BattleManager.h"
#include "Player.h"
#include "DamageFont.h"
#include "Effect.h"
#include "ParticleSystem.h"
#include "UI_font_Hits_number.h"
#include "UI_Combo_font_Hits.h"
#include "UI_Combo_font_Damages.h"
#include "UI_font_Damage_number.h"
#include "UI_Comboline.h"
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

	if(CGameInstance::Get_Instance()->Get_DestinationLevelIndex() == LEVEL_BATTLE)
		CBattleManager::Get_Instance()->Add_Monster(this);
	return S_OK;
}

int CMonster::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	if (m_bTakeDamage)
		m_fTime_TakeDamageDeltaAcc += fTimeDelta;

	if (0.2f <= m_fTime_TakeDamageDeltaAcc)
	{
		m_bTakeDamage = false;
		m_fTime_TakeDamageDeltaAcc = 0.f;
	}

	m_fTimeDeltaAcc += fTimeDelta;

	return OBJ_NOEVENT;
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_B) && false == m_bTakeDamage)
	{
		Take_Damage(2000, CPlayerManager::Get_Instance()->Get_ActivePlayer());
		m_bTakeDamage = true;
	}

	if (m_bGlowUp)
	{
		if (m_pDissolveParticles.empty())
		{
			_vector vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
			_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

			_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation;

			m_pDissolveParticles = CEffect::PlayEffectAtLocation(TEXT("Dissolve_Particles.dat"), mWorldMatrix);
		}
			
		if (m_bDissolve)
		{
			if (m_fDissolveTimer > m_fDissolveLifespan)
				m_bDead = true;
			else
				m_fDissolveTimer += fTimeDelta;
		}
		else
		{
			if (m_fGlowUpTimer > m_fGlowUpLifespan)
			{
				if (!m_pDissolveParticles.empty())
				{
					for (auto& pDissolveParticle : m_pDissolveParticles)
					{
						CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(pDissolveParticle);
						if (pDissolveParticle)
							pParticleSystem->Set_Stop(true);
					}
				}

				m_bDissolve = true;
			}
			else
				m_fGlowUpTimer += fTimeDelta;
		}
	}
	else
		Collision_Object(fTimeDelta);

	m_eCurLevel = CGameInstance::Get_Instance()->Get_CurrentLevelIndex();

	if (LEVEL_BATTLE == m_eCurLevel)
		m_bBattleMode = true;

	if (LEVEL_SNOWFIELD == m_eCurLevel && m_eMonsterID != ASTRAL_DOUBT)
	{
		CCollider* pPlayerCollider = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Collider();
		if (m_pSPHERECom->Collision(pPlayerCollider))
		{
			if(m_eMonsterID == RINWELL)
				CBattleManager::Get_Instance()->Set_BattleMode(true, m_eMonsterID, true);
			else
				CBattleManager::Get_Instance()->Set_BattleMode(true, m_eMonsterID);
		}
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

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
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
	if (!m_pTarget)
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
	else
	{
		cout << "LEFT" << endl;
		m_eDmg_Direction = LEFT;
	}

	return m_eDmg_Direction;
}

_vector CMonster::Calculate_DirectionByPos()
{
	if (!m_pTarget)
		return _vector();

	// New Logic
	_vector vMyPos = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
	_vector vTargetPos = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pTarget)->Get_TransformState(CTransform::STATE_TRANSLATION));

	return vTargetPos - vMyPos;
}

_bool CMonster::Check_AmILastMoster()
{
	list<CGameObject*>* pMonsterList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_BATTLE, TEXT("Layer_Monster"));

	_uint iMonsterSize = (_uint)pMonsterList->size();
	_uint iCheckIsDead = 0;

	for (auto& iter : *pMonsterList)
	{
		if (dynamic_cast<CMonster*>(iter)->Get_Stats().m_fCurrentHp <= 0 || iter->Get_Dead())
		{
			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_TargetPosition(Get_TransformState(CTransform::STATE_TRANSLATION));
			iCheckIsDead++;
		}
	}

	if ((iMonsterSize - iCheckIsDead) <= 0)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_TargetPosition(Get_TransformState(CTransform::STATE_TRANSLATION));
		pCamera->Set_CamMode(CCamera_Dynamic::CAM_BATTLE_CLEAR);

		if(CUI_Manager::Get_Instance()->Get_LockOn() != nullptr)
			CUI_Manager::Get_Instance()->Get_LockOn()->Set_Dead(true);
		return true;
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
	
	m_fMinLengh = MAXDISTANCE;
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

	_vector vOffset = XMVectorSet(0.f, m_fRadius + 1.5f, 0.f, 0.f);
	_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

	_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
	mWorldMatrix.r[3] = vLocation;

	CEffect::PlayEffectAtLocation(TEXT("Monster_Hit.dat"), mWorldMatrix);

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

	m_pTarget = DamageCauser;
	m_tStats.m_fCurrentHp-= (int)fDamage;
	

	if (m_pTarget != nullptr)
	{
		if (dynamic_cast<CPlayer*>(m_pTarget)->Get_BoostGuage() <= 100.f)

			dynamic_cast<CPlayer*>(m_pTarget)->Set_BoostGuage(dynamic_cast<CPlayer*>(m_pTarget)->Get_BoostGuage() + 3.f);

	}
	

	CDamageFont::DMGDESC testdesc;
	ZeroMemory(&testdesc, sizeof(CDamageFont::DMGDESC));
	testdesc.iDamage = fDamage;
	testdesc.pPointer = this;
	//dynamic_cast<CUI_Dialoguepopup*>(m_pUI_Manager->Get_Dialoguepopup())->Open_Dialogue(0, false, 1, 0);
	
	

	dynamic_cast<CUI_Combo_font_Hits*>(CUI_Manager::Get_Instance()->Get_Hitfont())->sethit();
	dynamic_cast<CUI_font_Hits_number*>(CUI_Manager::Get_Instance()->Get_HitMsg())->sethit();
	dynamic_cast<CUI_Combo_font_Damages*>(CUI_Manager::Get_Instance()->Get_DMGfont())->updatedmg();
	dynamic_cast<CUI_font_Damage_number*>(CUI_Manager::Get_Instance()->Get_DMGNUM())->updatedamage(fDamage);
	dynamic_cast<CUI_Comboline*>(CUI_Manager::Get_Instance()->Get_Comboline())->setline();


	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("dmg"), &testdesc)))
		return E_FAIL;
	//if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("dmg"), &testdesc)))
	//	return E_FAIL;
	//if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("dmg"), &testdesc)))
	//	return E_FAIL;
	
	
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont_Critical"), LEVEL_STATIC, TEXT("dmg"), &testdesc)))
		return E_FAIL;

	m_tStats.m_fLockonSmashGuage += 0.01f;
	

	if (m_tStats.m_fLockonSmashGuage >= 4.f)
		m_tStats.m_fLockonSmashGuage = 4.f;

	Make_GetAttacked_Effect(DamageCauser);

	if (m_tStats.m_fCurrentHp <= 0)
	{
		m_tStats.m_fCurrentHp = 0;
		CBattleManager::Get_Instance()->Update_LockOn();
		Check_AmILastMoster();

		return _int(m_tStats.m_fCurrentHp);
	}

	CBattleManager::Get_Instance()->Set_LackonMonster(this);
	m_bHit = true;
	m_dwHitTime = GetTickCount();
	m_bTakeDamage = true;

	return _int(m_tStats.m_fCurrentHp);
}

void CMonster::Collision_Object(_float fTimeDelta)
{

	if (m_pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(m_pCameraManager->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		return;

	CBaseObj* pCollisionMonster = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionMonster))
	{
		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - pCollisionMonster->Get_TransformState(CTransform::STATE_TRANSLATION);

		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);

		m_pTransformCom->Go_PosDir(fTimeDelta*0.5f, vDirection, m_pNavigationCom);
	}

	/*CBaseObj* pPlayer = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionMonster))
	{
		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION) - pCollisionMonster->Get_TransformState(CTransform::STATE_TRANSLATION);

		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);

		m_pTransformCom->Go_PosDir(fTimeDelta, vDirection, m_pNavigationCom);
	}*/
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
	if (!m_pShaderCom)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (m_pDissolveTexture)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DissolveTexture", m_pDissolveTexture->Get_SRV(0))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveTimer", &m_fDissolveTimer, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveLifespan", &m_fDissolveLifespan, sizeof(_float))))
			return E_FAIL;
		_float4 vDissolveColor = _float4(0.85f, 0.9f, 1.f, 1.f);
		if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveColor", &vDissolveColor, sizeof(_float4))))
			return E_FAIL;
		_float4 vDissolveHighlight = _float4(0.55f, 0.9f, 1.f, 1.f);
		if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveHighlight", &vDissolveHighlight, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fGlowUpTimer", &m_fGlowUpTimer, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fGlowUpLifespan", &m_fGlowUpLifespan, sizeof(_float))))
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