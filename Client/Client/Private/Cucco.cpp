#include "stdafx.h"
#include "..\Public\Cucco.h"
#include "Player.h"
#include "MonsterBullet.h"
#include "Navigation.h"

CCucco::CCucco(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

HRESULT CCucco::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCucco::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_tInfo.iMaxHp = 5;
	m_tInfo.iDamage = 20;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;

	m_fAttackRadius = 5.f;
	m_eMonsterID = MONSTER_CUCCO;

	_vector vecPostion = XMLoadFloat3((_float3*)pArg);
	vecPostion = XMVectorSetW(vecPostion, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	Set_Scale(_float3(1.2f, 1.2f, 1.2f));

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//char cName[MAX_PATH];
	//ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//pData_Manager->TCtoC(TEXT("Cucco"), cName);
	//pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	////ERR_MSG(TEXT("Save_Bin_Model"));
	//RELEASE_INSTANCE(CData_Manager);

	//RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

int CCucco::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_UI_OpenType() != CUI_Manager::UI_END)
		return OBJ_NOEVENT;

	if (m_bDead)
		return OBJ_DEAD;

	if(Check_IsinFrustum(1.f) == false)
		return OBJ_NOEVENT;

	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;


	AI_Behaviour(fTimeDelta);
	m_pModelCom->Set_NextAnimIndex(m_eState);
	Change_Animation(fTimeDelta);


	return OBJ_NOEVENT;
}

void CCucco::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
	Check_Navigation(fTimeDelta);


	if (m_bAngry)
	{
		CBaseObj* pCollisionBlock = nullptr;
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_TRAP, m_pSPHERECom, &pCollisionBlock))
		{

			_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pCollisionBlock->Get_TransformState(CTransform::STATE_POSITION);
			if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
				vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
			else
				vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
			m_pTransformCom->Go_PosDir(fTimeDelta, vDirection, m_pNavigationCom);
		}
	}
	else
	{
		CBaseObj* pCollisionBlock = nullptr;
		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSPHERECom, &pCollisionBlock))
		{

			_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pCollisionBlock->Get_TransformState(CTransform::STATE_POSITION);
			if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
				vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
			else
				vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
			m_pTransformCom->Go_PosDir(fTimeDelta, vDirection, m_pNavigationCom);
		}
	}


	
}

HRESULT CCucco::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

void CCucco::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CCucco::IDLE:
	case Client::CCucco::WALK:
		m_bMakeEffect = false;
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	case Client::CCucco::RUN:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pTransformCom->Go_Straight(fTimeDelta*4, m_pNavigationCom);
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	case Client::CCucco::FLY_ATTACK:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_fTime += 0.05f;
		//m_pTransformCom->Jump(m_fTime, 3.f, 1.5f, m_fHeight, m_fHeight);
		m_pTransformCom->Go_Straight(fTimeDelta*4);
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	case Client::CCucco::LANDING:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop))
			m_eState = IDLE;
		break;
	default:
		m_fAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_fAnimSpeed, m_bIsLoop);
		break;
	}
}

HRESULT CCucco::Ready_Components(void * pArg)
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

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_GAMEPLAY, TEXT("Cucco"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_SPHERE*/
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _float3(1.0f, 1.0f, 1.0f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Navigation */
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);
	NaviDesc.iCurrentCellIndex = 0;
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Field"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Field"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;



	return S_OK;
}


_bool CCucco::IsDead()
{
	if (m_bDead)
		return true;

	return false;
}

void CCucco::Find_Target()
{
	if (!m_bIsAttacking && !m_bDead)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CGameObject* pTarget = nullptr;
		if (m_bAngry)
			pTarget = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		else
			pTarget = pGameInstance->Get_Object(LEVEL_GAMEPLAY, TEXT("Layer_CuccoKeeper"));

		if (pTarget)
		{
			CTransform* PlayerTransform = nullptr;
			if (m_bAngry)
				 PlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));
			else
				PlayerTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_CuccoKeeper"), TEXT("Com_Transform"));
			_vector vTargetPos = PlayerTransform->Get_State(CTransform::STATE_POSITION);
			m_fDistanceToTarget = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_POSITION) - vTargetPos));
			m_pTarget = dynamic_cast<CBaseObj*>(pTarget);
		}
		else
			m_pTarget = nullptr;

	}
	else
		m_pTarget = nullptr;
}

void CCucco::Follow_Target(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return;

	if(m_bAngry)
		m_eState = STATE::FLY_ATTACK;
	else
		m_eState = STATE::WALK;

	_vector vTargetPos = (m_pTarget)->Get_TransformState(CTransform::STATE_POSITION);

	m_pTransformCom->LookAt(vTargetPos);
	m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);

	m_bIsAttacking = false;
}

void CCucco::Sound_By_State(_float fTimeDelta)
{
	if (m_bSoundOnce)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();
	_uint iNum = 0;
	_tchar	sz_SoundMonster[MAX_PATH];
	_float fVolume = 0.5f;


	switch (m_eState)
	{
	case Client::CCucco::RUN:
	case Client::CCucco::FLY_ATTACK:
		m_fSoundTime += fTimeDelta;
		m_fVoiceTime += fTimeDelta;

		m_fSoundEndTime = 0.2f;
		fVolume = 0.05f;
		iNum = rand() % 5 + 1;
		wcscpy_s(sz_SoundMonster, TEXT("3_Monster_Kokko_Wing (%d).wav"));
		wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
		break;
	default:
		break;
	}

	if (m_fSoundTime > m_fSoundEndTime || m_bSoundOnce)
	{
		pGameInstance->PlaySounds(sz_SoundMonster, SOUND_MEFFECT, fVolume);
		m_fSoundTime = 0.f;
	}

	if (m_fVoiceTime > 1.f)
	{
		iNum = rand() % 5 + 1;
		wcscpy_s(sz_SoundMonster, TEXT("3_Monster_Kokko_Shout (%d).wav"));
		wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);

		pGameInstance->PlaySounds(sz_SoundMonster, SOUND_MONSTER, 0.4);
		m_fVoiceTime = 0.f;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CCucco::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ACCESSIBLE)
	{
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_fHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);
		if (m_fHeight > XMVectorGetY(vPosition))
		{
			vPosition = XMVectorSetY(vPosition, m_fHeight);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
			m_fTime = 0.f;
		}
		else if (m_fHeight < XMVectorGetY(vPosition) && m_eState != FLY_ATTACK)
		{
			vPosition = XMVectorSetY(vPosition, m_fHeight);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
			m_fTime = 0.f;
		}

	}
}

void CCucco::AI_Behaviour(_float fTimeDelta)
{
	if (!m_bMove && m_eState == LANDING)
		return;

	// Check for Target, AggroRadius
	Find_Target();
	if (m_bAngry)
	{
		if (dynamic_cast<CPlayer*>(m_pTarget)->Get_AnimState() == CPlayer::DEAD)
		{
			Set_Angry(false);
			m_tInfo.iCurrentHp = m_tInfo.iMaxHp;
		}
			
		if (m_fDistanceToTarget > 10.f)
		{
			Follow_Target(fTimeDelta);
		}
		else if (m_fDistanceToTarget > 4.f)
			m_pTransformCom->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_POSITION));
	}
	else
	{
		if (m_fDistanceToTarget > 4.f)
			Follow_Target(fTimeDelta);
		else
			Patrol(fTimeDelta);
	}
}

void CCucco::Patrol(_float fTimeDelta)
{
	// Switch between Idle and Walk (based on time)
	if (m_eState == STATE::IDLE)
	{
		if (GetTickCount() > m_dwIdleTime + (rand() % 1500) * (rand() % 3 + 1) + 3000)
		{
			_vector vPlayerPos = dynamic_cast<CBaseObj*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")))->Get_TransformState(CTransform::STATE_POSITION);
			_float  vDistance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_POSITION) - vPlayerPos));

			if (vDistance < 10.f)
			{
				_uint iNum = rand() % 6 + 1;
				_tchar	sz_SoundMonster[MAX_PATH];
				_float fVolume = 0.5f;
				wcscpy_s(sz_SoundMonster, TEXT("3_Monster_Kokko (%d).wav"));
				wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
				CGameInstance::Get_Instance()->PlaySounds(sz_SoundMonster, SOUND_MONSTER, 0.2f);
			}
			

			m_eState = STATE::WALK;
			m_dwWalkTime = GetTickCount();

			m_eDir[DIR_X] = (rand() % 300)*0.01f - 1.f;
			m_eDir[DIR_Z] = (rand() % 300)*0.01f - 1.f;

		}
	}
	else if (m_eState == STATE::WALK)
	{
		if (GetTickCount() > m_dwWalkTime + (rand() % 3000) * (rand() % 3 + 1) + 1500)
		{
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

_uint CCucco::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{

	if (m_eState == LANDING || m_bAngry)
		return 0;

	_uint fHp = __super::Take_Damage(fDamage, DamageType, DamageCauser);

	if (fHp > 0)
	{
		if (!m_bDead)
		{
			m_bHit = true;
			m_eState = STATE::LANDING;
			m_bMove = true;

			_uint iNum = 0;
			_tchar	sz_SoundMonster[MAX_PATH];
			_float fVolume = 0.5f;
			iNum = rand() % 6 + 1;
			wcscpy_s(sz_SoundMonster, TEXT("3_Monster_Kokko_Shout (%d).wav"));
			wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
			CGameInstance::Get_Instance()->PlaySounds(sz_SoundMonster, SOUND_MONSTER, 0.4f);
			
		}

		m_bAggro = true;
		m_bIsAttacking = false;
		m_dwAttackTime = GetTickCount();

		return fHp;
	}
	else
	{
		m_bAngry = true;
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		_uint iNum = 0;
		_tchar	sz_SoundMonster[MAX_PATH];
		_float fVolume = 0.5f;
		iNum = rand() % 6 + 1;
		wcscpy_s(sz_SoundMonster, TEXT("3_Monster_Kokko_Shout (%d).wav"));
		wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_SoundMonster, SOUND_MONSTER, 0.4f);

		for (int i = 0; i < 10; ++i)
		{
			_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPosition = XMVectorSetX(vPosition, XMVectorGetX(vPosition) + (rand() % 4000)*0.01f - 20);
			//vPosition = XMVectorSetY(vPosition, XMVectorGetY(vPosition) + rand() % 5);
			vPosition = XMVectorSetZ(vPosition, XMVectorGetZ(vPosition) + (rand() % 2000)*0.01f - 20);

			_float3 InitPos;
			XMStoreFloat3(&InitPos, vPosition);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Cucco"), LEVEL_GAMEPLAY, TEXT("Layer_Cucco"), &InitPos)))
				return E_FAIL;

		}

	

		list<CGameObject*>* pCuccoList = pGameInstance->Get_ObjectList(LEVEL_GAMEPLAY, TEXT("Layer_Cucco"));
		if (pCuccoList != nullptr)
		{
			for (auto& iter : *pCuccoList)
			{
				if (iter == nullptr)
					continue;

				dynamic_cast<CCucco*>(iter)->Set_Angry(true);
				dynamic_cast<CCucco*>(iter)->Set_State(FLY_ATTACK);

			}
		}

		RELEASE_INSTANCE(CGameInstance);

	}

	return 0;
}

void CCucco::Set_Angry(_bool type)
{
	 m_bAngry = type; 

	 if (type == true)
	 {
		 CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
		 CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_TRAP, this);
		 m_eState = FLY_ATTACK;
		 m_fHitRedColor = 0.5f;
	 }
	 else
	 {
		 CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_TRAP, this);
		 CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
		 m_eState = IDLE;
		 m_dwWalkTime = GetTickCount();

		 CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		 list<CGameObject*>* pCuccoList = pGameInstance->Get_ObjectList(LEVEL_GAMEPLAY, TEXT("Layer_Cucco"));
		 if (pCuccoList != nullptr)
		 {
			 int i = 0;
			 for (auto& iter : *pCuccoList)
			 {

				 if (i >= 3)
					 dynamic_cast<CCucco*>(iter)->Set_Dead(true); 
				
				 ++i;
			 }
		 }

		 m_fHitRedColor = 0.f;
		 RELEASE_INSTANCE(CGameInstance);
	 }
	
}

HRESULT CCucco::SetUp_ShaderID()
{
	 if (m_bHit)
		m_eShaderID = SHADER_ANIMHIT;
	 else if (m_bAngry)
	 {
		 m_eShaderID = SHADER_ANIMCHARGE;
		 _vector m_vColor = { 255, -50, -50, 255 };
		 _float m_fColorPercent = 1.f;
		 if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
			 return E_FAIL;
		 if (FAILED(m_pShaderCom->Set_RawValue("g_fColorPercent", &m_fColorPercent, sizeof(_float))))
			 return E_FAIL;
	}
		
	else
		m_eShaderID = SHADER_ANIMDEFAULT;

	return S_OK;
}

CCucco * CCucco::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCucco*	pInstance = new CCucco(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCucco"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCucco::Clone(void * pArg)
{
	CCucco*	pInstance = new CCucco(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCucco"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCucco::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_TRAP, this);
}

