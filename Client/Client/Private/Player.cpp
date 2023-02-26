#include "stdafx.h"
#include "Player.h"

#include "GameInstance.h"
#include "Weapon.h"
#include "PlayerState.h"
#include "PlayerIdleState.h"
#include "AIState.h"
#include "PlayerCollectState.h"
#include "AICheckState.h"
#include "PlayerDeadState.h"
#include "PlayerHitState.h"
#include "PlayerPoseState.h"

#include "AI_HitState.h"
#include "AIDeadState.h"
#include "AI_BoostAttackState.h"
#include "AiState_WakeUp.h"
///////////////////////////////////
#include "AI_AlphenSion_Smash.h"
#include "AI_AlphenRinwell_Smash.h"
#include "AI_AlphenLaw_Smash.h"
#include "AI_SionRinwell_Smash.h"
#include "AI_SionLaw_Smash.h"
#include "AI_RinwellLaw_Smash.h"
//////////////////////////////////
#include "AI_Overlimit_State.h"
#include "AIPoseState.h"
#include "Damagefont_Critical.h"

#include "Level_Restaurant.h"

using namespace Player;
using namespace AIPlayer;


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

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));

	CAIState* pAIState = new AIPlayer::CAICheckState(this , CAIState::STATE_IDLE);
	m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);

	/* Set State */
	CPlayerState* pPlayerState = new Player::CIdleState(this, CIdleState::IDLE_SIDE);
	m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pPlayerState);

	m_pPlayerManager = GET_INSTANCE(CPlayerManager);
	
	

	m_eLevel = LEVEL_END;

	m_pPlayerManager->Set_PlayerEnum(this, m_ePlayerID);
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_PLAYER, this);

	m_tInfo.fCurrentOverlimitGauge = 100.f;

	return S_OK;
}

int CPlayer::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;

	CCameraManager* pCameraManager = CCameraManager::Get_Instance();

	if (pCameraManager->Get_CamState() == CCameraManager::CAM_ACTION && m_bIsActiveAtActionCamera == false)
		return OBJ_NOEVENT;

	m_eLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
	if(m_eLevel == LEVEL_LOADING || m_eLevel == LEVEL_LOGO)
		return OBJ_NOEVENT;

	if (m_eLevel == LEVEL_SNOWFIELD && CBattleManager::Get_Instance()->Get_IsBattleMode())
		return OBJ_NOEVENT;

	if (pCameraManager->Get_CamState()== CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		return OBJ_NOEVENT;

	PLAYER_MODE eMode = m_pPlayerManager->Check_ActiveMode(this);


	if (m_bOverLimit)
	{
		m_tInfo.fCurrentMp = m_tInfo.fMaxMp;
		m_fOverLimitTimer += fTimeDelta;
		if (m_fOverLimitTimer > 0.2f)
		{

			m_fOverLimitTimer = 0.f;
			m_tInfo.fCurrentOverlimitGauge -= 1.f;
			if (m_tInfo.fCurrentOverlimitGauge <= 0.f)
			{
				m_tInfo.fCurrentOverlimitGauge = 100.f;
				m_bOverLimit = false;
				m_fOverLimitTimer = 0.f;
			}
		}
	}


	if (m_bManaRecover)
		m_tInfo.fCurrentMp += 0.02f;

	if (m_tInfo.fCurrentMp >= m_tInfo.fMaxMp)
		m_tInfo.fCurrentMp = m_tInfo.fMaxMp;


	if (CBattleManager::Get_Instance()->Get_IsBattleMode())
	{	
		if (!m_bIsPose)
		{
			m_bIsPose = true;

			/* Set State */
			CPlayerState* pPlayerState = new Player::CPlayerPoseState(this, CPlayerState::STATE_POSE);
			m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pPlayerState);

			CAIState* pAIState = new AIPlayer::CAIPoseState(this, CAIState::STATE_POSE);
			m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		}

		if (!CBattleManager::Get_Instance()->IsAllMonsterDead())
		{
			/*_float debug = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fLockonSmashGuage;*/
			if (dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fLockonSmashGuage < 4.f)
				BoostAttack();
		}

		if (CGameInstance::Get_Instance()->Key_Up(DIK_8) && CUI_Manager::Get_Instance()->Get_CP() >= 0)
		{
			CUI_Manager::Get_Instance()->MinusCP(10);
			m_tInfo.fCurrentHp += 100.f;
			if (m_bDead)
				m_bDead = false;
			CPlayerState* pState = new CIdleState(this, CIdleState::IDLE_SIDE);
			m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pState);
		}
		else if (CGameInstance::Get_Instance()->Key_Up(DIK_9))
			Take_Damage((_int)m_tInfo.fCurrentHp, nullptr);
	}
	else if (m_bIsPose)
		m_bIsPose = false;

	switch (eMode)
	{
	case Client::ACTIVE:
		if (!m_bStrikeAttack)
		{
			HandleInput();
			Tick_State(fTimeDelta);
		}
		else
			Tick_AIState(fTimeDelta);
		
		break;
	case Client::AI_MODE:
		Tick_AIState(fTimeDelta);
		break;
	case Client::UNVISIBLE:
		return S_OK;
	}	

	if (nullptr != m_pAABBCom)
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (nullptr != m_pOBBCom)
		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (nullptr != m_pSPHERECom)
		m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	for (auto& pParts : m_Parts)
	{
		if(pParts != nullptr)
			pParts->Tick(fTimeDelta);
	}

	Reset_DodgeEffect(fTimeDelta);
	
	return OBJ_NOEVENT;
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return ;

	CCameraManager* pCameraManager = CCameraManager::Get_Instance();

	if (m_eLevel == LEVEL_LOADING || m_eLevel == LEVEL_LOGO)
		return;

	PLAYER_MODE eMode = m_pPlayerManager->Check_ActiveMode(this);

	if (nullptr != m_pRendererCom && eMode != UNVISIBLE)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_Parts[PARTS_WEAPON]);

		if(CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_ACTION)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EDGE_DETECTION, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EDGE_DETECTION, m_Parts[PARTS_WEAPON]);
#ifdef _DEBUG
		m_pRendererCom->Add_Debug(m_pNavigationCom);
		__super::Late_Tick(fTimeDelta);
#endif //_DEBUG
	}

	if (m_eLevel == LEVEL_SNOWFIELD && CBattleManager::Get_Instance()->Get_IsBattleMode())
		return;

	if (pCameraManager->Get_CamState() == CCameraManager::CAM_ACTION && m_bIsActiveAtActionCamera == false)
		return;

	if (pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera())->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		return;
	
	switch (eMode)
	{
	case Client::ACTIVE:
		if (!m_bStrikeAttack)
		LateTick_State(fTimeDelta);
		else
			LateTick_AIState(fTimeDelta);
		break;
	case Client::AI_MODE:
		LateTick_AIState(fTimeDelta);
		break;
	case Client::UNVISIBLE:
		return;
	}

	for (auto& pParts : m_Parts)
	{
		if (pParts != nullptr)
			pParts->Late_Tick(fTimeDelta);
	}

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	if (pBattleMgr->Get_IsBattleMode())
	{
		vector<CBaseObj*> Monsters = pBattleMgr->Get_BattleMonster();

		_vector vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

		for (auto& Monster : Monsters)
		{
			_vector vMonsterPos = Monster->Get_TransformState(CTransform::STATE_TRANSLATION);

			_vector vDirection = XMVectorSetY(vPlayerPos, XMVectorGetY(vMonsterPos)) - vMonsterPos;

			_float fRadiusSum = m_pSPHERECom->Get_SphereRadius() + Monster->Get_SPHERECollider()->Get_SphereRadius();

			_float fCollDistance = fRadiusSum - XMVectorGetX(XMVector4Length(vDirection));

			if (fCollDistance > 0)
			{
				_vector vNewPos = vMonsterPos + (XMVector4Normalize(vDirection) * fRadiusSum);
				_vector vLerpPos = XMVectorLerp(vPlayerPos, XMVectorSetY(vNewPos, XMVectorGetY(vPlayerPos)), 0.5f);

				if (true == m_pNavigationCom->isMove(vLerpPos))
					m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vLerpPos);
			}
		}
	}
}

HRESULT CPlayer::Render()
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

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 0)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CPlayer::Render_ShadowDepth()
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

HRESULT CPlayer::Render_EdgeDetection()
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
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 5)))
			return E_FAIL;
	}

	return S_OK;
}


_int CPlayer::Take_Damage(int fDamage, CBaseObj * DamageCauser, _bool isDown)
{
	if (fDamage <= 0 || m_bDead || m_bIsJustDodge)
		return 0;

	PLAYER_MODE eMode = m_pPlayerManager->Check_ActiveMode(this);
	if (eMode == ACTIVE)
		int a = 0;

	
	if (m_bTakeDamage_Delay == false)
	{
		m_tInfo.fCurrentHp -= (int)fDamage;
		m_bTakeDamage_Delay = true;
	}

	m_tInfo.fCurrentHp -= (int)fDamage;


	CDamagefont_Critical::DMGDESC testdesc;
	ZeroMemory(&testdesc, sizeof(CDamagefont_Critical::DMGDESC));
	testdesc.iDamage = fDamage;
	testdesc.pPointer = this;
	testdesc.itype = 4;

	if (false == (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Object(LEVEL_STATIC, TEXT("Layer_DamageCritical"), &testdesc)))
	{

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont_Critical"), LEVEL_STATIC, TEXT("Layer_DamageCritical"), &testdesc)))
			return OBJ_NOEVENT;
	}


	/*CDamageFont::DMGDESC testdesc;
	ZeroMemory(&testdesc, sizeof(CDamageFont::DMGDESC));
	testdesc.iDamage = fDamage;
	testdesc.pPointer = this;

	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("dmg"), &testdesc)))
		return E_FAIL;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("dmg"), &testdesc)))
		return E_FAIL;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Damagefont"), LEVEL_STATIC, TEXT("dmg"), &testdesc)))
		return E_FAIL;*/
	
	if (m_tInfo.fCurrentHp <= 0)
	{
		CPlayerState* pState = nullptr;
		CAIState* pAIState = nullptr;

		m_tInfo.fCurrentHp = 0;
		m_bDead = true;
		switch (eMode)
		{
		case Client::ACTIVE:
			pState = new CPlayerDeadState(this, CPlayerState::STATE_DEAD);
			m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pState);
			break;
		case Client::AI_MODE:
			pAIState = new AIPlayer::CDeadState(this);
			m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
			break;
		}
		return 0;
	}
	else
	{
		CPlayerState* pState = nullptr;
		CAIState* pAIState = nullptr;

		switch (eMode)
		{
		case Client::ACTIVE:
			if (CPlayerState::STATE_HIT != m_pPlayerState->Get_StateId())
			{
				_vector vCauserPos = DamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
				pState = new CHitState(this, vCauserPos, isDown, m_pPlayerState->Get_Time());
				m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pState);
			}
			break;
		case Client::AI_MODE:
			if (CAIState::STATE_HIT != m_pAIState->Get_StateId())
			{
				_vector vCauserPos = DamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
				pAIState = new AIPlayer::CAI_HitState(this, vCauserPos, isDown, m_pPlayerState->Get_Time());
				m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
				break;
			}
		}
	}

	return (_uint)m_tInfo.fCurrentHp;
}

void CPlayer::Set_PlayerCollectState(CInteractObject * pObject)
{
	CPlayerState* pPlayerState = new Player::CCollectState(this, pObject);
	m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pPlayerState);

}

void CPlayer::Play_AISkill(PLAYERID ePlayer)
{
	PLAYER_MODE eMode = m_pPlayerManager->Check_ActiveMode(this);
	if (m_tInfo.fCurrentBoostGuage < 100.f || eMode != Client::AI_MODE)
		return;
	switch (ePlayer)
	{
	case Client::CPlayer::ALPHEN:
	{
		CAIState* pAIState = new AIPlayer::CAI_BoostAttack(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		break;
	}
	case Client::CPlayer::SION:
	{
		CAIState* pAIState = new AIPlayer::CAI_BoostAttack(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		break;
	}
	case Client::CPlayer::RINWELL:
	{
		CAIState* pAIState = new AIPlayer::CAI_BoostAttack(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
	}
		
		
		break;
	case Client::CPlayer::LAW:
		CAIState* pAIState = new AIPlayer::CAI_BoostAttack(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		break;
	}

}



void CPlayer::Plus_EXP(_uint exp)
{

	m_tInfo.iCurrentExp += exp;
	
	if (m_tInfo.iCurrentExp >= m_tInfo.iMaxExp)
	{
		m_tInfo.iCurrentExp -= m_tInfo.iMaxExp;

		++m_tInfo.iLevel;

		m_bLevelup = true;

	}

}

void CPlayer::Revive()
{
	
	PLAYER_MODE eMode = m_pPlayerManager->Check_ActiveMode(this);

	CPlayerState* pState = nullptr;
	CAIState* pAIState = nullptr;

	
	//	m_tInfo.fCurrentHp += hp;
		if (eMode == Client::ACTIVE)
		{
			CPlayerState* pState = new CIdleState(this, CIdleState::IDLE_SIDE);
			m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pState);
		}
		else
		{
			CAIState* pAIState =  new CAiState_WakeUp(this);
			m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		}
		
			
	
}

void CPlayer::AI_check()
{
	CAIState* pAIState = nullptr;

	pAIState = new CAICheckState(this,CAIState::STATE_IDLE);
	m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
}

void CPlayer::RecoverHP(_float hp)
{
	 m_tInfo.fCurrentHp += hp;
	 if (m_tInfo.fCurrentHp > m_tInfo.fMaxHp)
		 m_tInfo.fCurrentHp = m_tInfo.fMaxHp; 
}

void CPlayer::HandleInput()
{
	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	if (pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC)
	{
		_uint eCamMode = dynamic_cast<CCamera_Dynamic*>(pCamera)->Get_CamMode();
		if (eCamMode == CCamera_Dynamic::CAM_AIBOOSTON )
			return;
	}

	CPlayerState* pNewState = m_pPlayerState->HandleInput();
	if (pNewState)
		m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pNewState);
}

void CPlayer::Tick_State(_float fTimeDelta)
{
	CPlayerState* pNewState = m_pPlayerState->Tick(fTimeDelta);
	if (pNewState)
		m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pNewState);
}

void CPlayer::LateTick_State(_float fTimeDelta)
{
	CPlayerState* pNewState = m_pPlayerState->LateTick(fTimeDelta);
	if (pNewState)
		m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pNewState);
}

void CPlayer::Tick_AIState(_float fTimeDelta)
{

	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	if (pCameraManager->Get_CamState() == CCameraManager::CAM_DYNAMIC)
	{
		_uint eCamMode = dynamic_cast<CCamera_Dynamic*>(pCamera)->Get_CamMode();
		if (eCamMode == CCamera_Dynamic::CAM_AIBOOSTON )
		{
			if(dynamic_cast<CCamera_Dynamic*>(pCamera)->Get_Target() != this)
			return;
		}
			
	}


	CAIState* pNewState = m_pAIState->Tick(fTimeDelta);
	if (pNewState)
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pNewState);
}

void CPlayer::LateTick_AIState(_float fTimeDelta)
{
	CAIState* pNewState = m_pAIState->LateTick(fTimeDelta);
	if (pNewState)
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pNewState);
}

void CPlayer::SmashAttack(_uint smashtype)
{
	switch (smashtype)
	{
	case ALPHEN_SION:
	{
		if (CBattleManager::Get_Instance()->Get_LackonMonster()!= nullptr)
		dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();

		CAIState* pAIState = new AIPlayer::CAI_AlphenSion_Smash(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		break;
	}
		

	case ALPHEN_RINWELL:
	{  if (CBattleManager::Get_Instance()->Get_LackonMonster()!= nullptr)
		dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();

		CAIState* pAIState = new AIPlayer::CAI_AlphenRinwell_Smash(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		break;
	}

		

	case ALPHEN_LAW:
	{if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
		dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();

		CAIState* pAIState = new AIPlayer::CAI_AlphenLaw_Smash(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		break;
	}
		

	case SION_RINWELL:
	{if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
		dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();

		CAIState* pAIState = new AIPlayer::CAI_SionRinwell_Smash(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		break;
	}


		

	case SION_LAW:
	{if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
		dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();

		CAIState* pAIState = new AIPlayer::CAI_SionLaw_Smash(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		break;
	}
		

	case RINWELL_LAW:
	{if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
		dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();

		CAIState* pAIState = new AIPlayer::CAI_RinwellLaw_Smash(this, CBattleManager::Get_Instance()->Get_LackonMonster());
		m_pAIState = m_pAIState->ChangeState(m_pAIState, pAIState);
		break;
	}
		


	}



}
	
			
	

void CPlayer::BoostAttack()
{
	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::ACTION)
		return;

	if (CGameInstance::Get_Instance()->Key_Up(DIK_1) && m_pPlayerManager->Get_EnumPlayer(0)->Get_BoostGuage() >= 100.f)
		Play_AISkill(ALPHEN);
	else if (CGameInstance::Get_Instance()->Key_Up(DIK_2) && m_pPlayerManager->Get_EnumPlayer(1)->Get_BoostGuage() >= 100.f)
		Play_AISkill(SION);
	else if (CGameInstance::Get_Instance()->Key_Up(DIK_3) && m_pPlayerManager->Get_EnumPlayer(2)->Get_BoostGuage() >= 100.f)
		Play_AISkill(RINWELL);
}




HRESULT CPlayer::SetUp_ShaderResources()
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

void CPlayer::Reset_DodgeEffect(_float fTimeDelta)
{
	if (!m_bDodgeEffect)
		return;

	if (m_fResetTimer < m_fResetDuration)
	{
		/* Saturation Lerp */
		_float fSaturationInterpFactor = m_fResetTimer / m_fResetDuration;
		if (fSaturationInterpFactor > 1.f)
			fSaturationInterpFactor = 1.f;

		_float fSaturationStart = 2.f;
		_float fSaturationEnd = 1.f;
		_float fSaturationLerp = fSaturationStart + fSaturationInterpFactor * (fSaturationEnd - fSaturationStart);
		m_pRendererCom->Set_Saturation(true, fSaturationLerp);

		/* Zoom Blur Lerp */
		_float fFocusPower = 5.f;

		_float fBlurInterpFactor = m_fResetTimer / m_fResetDuration;
		if (fBlurInterpFactor > 1.f)
			fBlurInterpFactor = 1.f;

		_int iDetailStart = 10;
		_int iDetailEnd = 1;
		_int iFocusDetailLerp = iDetailStart + fBlurInterpFactor * (iDetailEnd - iDetailStart);
		m_pRendererCom->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);

		m_fResetTimer += fTimeDelta;
	}
	else
	{
		m_fResetTimer = 0.f;
		m_bDodgeEffect = false;
		m_pRendererCom->Set_ZoomBlur(false);
		m_pRendererCom->Set_Saturation(false);
	}
}

void CPlayer::Change_Navigation(LEVEL eLevel)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (eLevel)
	{
	case Client::LEVEL_BATTLE:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_BattleNavigation"), m_ePlayerID));
		break;
	case Client::LEVEL_SNOWFIELD:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_FieldNavigation"), m_ePlayerID));
		break;
	case Client::LEVEL_BOSS:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_BossNavigation"), m_ePlayerID));
		break;
	case Client::LEVEL_CITY:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_CityNavigation"), m_ePlayerID));
		break;
	case Client::LEVEL_RESTAURANT:
	{
		CLevel_Restaurant* pLevel = dynamic_cast<CLevel_Restaurant*>(CGameInstance::Get_Instance()->Get_CurrentLevel());
		if (pLevel == nullptr || pLevel->Get_MiniGameStart() == false)
			m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_RestaurantNavigation"), m_ePlayerID));
		else
			m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_RestaurantMIniGameNavigation"), m_ePlayerID));

		break;

	}
	case Client::LEVEL_WORKTOOL:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_WorkToolNavigation"), m_ePlayerID));
		break;
		
	}
	
	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

_bool CPlayer::Check_Navigation_Jump(void)
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

void CPlayer::Compute_CurrentIndex(LEVEL eLevel)
{
	m_pNavigationCom->Compute_CurrentIndex_byXZ(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION));
}

void CPlayer::Change_Level(LEVEL eLevel)
{
	m_eLevel = eLevel;

	CPlayerState* pNewState = new Player::CIdleState(this, CIdleState::IDLE_SIDE);
	if (pNewState)
		m_pPlayerState = m_pPlayerState->ChangeState(m_pPlayerState, pNewState);
}

HRESULT CPlayer::SetUp_Controller()
{
	//CPhysX*		pPhysX = GET_INSTANCE(CPhysX);
	//PxCapsuleControllerDesc desc;
	//_vector Pos = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	//_float3 fPos;
	//XMStoreFloat3(&fPos, Pos);

	////�������� ������ ��ǥ��
	//desc.position = PxExtendedVec3(XMVectorGetX(Pos), XMVectorGetY(Pos), -XMVectorGetZ(Pos));
	//desc.radius = 1.f;
	//desc.height = 2.f;
	//desc.stepOffset = 2.f;
	//desc.volumeGrowth = 1.f;
	//desc.slopeLimit = cosf(XMConvertToRadians(15.f));
	//desc.nonWalkableMode = PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	//desc.upDirection = PxVec3(0.f, 1.f, 0.f);
	//desc.contactOffset = 0.01f;
	//PxMaterial* pMeterial = pPhysX->Get_Physics()->createMaterial(0.5f, 0.5f, 0.5f);
	//desc.material = pMeterial;
	//desc.reportCallback = this;
	//desc.behaviorCallback = this;
	//desc.userData = this;
	//gController = (PxCapsuleController*)pPhysX->Get_ControllerManager()->createController(desc);
	//if (gController == nullptr)
	//	return E_FAIL;
	//ControllerActor = gController->getActor();
	//
	//Px = gController->getActor();

	//RELEASE_INSTANCE(CPhysX);
	return S_OK;
}



void CPlayer::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_PLAYER, this);


	for (auto& pGameObject : m_Parts)
		Safe_Release(pGameObject);

	m_Parts.clear();

	for (auto& iter : m_vecNavigations)
		Safe_Release(iter);

	m_vecNavigations.clear();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pPlayerManager);

	Safe_Delete(m_pPlayerState);
	Safe_Delete(m_pAIState);
}