#include "stdafx.h"
#include "..\Public\Albatoss.h"
#include "Player.h"
#include "CameraManager.h"
#include "MessageBox.h"
#include "MonsterBullet.h"
#include "ObjectEffect.h"
#include "MonsterEffect.h"
#include "FightEffect.h"
#include "UIName.h"

CAlbatoss::CAlbatoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

HRESULT CAlbatoss::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAlbatoss::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tInfo.iMaxHp = 15;
	m_tInfo.iDamage = 4;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;

	m_fAttackRadius = 1.5f;
	m_eMonsterID = MONSTER_PAWN;

	_vector vecPostion = XMLoadFloat3((_float3*)pArg);
	vecPostion = XMVectorSetW(vecPostion, 1.f);

	Set_Scale(_float3(1.2f, 1.2f, 1.2f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
	
	m_pTarget = dynamic_cast<CBaseObj*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	return S_OK;
}

int CAlbatoss::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
	{
		Drop_Items();
		return OBJ_DEAD;
	}

	if (!m_bOpening)
		Opening_Motion(fTimeDelta);
	else
	{
		AI_Behaviour(fTimeDelta);
	}

	m_pModelCom->Set_NextAnimIndex(m_eState);
	Change_Animation(fTimeDelta);

	return OBJ_NOEVENT;
}

void CAlbatoss::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);


	if ( m_bIsAttacking &&m_pTarget != nullptr
		&& true == m_pSPHERECom->Collision((m_pTarget)->Get_Collider()))
	{
		CPlayer::ANIM ePlayerState = dynamic_cast<CPlayer*>(m_pTarget)->Get_AnimState();
		if (ePlayerState == CPlayer::SHIELD_HOLD_LP || ePlayerState == CPlayer::SHIELD ||
			ePlayerState == CPlayer::SHIELD_HOLD_F || ePlayerState == CPlayer::SHIELD_HOLD_L || ePlayerState == CPlayer::SHIELD_HOLD_B ||
			ePlayerState == CPlayer::SHIELD_HOLD_R )
		{
			if (m_eState == RUSH)
			{
				m_vLastDir = XMVectorSetY(m_vLastDir, XMVectorGetY(m_vLastDir) + 0.4f);
				m_eState = RUSH_RETURN;
			}

			dynamic_cast<CPlayer*>(m_pTarget)->Set_AnimState(CPlayer::SHIELD_HIT);
			dynamic_cast<CPlayer*>(m_pTarget)->Make_GuardEffect(this);
		}
		else if (dynamic_cast<CPlayer*>(m_pTarget)->Get_AnimState() != CPlayer::SHIELD_HIT)
		{
			if (m_eState == RUSH)
			{
				m_vLastDir = XMVectorSetY(m_vLastDir, XMVectorGetY(m_vLastDir) + 0.4f);
				m_eState = RUSH_RETURN;
			}

			CMonsterBullet::BULLETDESC BulletDesc;
			BulletDesc.eOwner = MONSTER_ALBATROSS;
			BulletDesc.eBulletType = CMonsterBullet::DEFAULT;
			BulletDesc.vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

			Make_GetAttacked_Effect(m_pTarget);
			dynamic_cast<CPlayer*>(m_pTarget)->Take_Damage(1.f, &BulletDesc, nullptr);

		}
	}


	Check_Navigation(fTimeDelta);
}

HRESULT CAlbatoss::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CAlbatoss::Check_Navigation(_float fTimeDelta)
{
	m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ACCESSIBLE && m_eState == RUSH)
	{
		m_fEffectEndTime = 0.1f;
		m_fEffectTime += fTimeDelta;
		if (m_fEffectTime > m_fEffectEndTime)
		{
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CMonsterEffect::CLAW_SMOKE;
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f,0.f,0.f);
			EffectDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
			EffectDesc.vLook = XMVectorSetY(EffectDesc.vLook, (rand() % 100 * 0.01f) - 0.5f);
			EffectDesc.vLook = XMVectorSetZ(EffectDesc.vLook, 0.f);
			EffectDesc.vLook *= -1.f;
			EffectDesc.fDeadTime = 2.f;
			EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
			EffectDesc.vInitScale = _float3(1.f, 1.f, 1.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
			m_fEffectTime = 0.f;



		}
	}
}

void CAlbatoss::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CAlbatoss::WEAK_HOVERING:
		m_fSoundTime += fTimeDelta;

		if (m_fSoundTime > 2.f)
		{
			_uint iNum = rand() % 4 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Flapping (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
			m_fSoundTime = 0.f;
		}

		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_bMakeEffect = false;
		m_fEffectEndTime = 0.3f;
		m_fEffectTime += fTimeDelta;
		if (m_fEffectTime > m_fEffectEndTime)
		{
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::MODEL;
			EffectDesc.eEffectID = CMonsterEffect::FEATHER;
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f, 0.f, 0.f);
			EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)*0.1f, (rand() % 20 - 10)* 0.1f, 0.f);
			EffectDesc.fDeadTime = 1.f;
			EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
			m_fEffectTime = 0.f;

		}
		break;
	case Client::CAlbatoss::PICCOLO_WAIT:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_bMakeEffect = false;
		break;
	case Client::CAlbatoss::HOVERING_LP:
	case Client::CAlbatoss::HOVERING_LP_FAST:
		m_fSoundTime += fTimeDelta;

		if (m_fSoundTime > 0.5f)
		{
			_uint iNum = rand() % 4 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Flapping (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
			m_fSoundTime = 0.f;
		}


		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_bMakeEffect = false;
		break;
	case Client::CAlbatoss::ATTACK_FLAPPING:
		m_fSoundTime += fTimeDelta;

		if (m_fSoundTime > 0.3f)
		{
			_uint iNum = rand() % 4 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Flapping (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
			m_fSoundTime = 0.f;
		}
	

		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_bMakeEffect = false;
		break;
	case Client::CAlbatoss::RUSH:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_fEffectEndTime = 0.3f;
		m_fEffectTime += fTimeDelta;
		if (m_fEffectTime > m_fEffectEndTime)
		{
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::MODEL;
			EffectDesc.eEffectID = CMonsterEffect::FEATHER;
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f, 0.f, 0.f);
			EffectDesc.vLook = XMVectorSet(XMVectorGetX(Get_TransformState(CTransform::STATE_LOOK))*-1.f, (rand() % 20 - 10)*0.1f, (rand() % 20 - 10)* 0.1f, 0.f);
			EffectDesc.fDeadTime = 1.f;
			EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
			m_fEffectTime = 0.f;

		}
		break;
	case Client::CAlbatoss::DEMO_POP:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;

		if (!m_bChanged)
		{
			g_fBGMVolume -= 0.002f;
			if (g_fBGMVolume <= 0.0f)
			{
				g_fBGMVolume = 0.f;
				CGameInstance::Get_Instance()->StopSound(SOUND_BGM);
				CGameInstance::Get_Instance()->PlayBGM(TEXT("1-09 Boss Battle (Eagle's Tower Version).mp3"), g_fBGMVolume);
				m_bChanged = true;
			}
		}
		
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bFirst = false;
			m_bOpening = true;
			m_eState = HOVERING_LP;
			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_TargetPosition(XMVectorSet(0.f, 19.f, 0.f, 1.f));
			dynamic_cast<CPlayer*>(m_pTarget)->Set_Stop(false);

			_vector vPosition = XMLoadFloat4(&m_RushLeftPos);
			vPosition = XMVectorSetZ(vPosition, 3.f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
			m_bIsAttacking = false;
			m_iRushCount = 0;
		}
		break;
	case Client::CAlbatoss::RUSH_ST:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		_vector vPlayerPos = m_pTarget->Get_TransformState(CTransform::STATE_POSITION);
		vPlayerPos = XMVectorSetZ(vPlayerPos, 3.f);
		m_pTransformCom->LookAt(vPlayerPos);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			_uint iNum = rand() % 2 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Rush (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Albatross_Wind.wav"), SOUND_MEFFECT, fVolume);
			m_eState = RUSH;
			m_iRushCount++;
		}
		break;
	case Client::CAlbatoss::RUSH_RETURN:
		if (!m_bFirst)
		{
			_uint iNum = rand() % 4 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Flapping (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MEFFECT, fVolume);
			m_fSoundTime = 0.f;
			m_bFirst = true;
		}
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bFirst = false;
			for (int i = 0; i<5; ++i)
			{
				CEffect::EFFECTDESC EffectDesc;
				EffectDesc.eEffectType = CEffect::MODEL;
				EffectDesc.eEffectID = CMonsterEffect::FEATHER;
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f, 0.f, 0.f);
				EffectDesc.vLook = XMVectorSet(XMVectorGetX(Get_TransformState(CTransform::STATE_LOOK))*-1.f, (rand() % 20 - 10)*0.1f, (rand() % 20 - 10)* 0.1f, 0.f);
				EffectDesc.fDeadTime = 1.f;
				EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

			}

			m_vLastDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			if (XMVectorGetY(m_vLastDir) < 0 )
			m_vLastDir = XMVectorSetY(m_vLastDir, XMVectorGetY(m_vLastDir) + 0.1f);
			
			m_pTransformCom->LookDir(m_vLastDir);
			m_eState = RUSH;
		}
		break;
	case Client::CAlbatoss::DAMAGE_HOVERING:
	case Client::CAlbatoss::DAMAGE_RUSH:
		if (!m_bFirst)
		{
			for (int i = 0; i<5; ++i)
			{
				CEffect::EFFECTDESC EffectDesc;
				EffectDesc.eEffectType = CEffect::MODEL;
				EffectDesc.eEffectID = CMonsterEffect::FEATHER;
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f, 0.f, 0.f);
				EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)*0.1f, (rand() % 20 - 10)* 0.1f, 0.f);
				EffectDesc.fDeadTime = 1.f;
				EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

			}
			m_bFirst = true;
		}

		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_eState = HOVERING_LP;
			m_bFirst = false;
			m_bMakeEffect = false;
		}
		break;
	case Client::CAlbatoss::ATTACK_CLAW_ED:
		if (!m_bFirst)
		{
			
			for (int i = 0; i<5; ++i)
			{
				CEffect::EFFECTDESC EffectDesc;
				EffectDesc.eEffectType = CEffect::MODEL;
				EffectDesc.eEffectID = CMonsterEffect::FEATHER;
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f, 0.f, 0.f);
				EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)*0.1f, (rand() % 20 - 10)* 0.1f, 0.f);
				EffectDesc.fDeadTime = 1.f;
				EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
				
			}
			m_bFirst = true;
		}
		
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_eState = HOVERING_LP;
			m_bFirst = false;
			m_bMakeEffect = false;
		}
		break;
	case Client::CAlbatoss::ATTACK_CLAW_ST:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			_uint iNum = rand() % 2 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.5f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Rush (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Albatross_Wind.wav"), SOUND_OBJECT, fVolume);
			m_bIsAttacking = true;
			m_bDownHovering = false;
		}
		break;
	case Client::CAlbatoss::ATTACK_CLAW:
		m_fSoundTime += fTimeDelta;

		if (m_fSoundTime > 0.5f)
		{
			_uint iNum = rand() % 4 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Flapping (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);

			iNum = rand() % 5 + 1;
			fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Claw (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MEFFECT, fVolume);
			m_fSoundTime = 0.f;
		}
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		Make_ClawEffect(fTimeDelta);
		if(m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_iClawCount++;

			if (m_iClawCount % 4 == 3)
				m_bIsAttacking = false;
		}
		break;
	case Client::CAlbatoss::ATTACK_CLAW_LP:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	case Client::CAlbatoss::HOVERING_ST:
		if (!m_bFirst)
		{
			_uint iNum = rand() % 4 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Flapping (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
			m_fSoundTime = 0.f;
			m_bFirst = true;
		}
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_eState = m_eHoverState;
			m_bFirst = false;
		}		
		break;
	case Client::CAlbatoss::WEAK_HOVERING_ST:
		if (!m_bFirst)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Albatross_WeakHovering.wav"), SOUND_MONSTER, 0.5f);
			for (int i = 0; i<5; ++i)
			{
				CEffect::EFFECTDESC EffectDesc;
				EffectDesc.eEffectType = CEffect::MODEL;
				EffectDesc.eEffectID = CMonsterEffect::FEATHER;
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 2.f, 0.f, 0.f);
				EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, (rand() % 20 - 10)*0.1f, (rand() % 20 - 10)* 0.1f, 0.f);
				EffectDesc.fDeadTime = 1.f;
				EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

			}
			m_bFirst = true;
		}


		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bFirst = false;
			m_eState = m_eHoverState;
		}
		break;
	case Client::CAlbatoss::ATTACK_FLAPPING_ST:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_dwFlappingTime = GetTickCount();
			m_eState = ATTACK_FLAPPING;
			m_bIsAttacking = true;
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Albatross_Wind_High.wav"), SOUND_OBJECT, 0.5f);
		}
		break;
	case Client::CAlbatoss::ATTACK_FLAPPING_ED:
		if (!m_bFirst)
		{
			_uint iNum = rand() % 4 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Flapping (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
			m_fSoundTime = 0.f;
			m_bFirst = true;
		}
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_eState = HOVERING_LP;
			m_bIsAttacking = false;
			m_bDownHovering = false;
			m_eAttackMode = RUSH_STATE;
			m_bFirst = false;

		}
		break;
	case Client::CAlbatoss::DEAD:

		g_fBGMVolume -= 0.02f;
		if (g_fBGMVolume <= 0.0f)
		{
			g_fBGMVolume = 0.f;
			CGameInstance::Get_Instance()->StopSound(SOUND_BGM);
		}

		m_fAlpha += 0.01f;
		m_fAnimSpeed = 1.5f;

		m_fEffectEndTime = 0.2f;
		m_fEffectTime += fTimeDelta;
		if (m_fEffectTime > m_fEffectEndTime)
		{
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.pTarget = this;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CMonsterEffect::ALBATROSSDEAD_SMOKE;
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(rand()%2 == 0 ? -1.f : 1.f, 0.f, 0.f, 0.f);
			EffectDesc.fDeadTime = 2.f;
			EffectDesc.vColor = XMVectorSet(255, 63, 255, 255);
			EffectDesc.vInitScale = _float3(1.f, 1.f, 1.f);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
			
			EffectDesc.eEffectID = CMonsterEffect::GLOW_SPHERE;
			EffectDesc.vDistance = XMVectorSet((rand() % 200 * 0.01f) - 1.f, (rand() % 200 * 0.01f) - 1.f, 0.f, 0.f);
			EffectDesc.iTextureNum = rand()% 2 == 0 ? 0 : 2;
			EffectDesc.fDeadTime = 1.0f;
			EffectDesc.vInitScale = _float3(0.0f, 0.0f, 0.0f);
			EffectDesc.vColor = XMVectorSet(226, 0, 255, 255);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
			
			EffectDesc.eEffectID = CFightEffect::DEAD_FLASH;
			EffectDesc.vDistance = XMVectorSet((rand() % 200*0.01f) - 1.f, (rand() % 200 * 0.01f) - 1.f, 0.f, 0.f);
			EffectDesc.iTextureNum = rand()%2 + 3;
			EffectDesc.fDeadTime = 1.0f;
			_float iRandNum = rand() % 20 * 0.1f + 0.5f;
			EffectDesc.vInitScale = _float3(iRandNum, iRandNum, iRandNum);
			EffectDesc.vColor = XMVectorSet(226, 0, 255, 255);
			CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

			m_fEffectTime = 0.f;			
		}	
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			CGameInstance::Get_Instance()->StopSound(SOUND_BGM);
			m_bDead = true;
			Make_DeadEffect();
		}
		break;
	default:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);
		break;
	}
	
	
}

HRESULT CAlbatoss::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 6.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TOWER, TEXT("Albatoss"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_SHPERE */
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 2.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_TOWER, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iCurrentCellIndex = 0;
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Tower"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Tower"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	return S_OK;
}


_bool CAlbatoss::IsDead()
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

void CAlbatoss::Find_Target()
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
					m_eState = STATE::HOVERING_LP;
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

void CAlbatoss::Follow_Target(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return;

	_vector vTargetPos = (m_pTarget)->Get_TransformState(CTransform::STATE_POSITION);

	m_pTransformCom->LookAt(vTargetPos);
	m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

	m_bIsAttacking = false;
}

void CAlbatoss::AI_Behaviour(_float fTimeDelta)
{
	if ( m_eState == DAMAGE_HOVERING || m_eState == DAMAGE_RUSH || m_eState == DEAD)
		return;

	// Check for Target, AggroRadius
	Find_Target();

	if (XMVectorGetY(m_pTarget->Get_TransformState(CTransform::STATE_POSITION)) < 15.f)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
	}
	else
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_CamMode(CCamera_Dynamic::CAM_TARGET);
		pCamera->Set_TargetPosition(XMVectorSet(0.f, 19.f, 2.f, 1.f));
	}



	switch (m_eAttackMode)
	{
	case RUSH_STATE:
		Rush_Attack(fTimeDelta);
		break;
	case CLAW_STATE:
		Claw_Attack(fTimeDelta);
		break;
	case FLAPPING:
		Flapping_Attack(fTimeDelta);
		break;
	default:
		break;
	}


	
	
	
}

HRESULT CAlbatoss::Drop_Items()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	CPrizeItem::ITEMDESC ItemDesc;
	ItemDesc.vPosition = _float3(0.f, 16.5, 3.f);
	ItemDesc.eInteractType = CPrizeItem::PRIZE;
	ItemDesc.eType = CPrizeItem::TAIL_KEY;
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), iLevel, TEXT("PrizeItem"), &ItemDesc);


	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.eEffectID = CFightEffect::GLOW_SPHERE;
	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.vInitPositon = XMVectorSet(0.f, 17.5, 3.5f, 1.f);
	EffectDesc.iTextureNum = 0;
	EffectDesc.fDeadTime = 0.8f;
	EffectDesc.vInitScale = _float3(0.0f, 0.0f, 0.0f);
	EffectDesc.vColor = XMVectorSet(226, 255, 255, 255);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	EffectDesc.fDeadTime = 1.0f;
	EffectDesc.vInitScale = _float3(0.5f, 0.5f, 0.5f);
	EffectDesc.vColor = XMVectorSet(255, 226, 226, 255);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	EffectDesc.eEffectID = CFightEffect::GLOW_LARGE;
	EffectDesc.iTextureNum = 1;
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vInitScale = _float3(1.f, 1.f, 0.0f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CAlbatoss::Opening_Motion(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	if (CGameInstance::Get_Instance()->Key_Up(DIK_SPACE))
	{
		m_bFirst = false;
		m_bOpening = true;
		m_eState = HOVERING_LP;
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_TargetPosition(XMVectorSet(0.f, 19.f, 0.f, 1.f));
		dynamic_cast<CPlayer*>(m_pTarget)->Set_Stop(false);
		m_iRushCount = 0;
		_vector vPosition = XMLoadFloat4(&m_RushLeftPos);
		vPosition = XMVectorSetZ(vPosition, 3.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		m_bIsAttacking = false;
	}

	Find_Target();

	_vector vPlayerPos = m_pTarget->Get_TransformState(CTransform::STATE_POSITION);
	if (!m_bOpening && XMVectorGetY(vPlayerPos) > 15)
	{
		if (!m_bFirst)
		{
			
			CGameObject* pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);

			CMessageBox::MSGDESC MessageDesc;
			MessageDesc.m_eMsgType = CMessageBox::ALBATOSS_TALK;
			//MessageDesc.fPosition.y = 100.f;
			pUI_Manager->Set_Talking(true);

			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MessageBox"), LEVEL_STATIC, TEXT("Layer_UI"), &MessageDesc);

			CUI_Manager::MSGDESC eMsgDesc;
			eMsgDesc.eMsgType = CUI_Manager::PASSABLE;
			eMsgDesc.iTextureNum = MSG1;
			pUI_Manager->Add_MessageDesc(eMsgDesc);

			eMsgDesc.eMsgType = CUI_Manager::PASSABLE;
			eMsgDesc.iTextureNum = MSG2;
			pUI_Manager->Add_MessageDesc(eMsgDesc);

			pUI_Manager->Open_Message(true);
			m_dwOpeningTime = GetTickCount();
			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_CamMode(CCamera_Dynamic::CAM_TARGET);
			dynamic_cast<CPlayer*>(m_pTarget)->Set_Stop(true);
			m_vTargetDistance.y = 2.f;
			m_vTargetDistance.z = 7.f;
			m_bFirst = true;
		}

		m_eState = DEMO_POP;
		
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		_vector vPosition = vPlayerPos;
		vPosition += XMLoadFloat3(&m_vTargetDistance);
		pCamera->Set_TargetPosition(vPosition);

		if (m_bFirst && m_dwOpeningTime + 18000 < GetTickCount())
		{
			m_vTargetDistance.x = 3.f;
			m_vTargetDistance.y = 3.f;
			m_vTargetDistance.z = 6.f;
			
		}
		else if (m_bFirst && m_dwOpeningTime + 15500 < GetTickCount())
		{
			m_vTargetDistance.x = 3.f;
			m_vTargetDistance.y = 3.f;
			m_vTargetDistance.z += 0.01f;

		
			if (!m_bMakeUIName)
			{
				_uint iNum = rand() % 2 + 1;
				_tchar	sz_Sound[MAX_PATH];
				_float fVolume = 0.3f;
				wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Rush (%d).wav"));
				wsprintf(sz_Sound, sz_Sound, iNum);
				CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);

				CUIName* pUIName = dynamic_cast<CUIName*>(CUI_Manager::Get_Instance()->Get_NameBox());
				pUIName->Set_TexType(CUIName::NAME_ALBATROSS);
				pUIName->Set_Position(_float2(g_iWinSizeX*0.5f, g_iWinSizeY*0.5f + 200.f));
				pUIName->Set_Visible(true);
				CUI_Manager::Get_Instance()->Set_NameTimeZero();
				m_bMakeUIName = true;
			}
		}
		else if(m_bFirst && m_dwOpeningTime + 13500 < GetTickCount())
		{
			m_fSoundTime += fTimeDelta;

			if (m_fSoundTime > 0.9f)
			{
				_uint iNum = rand() % 4 + 1;
				_tchar	sz_Sound[MAX_PATH];
				_float fVolume = 0.3f;
				wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Flapping (%d).wav"));
				wsprintf(sz_Sound, sz_Sound, iNum);
				CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
				m_fSoundTime = 0.f;
			}

			m_vTargetDistance.x = 3.f;
			m_vTargetDistance.y = 3.f;
			m_vTargetDistance.z = 6.f;
		}
		else if (m_bFirst && m_dwOpeningTime + 8000 < GetTickCount())
		{
			m_vTargetDistance.x = 1.f;
			m_vTargetDistance.y = 7.f;
			m_vTargetDistance.z = 8.f;

			m_fSoundTime += fTimeDelta;

			if (m_fSoundTime > 1.9f)
			{
			_uint iNum = rand() % 2 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.3f;
			wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Rush (%d).wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Albatross_Wind.wav"), SOUND_MEFFECT, fVolume);
			m_fSoundTime = 0.f;
			}

		}
		else if (m_bFirst && m_bMessageAutoPass && m_dwOpeningTime + 7000 < GetTickCount())
		{
			pUI_Manager->Set_NextMessage();
			m_bMessageAutoPass = false;
		}
		else if (m_bFirst&& !m_bMessageAutoPass && m_dwOpeningTime + 4000 < GetTickCount())
		{
			pUI_Manager->Set_NextMessage();
			m_bMessageAutoPass = true;
		}
	}

	RELEASE_INSTANCE(CUI_Manager);
	RELEASE_INSTANCE(CGameInstance);
}

void CAlbatoss::Rush_Attack(_float fTimeDelta)
{
	_vector vTargetPos = (m_pTarget)->Get_TransformState(CTransform::STATE_POSITION);
	_vector vPosition = Get_TransformState(CTransform::STATE_POSITION);

	if (!m_bIsAttacking)
	{
		if (m_eState != m_eHoverState && m_eState != m_eHoverStState)
		{
			m_eRushDir =  rand() % 2 == 0 ? RUSH_RIGHT : RUSH_LEFT;
			m_eState = m_eHoverStState;
		}
			
		if (m_eRushDir == RUSH_LEFT)
		{
			m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(-1.f, 0.5f, 0.f, 0.f));
			if (XMVectorGetX(vPosition) < m_RushLeftPos.x)
			{
				vPosition = XMVectorSetX(vPosition, m_RushLeftPos.x);
				vPosition = XMVectorSetY(vPosition, m_RushLeftPos.y);
				m_bMakeEffect = false;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
				m_bIsAttacking = true;
			}
		}
		else
		{
			m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(1.f, 0.5f, 0.f, 0.f));
			if (XMVectorGetX(vPosition) > m_RushRightPos.x)
			{
				vPosition = XMVectorSetX(vPosition, m_RushRightPos.x);
				vPosition = XMVectorSetY(vPosition, m_RushLeftPos.y);
				m_bMakeEffect = false;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
				m_bIsAttacking = true;
			}
		}

		
	}


	if (m_bIsAttacking)
	{
		if (m_eAttackMode == RUSH_STATE)
		{
			if (m_eState != RUSH_ST &&m_eState != RUSH_RETURN && m_eState != RUSH)
			{
				m_eState = RUSH_ST;
				m_bMakeEffect = false;
			}

			if (m_eState == RUSH || m_eState == RUSH_RETURN)
			{
				if (XMVectorGetY(Get_TransformState(CTransform::STATE_POSITION)) <= XMVectorGetY(vTargetPos) &&
					XMVectorGetY(Get_TransformState(CTransform::STATE_LOOK)) < 0.f)
				{
					m_eState = RUSH_RETURN;
				}

				

				if (XMVectorGetX(vPosition) < m_RushLeftPos.x)
				{
					m_eState = RUSH_ST;
					vPosition = XMVectorSetX(vPosition, m_RushLeftPos.x);
					vPosition = XMVectorSetY(vPosition, m_RushLeftPos.y);
					m_bMakeEffect = false;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
					
				}
				else if (XMVectorGetX(vPosition) > m_RushRightPos.x)
				{
					m_eState = RUSH_ST;
					vPosition = XMVectorSetX(vPosition, m_RushRightPos.x);
					vPosition = XMVectorSetY(vPosition, m_RushLeftPos.y);
					m_bMakeEffect = false;
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	
				}
				else
				{
					if (XMVectorGetY(Get_TransformState(CTransform::STATE_POSITION)) < 16.f)
					{
						m_vLastDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
						if (XMVectorGetY(m_vLastDir) < -0.5)
							m_vLastDir = XMVectorSetY(m_vLastDir, XMVectorGetY(m_vLastDir) + 0.1f);
						m_pTransformCom->LookDir(m_vLastDir);
					}
					
					m_pTransformCom->Go_Straight(fTimeDelta*2);
				}


				if (m_iRushCount > 3 && m_eState == RUSH_ST)
				{
					m_bIsAttacking = false;
					m_eAttackMode = CLAW_STATE;
					m_dwClawCount = GetTickCount();
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(Get_TransformState(CTransform::STATE_POSITION), m_ClawingPos.y));
				}
			}
		}
	}
}

void CAlbatoss::Claw_Attack(_float fTimeDelta)
{
	_vector vTargetPos = (m_pTarget)->Get_TransformState(CTransform::STATE_POSITION);
	_vector vPosition = Get_TransformState(CTransform::STATE_POSITION);

	if (!m_bIsAttacking)
	{
		if (!m_bDownHovering)
		{
			if (m_eState != m_eHoverState && m_eState != m_eHoverStState)
				m_eState = m_eHoverStState;

			if (m_eState == m_eHoverState)
			{
				m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, 1.f, 0.f, 0.f));
				if (XMVectorGetY(vPosition) > m_ClawingPos.y)
				{
					_vector vDir = vTargetPos - vPosition;
					vDir = XMVectorSetY(vDir, 0.f);
					m_pTransformCom->LookDir(vDir);
					vPosition = XMVectorSetX(vPosition, rand() % 2 == 0 ? 5.f : -5.f);
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
					m_bDownHovering = true;
					m_bMakeEffect = false;
				}
			}
			
		}
		else if (m_bDownHovering)
		{
			if (m_eState == m_eHoverState)
			{
				_vector vDir = vTargetPos - vPosition;
				vDir = XMVectorSetY(vDir, 0.f);
				m_pTransformCom->LookDir(vDir);
				m_pTransformCom->Go_PosDir(fTimeDelta*0.5f, XMVectorSet(0.f, -1.f, 0.f, 0.f));
				if (XMVectorGetY(vPosition) < XMVectorGetY(vTargetPos) + 5)
				{
					m_eState = ATTACK_CLAW_ST;
				}
					
			}
		}
	}


	if (m_bIsAttacking)
	{
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom))
		{
			m_eState = ATTACK_CLAW;
		}
		else
		{
			if (m_eState == ATTACK_CLAW)
			{
				m_bIsAttacking = false;
				m_bDownHovering = false;
			}
			else
				m_eState = ATTACK_CLAW_LP;
		}

		if (m_eState == ATTACK_CLAW_LP)
		{
			m_pTransformCom->Go_PosTarget(fTimeDelta*2, vTargetPos);

			if (XMVectorGetY(Get_TransformState(CTransform::STATE_POSITION)) < XMVectorGetY(vTargetPos))
			{
				m_bIsAttacking = false;
				m_bDownHovering = false;
			}
		}
		else if (m_eState == ATTACK_CLAW)
		{
			_vector vDir = XMVector3Normalize(vTargetPos - vPosition); 
			vDir = XMVectorSetY(vDir, 0.f);
			m_pTransformCom->Go_PosDir(fTimeDelta*0.2f, vDir);
		}


 		if (m_dwClawCount + 15000 < GetTickCount() && m_bIsAttacking == false)
		{
			m_bIsAttacking = false;
			m_eAttackMode = FLAPPING;

		}
		
	}

	
}

void CAlbatoss::Flapping_Attack(_float fTimeDelta)
{
	_vector vTargetPos = (m_pTarget)->Get_TransformState(CTransform::STATE_POSITION);
	_vector vPosition = Get_TransformState(CTransform::STATE_POSITION);

	if (!m_bIsAttacking)
	{
		if (!m_bDownHovering)
		{
			if (m_eState != m_eHoverState && m_eState != m_eHoverStState)
				m_eState = m_eHoverStState;

			if (m_eState == m_eHoverState)
			{
				m_pTransformCom->Go_PosDir(fTimeDelta*0.5f, XMVectorSet(0.f, 1.f, 0.f, 0.f));
				if (XMVectorGetY(vPosition) > m_ClawingPos.y)
				{
					_vector vDir = vTargetPos - vPosition;
					vDir = XMVectorSetY(vDir, 0.f);
					m_pTransformCom->LookDir(vDir);
					vPosition = XMVectorSetX(vPosition, rand() % 2 == 0 ? 7.f : -7.f);
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
					m_bDownHovering = true;
				}
			}

		}
		else if (m_bDownHovering)
		{
			if (m_eState == m_eHoverState)
			{
				_vector vDir = vTargetPos - vPosition;
				vDir = XMVectorSetY(vDir, 0.f);
				m_pTransformCom->LookDir(vDir);
				m_pTransformCom->Go_PosDir(fTimeDelta*0.5f, XMVectorSet(0.f, -1.f, 0.f, 0.f));
				if (XMVectorGetY(vPosition) < XMVectorGetY(vTargetPos) + 1)
				{
					m_eState = ATTACK_FLAPPING_ST;
					m_dwBulletTime = GetTickCount();
				}

			}
		}
	}


	if (m_bIsAttacking)
	{
		
		if (m_eState == ATTACK_FLAPPING)
		{
			_vector vDir = XMVector3Normalize(vTargetPos - vPosition);
			vDir = XMVectorSetY(vDir, 0.f);
			m_pTransformCom->LookDir(vDir);

			if (m_dwBulletTime + 200 < GetTickCount())
			{
				m_bMakeBullet = true;
				m_dwBulletTime = GetTickCount();
			}

			if (m_bMakeBullet)
			{
				CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
				CMonsterBullet::BULLETDESC BulletDesc;
				BulletDesc.eBulletType = CMonsterBullet::ALBATOSS;
				BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
				BulletDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
				if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MonsterBullet"), LEVEL_TOWER, TEXT("Layer_Bullet"), &BulletDesc)))
					return;
				RELEASE_INSTANCE(CGameInstance);
				m_bMakeBullet = false;


				CEffect::EFFECTDESC EffectDesc;
				EffectDesc.eEffectType = CEffect::MODEL;
				EffectDesc.eEffectID = CMonsterEffect::FLAPPING_SMOKE;
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y, 0.f, 0.f) + Get_TransformState(CTransform::STATE_LOOK);
				EffectDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
				EffectDesc.vLook = XMVectorSetZ(EffectDesc.vLook, 0.f);
				EffectDesc.fDeadTime = 2.f;
				EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
				EffectDesc.vInitScale = _float3(1.f, 1.f, 1.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

			}

			m_fEffectEndTime = 0.1f;
			m_fEffectTime += fTimeDelta;
			if (m_fEffectTime > m_fEffectEndTime)
			{
				CEffect::EFFECTDESC EffectDesc;
				EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
				EffectDesc.eEffectID = CMonsterEffect::CLAW_SMOKE;
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
				EffectDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
				EffectDesc.vLook = XMVectorSetY(EffectDesc.vLook, (rand() % 100 * 0.01f) - 0.5f);
				EffectDesc.vLook = XMVectorSetZ(EffectDesc.vLook, 0.f);
				EffectDesc.fDeadTime = 2.f;
				EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
				EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
				m_fEffectTime = 0.f;
			}
		}
		
		

	

		if (m_dwFlappingTime + 5000 < GetTickCount())
		{
			m_eState = ATTACK_FLAPPING_ED;
			m_bMakeBullet = false;
			m_iRushCount = 0;
		}

	}

}

void CAlbatoss::Make_ClawEffect(_float fTimeDelta)
{
	m_fEffectEndTime = 0.1f;
	m_fEffectTime += fTimeDelta;
	if (m_fEffectTime > m_fEffectEndTime)
	{
		CEffect::EFFECTDESC EffectDesc;
		EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
		EffectDesc.eEffectID = CMonsterEffect::CLAW_SMOKE;
		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
		EffectDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
		EffectDesc.vLook = XMVectorSetY(EffectDesc.vLook, (rand() % 100 * 0.01f) - 0.5f);
		EffectDesc.vLook = XMVectorSetZ(EffectDesc.vLook, 0.f);
		EffectDesc.vLook *= -1.f;
		EffectDesc.fDeadTime = 2.f;
		EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
		EffectDesc.vInitScale = _float3(1.f, 1.f, 1.f);
		CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
		m_fEffectTime = 0.f;

	

	}

	m_fClawTimeEnd = 0.8f;
	m_fClawTime += fTimeDelta;
	if (m_fClawTime > m_fClawTimeEnd)
	{
		CEffect::EFFECTDESC EffectDesc;
		EffectDesc.eEffectType = CEffect::MODEL;
		EffectDesc.eEffectID = CMonsterEffect::CLAW_RING;
		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f,0.f,3.f,0.f);
		EffectDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
		EffectDesc.fDeadTime = 3.f;
		EffectDesc.vColor = XMVectorSet(255, 255, 255, 255);
		EffectDesc.vInitScale = _float3(0.5f, 0.5f, 0.5f);
		CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);
		m_fClawTime = 0.f;
	}
}

void CAlbatoss::Make_DeadEffect(CBaseObj * Target)
{
	if (m_bMakeEffect)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) +XMVectorSet(0.f, Get_Scale().y + 2.f, 0.f, 0.f);
	EffectDesc.pTarget = this;


	EffectDesc.eEffectID = CFightEffect::SMOKEBACK;
	EffectDesc.iTextureNum = 0;
	EffectDesc.fDeadTime = 1.5f;
	EffectDesc.vInitScale = _float3(0.5f, 0.5f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, m_vScale.y - 0.2f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	EffectDesc.eEffectID = CFightEffect::SMOKEFRONT;
	EffectDesc.iTextureNum = 0;
	EffectDesc.fDeadTime = 1.5f;
	EffectDesc.vInitScale = _float3(0.0f, 0.0f, 0.0f);
	EffectDesc.vDistance = XMVectorSet(0.5f, 0.5f, 0.f, 0.f);
	EffectDesc.vColor = XMVectorSet(114, 0, 153, 255);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	EffectDesc.vDistance = XMVectorSet(-0.5f, m_vScale.y - 0.2f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(1.0f, 1.0f, 0.0f);
	EffectDesc.vColor = XMVectorSet(176, 21, 184, 255);
	EffectDesc.fDeadTime = 1.0f;
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	EffectDesc.vDistance = XMVectorSet(1.0f, m_vScale.y - 0.5f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(0.0f, 0.0f, 0.0f);
	EffectDesc.vColor = XMVectorSet(38, 0, 63, 255);
	EffectDesc.vColor = XMVectorSet(240, 0, 250, 255);
	EffectDesc.fDeadTime = 0.7f;
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	EffectDesc.vDistance = XMVectorSet(-1.0f, m_vScale.y - 0.5f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(0.0f, 0.0f, 0.0f);
	EffectDesc.vColor = XMVectorSet(63, 0, 38, 255);
	EffectDesc.vColor = XMVectorSet(176, 0, 184, 255);
	EffectDesc.fDeadTime = 0.7f;
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	EffectDesc.eEffectID = CFightEffect::GLOW_SPHERE;
	EffectDesc.vDistance = XMVectorSet(0.f, m_vScale.y, 0.f, 0.f);
	EffectDesc.iTextureNum = 0;
	EffectDesc.fDeadTime = 1.0f;
	EffectDesc.vInitScale = _float3(0.0f, 0.0f, 0.0f);
	EffectDesc.vColor = XMVectorSet(226, 0, 255, 255);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	EffectDesc.fDeadTime = 1.3f;
	EffectDesc.vInitScale = _float3(1.0f, 1.0f, 1.0f);
	EffectDesc.vColor = XMVectorSet(0, 0, 0, 255);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vInitScale = _float3(0.0f, 0.0f, 0.0f);
	EffectDesc.vColor = XMVectorSet(0, 255, 0, 255);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	EffectDesc.eEffectID = CFightEffect::GLOW_LARGE;
	EffectDesc.iTextureNum = 1;
	EffectDesc.fDeadTime = 0.7f;
	EffectDesc.vInitScale = _float3(1.5f, 1.5f, 0.0f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);



	RELEASE_INSTANCE(CGameInstance);

	m_bMakeEffect = true;

}



_uint CAlbatoss::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{
	if (m_eState == DAMAGE_HOVERING || m_eState == STATE::DAMAGE_RUSH)
		return 0;

	_uint fHp = __super::Take_Damage(fDamage, DamageType, DamageCauser);

	if (fHp > 0)
	{
		if (!m_bDead)
		{
			m_bHit = true;
			if(m_eAttackMode == RUSH_STATE)
				m_eState = STATE::DAMAGE_RUSH;
			else
				m_eState = STATE::DAMAGE_HOVERING;

			if (fHp < 10 && m_eHoverState != WEAK_HOVERING)
			{
				m_eHoverStState = WEAK_HOVERING_ST;
				m_eHoverState = WEAK_HOVERING;
			}

		}


		_uint iNum = rand() % 4 + 1;
		_tchar	sz_Sound[MAX_PATH];
		_float fVolume = 0.3f;
		wcscpy_s(sz_Sound, TEXT("3_Monster_Albatross_Flapping (%d).wav"));
		wsprintf(sz_Sound, sz_Sound, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_ACTOR, fVolume);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Albatross_Hit.wav"), SOUND_MONSTER, 0.5f);

		m_bAggro = true;
		m_bIsAttacking = false;
		m_dwAttackTime = GetTickCount();

		return fHp;
	}
	else
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Albatross_Dead.wav"), SOUND_MONSTER, 0.5f);
		m_eState = STATE::DEAD;
		m_fAlpha = 0.f;
	}
		

	return 0;
}

HRESULT CAlbatoss::SetUp_ShaderID()
{
	if (m_eState == DEAD )
		m_eShaderID = SHADER_ANIMDEAD;
	else if (m_bHit)
		m_eShaderID = SHADER_ANIMHIT;
	else
		m_eShaderID = SHADER_ANIMDEFAULT;

	return S_OK;
}

CAlbatoss * CAlbatoss::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CAlbatoss*	pInstance = new CAlbatoss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CAlbatoss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CAlbatoss::Clone(void * pArg)
{
	CAlbatoss*	pInstance = new CAlbatoss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CAlbatoss"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAlbatoss::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
	Safe_Release(m_pSPHERECom);
}

