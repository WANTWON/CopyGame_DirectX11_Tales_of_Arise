#include "stdafx.h"
#include "..\Public\TailBoss.h"
#include "Player.h"
#include "CameraManager.h"
#include "UIName.h"
#include "MonsterEffect.h"
#include "FightEffect.h"
#include "UIIcon.h"

CTailBoss::CTailBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

HRESULT CTailBoss::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTailBoss::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_TailDesc, pArg, sizeof(TAILDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fHitRedColor = 1.f;
	m_tInfo.iMaxHp = 10;
	m_tInfo.iDamage = 4;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;

	m_fAttackRadius = 1.5f;
	m_eMonsterID = MONSTER_TAIL;

	m_TailDesc.InitPostion = XMVectorSetY(m_TailDesc.InitPostion, 10.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_TailDesc.InitPostion);
	m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, -1.f, 0.f));
	m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);

	if (m_TailDesc.eTailType == TAIL1)
	{
		Set_Scale(_float3(1.3f, 1.3f, 1.3f));

		CUIIcon::ICONDESC IconDesc;
		IconDesc.iTexureNum = CUIIcon::ICON_BOSS;
		IconDesc.pTarget = this;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_TAILCAVE, TEXT("UI_ICON"), &IconDesc)))
			return E_FAIL;
	}
		

	if (m_TailDesc.eTailType != TAIL1)
	{
		_float3 fScale = m_TailDesc.pParent->Get_Scale();

		if (m_TailDesc.eTailType != TAIL5)
			Set_Scale(_float3(fScale.x*0.8f, fScale.y*0.8f, fScale.z*0.8f));
		else
			Set_Scale(_float3(0.8f, 0.8f, 0.8f));
	}


	Create_Tail(m_TailDesc.eTailType);



	return S_OK;
}

int CTailBoss::Tick(_float fTimeDelta)
{

	if (Check_IsinFrustum(2.f) == false)
		return OBJ_NOEVENT;

	if (IsDead())
	{
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
		return OBJ_DEAD;
	}


	if (!m_bOpeningMotion)
		Opening_Motion(fTimeDelta);
	else
	{
		AI_Behaviour(fTimeDelta);
		Check_Navigation(fTimeDelta);
	}


	if (m_TailDesc.eTailType != TAIL1)
	{
		m_eState = dynamic_cast<CTailBoss*>(m_TailDesc.pParent)->Get_AnimState();
	}


	m_pModelCom->Set_NextAnimIndex(m_eState);
	Change_Animation(fTimeDelta);


	return OBJ_NOEVENT;
}

void CTailBoss::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	CBaseObj* pCollisionBlock = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_BLOCK, m_pSPHERECom, &pCollisionBlock))
	{
		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pCollisionBlock->Get_TransformState(CTransform::STATE_POSITION);
		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
		m_pTransformCom->Go_PosDir(fTimeDelta*1.5f, vDirection);

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180);
		m_fTurnAngle = (rand() % 2000) * 0.001f + 0.1f;
		m_fTurnAngle = rand() % 2 == 0 ? m_fTurnAngle : -m_fTurnAngle;
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Guard_0.wav"), SOUND_MEFFECT, 0.5f);
	}


}

HRESULT CTailBoss::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

void CTailBoss::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::DROP)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180);
		m_fTurnAngle = (rand() % 2000) * 0.001f + 0.1f;
		m_fTurnAngle = rand() % 2 == 0 ? m_fTurnAngle : -m_fTurnAngle;
	}
	else if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ACCESSIBLE)
	{
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float fHeight = m_pNavigationCom->Compute_Height(vPosition, Get_Scale().y*0.25f);
		vPosition = XMVectorSetY(vPosition, fHeight);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	}


}

void CTailBoss::Change_Animation(_float fTimeDelta)
{
	CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();

	switch (m_eState)
	{
	case Client::CTailBoss::DAMAGE_TURN:
	case Client::CTailBoss::DAMAGE:
		m_fAnimSpeed = 3.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bIsAttacking = false;
			m_eState = IDLE;

			//m_fTurnAngle += 1.f;
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180);
			m_fTurnAngle = rand() % 2 == 0 ? m_fTurnAngle : -m_fTurnAngle;

			CTransform::TRANSFORMDESC TransformDesc = m_pTransformCom->Get_TransformDesc();
			TransformDesc.fSpeedPerSec += 0.5f;
			m_pTransformCom->Set_TransformDesc(TransformDesc);
		}
		break;
	case Client::CTailBoss::FEINT:
	case Client::CTailBoss::GUARD:
		m_fAnimSpeed = 3.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bIsAttacking = false;
			m_eState = IDLE;
		}
		break;
	case Client::CTailBoss::DEAD:
		if (m_TailDesc.eTailType == TAIL1)
		{
			g_fBGMVolume -= 0.005f;
			if (g_fBGMVolume <= 0.f)
			{
				CGameInstance::Get_Instance()->StopSound(SOUND_BGM);
			}

			m_fEffectEndTime = 0.3f;
			m_fEffectTime += fTimeDelta;
			if (m_fEffectTime > m_fEffectEndTime)
			{
				CEffect::EFFECTDESC EffectDesc;
				EffectDesc.pTarget = this;
				EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
				EffectDesc.eEffectID = CMonsterEffect::ALBATROSSDEAD_SMOKE;
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(rand() % 2 == 0 ? -1.f : 1.f, 0.f, 0.f, 0.f);
				EffectDesc.fDeadTime = 2.f;
				EffectDesc.vColor = XMVectorSet(255, 63, 255, 255);
				EffectDesc.vInitScale = _float3(1.f, 1.f, 1.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

				EffectDesc.eEffectID = CMonsterEffect::GLOW_SPHERE;
				EffectDesc.vDistance = XMVectorSet((rand() % 200 * 0.01f) - 1.f, (rand() % 200 * 0.01f) - 1.f, 0.f, 0.f);
				EffectDesc.iTextureNum = rand() % 2 == 0 ? 0 : 2;
				EffectDesc.fDeadTime = 1.0f;
				EffectDesc.vInitScale = _float3(0.0f, 0.0f, 0.0f);
				EffectDesc.vColor = XMVectorSet(226, 0, 255, 255);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

				EffectDesc.eEffectID = CFightEffect::DEAD_FLASH;
				EffectDesc.vDistance = XMVectorSet((rand() % 200 * 0.01f) - 1.f, (rand() % 200 * 0.01f) - 1.f, 0.f, 0.f);
				EffectDesc.iTextureNum = rand() % 2 + 3;
				EffectDesc.fDeadTime = 1.0f;
				_float iRandNum = rand() % 20 * 0.1f + 0.5f;
				EffectDesc.vInitScale = _float3(iRandNum, iRandNum, iRandNum);
				EffectDesc.vColor = XMVectorSet(226, 0, 255, 255);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

				m_fEffectTime = 0.f;
			}

			dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_ZoomValue(m_fZoomValue);

			m_fZoomValue -= 0.1f;
			if (m_fZoomValue <= -1.f)
				m_fZoomValue = -1.f;

		}
		m_fAlpha += 0.025f;
		m_fAnimSpeed = 1.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_eState = DEAD;
			Make_DeadEffect();
			m_fZoomValue = 0.f;
			dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_ZoomValue(m_fZoomValue);
			m_bDead = true;

			g_fBGMVolume = 0.f;
			CGameInstance::Get_Instance()->PlayBGM(TEXT("1-20 Level 1 - Tail Cave.mp3"), g_fBGMVolume);

		}
		break;
	case Client::CTailBoss::APPEAR:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	case Client::CTailBoss::WAIT_MOVE:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	case Client::CTailBoss::IDLE:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_fEffectEndTime = 0.3f;
		m_fEffectTime += fTimeDelta;
		if (m_fEffectTime > m_fEffectEndTime)
		{
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CMonsterEffect::ROLA_SMOKE;
			EffectDesc.fDeadTime = 0.5f;
			EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
			EffectDesc.vInitScale = _float3(2.f, 2.f, 0.f);

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVector3Normalize(Get_TransformState(CTransform::STATE_RIGHT))*Get_Scale().x + XMVectorSet(0.f, 0.2f, 0.f, 0.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) - XMVector3Normalize(Get_TransformState(CTransform::STATE_RIGHT))*Get_Scale().x + XMVectorSet(0.f, 0.2f, 0.f, 0.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
			m_fEffectTime = 0.f;
			break;
		}

		m_fSoundTime += fTimeDelta;
		if (m_fSoundTime > 2.f && m_bOpeningMotion)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_TailBoss_Move.wav"), SOUND_ACTOR, 0.05f);
			m_fSoundTime = 0.f;
		}
	default:
		break;
	}
}

HRESULT CTailBoss::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	switch (m_TailDesc.eTailType)
	{
	case TAIL1:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_TailBoss1"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case TAIL2:
	case TAIL3:
	case TAIL4:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_TailBoss2"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case TAIL5:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_TailBoss3"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	default:
		break;
	}


	///* For.Com_OBB*/
	CCollider::COLLIDERDESC		ColliderDesc;
	//ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	//ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	//ColliderDesc.vPosition = _float3(0.f, 0.0f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
	//	return E_FAIL;

	/* For.Com_SHPERE */
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	if (m_TailDesc.eTailType == TAIL5)
		ColliderDesc.vScale = _float3(0.5f, 1.5f, 0.5f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);


	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;



	/* For.Com_Navigation */
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iCurrentCellIndex = 0;
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TailCave"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	return S_OK;
}


HRESULT CTailBoss::SetUp_ShaderID()
{
	if (m_eState == DEAD)
		m_eShaderID = SHADER_ANIMDEAD;
	else if (m_bHit)
		m_eShaderID = SHADER_ANIMHIT;
	else
		m_eShaderID = SHADER_ANIMDEFAULT;

	return S_OK;
}

_bool CTailBoss::IsDead()
{
	if (m_bDead && m_eState == STATE::DEAD)//&& m_dwDeathTime + 1000 < GetTickCount())
		return true;
	else if (m_bDead && m_eState != STATE::DEAD)
	{
		m_dwDeathTime = GetTickCount();
		m_eState = STATE::DEAD;
	}

	return false;
}

void CTailBoss::Make_GuardEffect(CBaseObj * Target)
{

	_vector vDirection =  XMVector3Normalize(Target->Get_TransformState(CTransform::STATE_POSITION) - Get_TransformState(CTransform::STATE_POSITION));

	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CFightEffect::GUARD_FLASH;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + vDirection*1.5f + XMVectorSet(0.f, Get_Scale().y, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_POSITION) - Target->Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.vInitScale = _float3(1.5f, 1.5f, 1.5f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	EffectDesc.eEffectID = CFightEffect::GUARD_RING;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + vDirection*0.8f + XMVectorSet(0.f, Get_Scale().y*0.5f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_POSITION) - Target->Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.vInitScale = _float3(1.0f, 1.0f, 1.0f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);


	for (int i = 0; i < 10; ++i)
	{
		EffectDesc.eEffectID = CFightEffect::GUARD_DUST;
		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + vDirection + XMVectorSet(0.f, Get_Scale().y - 0.4f, 0.f, 0.f);
		_float iRandNum = (rand() % 10 + 10) * 0.1f;
		EffectDesc.vInitScale = _float3(iRandNum, iRandNum, iRandNum);
		EffectDesc.fDeadTime = 0.8f;
		CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	}
}




void CTailBoss::AI_Behaviour(_float fTimeDelta)
{


	if (!m_bMove || m_eState == DAMAGE || m_eState == DEAD || m_eState == FEINT)
		return;


	if (m_TailDesc.eTailType == TAIL1)
		Behaviour_Head(fTimeDelta);
	else
		Behaviour_Tail(fTimeDelta);

}

void CTailBoss::Behaviour_Head(_float fTimeDelta)
{
	if (m_eState == FEINT)
		return;

	Find_Target();
	if (!m_bStartIntro && m_fDistanceToTarget < 5.f)
	{
		m_bStartIntro = true;
		m_eState = IDLE;
	}

	if (m_eState == APPEAR)
		return;

	if (!m_bIsAttacking && m_pSPHERECom->Collision(m_pTarget->Get_Collider()) == true)
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("3_TailBoss_Attack.wav"), SOUND_MONSTER, 0.5f);
		m_bAggro = true;
		m_bIsAttacking = true;

		CPlayer::ANIM ePlayerState = dynamic_cast<CPlayer*>(m_pTarget)->Get_AnimState();

		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_pTarget->Get_TransformState(CTransform::STATE_POSITION);
		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
		m_pTransformCom->Go_PosDir(fTimeDelta*1.5f, vDirection);

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180);
		m_fTurnAngle = (rand() % 2000) * 0.001f + 0.1f;
		m_fTurnAngle = rand() % 2 == 0 ? m_fTurnAngle : -m_fTurnAngle;
		if (m_TailDesc.eTailType == TAIL1)
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Guard_0.wav"), SOUND_MEFFECT, 0.5f);

		if (ePlayerState == CPlayer::SHIELD_HOLD_LP || ePlayerState == CPlayer::SHIELD)
		{
			dynamic_cast<CPlayer*>(m_pTarget)->Set_AnimState(CPlayer::SHIELD_HIT);
			dynamic_cast<CPlayer*>(m_pTarget)->Make_GuardEffect(this);
		}
		else
		{

			Make_GetAttacked_Effect(m_pTarget);
			dynamic_cast<CPlayer*>(m_pTarget)->Take_Damage(1.f, nullptr, this);
		}


	}
	else
	{
		m_bChangeDirection = m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		if (m_bChangeDirection == false)
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 180);
			m_fTurnAngle = rand() % 2 == 0 ? 3.f : -3.f;
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Guard_0.wav"), SOUND_MEFFECT, 0.5f);
		}

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnAngle);
	}
}

void CTailBoss::Behaviour_Tail(_float fTimeDelta)
{
	if (m_TailDesc.pParent == nullptr)
		return;

	if (m_TailDesc.eTailType != TAIL1)
	{
		m_eState = m_TailDesc.pParent->Get_AnimState();
		_vector vPosition = m_TailDesc.pParent->Get_TransformState(CTransform::STATE_POSITION) - m_TailDesc.pParent->Get_TransformState(CTransform::STATE_LOOK);
		m_pTransformCom->LookAt(vPosition);
		m_pTransformCom->Fix_Look_byFloor(XMVectorSet(0.f, 1.f, 0.f, 0.f));
		m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

	}

}

HRESULT CTailBoss::Create_Tail(TAILTYPE eType)
{
	if (eType == TAIL5)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance)

		CTailBoss::TAILDESC TailDesc;
	TailDesc.eTailType = TAILTYPE(eType + 1);
	TailDesc.InitPostion = m_TailDesc.InitPostion;
	TailDesc.pParent = this;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_TailBoss"), LEVEL_GAMEPLAY, TEXT("Layer_Monster"), &TailDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CTailBoss::Opening_Motion(_float fTimeDelta)
{

	if (m_bOpeningMotion)
		return;

	if (m_TailDesc.eTailType == TAIL1)
		Opening_MotionHead(fTimeDelta);
	else
		Opening_MotionTail(fTimeDelta);

}

void CTailBoss::Opening_MotionHead(_float fTimeDelta)
{
	if (Check_IsinFrustum() == false)
		return;
	else if (!m_bSoundPlay && m_TailDesc.eTailType == TAIL1)
	{
		g_fBGMVolume -= 0.01f;
		if (g_fBGMVolume <= 0.f)
		{
			g_fBGMVolume = 0.f;
			CGameInstance::Get_Instance()->StopSound(SOUND_BGM);

		}
	}


	Find_Target();
	if (!m_bStartIntro && !m_bFinishIntro &&  XMVectorGetZ(Get_TransformState(CTransform::STATE_POSITION)) - XMVectorGetZ(m_pTarget->Get_TransformState(CTransform::STATE_POSITION)) < 5.f)
	{
		m_bStartIntro = true;
		m_eState = IDLE;
	}

	CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_ZoomValue(m_fZoomValue);

	if (m_bStartIntro && !m_bFinishIntro)
	{
		m_fZoomValue -= 0.1f;
		if (m_fZoomValue <= -1.f)
			m_fZoomValue = -1.f;

		_float fHeight = m_pNavigationCom->Compute_Height(Get_TransformState(CTransform::STATE_POSITION), Get_Scale().y*0.25f);
		_float fPositionY = XMVectorGetY(Get_TransformState(CTransform::STATE_POSITION));
		if (fPositionY > fHeight)
			m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, -1.f, 0.f, 0.f));
		else
		{
			if (m_fNameBoxTime == 0.f)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("3_TailBoss_Appear.wav"), SOUND_MEFFECT, 0.5f);
				CEffect::EFFECTDESC EffectDesc;
				EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
				EffectDesc.eEffectID = CMonsterEffect::ROLA_SMOKE;
				EffectDesc.fDeadTime = 0.5f;
				EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
				EffectDesc.vInitScale = _float3(2.f, 2.f, 0.f);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(1.f, 0.2f, 1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(1.f, 0.2f, -1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-1.f, 0.2f, 1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-1.f, 0.2f, -1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
			}
			m_fNameBoxTime += fTimeDelta;

			if (m_fNameBoxTime > 1.f)
			{
				CUIName* pUIName = dynamic_cast<CUIName*>(CUI_Manager::Get_Instance()->Get_NameBox());
				pUIName->Set_TexType(CUIName::NAME_TAILBOSS);
				pUIName->Set_Position(_float2(g_iWinSizeX*0.5f, g_iWinSizeY*0.5f + 200.f));
				pUIName->Set_Visible(true);
				CUI_Manager::Get_Instance()->Set_NameTimeZero();
				m_bFinishIntro = true;
			}

		}
	}


	if (m_bFinishIntro)
	{
		CUIName* pUIName = dynamic_cast<CUIName*>(CUI_Manager::Get_Instance()->Get_NameBox());
		if (pUIName->Get_Visible() == false)
		{
			m_fZoomValue += 0.1f;
			if (m_fZoomValue >= 0.f)
			{
				m_bOpeningMotion = true;
				m_fZoomValue = 0.f;
				m_bSoundPlay = true;
				CGameInstance::Get_Instance()->PlayBGM(TEXT("1-23 Boss Battle.mp3"), g_fBGMVolume);
			}

		}
	}
}

void CTailBoss::Opening_MotionTail(_float fTimeDelta)
{
	if (m_TailDesc.pParent == nullptr)
		return;

	if (m_TailDesc.eTailType != TAIL1)
	{
		_vector vPosition = m_TailDesc.pParent->Get_TransformState(CTransform::STATE_POSITION);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		m_bOpeningMotion = m_TailDesc.pParent->Get_IsOpening();
	}
}



_uint CTailBoss::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{
	if (m_eState == DEAD || m_eState == STATE::DAMAGE || m_eState == GUARD)
		return 0;

	if (m_TailDesc.eTailType == TAIL5)
	{
		_uint fHp = __super::Take_Damage(fDamage, DamageType, DamageCauser);

		if (fHp > 0)
		{
			if (!m_bDead)
			{
				m_bHit = true;
				m_eState = STATE::DAMAGE;
				m_bMove = true;
			}

			Make_GetAttacked_Effect();

			_uint iNum = rand() % 2 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.5f;
			wcscpy_s(sz_Sound, TEXT("3_TailBoss_Damage%d.wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);

			m_bAggro = true;
			m_bIsAttacking = false;
			m_dwAttackTime = GetTickCount();
			Change_Parent_State(m_eState);
			return fHp;
		}
		else
		{
			m_eState = STATE::DEAD;
			m_fAlpha = 0.f;

			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_TailBoss_Dead.wav"), SOUND_MONSTER, 0.5f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Explosion.wav"), SOUND_ACTOR, 0.5f);
		}

		Change_Parent_State(m_eState);
	}
	else
	{
		m_eState = STATE::FEINT;
		_uint iNum = rand() % 4 + 1;
		_tchar	sz_Sound[MAX_PATH];
		_float fVolume = 0.3f;
		wcscpy_s(sz_Sound, TEXT("Guard_Metal_Metal_%d.wav"));
		wsprintf(sz_Sound, sz_Sound, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MEFFECT, 0.5f);

		Change_Parent_State(m_eState);
		CBaseObj* pPlayer = dynamic_cast<CBaseObj*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		Make_GuardEffect(pPlayer);
	}

	return 0;
}

void CTailBoss::Change_Parent_State(STATE TailState)
{
	m_eState = TailState;

	if (TailState == DAMAGE)
	{
		m_fHitRedColor -= 0.05f;
		m_bHit = true;
	}

	if (TailState == DEAD)
		m_fAlpha = 0.f;

	if (m_TailDesc.pParent == nullptr)
		return;

	dynamic_cast<CTailBoss*>(m_TailDesc.pParent)->Set_AnimState(m_eState);
	dynamic_cast<CTailBoss*>(m_TailDesc.pParent)->Change_Parent_State(m_eState);
}

CTailBoss * CTailBoss::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTailBoss*	pInstance = new CTailBoss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTailBoss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTailBoss::Clone(void * pArg)
{
	CTailBoss*	pInstance = new CTailBoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTailBoss"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTailBoss::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSPHERECom);

}

