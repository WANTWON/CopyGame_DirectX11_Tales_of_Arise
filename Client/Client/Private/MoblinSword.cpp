#include "stdafx.h"
#include "..\Public\MoblinSword.h"
#include "Player.h"
#include "MonsterBullet.h"
#include "FightEffect.h"
#include "ObjectEffect.h"

CMoblinSword::CMoblinSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

HRESULT CMoblinSword::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMoblinSword::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tInfo.iMaxHp = 5;
	m_tInfo.iDamage = 20;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;

	m_fAttackRadius = 5.f;
	m_fPatrolRadius = 7.f;
	m_eMonsterID = MONSTER_MOBLINSWORD;

	_vector vecPostion = XMLoadFloat3((_float3*)pArg);
	vecPostion = XMVectorSetW(vecPostion, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	Set_Scale(_float3(1.2f, 1.2f, 1.2f));

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//char cName[MAX_PATH];
	//ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//pData_Manager->TCtoC(TEXT("MoblinSword"), cName);
	//pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	////ERR_MSG(TEXT("Save_Bin_Model"));
	//RELEASE_INSTANCE(CData_Manager);

	//RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

int CMoblinSword::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_UI_OpenType() != CUI_Manager::UI_END)
		return OBJ_NOEVENT;

	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;

	if (Check_IsinFrustum(2.f) == false)
		return OBJ_NOEVENT;

	AI_Behaviour(fTimeDelta);

	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
		m_bSoundOnce = false;
		m_bFirst = false;
	}
	Change_Animation(fTimeDelta);
	return OBJ_NOEVENT;
}

void CMoblinSword::Late_Tick(_float fTimeDelta)
{

	if (Check_IsinFrustum(2.f) == false)
		return;

	__super::Late_Tick(fTimeDelta);

	Sound_By_State(fTimeDelta);


	if (m_bAggro&& m_bIsAttacking &&m_pTarget != nullptr 
		&& true == m_pOBBCom->Collision((m_pTarget)->Get_Collider()))
	{
		CPlayer::ANIM ePlayerState = dynamic_cast<CPlayer*>(m_pTarget)->Get_AnimState();
		if (ePlayerState == CPlayer::SHIELD_HOLD_LP || ePlayerState == CPlayer::SHIELD)
		{
			m_eState = STAGGER;
			dynamic_cast<CPlayer*>(m_pTarget)->Set_AnimState(CPlayer::SHIELD_HIT);
			dynamic_cast<CPlayer*>(m_pTarget)->Make_GuardEffect(this);
			m_bIsAttacking = false;
		}
		else
		{
			m_eState = STAGGER;
			m_dwAttackTime = GetTickCount();
			m_dwIdleTime = GetTickCount();
			m_bIsAttacking = false;
			
			CMonsterBullet::BULLETDESC BulletDesc;
			BulletDesc.eOwner = MONSTER_MOBLINSWORD;
			BulletDesc.eBulletType = CMonsterBullet::DEFAULT;
			BulletDesc.vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			
			Make_GetAttacked_Effect(m_pTarget);
			dynamic_cast<CPlayer*>(m_pTarget)->Take_Damage(1.f, &BulletDesc, nullptr);
			
		}
	}

	Check_Navigation(fTimeDelta);


}

HRESULT CMoblinSword::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

void CMoblinSword::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CMoblinSword::KYOROKYORO:
	case Client::CMoblinSword::IDLE:
	case Client::CMoblinSword::STANCE_WAIT:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta * 2, m_bIsLoop);
		m_bMakeEffect = false;
		break;
	case Client::CMoblinSword::WALK:
		m_bIsLoop = true;
		if (m_bIsAttacking)
			m_pModelCom->Play_Animation(fTimeDelta * 4, m_bIsLoop);
		else
			m_pModelCom->Play_Animation(fTimeDelta * 2, m_bIsLoop);
		break;
	case Client::CMoblinSword::DAMAGE_F:
	case Client::CMoblinSword::DAMAGE_B:
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
		{
			m_eState = IDLE;
			m_bHit = false;
			m_bMakeEffect = false;
		}
		break;
	case Client::CMoblinSword::GUARD:
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*2, m_bIsLoop))
		{
			m_eState = IDLE;
			m_bHit = false;
		}
		break;
	case Client::CMoblinSword::FIND:
	case Client::CMoblinSword::STANCE_WALK:
		if (!m_bFirst)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Moriblin_Vo_Attack00.wav"), SOUND_MONSTER, 0.4f);
			m_bFirst = true;
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


			for (int i = 0; i < 3; ++i)
			{
				EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
				EffectDesc.eEffectID = CObjectEffect::GRASS_TEX;
				EffectDesc.vInitScale = _float3(1.f, 1.f, 1.f);
				EffectDesc.fDeadTime = 0.5f;
				EffectDesc.iTextureNum = rand() % 3;
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);

			}
		}


		m_bIsLoop = true;
		m_bHit = false;
		m_pTransformCom->Go_Straight(fTimeDelta * 3, m_pNavigationCom);
		m_pModelCom->Play_Animation(fTimeDelta * 4, m_bIsLoop);
		break;
	case Client::CMoblinSword::STAGGER:
		m_bIsLoop = false;
		m_pTransformCom->Go_Backward(fTimeDelta, m_pNavigationCom);
		if (m_pModelCom->Play_Animation(fTimeDelta * 2, m_bIsLoop))
		{
			m_bIsAttacking = false;
			m_eState = IDLE;
			m_bHit = false;
		}
		break; 
	case Client::CMoblinSword::DEAD_F:
		Make_DeadEffect();
		m_fAlpha += 0.02f;
		m_pTransformCom->Go_Backward(fTimeDelta * 1, m_pNavigationCom);
		m_bIsLoop = false;
		m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, 0.1f, 0.f, 0.f));
		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
			m_bDead = true;
		break;
	case Client::CMoblinSword::DEAD_B:
		Make_DeadEffect();
		m_fAlpha += 0.02f;
		m_pTransformCom->Go_Straight(fTimeDelta * 1, m_pNavigationCom);
		m_bIsLoop = false;
		m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, 0.1f, 0.f, 0.f));
		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
			m_bDead = true;
		break;
	case Client::CMoblinSword::DEAD_FIRE:
		Make_DeadEffect();
		m_fAlpha += 0.02f;
		m_bIsLoop = false;
		m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, 0.1f, 0.f, 0.f));
		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
			m_bDead = true;
		break;
	default:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);
		break;
	}
}

HRESULT CMoblinSword::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("MoblinSword"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;


	CCollider::COLLIDERDESC		ColliderDesc;

	///* For.Com_AABB */
	//ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	//ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	//ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
	//	return E_FAIL;

	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(1.f, 2.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	///* For.Com_SPHERE */
	//ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	//ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	//ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
	//	return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iCurrentCellIndex = 0;
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Field"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Field"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CMoblinSword::SetUp_ShaderID()
{
	if (m_eState == DEAD_B || m_eState == DEAD_F || m_eState == DEAD_FIRE)
		m_eShaderID = SHADER_ANIMDEAD;
	else if (m_bHit)
		m_eShaderID = SHADER_ANIMHIT;
	else
		m_eShaderID = SHADER_ANIMDEFAULT;

	return S_OK;
}


_bool CMoblinSword::IsDead()
{
	if (m_bDead)
		return true;

	return false;
}

void CMoblinSword::Find_Target()
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

void CMoblinSword::Follow_Target(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return;

	m_bIsAttacking = true;
	m_eState = STATE::WALK;
	_vector vTargetPos = (m_pTarget)->Get_TransformState(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(vTargetPos);
	m_pTransformCom->Go_Straight(fTimeDelta*1.5f, m_pNavigationCom);
}

void CMoblinSword::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ACCESSIBLE)
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

void CMoblinSword::AI_Behaviour(_float fTimeDelta)
{
	if (!m_bMove || m_bHit ||
		m_eState == DEAD_F || m_eState == STAGGER || m_eState == DEAD_B || m_eState == GUARD)
		return;

	// Check for Target, AggroRadius
	Find_Target();
	if (m_bAggro && m_fDistanceToTarget < m_fPatrolRadius)
	{
		if (!m_bIsAttacking && m_dwIdleTime + 3000 < GetTickCount())
		{
			m_bIsAttacking = true;
			m_dwAttackTime = GetTickCount();
			CCollider::COLLIDERDESC CollisionDesc = m_pOBBCom->Get_CollisionDesc();
			CollisionDesc.vScale = _float3(2.f, 2.f, 2.f);
			m_pOBBCom->Set_CollisionDesc(CollisionDesc);
		}
		if (m_bIsAttacking && GetTickCount() > m_dwAttackTime + 3000)
		{
			m_dwIdleTime = GetTickCount();
			m_bIsAttacking = false;
			CCollider::COLLIDERDESC CollisionDesc = m_pOBBCom->Get_CollisionDesc();
			CollisionDesc.vScale = _float3(1.f, 2.f, 1.f);
			m_pOBBCom->Set_CollisionDesc(CollisionDesc);
		}


		if (m_fDistanceToTarget < m_fAttackRadius)
		{

			if (m_pTarget)
			{
				if (m_bIsAttacking)
				{
					m_eState = STATE::STANCE_WALK;
				}
				else if (!m_bIsAttacking)
				{
					m_pTransformCom->LookAt((m_pTarget)->Get_TransformState(CTransform::STATE_POSITION));
					m_eState = STATE::STANCE_WAIT;
				}
			}
		}
		else
		{
			Follow_Target(fTimeDelta);
			m_bIsAttacking = false;
			m_dwAttackTime = GetTickCount();
			m_dwIdleTime = GetTickCount();
		}
			
	}
	else
		Patrol(fTimeDelta);
}

void CMoblinSword::Patrol(_float fTimeDelta)
{
	// Switch between Idle and Walk (based on time)
	m_bAggro = false;


	if (m_eState == STATE::IDLE || m_eState == STATE::KYOROKYORO || m_eState == STATE::STANCE_WALK)
	{
		if (GetTickCount() > m_dwIdleTime + (rand() % 1500) * (rand() % 2 + 1) + 3000)
		{
			m_eState = STATE::WALK;
			m_dwWalkTime = GetTickCount();

			m_eDir[DIR_X] = rand() % 3 - 1.f;
			m_eDir[DIR_Z] = m_eDir[DIR_X] == 0 ? rand() % 3 - 1.f : 0.f;

		}
	}
	else if (m_eState == STATE::WALK)
	{
		if (GetTickCount() > m_dwWalkTime + (rand() % 3000) * (rand() % 2 + 1) + 1500)
		{
			m_eState = STATE::KYOROKYORO;
			m_dwIdleTime = GetTickCount();
		}
	}

	// Movement
	if (m_eState == STATE::WALK)
	{
		Change_Direction();
		m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
	}
}

_uint CMoblinSword::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{
	if (m_eState == DAMAGE_B || m_eState == DAMAGE_F || m_eState == GUARD || 
		m_eState == DEAD_B || m_eState == DEAD_F || m_eState == DEAD_FIRE)
		return 0;

	_int iNumRand = rand() % 3;
	if (iNumRand == 0) //random È®·ü·Î Shield Mode
	{
		m_eState = GUARD;

		_uint iNum = rand() % 4 + 1;
		_tchar	sz_Sound[MAX_PATH];
		_float fVolume = 0.3f;
		wcscpy_s(sz_Sound, TEXT("Guard_Metal_Metal_%d.wav"));
		wsprintf(sz_Sound, sz_Sound, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MEFFECT, 0.5f);


		Make_GuardEffect(m_pTarget);

		m_pTransformCom->LookAt(dynamic_cast<CPlayer*>(m_pTarget)->Get_TransformState(CTransform::STATE_POSITION));
		m_bAggro = true;
		return 0;
	}


	_uint iHp = __super::Take_Damage(fDamage, DamageType, DamageCauser);

	if (iHp > 0)
	{
		if (!m_bDead)
		{
			m_bHit = true;
			if (Calculate_Direction() == FRONT)
				m_eState = STATE::DAMAGE_F;
			else
				m_eState = STATE::DAMAGE_B;
			m_bMove = true;

			_uint iNum = rand() % 2 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("Moriblin_Vo_Damage%02d.wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
		
		}


		m_bAggro = true;
		m_bIsAttacking = false;
		m_dwIdleTime = GetTickCount();
		m_dwAttackTime = GetTickCount();

		return iHp;
	}
	else
	{
		m_fAlpha = 0.f;
		if (Calculate_Direction() == FRONT)
			m_eState = STATE::DEAD_F;
		else
			m_eState = STATE::DEAD_B;

		_uint iNum = rand() % 2 + 1;
		_tchar	sz_Sound[MAX_PATH];
		_float fVolume = 0.3f;
		wcscpy_s(sz_Sound, TEXT("Moriblin_Vo_Die%02d.wav"));
		wsprintf(sz_Sound, sz_Sound, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);

		CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Explosion.wav"), SOUND_ACTOR, fVolume);

	}


	return 0;
}

void CMoblinSword::Sound_By_State(_float fTimeDelta)
{
	if (m_bSoundOnce)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();
	_uint iNum = 0;
	_tchar	sz_SoundMonster[MAX_PATH];
	_float fVolume = 0.5f;


	_tchar	sz_SoundEffect[MAX_PATH];
	_float fEffectVolume = 0.3f;


	m_fSoundTime += fTimeDelta;

	switch (m_eState)
	{
	case Client::CMoblinSword::KYOROKYORO:
		m_bSoundOnce = true;
		fVolume = 0.3f;
		iNum = rand() % 2 + 1;
		wcscpy_s(sz_SoundMonster, TEXT("Moriblin_Vo_Search%02d.wav"));
		wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
		break;
	case Client::CMoblinSword::WALK:
		m_fSoundEndTime = 1.f;
		fVolume = 0.3f;
		iNum = rand() % 4 + 1;
		wcscpy_s(sz_SoundMonster, TEXT("Moriblin_Walk (%d).wav"));
		wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
		break;
	case Client::CMoblinSword::STANCE_WALK:
		m_fSoundEndTime = 0.5f;
		fVolume = 0.2f;
		iNum = rand() % 4 + 1;
		wcscpy_s(sz_SoundMonster, TEXT("Moriblin_Walk (%d).wav"));
		wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
		break;
	case Client::CMoblinSword::STAGGER:
		m_bSoundOnce = true;
		fVolume = 0.3f;
		wcscpy_s(sz_SoundMonster, TEXT("3_Monster_Albatross_Hit.wav"));
		break;
	default:
		break;
	}

	

	if ((m_fSoundTime > m_fSoundEndTime || m_bSoundOnce) && m_fDistanceToTarget < 10.f)
	{
		//pGameInstance->PlaySounds(sz_SoundMonster, SOUND_MONSTER, fVolume);
		pGameInstance->PlaySounds(sz_SoundMonster, SOUND_MEFFECT, fVolume);
		m_fSoundTime = 0.f;
	}

	RELEASE_INSTANCE(CGameInstance);
}

CMoblinSword * CMoblinSword::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CMoblinSword*	pInstance = new CMoblinSword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CMoblinSword"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMoblinSword::Clone(void * pArg)
{
	CMoblinSword*	pInstance = new CMoblinSword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMoblinSword"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoblinSword::Free()
{
	__super::Free();
}

