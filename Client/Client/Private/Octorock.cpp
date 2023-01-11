#include "stdafx.h"
#include "..\Public\Octorock.h"
#include "Player.h"
#include "MonsterBullet.h"
#include "Navigation.h"
#include "ObjectEffect.h"
#include "MonsterEffect.h"

COctorock::COctorock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

HRESULT COctorock::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT COctorock::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tInfo.iMaxHp = 3;
	m_tInfo.iDamage = 20;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;

	m_fAttackRadius = 5.f;
	m_eMonsterID = MONSTER_OCTOROCK;

	_vector vecPostion = XMLoadFloat3((_float3*)pArg);
	vecPostion = XMVectorSetW(vecPostion, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	Set_Scale(_float3(1.2f, 1.2f, 1.2f));

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//char cName[MAX_PATH];
	//ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//pData_Manager->TCtoC(TEXT("Octorock"), cName);
	//pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	////ERR_MSG(TEXT("Save_Bin_Model"));
	//RELEASE_INSTANCE(CData_Manager);

	//RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

int COctorock::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_UI_OpenType() != CUI_Manager::UI_END)
		return OBJ_NOEVENT;

	_float3 vScale = Get_Scale();
	_float fCullingRadius = max(max(vScale.x, vScale.y), vScale.z);
	if (CGameInstance::Get_Instance()->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), fCullingRadius + 2) == false)
		return OBJ_NOEVENT;

	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;
		

	AI_Behaviour(fTimeDelta);
	m_pModelCom->Set_NextAnimIndex(m_eState);
	Change_Animation(fTimeDelta);


	return OBJ_NOEVENT;
}

void COctorock::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	Check_Navigation(fTimeDelta);
}

HRESULT COctorock::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

void COctorock::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::COctorock::IDLE:
		m_bFirst = false;
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_bMakeEffect = false;
		break;
	case Client::COctorock::WALK:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_bMakeEffect = false;

		break;
	case Client::COctorock::ATTACK_ST:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Octarock_Shot01.wav"), SOUND_MONSTER, 0.5f);
			m_eState = ATTACK_ED;
			Make_Bullet();
		}
		break;
	case Client::COctorock::ATTACK_ED:
		m_bIsLoop = false;
		m_fAnimSpeed = 2.f;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bIsAttacking = false;
			m_eState = IDLE;
			m_bHit = false;
			m_bMakeBullet = false;
		}
		break;
	case Client::COctorock::DAMAGE:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_eState = IDLE;
			m_bHit = false;
			m_bMakeEffect = false;
		}
		break;
	case Client::COctorock::DEAD:
	case Client::COctorock::DEAD_FIRE:
		Make_DeadEffect();
		m_fAlpha += 0.01f;
		m_bIsLoop = false;
		m_fAnimSpeed = 1.5f;
		m_pTransformCom->Go_Backward(fTimeDelta*4);
		m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, 0.1f, 0.f, 0.f));
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
			m_bDead = true;
		break;
	default:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop);
		break;
	}
}

HRESULT COctorock::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Octorock"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_OBB*/
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iCurrentCellIndex = 0;
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Field"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Field"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;



	return S_OK;
}


_bool COctorock::IsDead()
{
	if (m_bDead && m_eState == STATE::DEAD )//&& m_dwDeathTime + 1000 < GetTickCount())
		return true;
	else if (m_bDead && m_eState != STATE::DEAD)
	{
		m_dwDeathTime = GetTickCount();
		m_eState = STATE::DEAD;
	}

	return false;
}

void COctorock::Find_Target()
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
				CTransform* PlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC,TEXT("Layer_Player"), TEXT("Com_Transform"));
				_vector vTargetPos = PlayerTransform->Get_State(CTransform::STATE_POSITION);
				m_fDistanceToTarget =  XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_POSITION) - vTargetPos));
				m_pTarget = dynamic_cast<CBaseObj*>(pTarget);
			}
			else
				m_pTarget = nullptr;
		}
	}
	else
		m_pTarget = nullptr;
}

HRESULT COctorock::SetUp_ShaderID()
{
	if (m_eState == DEAD || m_eState == DEAD_FIRE)
		m_eShaderID = SHADER_ANIMDEAD;
	else if(m_bHit)
		m_eShaderID = SHADER_ANIMHIT;
	else
		m_eShaderID = SHADER_ANIMDEFAULT;

	return S_OK;
}


void COctorock::Follow_Target(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return;

	m_eState = STATE::WALK;

	_vector vTargetPos = (m_pTarget)->Get_TransformState(CTransform::STATE_POSITION);

	m_pTransformCom->LookAt(vTargetPos);
	m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

	m_bIsAttacking = false;
}

void COctorock::Check_Navigation(_float fTimeDelta)
{
	 if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ACCESSIBLE)
	{
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float fHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);
		
		vPosition = XMVectorSetY(vPosition, fHeight);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		

	}
}

void COctorock::AI_Behaviour(_float fTimeDelta)
{
	if (!m_bMove || m_eState == DEAD)
		return;

	// Check for Target, AggroRadius
	Find_Target();
	if (m_bAggro)
	{
		
		if (m_pTarget)
		{
			// If in AttackRadius > Attack
			if (m_fDistanceToTarget < m_fAttackRadius)
			{
				m_pTransformCom->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_POSITION));
				if (!m_bIsAttacking && GetTickCount() > m_dwAttackTime + 1500)
				{
					m_eState = STATE::ATTACK_ST;
					m_bIsAttacking = true;
				}
				else if (!m_bIsAttacking)
					m_eState = STATE::IDLE;
			}
			// If NOT in AttackRadius > Follow Target
			else
				Follow_Target(fTimeDelta);
		}
	}
	else
		Patrol(fTimeDelta);
}

void COctorock::Patrol(_float fTimeDelta)
{
	// Switch between Idle and Walk (based on time)
	if (m_eState == STATE::IDLE)
	{
		if (GetTickCount() > m_dwIdleTime + (rand() % 1500) * (rand() % 2 + 1) + 3000)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Octarock_Walk01.wav"), SOUND_MONSTER, 0.03f);
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
			CGameInstance::Get_Instance()->StopSound(SOUND_MONSTER);
			m_eState = STATE::IDLE;
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

void COctorock::Make_Bullet()
{
	if (m_bMakeBullet)
		return;

	m_bMakeBullet = true;

	//make bullet
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CMonsterBullet::BULLETDESC BulletDesc;
	BulletDesc.eBulletType = CMonsterBullet::OCTOROCK;
	BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 0.5f, 0.f, 0.f) + Get_TransformState(CTransform::STATE_LOOK)*0.5f;
	BulletDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
	BulletDesc.fDeadTime = 2.f;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MonsterBullet"), LEVEL_GAMEPLAY, TEXT("Layer_Bullet"), &BulletDesc)))
		return;

	//make bullet Effect
	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.eEffectID = CObjectEffect::SMOKE;
	EffectDesc.fDeadTime = 0.3f;
	EffectDesc.vColor = XMVectorSet(178, 159, 134, 255);
	EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + BulletDesc.vLook*0.5f;
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);

	
	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CMonsterEffect::BLAST_RING;
	EffectDesc.fDeadTime = 0.3f;
	EffectDesc.vInitScale = _float3(3.f, 3.f, 3.f);
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_LOOK);
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + BulletDesc.vLook*0.7f + XMVectorSet(0.f, 0.5f, 0.f, 0.f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	RELEASE_INSTANCE(CGameInstance);
}

_uint COctorock::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{
	if (m_eState == DAMAGE)
		return 0;

	_uint fHp = __super::Take_Damage(fDamage, DamageType, DamageCauser);

	if (fHp > 0)
	{
		if (!m_bDead)
		{
			_uint iNum = rand() % 3 + 1;
			_tchar	sz_Sound[MAX_PATH];
			_float fVolume = 0.5f;
			wcscpy_s(sz_Sound, TEXT("Octarock_Vo_Damage0%d.wav"));
			wsprintf(sz_Sound, sz_Sound, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);

			m_bHit = true;
			m_eState = STATE::DAMAGE;
			m_bMove = true;
		}

		m_bAggro = true;
		m_bIsAttacking = false;
		m_dwAttackTime = GetTickCount();

		return fHp;
	}
	else
	{
		_uint iNum = rand() % 2 + 1;
		_tchar	sz_Sound[MAX_PATH];
		_float fVolume = 0.5f;
		wcscpy_s(sz_Sound, TEXT("Octarock_Vo_Damage_Electric0%d.wav"));
		wsprintf(sz_Sound, sz_Sound, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_Sound, SOUND_MONSTER, fVolume);

		CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Explosion.wav"), SOUND_ACTOR, fVolume);

		m_eState = STATE::DEAD;
		m_fAlpha = 0.f;
	}
		

	return 0;
}

COctorock * COctorock::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	COctorock*	pInstance = new COctorock(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : COctorock"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * COctorock::Clone(void * pArg)
{
	COctorock*	pInstance = new COctorock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : COctorock"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void COctorock::Free()
{
	__super::Free();
}

