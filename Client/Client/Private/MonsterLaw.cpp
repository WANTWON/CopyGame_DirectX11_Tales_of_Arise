#include "stdafx.h"
#include "MonsterLaw.h"
#include "CameraManager.h"
#include "Monster_LawPoseState.h"
#include "Monster_Lawhit.h"
#include "UI_Dialogue.h"
#include "Fascinate.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "Monster_Lawhit.h"
#include "Level_LawBattle.h"

using namespace MonsterLaw;

CMonsterLaw::CMonsterLaw(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CMonster(pDevice, pContext)
{
}

CMonsterLaw::CMonsterLaw(const CMonsterLaw & rhs)
	: CMonster(rhs)
{
}


HRESULT CMonsterLaw::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonsterLaw::Initialize(void * pArg)
{
	NONANIMDESC ModelDesc;

	if (pArg != nullptr)
		memcpy(&ModelDesc, pArg, sizeof(NONANIMDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	m_tStats.m_fMaxHp = 100000.f;

	m_tStats.m_fCurrentHp = m_tStats.m_fMaxHp;
	m_tStats.m_fAttackPower = 10.f;
	m_tStats.m_fWalkSpeed = 0.05f;
	m_tStats.m_fRunSpeed = 5.f;

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(ModelDesc.vScale);

		if (ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(ModelDesc.m_fAngle));
	}


	/* Set State */
	CMonsterLawState* pState = new CPoseState(this, CMonsterLawState::STATE_BATTLESTART);
	m_pState = m_pState->ChangeState(m_pState, pState);
	m_pTransformCom->LookAt(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_eMonsterID = LAW_MONSTER;
	m_pNavigationCom->Compute_CurrentIndex_byXZ(Get_TransformState(CTransform::STATE_TRANSLATION));
	return S_OK;

}

void CMonsterLaw::Reset_StrikeBlur(_float fTimeDelta)
{
	if (!m_bResetStrikeBlur)
		return;

	if (m_fStrikeBlurResetTimer < m_fStrikeBlurResetDuration)
	{
		/* Zoom Blur Lerp */
		_float fFocusPower = 10.f;

		_float fBlurInterpFactor = m_fStrikeBlurResetTimer / m_fStrikeBlurResetDuration;
		if (fBlurInterpFactor > 1.f)
			fBlurInterpFactor = 1.f;

		_int iDetailStart = 10;
		_int iDetailEnd = 1;
		_int iFocusDetailLerp = iDetailStart + fBlurInterpFactor * (iDetailEnd - iDetailStart);
		m_pRendererCom->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);

		m_fStrikeBlurResetTimer += fTimeDelta;
	}
	else
	{
		m_fStrikeBlurResetTimer = 0.f;
		m_bResetStrikeBlur = false;
		m_pRendererCom->Set_ZoomBlur(false);
	}
}

HRESULT CMonsterLaw::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 6.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("AILaw"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 2.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;


	if (FAILED(__super::Add_Components(TEXT("Com_LawBattleNavigation"), LEVEL_STATIC, TEXT("Prototype_Component_LawBattle_Navigation"), (CComponent**)&m_vecNavigation[LEVEL_LAWBATTLE])))
		return E_FAIL;

	m_pNavigationCom = m_vecNavigation[LEVEL_LAWBATTLE];

	return S_OK;
}

int CMonsterLaw::Tick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->Get_KillLawbosslevel() && m_tStats.m_fCurrentHp > 0)
	{
		HITLAGDESC m_HitLagDesc;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bShaking = false;
		Take_Damage(1, CPlayerManager::Get_Instance()->Get_ActivePlayer(), m_HitLagDesc);
	}

	if (m_bDead)
	{
		CBattleManager::Get_Instance()->Set_BossMonster(nullptr);
		CBattleManager::Get_Instance()->Out_Monster(this);
		if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_LAWBATTLE)
			dynamic_cast<CLevel_LawBattle*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Plus_DeadCount();
		return OBJ_DEAD;
	}

	if (CGameInstance::Get_Instance()->Key_Down(DIK_PGDN))
		m_bReadytokill = true;

	if (CPlayerManager::Get_Instance()->Get_Changetoboss())
	{
		//m_fFinalTimer += fTimeDelta;

		if (m_bReadytokill)
		{
	//		if (m_fFinalStrikeOnetime)
	//		{
				m_tStats.m_fLockonSmashGuage = 4.f;
				m_fFinalStrikeOnetime = false;

	//		}
			
		}
	}
	
	if (m_bDaguri)
	{
		
		m_fRandomTargetTimer += fTimeDelta;

		if (m_fRandomTargetTimer > 20.f)
		{
			m_fRandomTargetTimer = 0.f;
			m_bReadytoChangeTarget = true;
		}
			

		if (m_bReadytoChangeTarget)
		{
			_bool loop = true;
			_int PrePhase = m_iPhase;
			while(loop)
			{
				m_iPhase = rand() % 3;
				if (PrePhase != m_iPhase)
					break;
			}
			m_bReadytoChangeTarget = false;
		}
	}
	else
	m_iPhase = CBattleManager::Get_Instance()->Get_LawBattlePhase();
		
	

	if(CGameInstance::Get_Instance()->Key_Up(DIK_M))
	{
		CCameraManager* pCameraManager = CCameraManager::Get_Instance();
		pCameraManager->Set_CamState(CCameraManager::CAM_ACTION);
		pCameraManager->Play_ActionCamera(TEXT("SexyLaw1.dat"), XMMatrixIdentity());
	    Set_IsActionMode(true);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, 0.f, 64.f, 1.f));
		m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, 1.f, 0.f));
		dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(13);
		CPlayerManager::Get_Instance()->Get_EnumPlayer(1)->Set_IsActionMode(false);
		CPlayerManager::Get_Instance()->Get_EnumPlayer(2)->Set_IsActionMode(false);

		CMonsterLawState* pState = new CFascinate(this);
		m_pState = m_pState->ChangeState(m_pState, pState);
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(31);
		CBattleManager::Get_Instance()->Set_IsStrike(true);
		CGameInstance::Get_Instance()->StopAll();
		CGameInstance::Get_Instance()->PlayBGM(TEXT("hentai.wav"), g_fSoundVolume);
		

	}
	if (CGameInstance::Get_Instance()->Key_Up(DIK_HOME))
		m_bdoStrike1 = true;
	if (CGameInstance::Get_Instance()->Key_Up(DIK_END))
		m_bdoStrike2 = true;




	if(m_bAfterThunder)
	m_fThunderHitTime += fTimeDelta;

	if (m_bAfterKick)
		m_fKickHitTIme += fTimeDelta;


	if (m_fThunderHitTime > 4.f && m_bAfterThunder)
	{
		m_bAfterThunder = false;
		m_fThunderHitTime = 0.f;
	}

	if (m_fKickHitTIme > 3.f && m_bAfterKick)
	{
		m_bAfterKick = false;
		m_fKickHitTIme = 0.f;
	}
		


	_int iSuperTick = __super::Tick(fTimeDelta);
	if (iSuperTick == OBJ_DEAD)
		return OBJ_DEAD;

	if (iSuperTick == OBJ_NOSHOW)
		return OBJ_NOSHOW;

	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return OBJ_NOSHOW;

	AI_Behavior(fTimeDelta);
	Tick_State(fTimeDelta);

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	if (m_pState->Get_StateId() == CMonsterLawState::STATE_ID::STATE_DOWN)
		m_fFresnelTimer += fTimeDelta * 6;
	else
		m_fFresnelTimer = 0.f;

	return OBJ_NOEVENT;
}

void CMonsterLaw::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || m_eLevel == LEVEL_LOADING || m_eLevel == LEVEL_LOGO)
		return;
	if (!Check_IsinFrustum(2.f) && !m_bBattleMode)
		return;

	__super::Late_Tick(fTimeDelta);

	if (ExceptionHanding() == false)
		return;

	if (m_pRendererCom && m_bGlowUp)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	if (!m_bDissolve)
	{
		LateTick_State(fTimeDelta);
	}



}

HRESULT CMonsterLaw::Render()
{
	if (m_pState->Get_StateId() == CMonsterLawState::STATE_ID::STATE_DOWN)
	{
		_bool bDownState = true;
		m_pShaderCom->Set_RawValue("g_bRimLight", &bDownState, sizeof(_bool));
	}

	__super::Render();

	return S_OK;
}

HRESULT CMonsterLaw::Render_Glow()
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
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_bDissolve ? SHADER_ANIM_GLOW_DISSOLVE : SHADER_ANIM_GLOW)))
			return E_FAIL;
	}

	return S_OK;
}

void CMonsterLaw::AI_Behavior(_float fTimeDelta)
{
	CMonsterLawState* pNewState = m_pState->AI_Behaviour(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);
}


void CMonsterLaw::Tick_State(_float fTimeDelta)
{
	CMonsterLawState* pNewState = m_pState->Tick(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);

	
	

}

void CMonsterLaw::LateTick_State(_float fTimeDelta)
{
	CMonsterLawState* pNewState = m_pState->LateTick(fTimeDelta);
	if (pNewState)
		m_pState = m_pState->ChangeState(m_pState, pNewState);
}


void CMonsterLaw::Battle_Animation(_float fTimeDelta)
{
	_bool isAnimationFinised = m_pModelCom->Play_Animation(fTimeDelta, Is_AnimationLoop(m_eAnim), "TransN");

	if (isAnimationFinised)
	{
		m_bMotion_Finished = true;
		m_pModelCom->Set_CurrentAnimIndex(DASH);
	}
}


void CMonsterLaw::Set_BattleMode(_bool type)
{
	m_bBattleMode = type;

	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
}

void CMonsterLaw::Set_HitState()
{
}


_bool CMonsterLaw::Is_AnimationLoop(_uint eAnimId)
{
	switch ((ANIM)eAnimId)
	{
	case IDLE_SIDE:
	case RUN:
	case DASH:
	case BTL_MOVE_IDLE:
	case BTL_MOVE_RUN:
		return true;
	default:
		return false;
	}
}

_int CMonsterLaw::Take_Damage(int fDamage, CBaseObj* DamageCauser, HITLAGDESC HitDesc)
{
	if (fDamage <= 0 || m_bDead || m_bTakeDamage )
		return 0;

	

	_int iHp = __super::Take_Damage(fDamage, DamageCauser, HitDesc);

	if (!m_fFinalStrikeOnetime&&CBattleManager::Get_Instance()->Get_KillLawbosslevel())
	{
		iHp = 0;
		m_bLastStrikeAttack = true;
	}

	//if(CBattleManager::Get_Instance()->Get_IsOneonOneMode() || CBattleManager::Get_Instance()->Get_Rinwellboss())
	if (CBattleManager::Get_Instance()->Get_IsOneonOneMode() && !m_fFinalStrikeOnetime)
	{
		
		m_tStats.m_fLockonSmashGuage = 0.f;
	}
	if (CBattleManager::Get_Instance()->Get_Rinwellboss() && !m_fFinalStrikeOnetime)
	{
		m_tStats.m_fLockonSmashGuage = 0.f;
	}

	++m_iLawhitcount;

	if (m_iLawhitcount >= 15)
	{

		m_bDownState = true;
		m_iLawhitcount = 0;

		m_pTarget = DamageCauser;
		m_eDmg_Direction = Calculate_DmgDirection();

		CMonsterLawState* pState = new CMonster_Lawhit(this, m_eDmg_Direction, CMonsterLawState::STATE_DAMAGE);
		m_pState = m_pState->ChangeState(m_pState, pState);
	
	}

	if (m_bAfterThunder)
	{
		m_bDownState;
		m_pTarget = DamageCauser;
		m_eDmg_Direction = Calculate_DmgDirection();

		CMonsterLawState* pState = new CMonster_Lawhit(this, m_eDmg_Direction, CMonsterLawState::STATE_DAMAGE);
		m_pState = m_pState->ChangeState(m_pState, pState);
		m_bAfterThunder = false;
	}

	if (m_bAfterKick)
	{
		m_bDownState;
		m_pTarget = DamageCauser;
		m_eDmg_Direction = Calculate_DmgDirection();

		CMonsterLawState* pState = new CMonster_Lawhit(this, m_eDmg_Direction, CMonsterLawState::STATE_DAMAGE);
		m_pState = m_pState->ChangeState(m_pState, pState);
		m_bAfterKick = false;
	}
	

	/*if (m_tStats.m_iBedamagedCount >= 20)
	{
		m_bBedamageAnim = true;
		m_tStats.m_iBedamagedCount = 0;
		
	}*/



	if (iHp <= 0)
	{
		if (!m_bLastStrikeAttack)
		{
			m_tStats.m_fLockonSmashGuage = 4.f;
			m_tStats.m_fCurrentHp = 0;
			m_bTakeDamage = true;
			CMonsterLawState* pState = new CMonster_Lawhit(this, m_eDmg_Direction, CMonsterLawState::STATE_DAMAGE);
			m_pState = m_pState->ChangeState(m_pState, pState);
		}
		else
		{
			m_tStats.m_fCurrentHp = 0;
			CBattleManager::Get_Instance()->Update_LockOn();
			Check_AmILastMoster();

			m_bTakeDamage = true;
			CMonsterLawState* pState = new CMonster_Lawhit(this, m_eDmg_Direction, CMonsterLawState::STATE_DEAD);
			m_pState = m_pState->ChangeState(m_pState, pState);
			//CRinwellState* pState = new CDamageState(this, m_eDmg_Direction, CRinwellState::STATE_DEAD);
			//m_pState = m_pState->ChangeState(m_pState, pState);
		}
	}
	else
	{
		m_pTarget = DamageCauser;
		m_eDmg_Direction = Calculate_DmgDirection();
		m_bTakeDamage = true;

		if (fDamage > 300)
		{
			//CRinwellState* pState = new CDamageState(this, m_eDmg_Direction, CRinwellState::STATE_DAMAGE);
			//m_pState = m_pState->ChangeState(m_pState, pState);
		}

	}

	return iHp;
}

HRESULT CMonsterLaw::SetUp_ShaderID()
{
	if (false == m_bDissolve)
		m_eShaderID = SHADER_ANIMDEFAULT;
	else
		m_eShaderID = SHADER_ANIM_DISSOLVE;

	return S_OK;
}

void CMonsterLaw::Check_Navigation()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);
	_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);

	vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

//_bool CMonsterLaw::Check_Navigation_Jump()
//{
//	return _bool();
//}

_bool CMonsterLaw::Check_Navigation_Jump()
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


CMonsterLaw * CMonsterLaw::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMonsterLaw* pInstance = new CMonsterLaw(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMonsterLaw"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMonsterLaw::Clone(void * pArg)
{
	CMonsterLaw* pInstance = new CMonsterLaw(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMonsterLaw"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonsterLaw::Free()
{
	__super::Free();

	Safe_Delete(m_pState);

}
