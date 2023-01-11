#include "stdafx.h"
#include "..\Public\Rola.h"
#include "Player.h"
#include "CameraManager.h"
#include "MonsterBullet.h"
#include "ObjectEffect.h"
#include "MonsterEffect.h"
#include "FightEffect.h"
#include "UIIcon.h"

CRola::CRola(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

HRESULT CRola::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRola::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tInfo.iMaxHp = 10;
	m_tInfo.iDamage = 20;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;

	m_fAttackRadius = 1.f;
	m_fPatrolRadius = 10.f;
	m_eMonsterID = MONSTER_ROLA;

	_vector vecPostion = XMLoadFloat3((_float3*)pArg);
	vecPostion = XMVectorSetW(vecPostion, 1.f);
	vecPostion = XMVectorSetZ(vecPostion, XMVectorGetZ(vecPostion) - 1.5f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	Set_Scale(_float3(1.2f, 1.2f, 1.2f));
	m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pTransformCom->LookDir(XMVectorSet(-1.f, 0.f, 0.f, 0.f));


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CMonsterBullet::BULLETDESC BulletDesc;
	BulletDesc.eOwner = MONSTER_ROLA;
	BulletDesc.eBulletType = CMonsterBullet::ROLA;
	BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + Get_TransformState(CTransform::STATE_LOOK) + XMVectorSet(0.f,0.5f,0.f,0.f);
	BulletDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
	BulletDesc.vLook = XMVectorSetY(BulletDesc.vLook, 0.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MonsterBullet"), LEVEL_TAILCAVE, TEXT("Layer_RulaRolling"), &BulletDesc)))
		return E_FAIL;

	CMonsterBullet* pRollingBullet = dynamic_cast<CMonsterBullet*>(pGameInstance->Get_Object(LEVEL_TAILCAVE, TEXT("Layer_RulaRolling")));
	if (m_eAttackDir == RIGHT)
		m_vAttackPos = pRollingBullet->Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(2.f, 0.f, 0.f, 0.f);
	else
		m_vAttackPos = pRollingBullet->Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-2.f, 0.f, 0.f, 0.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vAttackPos);



	CUIIcon::ICONDESC IconDesc;
	IconDesc.iTexureNum = CUIIcon::ICON_BOSS;
	IconDesc.pTarget = this;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_TAILCAVE, TEXT("UI_ICON"), &IconDesc)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

int CRola::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;


	AI_Behaviour(fTimeDelta);
	Check_Navigation(fTimeDelta);
	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
	}
	Change_Animation(fTimeDelta);

	return OBJ_NOEVENT;
}

void CRola::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


	if (m_pTarget != nullptr && true == m_pSPHERECom->Collision(m_pTarget->Get_Collider()) && m_eState == JUMP_ED)
	{
		CPlayer::ANIM ePlayerState = dynamic_cast<CPlayer*>(m_pTarget)->Get_AnimState();
		if (ePlayerState == CPlayer::SHIELD_HOLD_LP || ePlayerState == CPlayer::SHIELD)
		{
			Take_Damage(1.f, nullptr, nullptr);
			dynamic_cast<CPlayer*>(m_pTarget)->Set_AnimState(CPlayer::SHIELD_HIT);
			m_bIsAttacking = false;
		}
		else
		{
			m_dwAttackTime = GetTickCount();
			m_dwIdleTime = GetTickCount();
			m_bIsAttacking = false;

			CMonsterBullet::BULLETDESC BulletDesc;
			BulletDesc.eOwner = MONSTER_ROLA;
			BulletDesc.eBulletType = CMonsterBullet::DEFAULT;
			BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
			BulletDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
			dynamic_cast<CPlayer*>(m_pTarget)->Take_Damage(1.f, &BulletDesc, nullptr);
			m_bBackStep = true;

		}
	}
}

HRESULT CRola::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CRola::Change_Animation(_float fTimeDelta)
{
	CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();

	switch (m_eState)
	{
	case Client::CRola::IDLE:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta * m_fAnimSpeed, m_bIsLoop);
		break;
	case Client::CRola::PUSH:
		if(m_dwAttackTime + 1000 < GetTickCount())
			Make_PushEffect();

		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_eAttackDir = m_eAttackDir == RIGHT ? LEFT : RIGHT;
			m_eState = IDLE;
			m_bIsAttacking = false;
			m_bSpecialAttack = false;
			m_bHit = false;
			m_bMakeEffect = false;
		}
		break;
	case Client::CRola::DAMAGE:
		m_bIsLoop = false;
		_vector vDir = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_pTarget->Get_TransformState(CTransform::STATE_POSITION);

		m_pTransformCom->Go_PosDir(fTimeDelta*0.5f, vDir);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bMakeEffect = false;
			m_eState = JUMP_ST;
			m_bIsAttacking = false;
			m_bHit = false;
		}
		break;		break;
	case Client::CRola::DEAD:
		g_fBGMVolume -= 0.005f;
		if (g_fBGMVolume <= 0.f)
		{
			CGameInstance::Get_Instance()->StopSound(SOUND_BGM);
		}
		dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_ZoomValue(m_fZoomValue);

		m_fZoomValue += 0.1f;
		if (m_fZoomValue >= 0.f)
			m_fZoomValue = 0.f;

		m_fAnimSpeed = 1.f;
		m_fAlpha += 0.01f;
		Make_DeadEffect();
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_fZoomValue = 0.f;
			g_fBGMVolume = 0.f;
			CGameInstance::Get_Instance()->PlayBGM(TEXT("1-20 Level 1 - Tail Cave.mp3"), g_fBGMVolume);
			dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_ZoomValue(m_fZoomValue);
			m_bDead = true;
		}
		break;
	case Client::CRola::DEAD_ST:
		g_fBGMVolume -= 0.005f;
		if (g_fBGMVolume <= 0.f)
		{
			CGameInstance::Get_Instance()->StopSound(SOUND_BGM);
		}

		dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_ZoomValue(m_fZoomValue);
		m_fAlpha += 0.01f;
		m_fZoomValue -= 0.1f;
		if (m_fZoomValue <= -1.f)
			m_fZoomValue = -1.f;

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
		m_fAnimSpeed = 0.5f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
			m_eState = DEAD;
		break;
	case Client::CRola::JUMP_ST:
		m_fAnimSpeed = 3.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			if (!m_bSpecialAttack &&  m_iDmgCount%4 != 3)
			{
				m_vDirection = m_pTarget->Get_TransformState(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			}
			m_vDirection = XMVectorSetY(m_vDirection, 0.f);
			m_eState = JUMP;
		}

		break;
	case Client::CRola::JUMP:
		m_fAnimSpeed = 3.f;
		m_bIsLoop = false;
		if (m_iDmgCount % 4 != 3 && !m_bBackStep)
			Jumping_Attack(fTimeDelta);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Rola_Jump_End.wav"), SOUND_MEFFECT, 0.5f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Rola_Boom (1).wav"), SOUND_ACTOR, 0.5f);

			m_eState = JUMP_ED;


			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CMonsterEffect::ROLA_SMOKE;
			EffectDesc.fDeadTime = 0.5f;
			EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
			EffectDesc.vInitScale = _float3(2.f, 2.f, 0.f);

			if (rand() % 2 == 0)
			{
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(1.f, 0.2f, 1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(1.f, 0.2f, -1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-1.f, 0.2f, 1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-1.f, 0.2f, -1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

			}
			else
			{
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(1.f, 0.2f, 0.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 0.2f, -1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-1.f, 0.2f, 0.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 0.2f, -1.f, 0.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
			}
		
		}
		break;
	case Client::CRola::JUMP_ED:
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_CamMode(CCamera_Dynamic::CAM_SHAKING, 0.1f, 0.1f, 0.01f);
		m_fAnimSpeed = 3.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta* m_fAnimSpeed, m_bIsLoop))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Rola_Jump_Start.wav"), SOUND_MEFFECT, 0.5f);

			m_eState = JUMP_ST;
		}
		break;
	}
	default:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);
		break;
	}

	m_fAnimSpeed = 1.f;
}

HRESULT CRola::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(3.f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_Rola"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;


	/* For.Com_SHPERE */
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
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


_bool CRola::IsDead()
{
	if (m_bDead && m_eState == STATE::DEAD)//&& m_dwDeathTime + 1000 < GetTickCount())
		return true;
	else if (m_bDead && m_eState != STATE::DEAD && m_eState != STATE::DEAD_ST)
	{
		m_dwDeathTime = GetTickCount();
		m_eState = STATE::DEAD_ST;
	}

	return false;
}

void CRola::Find_Target()
{
	if (!m_bHit && !m_bDead)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);

		if (pPlayer)
		{
			if (pPlayer->Get_Dead())
			{
				if (m_bAggro)
				{
					m_pTarget = nullptr;
					m_eState = STATE::IDLE;
					m_bAggro = false;
				}
				return;
			}

			if (pTarget)
			{
				CTransform* PlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
				_vector vTargetPos = PlayerTransform->Get_State(CTransform::STATE_POSITION);
				m_fDistanceToTarget = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_POSITION) - vTargetPos));
				m_pTarget = dynamic_cast<CBaseObj*>(pTarget);
			}
			else
				m_pTarget = nullptr;
		}
	}
}

void CRola::Follow_Target(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return;

	_vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(vTargetPos);
	m_pTransformCom->Go_Straight(fTimeDelta*1.5f, m_pNavigationCom);
}

void CRola::AI_Behaviour(_float fTimeDelta)
{
	if (Check_IsinFrustum() == false)
		return;
	else if(!m_bSoundPlay)
	{
		g_fBGMVolume -= 0.01f;
		if (g_fBGMVolume <= 0.f)
		{
			CGameInstance::Get_Instance()->StopSound(SOUND_BGM);
			CGameInstance::Get_Instance()->PlayBGM(TEXT("1-22 Mini Boss Battle.mp3"), g_fBGMVolume);
			m_bSoundPlay = true;
		}
	}

	if (!m_bMove || m_eState == DEAD || m_eState == DEAD_ST || m_bHit || m_bIsAttacking)
		return;

	// Check for Target, AggroRadius
	Find_Target();
	if (m_fDistanceToTarget > 10.f)
	{
		m_eState = IDLE;
		return;
	}


	if (m_bBackStep)
	{
		if (m_fDistanceToTarget > 3.f)
		{
			m_iDmgCount = 3;
			m_bBackStep = false;
		}
			

		m_pTransformCom->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_POSITION));
		m_pTransformCom->Go_Backward(fTimeDelta*0.5f, m_pNavigationCom);
		return;
	}

	if (m_iDmgCount % 4 == 3 && m_fDistanceToTarget < m_fPatrolRadius)
	{
		Moving_AttackPosition(fTimeDelta);

		if (m_bSpecialAttack == true)
		{
			if (!m_bIsAttacking && GetTickCount() > m_dwAttackTime + 2000)
			{
				m_eState = STATE::PUSH;
				m_dwAttackTime = GetTickCount();
				m_bIsAttacking = true;
				m_iDmgCount++;
				m_bSpecialAttack = false;
			}
			else if (!m_bIsAttacking)
				m_eState = STATE::JUMP_ST;
		}
	}
	else
		Patrol(fTimeDelta);
}

void CRola::Patrol(_float fTimeDelta)
{
	m_bAggro = false;
	if (m_fDistanceToTarget > m_fPatrolRadius)
		return;

	// Switch between Idle and Walk (based on time)

	if (m_eState == STATE::IDLE)
	{
		m_bJump = true;
		m_eState = STATE::JUMP_ST;
	}

}

_bool CRola::Moving_AttackPosition(_float fTimeDelta)
{
	if (m_bSpecialAttack == true)
		return true;


	if (m_eState == STATE::IDLE)
	{
		m_bJump = true;
		m_eState = STATE::JUMP_ST;
	}

	CMonsterBullet* pRollingBullet = dynamic_cast<CMonsterBullet*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_TAILCAVE, TEXT("Layer_RulaRolling")));

	if (m_eAttackDir == RIGHT)
		m_vAttackPos = pRollingBullet->Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(2.f, 0.f, 0.f, 0.f);
	else
		m_vAttackPos = pRollingBullet->Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-2.f, 0.f, 0.f, 0.f);

	_float Distance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_POSITION) - m_vAttackPos));
	if (Distance < 1.f)
	{
		_vector vLook = m_eAttackDir == RIGHT ? XMVectorSet(-1.f, 0.f, 0.f, 0.f) : XMVectorSet(1.f, 0.f, 0.f, 0.f);
		if (0.1 < fabs(XMVectorGetX((Get_TransformState(CTransform::STATE_LOOK) - vLook))))
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 1.f);
		else
		{
			m_pTransformCom->LookDir(vLook);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vAttackPos);
			m_bSpecialAttack = true;
			m_bJump = false;
		}


	}
	else
	{
		if (m_eState == JUMP_ED || m_eState == JUMP_ST)
		{
			m_vDirection = pRollingBullet->Get_TransformState(CTransform::STATE_POSITION) - Get_TransformState(CTransform::STATE_POSITION);
			m_vDirection = XMVectorSetY(m_vDirection, 0.f);
			m_pTransformCom->LookDir(m_vDirection);
			return false;
		}

		m_pTransformCom->Go_PosTarget(fTimeDelta * 2, m_vAttackPos);

	}

	return false;
}

void CRola::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::DROP)
		m_eState = IDLE;
	else if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ACCESSIBLE)
	{
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float fHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);
		
		vPosition = XMVectorSetY(vPosition, fHeight);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		
	}
}

void CRola::Jumping_Attack(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return;

	_vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_POSITION);

	m_pTransformCom->LookDir(m_vDirection);
	m_pTransformCom->Go_Straight(fTimeDelta*1.5f, m_pNavigationCom);
}

void CRola::Make_PushEffect()
{
	if (m_bMakeEffect)
		return;

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Guard_0.wav"), SOUND_MEFFECT, 0.5f);

	m_bMakeEffect = true;
	CMonsterBullet* pRollingBullet = dynamic_cast<CMonsterBullet*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_TAILCAVE, TEXT("Layer_RulaRolling")));
	pRollingBullet->Set_Rolling(m_eAttackDir);


	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CMonsterEffect::ROLA_PUSHRING;
	EffectDesc.fDeadTime = 1.f;
	EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
	EffectDesc.vInitScale = _float3(1.5f, 1.5f, 1.5f);
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);

	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook + XMVectorSet(0.f, 0.5f, 0.5f, 0.f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook + XMVectorSet(0.f, 0.5f, -0.5f, 0.f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.eEffectID = CMonsterEffect::ROLA_PUSHSPARK;
	EffectDesc.fDeadTime = 1.f;
	EffectDesc.iTextureNum = 3;
	EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
	EffectDesc.vInitScale = _float3(3.f, 3.f, 0.f);
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);

	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook*1.5f + XMVectorSet(0.f, 0.6f, 0.5f, 0.f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook*1.5f + XMVectorSet(0.f, 0.6f, -0.5f, 0.f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	EffectDesc.eEffectID = CMonsterEffect::ROLA_PUSHFLASH;
	EffectDesc.iTextureNum = 1;
	EffectDesc.vInitScale = _float3(3.f, 3.f, 0.f);
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook*1.5f + XMVectorSet(0.f, 0.6f, 0.5f, 0.f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook*1.5f + XMVectorSet(0.f, 0.6f, -0.5f, 0.f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

}

HRESULT CRola::SetUp_ShaderID()
{
	if (m_eState == DEAD || m_eState == DEAD_ST)
		m_eShaderID = SHADER_ANIMDEAD;
	else if (m_bHit)
		m_eShaderID = SHADER_ANIMHIT;
	else
		m_eShaderID = SHADER_ANIMDEFAULT;

	return S_OK;
}



_uint CRola::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{
	if (m_eState == DAMAGE)
		return 0;

	_uint iHp = __super::Take_Damage(fDamage, DamageType, DamageCauser);

	if (iHp > 0)
	{
		if (!m_bDead)
		{
			m_bHit = true;
			m_iDmgCount++;
			m_eState = STATE::DAMAGE;
			m_bMove = true;
		}

		_uint iNum = rand() % 3 + 1;
		_tchar	sz_Sound[MAX_PATH];
		_float fVolume = 0.5f;
		wcscpy_s(sz_Sound, TEXT("3_Monster_Rola_Damage (%d).wav"));
		wsprintf(sz_Sound, sz_Sound, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);

		m_bAggro = true;
		m_bIsAttacking = false;
		m_dwAttackTime = GetTickCount();

		return iHp;
	}
	else
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Rola_Dead.wav"), SOUND_MONSTER, 0.5f);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Explosion.wav"), SOUND_ACTOR, 0.5f);

		m_eState = STATE::DEAD_ST;
		m_fAlpha = 0.f;
	}
		

	return 0;
}

CRola * CRola::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRola*	pInstance = new CRola(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRola"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRola::Clone(void * pArg)
{
	CRola*	pInstance = new CRola(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRola"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRola::Free()
{
	__super::Free();
}

