#include "stdafx.h"
#include "..\Public\Togezo.h"
#include "Player.h"
#include "FightEffect.h"
#include "MonsterBullet.h"
#include "ObjectEffect.h"

CTogezo::CTogezo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

HRESULT CTogezo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTogezo::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tInfo.iMaxHp = 5;
	m_tInfo.iDamage = 4;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;

	m_fAttackRadius = 1.5f;
	m_eMonsterID = MONSTER_PAWN;

	_vector vecPostion = XMLoadFloat3((_float3*)pArg);
	vecPostion = XMVectorSetW(vecPostion, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	
	m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));


	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);

	return S_OK;
}

int CTogezo::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
	{
		Drop_Items();
		return OBJ_DEAD;
	}

	if (Check_IsinFrustum() == false)
		return OBJ_NOEVENT;


	AI_Behaviour(fTimeDelta);
	Check_Navigation(fTimeDelta);

	m_pModelCom->Set_NextAnimIndex(m_eState);
	Change_Animation(fTimeDelta);


	return OBJ_NOEVENT;
}

void CTogezo::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (Check_IsinFrustum() == false)
		return;

	if (m_pTarget != nullptr && m_pOBBCom->Collision(m_pTarget->Get_Collider()))
	{
		
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pTarget);
		CPlayer::ANIM ePlayerState = pPlayer->Get_AnimState();
		
		if (ePlayerState == CPlayer::SHIELD_HOLD_LP)
		{
			if (m_eState != REBOUND_ST)
			{
				pPlayer->Set_AnimState(CPlayer::SHIELD_HIT);
				pPlayer->Make_GuardEffect(this);
				m_eState = REBOUND_ST;
				m_dwStunTime = GetTickCount();
			}
		
		}
		else if (m_eState != REBOUND_ST && m_eState != STUN && m_eState != STUN_ED &&  m_eState != IDLE)
		{
			m_eState = RUN_ED;

			CMonsterBullet::BULLETDESC BulletDesc;
			BulletDesc.eOwner = MONSTER_TOGEZO;
			BulletDesc.eBulletType = CMonsterBullet::DEFAULT;
			BulletDesc.vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

			Make_GetAttacked_Effect(m_pTarget);
			dynamic_cast<CPlayer*>(m_pTarget)->Take_Damage(1.f, &BulletDesc, nullptr);
		}
			
	
	}
		
	CBaseObj* pCollisionBlock = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_BLOCK, m_pOBBCom, &pCollisionBlock))
	{
		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pCollisionBlock->Get_TransformState(CTransform::STATE_POSITION);
		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
		m_pTransformCom->Go_PosDir(fTimeDelta*1.5f, vDirection, m_pNavigationCom);
	}

	CBaseObj* pCollisionMonster = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pOBBCom, &pCollisionMonster))
	{

		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pCollisionMonster->Get_TransformState(CTransform::STATE_POSITION);
		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
		m_pTransformCom->Go_PosDir(fTimeDelta, vDirection, m_pNavigationCom);
	}
	
}

HRESULT CTogezo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

void CTogezo::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::DROP)
	{
		if (m_eState == DAMAGE || m_eState == RUN)
		{
			m_eState = DEADFALL;
			return;
		}

		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_pNavigationCom->Get_CurrentCellCenter();
		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
		m_pTransformCom->Go_PosDir(fTimeDelta*1.5f, vDirection, m_pNavigationCom);
	}
	else if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ACCESSIBLE)
	{
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float fHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);
		if (fHeight > XMVectorGetY(vPosition))
		{
			vPosition = XMVectorSetY(vPosition, fHeight);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		}
			
	}
		
}

void CTogezo::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CTogezo::IDLE:
		m_bMakeEffect = false;
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	case Client::CTogezo::WALK:
		m_bMakeEffect = false;
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);

		m_fSoundTime += fTimeDelta;
		if (m_fSoundTime > 0.5f)
		{
			_uint iNum = rand() % 2 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.2f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Step_%d.wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_ACTOR, fVolume);

			m_fSoundTime = 0.f;
		}

		break;
	case Client::CTogezo::STUN:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);

		if (m_dwStunTime +5000 < GetTickCount())
			m_eState = STUN_ED;
		break;
	case Client::CTogezo::RUN_ST:
		m_pTransformCom->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_POSITION));
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_eState = RUN;
		}
		break;
	case Client::CTogezo::RUN:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);

		m_fSoundTime += fTimeDelta;
		if (m_fSoundTime > 0.2f)
		{
			_uint iNum = rand() % 2 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.2f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Step_%d.wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_ACTOR, fVolume);

			m_fSoundTime = 0.f;
		}

		m_fEffectEndTime = 0.1f;
		m_fEffectTime += fTimeDelta;
		if (m_fEffectTime > m_fEffectEndTime)
		{
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CObjectEffect::SMOKE;
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 0.2f, 0.f, 0.f);
			EffectDesc.fDeadTime = 0.1f;
			EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
			EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
			m_fEffectTime = 0.f;
		}

		if (m_pTransformCom->Go_Straight(fTimeDelta*2, m_pNavigationCom) == false)
			m_eState = RUN_ED;
		break;
	case Client::CTogezo::RUN_ED:
	case Client::CTogezo::STUN_ED:
	case Client::CTogezo::DISCOVER:
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bMakeEffect = false;
			m_bIsAttacking = false;
			m_dwAttackTime = GetTickCount();
			m_eState = IDLE;
		}		
		break;
	case Client::CTogezo::REBOUND_ST:
		m_bIsLoop = false;
		m_pTransformCom->Go_Backward(fTimeDelta, m_pNavigationCom);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
			m_eState = STUN;
		break;
	case Client::CTogezo::DEAD:
	case Client::CTogezo::DEADFIRE:
	{
		Make_DeadEffect();
		m_fAnimSpeed = 1.f;
		m_bIsLoop = false;
		m_pTransformCom->Go_PosDir(fTimeDelta * 2, XMVectorSet(0.f, -0.1f, 0.f, 0.f), m_pNavigationCom);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bDead = true;
		}
		break;
	}
	case Client::CTogezo::DAMAGE:
	{
		m_fAnimSpeed = 3.f;
		_vector vDir = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_pTarget->Get_TransformState(CTransform::STATE_POSITION);
		vDir = XMVectorSetY(vDir, 0.f);
		m_pTransformCom->Go_PosDir(fTimeDelta*2, vDir, m_pNavigationCom);
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bMakeEffect = false;
			m_dwStunTime = GetTickCount();
			m_eState = STUN;
		}	
		break;
	}
	default:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);
		break;
	}
}

HRESULT CTogezo::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_Togezo"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_OBB*/
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _float3(1.0f, 1.0f, 1.0f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.0f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SHPERE */
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
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

HRESULT CTogezo::SetUp_ShaderResources()
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

_bool CTogezo::IsDead()
{
	if (m_bDead && m_eState == STATE::DEADFALL)//&& m_dwDeathTime + 1000 < GetTickCount())
		return true;
	else if (m_bDead && m_eState != STATE::DEADFALL)
	{
		m_dwDeathTime = GetTickCount();
		m_eState = STATE::DEADFALL;
	}

	return false;
}

void CTogezo::Find_Target()
{
	if (!m_bIsAttacking && !m_bHit && !m_bDead)
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

void CTogezo::Follow_Target(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return;

	m_eState = STATE::WALK;

	_vector vTargetPos = (m_pTarget)->Get_TransformState(CTransform::STATE_POSITION);

	m_pTransformCom->LookAt(vTargetPos);
	m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

	m_bIsAttacking = false;
}

void CTogezo::AI_Behaviour(_float fTimeDelta)
{
	if (!m_bMove || m_eState == DAMAGE  || m_eState == DEADFALL || m_eState == DEAD ||  m_eState == DEADFIRE ||
		m_eState == DISCOVER || m_eState == REBOUND || m_eState == REBOUND_ED || m_eState == REBOUND_ST || m_eState == RUN||
		m_eState == RUN_ST || m_eState == RUN_ED || m_eState == STUN || m_eState == STUN_ED )
		return;

	// Check for Target, AggroRadius
	Find_Target();

	if (m_pOBBCom->Collision(m_pTarget->Get_Collider()) == true)
	{
		m_eState = IDLE;
		return;
	}
		
	if (m_bAggro)
	{
		if (m_bIsAttacking)
		{
		
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Togezo_Runst.wav"), SOUND_MONSTER, 0.2f);
			m_eState = STATE::RUN_ST;
		}
		if (!m_bIsAttacking && m_dwAttackTime + 1500 < GetTickCount())
		{
			m_bIsAttacking = true;
			//m_dwAttackTime = GetTickCount();
		}
	}
	
	if (m_pTarget && m_pSPHERECom->Collision(m_pTarget->Get_Collider()) == true)
	{
		m_bAggro = true;
		

		
		if (!m_bIsAttacking)
		{
			m_pTransformCom->LookAt((m_pTarget)->Get_TransformState(CTransform::STATE_POSITION));
			m_eState = STATE::WALK;
			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		}
			
	}
	else
	{
		if (m_bAggro)
		{
			Follow_Target(fTimeDelta);
			m_bIsAttacking = false;
			m_dwAttackTime = GetTickCount();
			m_dwIdleTime = GetTickCount();
		}
		else
			m_eState = STATE::IDLE;
	}
	
}



_uint CTogezo::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{
	if (m_eState == DEAD || m_eState == STATE::DAMAGE || m_eState == STUN_ED)
		return 0;

	if (m_eState == REBOUND || m_eState == STUN || m_eState == STUN_ED)
	{
		_uint fHp = __super::Take_Damage(fDamage, DamageType, DamageCauser);

		if (fHp > 0)
		{
			if (!m_bDead)
			{
				Make_GetAttacked_Effect(DamageCauser);
				m_bHit = true;
				m_eState = STATE::DAMAGE;
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Pawn_Damage.wav"), SOUND_MONSTER, 0.f);
				m_bMove = true;
			}

			m_bAggro = true;
			m_bIsAttacking = false;
			m_dwAttackTime = GetTickCount();

			return fHp;
		}
		else
		{
			m_eState = STATE::DEAD;
			_uint iNum = rand() % 2 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("Pawn_Dead_%d.wav"));
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Explosion.wav"), SOUND_ACTOR, 0.5f);

			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
		}
			
	}
	else
	{
		Make_GuardEffect(m_pTarget);
		m_eState = DISCOVER;

		_uint iNum = rand() % 4;
		_tchar	sz_Sound[MAX_PATH];
		_float fVolume = 0.3f;
		wcscpy_s(sz_Sound, TEXT("Guard_%d.wav"));
		wsprintf(sz_Sound, sz_Sound, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);

	}

	return 0;
}

void CTogezo::Make_GuardEffect(CBaseObj * Target)
{
	if (m_bMakeEffect)
		return;
	m_bMakeEffect = true;

	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CFightEffect::GUARD_FLASH;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_POSITION) - m_pTarget->Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.vInitScale = _float3(1.5f, 1.5f, 1.5f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	EffectDesc.eEffectID = CFightEffect::GUARD_RING;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK))*0.5f + XMVectorSet(0.f, Get_Scale().y*0.5f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_POSITION) - Target->Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.vInitScale = _float3(1.0f, 1.0f, 1.0f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);


	for (int i = 0; i < 10; ++i)
	{
		EffectDesc.eEffectID = CFightEffect::GUARD_DUST;
		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - g_fUIVolume, 0.f, 0.f);
		_float iRandNum = (rand() % 10 + 10) * 0.1f;
		EffectDesc.vInitScale = _float3(iRandNum, iRandNum, iRandNum);
		EffectDesc.fDeadTime = 0.8f;
		CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	}



}

void CTogezo::Make_GetAttacked_Effect(CBaseObj * DamageCauser)
{
	if (m_bMakeEffect)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CBaseObj* pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	CEffect::EFFECTDESC EffectDesc;

	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CFightEffect::HITFLASH;

	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = XMVector3Normalize((pTarget)->Get_TransformState(CTransform::STATE_POSITION) - Get_TransformState(CTransform::STATE_POSITION));
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - 0.2f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(2.5f, 2.5f, 2.5f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	EffectDesc.eEffectID = CFightEffect::HITRING;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - 0.3f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(2.5f, 2.5f, 2.5f);
	EffectDesc.fDeadTime = 0.8f;
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	for (int i = 0; i < 10; ++i)
	{
		EffectDesc.eEffectID = CFightEffect::HITSPARK;
		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - g_fUIVolume, 0.f, 0.f);
		_float iRandNum = (rand() % 10 + 10) * 0.1f;
		EffectDesc.vInitScale = _float3(iRandNum, iRandNum, iRandNum);
		EffectDesc.fDeadTime = 0.8f;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	}



	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.eEffectID = CFightEffect::HITFLASH_TEX;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - 0.15f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.iTextureNum = 0;
	EffectDesc.vInitScale = _float3(2.f, 2.f, 2.2f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	EffectDesc.eEffectID = CFightEffect::HITFLASH_TEX;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - 0.3f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.7f;
	EffectDesc.iTextureNum = 1;
	EffectDesc.vInitScale = _float3(3.5f, 3.5f, 3.5f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	EffectDesc.eEffectID = CFightEffect::HITFLASH_TEX;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - 0.1f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.iTextureNum = 2;
	EffectDesc.vInitScale = _float3(1.0f, 1.0f, 1.0f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	m_bMakeEffect = true;

	RELEASE_INSTANCE(CGameInstance);
}

CTogezo * CTogezo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTogezo*	pInstance = new CTogezo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTogezo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTogezo::Clone(void * pArg)
{
	CTogezo*	pInstance = new CTogezo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTogezo"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTogezo::Free()
{
	__super::Free();
}

