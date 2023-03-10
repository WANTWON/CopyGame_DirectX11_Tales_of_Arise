#include "stdafx.h"
#include "AstralDoubt.h"
#include "AstralDoubt_IdleState.h"
#include "AstralDoubt_Battle_IdleState.h"
#include "AstralDoubt_State.h"
#include "AstralDoubt_TurnState.h"
#include "AstralDoubt_Battle_Hit_AndDead.h"
#include "AstralDoubt_Battle_SpearMultiState.h"
#include "AstralDoubt_Battle_UpperState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"
#include "AstralDoubt_Battle_HeadBeamState.h"
#include "AstralDoubt_Battle_RushState.h"
#include "AstralDoubt_TurnState.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_TeleportState.h"
#include "Level_BossZone.h"

using namespace Astral_Doubt;

CAstralDoubt::CAstralDoubt(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CMonster(pDevice, pContext)
{
}

CAstralDoubt::CAstralDoubt(const CAstralDoubt & rhs)
	: CMonster(rhs)
{
}

HRESULT CAstralDoubt::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAstralDoubt::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();
	//if (m_bBattleMode)
	//{
	//	/* Set_Battle State */
	//	CAstralDoubt_State* pBattleState = new CBattle_IdleState(this, CAstralDoubt_State::STATE_ID::START_BATTLE);
	//	m_pState = m_pState->ChangeState(m_pState, pBattleState);
	//}
	//else
	//{
	//	/* Set State */
	//	CAstralDoubt_State* pState = new CIdleState(this, CAstralDoubt_State::FIELD_STATE_ID::FIELD_STATE_START);
	//	m_pState = m_pState->ChangeState(m_pState, pState);
	//}

		///* Set State */
	CAstralDoubt_State* pState = new CBattle_IdleState(this, CAstralDoubt_State::STATE_ID::START_BATTLE);
	m_pState = m_pState->ChangeState(m_pState, pState);



	m_eMonsterID = ASTRAL_DOUBT;

	m_tStats.m_fMaxHp = 50000;
	m_tStats.m_fCurrentHp = m_tStats.m_fMaxHp;
	m_tStats.m_fAttackPower = 10.f;
	m_tStats.m_fWalkSpeed = 0.05f;
	m_tStats.m_fRunSpeed = 5.f;

	m_fRadius = 1.f;

	/*NONANIMDESC ModelDesc;
	if (pArg)
		memcpy(&ModelDesc, pArg, sizeof(NONANIMDESC));

	if (pArg)
	{
		_vector vPosition = XMLoadFloat3(&ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(ModelDesc.vScale);

		if (ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMLoadFloat3(&ModelDesc.vRotation), XMConvertToRadians(ModelDesc.m_fAngle));
	}

	Check_NearTrigger();*/
	//m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));

	m_fTimeDeltaAcc = 0;
	m_fCntChanceTime = ((rand() % 1000) *0.001f)*((rand() % 100) * 0.01f);
	m_bDone_HitAnimState = false;

	//CBattleManager::Get_Instance()->Out_Monster(this);

	return S_OK;
}

HRESULT CAstralDoubt::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));
	TransformDesc.fSpeedPerSec = 6.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Astral_Doubt"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	//switch (iLevel)
	//{
	//case Client::LEVEL_SNOWFIELD:
	//	/* For.Com_Navigation */
	//	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_pNavigationCom)))
	//		return E_FAIL;
	//	break;
	//case Client::LEVEL_BATTLE:
	//	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_pNavigationCom)))
	//		return E_FAIL;
	//	break;
	//default:
	//	break;
	//}
	if (FAILED(__super::Add_Components(TEXT("Com_FieldNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowField_Navigation"), (CComponent**)&m_vecNavigation[LEVEL_SNOWFIELD])))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_BattleNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_SnowPlaneBattleNavigation"), (CComponent**)&m_vecNavigation[LEVEL_BATTLE])))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_BossNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_Boss_Navigation"), (CComponent**)&m_vecNavigation[LEVEL_BOSS])))
		return E_FAIL;

	m_pNavigationCom = m_vecNavigation[LEVEL_BOSS];

	return S_OK;
}

int CAstralDoubt::Tick(_float fTimeDelta)
{
	_int iSuperTick = __super::Tick(fTimeDelta);
	if (iSuperTick == OBJ_DEAD)
		return OBJ_DEAD;

	if (iSuperTick == OBJ_NOSHOW)
		return OBJ_NOSHOW;

	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return OBJ_NOSHOW;

	__super::Tick(fTimeDelta);

	AI_Behaviour(fTimeDelta);
	Tick_State(fTimeDelta);

	m_fTimeDeltaAcc += fTimeDelta;

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	if (m_fTimeDeltaAcc > m_fCntChanceTime)
		m_iRand = rand() % 3;

	if (m_pState->Get_StateId() == CAstralDoubt_State::STATE_ID::STATE_DOWN)
		m_fFresnelTimer += fTimeDelta * 6;
	else
		m_fFresnelTimer = 0.f;

	if (CGameInstance::Get_Instance()->Key_Up(DIK_I))
	{
		CAstralDoubt_State* pState = new CBattle_SpearMultiState(this, CAstralDoubt_State::STATE_ID::STATE_SPEARMULTI);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_K))
	{
		CAstralDoubt_State* pState = new CBattle_SpearMultiState(this, CAstralDoubt_State::STATE_ID::STATE_FOOTPRESS);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}


	return OBJ_NOEVENT;
}

void CAstralDoubt::Late_Tick(_float fTimeDelta)
{
	m_eLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
	if (CUI_Manager::Get_Instance()->Get_StopTick() || m_eLevel == LEVEL_LOADING || m_eLevel == LEVEL_LOGO)
		return;

	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return;

	if (ExceptingActionCamHanding() == false)
		return;

	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom && m_bGlowUp)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	if (ExceptionHanding() == false)
		return;

	LateTick_State(fTimeDelta);
}

HRESULT CAstralDoubt::Render()
{
	if (m_pState->Get_StateId() == CAstralDoubt_State::STATE_ID::STATE_DOWN)
	{
		_bool bDownState = true;
		m_pShaderCom->Set_RawValue("g_bRimLight", &bDownState, sizeof(_bool));
	}

	__super::Render();

	return S_OK;
}

HRESULT CAstralDoubt::Render_Glow()
{
	if (!m_pShaderCom || !m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_bool bUseDiffuseColor = true;
	if (FAILED(m_pShaderCom->Set_RawValue("g_bUseDiffuseColor", &bUseDiffuseColor, sizeof(_bool))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GlowTexture", nullptr)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_bDissolve ? SHADER_ANIM_GLOW_DISSOLVE : 6)))
			return E_FAIL;
	}

	return S_OK;
}

void CAstralDoubt::AI_Behavior(_float fTimeDelta)
{
	CAstralDoubt_State* pNewState = m_pState->AI_Behaviour(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);
}

void CAstralDoubt::Tick_State(_float fTimeDelta)
{
	CAstralDoubt_State* pNewState = m_pState->Tick(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);

}

void CAstralDoubt::LateTick_State(_float fTimeDelta)
{
	CAstralDoubt_State* pNewState = m_pState->LateTick(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);
}

void CAstralDoubt::Set_BattleMode(_bool type)
{
	m_bBattleMode = type;
	if (m_bBattleMode)
	{
		m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, -1.f, 0.f));

		/* Set_Battle State */
		CAstralDoubt_State* pBattleState = new CBattle_IdleState(this, CAstralDoubt_State::STATE_ID::STATE_ADVENT);
		m_pState = m_pState->ChangeState(m_pState, pBattleState);
	}
	else
	{
		Check_NearTrigger();
		/* Set State */
		CAstralDoubt_State* pState = new CIdleState(this, CAstralDoubt_State::FIELD_STATE_ID::FIELD_STATE_IDLE);
		m_pState = m_pState->ChangeState(m_pState, pState);
	}
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
}

void CAstralDoubt::UpdatePosition()
{
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC ModelDesc1;
	NONANIMDESC ModelDesc2;
	_uint iNum = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/BossMap/BossPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;
	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	ReadFile(hFile, &(ModelDesc1), sizeof(NONANIMDESC), &dwByte, nullptr);
	if (m_AmIFirstBoss == true)
	{
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&ModelDesc1.vPosition), 1.f));
		m_pTransformCom->Set_Rotation(ModelDesc1.vRotation);
	}
		
	ReadFile(hFile, &(ModelDesc2), sizeof(NONANIMDESC), &dwByte, nullptr);
	CloseHandle(hFile);

	if (m_bCreatedMonster == false &&
		m_tStats.m_fCurrentHp < m_tStats.m_fMaxHp*0.5f &&
		m_AmIFirstBoss == true)
	{
		CBattleManager*			pBattleManager = GET_INSTANCE(CBattleManager);
		CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);
		NONANIMDESC ModelDesc;

		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_SecondBoss"));
		CBaseObj* pBossMonsterFirst = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_SecondBoss")));
		pBattleManager->Add_BattleMonster(pBossMonsterFirst);

		pBossMonsterFirst->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&ModelDesc2.vPosition), 1.f));
		dynamic_cast<CMonster*>(pBossMonsterFirst)->Compute_CurrentIndex(LEVEL_BOSS);
		dynamic_cast<CMonster*>(pBossMonsterFirst)->Set_BattleMode(true);
		dynamic_cast<CMonster*>(pBossMonsterFirst)->Set_IsActionMode(true);

		dynamic_cast<CLevel_BossZone*>(pGameInstance->Get_CurrentLevel())->Set_SecondCreated(true);

		RELEASE_INSTANCE(CGameInstance);
		RELEASE_INSTANCE(CBattleManager);
		m_bCreatedMonster = true;
	}

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	pPlayer->Set_IsActionMode(false);

	vector<CPlayer*> pAIList = CPlayerManager::Get_Instance()->Get_AIPlayers();
	for (auto& iter : pAIList)
	{
		iter->Set_IsActionMode(false);
	}
	
}

_bool CAstralDoubt::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{
	case MOVE_WALK_F:
	case SYMBOL_RUN:
	case MOVE_IDLE:
	case SYMBOL_LOOKOUT:
	case ATTACK_JUMP_SHOOT_DROP_RISE_LOOP:
	case ATTACK_JUMP_SHOOT_DROP_DESCEND_LOOP:
		return true;


	case EVENT_ADVENT:
	case TURN_L:
	case TURN_R:
	case SYMBOL_DETECT_STOP:
	case ATTACK_SWORD_720_FIRST:
	case ATTACK_SWORD_720_LAST:
	case ATTACK_HEAD_BEAM:
	case ATTACK_IN_OUT_UPPER:
	case ATTACK_SPEAR_MULTI:
	case SYMBOL_TURN_LEFT:
	case SYMBOL_TURN_RIGHT:
	case ATTACK_SWING_360:
	case DOWN_B:
	case DOWN_UNIQUE:
	case ARISE_B:
	case ATTACK_BRAVE:
	case ATTACK_SPEAR_RUSH_START:
	case ATTACK_SPEAR_RUSH_LOOP:
	case ATTACK_SPEAR_RUSH_END:
	case ATTACK_SPEAR_HANDSTAND_FOOTPRESS:
		return false;
	}

	return false;
}

_int CAstralDoubt::Take_Damage(int fDamage, CBaseObj* DamageCauser, HITLAGDESC HitDesc)
{
	if (fDamage <= 0 || m_bDead || m_bGlowUp || m_bDissolve || m_bTakeDamage || m_pState->Get_StateId() == CAstralDoubt_State::STATE_DEAD)
		return 0;

	_int iHp = __super::Take_Damage(fDamage, DamageCauser, HitDesc);

	m_fDamage += fDamage;

	if (iHp <= 0)
	{
		m_tStats.m_fCurrentHp = 0;
		CBattleManager::Get_Instance()->Update_LockOn();
		Check_AmILastMoster();


		m_pModelCom->Set_TimeReset();
		CAstralDoubt_State* pState = new CBattle_Hit_AndDead(this, CAstralDoubt_State::STATE_DEAD);
		m_pState = m_pState->ChangeState(m_pState, pState);

		return 0;
	}
	else
	{
		if (m_bOnGoingDown == false)
		{
			m_iBeDamaged_Cnt++;

			if (m_bDownState == true)
			{
				CAstralDoubt_State* pState = new CBattle_Hit_AndDead(this, CAstralDoubt_State::STATE_DOWN);
				m_pState = m_pState->ChangeState(m_pState, pState);
			}
		}
		if (!m_bIsHalf && (m_tStats.m_fCurrentHp < m_tStats.m_fMaxHp * 0.5f) && m_AmIFirstBoss)
		{
			m_bIsHalf = true;
			UpdatePosition();
			CAstralDoubt_State* pState = new CBattle_IdleState(this, CAstralDoubt_State::STATE_HALF);
			m_pState = m_pState->ChangeState(m_pState, pState);
		}
		else if (m_fDamage >= (m_tStats.m_fMaxHp * 0.15f))
		{
			m_fDamage = 0.f;

			CAstralDoubt_State* pState = new CAstralDoubt_TeleportState(this);
			m_pState = m_pState->ChangeState(m_pState, pState);
		}
	}

	return iHp;
}

HRESULT CAstralDoubt::SetUp_ShaderID()
{
	if (m_bDissolve)
		m_eShaderID = SHADER_ANIM_DISSOLVE;
	else
		m_eShaderID = SHADER_ANIMDEFAULT;

	return S_OK;
}

void CAstralDoubt::Set_HitState()
{
	CAstralDoubt_State* pState = new CBattle_Hit_AndDead(this, CAstralDoubt_State::STATE_SMASHHIT);
	m_pState = m_pState->ChangeState(m_pState, pState);
}

void CAstralDoubt::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

_float CAstralDoubt::ForTheBossTarget_Distance(CBaseObj* pTarget)
{
	_vector fDir = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - Get_TransformState(CTransform::STATE_TRANSLATION);
	XMVectorSetY(fDir, 0.f);
	_float fDistance = XMVectorGetX(XMVector3Length(fDir));


	return fDistance;
}

_bool CAstralDoubt::Check_Navigation_Jump(void)
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	if (m_fWalkingHeight >= XMVectorGetY(vPosition))
	{
		vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		return true;
	}

	return false;
}

CAstralDoubt * CAstralDoubt::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAstralDoubt* pInstance = new CAstralDoubt(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CAstralDoubt"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAstralDoubt::Clone(void * pArg)
{
	CAstralDoubt* pInstance = new CAstralDoubt(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CAstralDoubt"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAstralDoubt::Free()
{
	__super::Free();

	Safe_Delete(m_pState);
}