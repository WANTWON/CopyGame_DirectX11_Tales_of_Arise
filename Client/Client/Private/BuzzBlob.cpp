#include "stdafx.h"
#include "..\Public\BuzzBlob.h"
#include "Player.h"

CBuzzBlob::CBuzzBlob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

HRESULT CBuzzBlob::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBuzzBlob::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tInfo.iMaxHp = 2;
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

int CBuzzBlob::Tick(_float fTimeDelta)
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

void CBuzzBlob::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

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

HRESULT CBuzzBlob::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

void CBuzzBlob::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::DROP)
		m_eState = DEAD;
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

void CBuzzBlob::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CBuzzBlob::DAMAGE:
	{
		m_fAnimSpeed = 3.f;
		_vector vDir = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_pTarget->Get_TransformState(CTransform::STATE_POSITION);
		m_pTransformCom->Go_PosDir(fTimeDelta * 2, vDir, m_pNavigationCom);
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
			m_eState = WALK;
		break;
	}
	case Client::CBuzzBlob::DEAD:
	{
		Make_DeadEffect();
		m_fAnimSpeed = 1.f;
		m_pTransformCom->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_POSITION));
		m_pTransformCom->Go_Backward(fTimeDelta * 2, m_pNavigationCom);
		m_pTransformCom->Go_PosDir(fTimeDelta * 2, XMVectorSet(0.f, -0.1f, 0.f, 0.f), m_pNavigationCom);
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
		{
			m_bDead = true;
		}
		break;
	}
	case Client::CBuzzBlob::IDLE:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_bMakeEffect = false;
		break;
	case Client::CBuzzBlob::WALK:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		m_bMakeEffect = false;

		m_fSoundTime += fTimeDelta;
		if (m_fSoundTime > 0.5f)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Octarock_Walk01.wav"), SOUND_ACTOR, 0.05f);
			m_fSoundTime = 0.f;
		}
		break;
	default:
		break;
	}
}

HRESULT CBuzzBlob::Ready_Components(void * pArg)
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_DissolveTexture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_BuzzBlob"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_OBB*/
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
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


_bool CBuzzBlob::IsDead()
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

void CBuzzBlob::Find_Target()
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

void CBuzzBlob::Follow_Target(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return;

	m_eState = STATE::WALK;

	_vector vTargetPos = (m_pTarget)->Get_TransformState(CTransform::STATE_POSITION);

	m_pTransformCom->LookAt(vTargetPos);
	m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom, 0.f);

	m_bIsAttacking = false;
}

void CBuzzBlob::AI_Behaviour(_float fTimeDelta)
{
	if (!m_bMove || m_eState == DEAD || m_eState == DAMAGE)
		return;

	// Check for Target, AggroRadius
	Find_Target();

	if (m_pTarget /*&& m_pSPHERECom->Collision(m_pTarget->Get_Collider()) == true*/)
	{
		m_bAggro = true;

		if (m_pOBBCom->Collision(m_pTarget->Get_Collider()) == true)
		{
			m_eState = IDLE;
			return;
		}

		m_pTransformCom->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_POSITION));
		Follow_Target(fTimeDelta);
		m_eState = STATE::WALK;
		m_bIsAttacking = true;

	}
	else
	{
		if (m_bAggro)
			Follow_Target(fTimeDelta);
		else
			m_eState = IDLE;
	}

}



_uint CBuzzBlob::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{
	if (m_eState == DEAD || m_eState == STATE::DAMAGE)
		return 0;

	_uint fHp = __super::Take_Damage(fDamage, DamageType, DamageCauser);

	if (fHp > 0)
	{
		if (!m_bDead)
		{
			m_bHit = true;
			m_eState = STATE::DAMAGE;
			m_bMove = true;

			CGameInstance::Get_Instance()->PlaySounds(TEXT("ChuChu_Damage.wav"), SOUND_MEFFECT, 0.5f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("ChuChu_Vo_Damage.wav"), SOUND_MONSTER, 0.5f);
		}

		m_bAggro = true;
		m_bIsAttacking = false;
		m_dwAttackTime = GetTickCount();

		return fHp;
	}
	else
	{
		m_eState = STATE::DEAD;
		CGameInstance::Get_Instance()->PlaySounds(TEXT("ChuChu_Vo_Dead.wav"), SOUND_MONSTER, 0.5f);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Explosion.wav"), SOUND_ACTOR, 0.5f);

	}
		

	return 0;
}

HRESULT CBuzzBlob::SetUp_ShaderID()
{
	if (m_eState == DEAD)
		m_eShaderID = SHADER_ANIMDEAD;
	else if (m_bHit)
		m_eShaderID = SHADER_ANIMHIT;
	else
		m_eShaderID = SHADER_ANIMDEFAULT;

	return S_OK;
}

CBuzzBlob * CBuzzBlob::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBuzzBlob*	pInstance = new CBuzzBlob(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CZol"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBuzzBlob::Clone(void * pArg)
{
	CBuzzBlob*	pInstance = new CBuzzBlob(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CZol"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBuzzBlob::Free()
{
	__super::Free();
}

