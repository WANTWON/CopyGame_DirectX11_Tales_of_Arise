#include "stdafx.h"
#include "..\Public\Monster.h"
#include "Player.h"
#include "Level_Manager.h"
#include "CameraManager.h"
#include "PrizeItem.h"
#include "FightEffect.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBaseObj(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_eObjectID = OBJID::OBJ_MONSTER;
	//Set_Scale(_float3(0.5, 0.5, 0.5));

	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
	Change_Animation(0.f);
	return S_OK;
}

int CMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (IsDead())
	{
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
		return OBJ_DEAD;
	}
		

	if (m_bHit)
	{
		
		if (!m_bRed)
		{
			m_fHitRedColor += 0.05f;
			if (m_fHitRedColor >= m_fMaxRed)
			{
				m_fHitRedColor = m_fMaxRed;
				m_bRed = true;
			}
				

		}
		else
		{
			m_fHitRedColor -= 0.05f;
			if (m_fHitRedColor <= m_fMinRed)
			{
				m_bRed = false;
				m_fHitRedColor = m_fMinRed;
			}
		}
	}

	if (m_dwHitTime + 1500 < GetTickCount() && !m_bRed)
	{
		m_bHit = false;
	}
		

	return OBJ_NOEVENT;
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	Sound_By_State(fTimeDelta);

	_float3 vScale = Get_Scale();
	_float fCullingRadius = max(max(vScale.x, vScale.y), vScale.z);
	if (CGameInstance::Get_Instance()->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION),fCullingRadius + 2) == false)
		return;

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
	}
		

#ifdef _DEBUG
	if (m_pAABBCom != nullptr)
		m_pRendererCom->Add_Debug(m_pAABBCom);
	if (m_pOBBCom != nullptr)
		m_pRendererCom->Add_Debug(m_pOBBCom);
	if (m_pSPHERECom != nullptr)
		m_pRendererCom->Add_Debug(m_pSPHERECom);
#endif



	if (CGameInstance::Get_Instance()->Key_Up(DIK_9))
		m_bDead = true;
}

HRESULT CMonster::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CMonster::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_ShadowLightView(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	for(_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, SHADER_ANIMSHADOW)))
			return S_OK;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


CMonster::DMG_DIRECTION CMonster::Calculate_Direction()
{
	if (m_pTarget == nullptr)
		return FRONT;

	// New Logic
	_vector vLook = XMVector3Normalize( m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	_vector vTargetDir = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pTarget)->Get_TransformState(CTransform::STATE_LOOK));

	_vector fDot = XMVector3Dot(vTargetDir, vLook);
	_float fAngleRadian = acos(XMVectorGetX(fDot));
	_float fAngleDegree = XMConvertToDegrees(fAngleRadian);
	_vector vCross = XMVector3Cross(vTargetDir, vLook);


	if (XMVectorGetY(vCross) > 0.f)
	{
		if (fAngleDegree > 0.f && fAngleDegree <= 90.f)
			m_eDmg_Direction = BACK;
		else if (fAngleDegree > 90.f && fAngleDegree <= 180.f)
			m_eDmg_Direction = FRONT;
	}
	else
	{
		if (fAngleDegree > 0.f && fAngleDegree <= 90.f)
			m_eDmg_Direction = BACK;
		else if (fAngleDegree > 90.f && fAngleDegree <= 180.f)
			m_eDmg_Direction = FRONT;
	}

	return m_eDmg_Direction;
}

_vector CMonster::Calculate_PosDirction()
{
	if (m_pTarget == nullptr)
		return _vector();

	// New Logic
	_vector vMyPos = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	_vector vTargetPos = XMVector3Normalize(dynamic_cast<CPlayer*>(m_pTarget)->Get_TransformState(CTransform::STATE_POSITION));

	return vTargetPos - vMyPos;
}

void CMonster::Find_Target()
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
				
				m_pTarget = nullptr;
				m_bAggro = false;
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

HRESULT CMonster::Drop_Items()
{
	int iRadomItem = rand() % 3;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	CPrizeItem::ITEMDESC ItemDesc;
	XMStoreFloat3(&ItemDesc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	ItemDesc.vPosition.y += 0.5f;
	ItemDesc.eInteractType = CPrizeItem::DEFAULT;

	
	if (iRadomItem == 1)
		ItemDesc.eType = CPrizeItem::HEART_RECOVERY;
	else
		ItemDesc.eType = CPrizeItem::RUBY_GREEN;

	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), iLevel, TEXT("PrizeItem"), &ItemDesc);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CMonster::Make_GetAttacked_Effect(CBaseObj* DamageCauser)
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
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION)+ EffectDesc.vLook + XMVectorSet(0.f, Get_Scale().y - 0.2f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(2.5f, 2.5f, 2.5f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	EffectDesc.eEffectID = CFightEffect::HITRING;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook + XMVectorSet(0.f, Get_Scale().y - 0.3f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(2.5f, 2.5f, 2.5f);
	EffectDesc.fDeadTime = 0.8f;
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	for (int i = 0; i < 10; ++i)
	{
		EffectDesc.eEffectID = CFightEffect::HITSPARK;
		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook + XMVectorSet(0.f, Get_Scale().y - 0.4f, 0.f, 0.f);
		_float iRandNum = (rand() % 10 + 10) * 0.1f;
		EffectDesc.vInitScale = _float3(iRandNum, iRandNum, iRandNum);
		EffectDesc.fDeadTime = 0.8f;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	}
	


	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.eEffectID = CFightEffect::HITFLASH_TEX;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook + XMVectorSet(0.f, Get_Scale().y - 0.15f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.iTextureNum = 0;
	EffectDesc.vInitScale = _float3(2.f, 2.f, 2.2f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);


	EffectDesc.eEffectID = CFightEffect::HITFLASH_TEX;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook + XMVectorSet(0.f, Get_Scale().y - 0.3f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.7f;
	EffectDesc.iTextureNum = 1;
	EffectDesc.vInitScale = _float3(3.5f, 3.5f, 3.5f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	EffectDesc.eEffectID = CFightEffect::HITFLASH_TEX;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook + XMVectorSet(0.f, Get_Scale().y - 0.1f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.iTextureNum = 2;
	EffectDesc.vInitScale = _float3(1.0f, 1.0f, 1.0f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	m_bMakeEffect = true;

	RELEASE_INSTANCE(CGameInstance);
}

void CMonster::Make_DeadEffect(CBaseObj * Target)
{
	if (m_bMakeEffect)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);// +XMVectorSet(0.f, Get_Scale().y - 0.4f, 0.f, 0.f);
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

void CMonster::Make_GuardEffect(CBaseObj * Target)
{

	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CFightEffect::GUARD_FLASH;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK))*1.5f + XMVectorSet(0.f, Get_Scale().y, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_POSITION) - Target->Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.vInitScale = _float3(1.5f, 1.5f, 1.5f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	EffectDesc.eEffectID = CFightEffect::GUARD_RING;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK))*0.8f + XMVectorSet(0.f, Get_Scale().y*0.5f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_POSITION) - Target->Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.vInitScale = _float3(1.0f, 1.0f, 1.0f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);


	for (int i = 0; i < 10; ++i)
	{
		EffectDesc.eEffectID = CFightEffect::GUARD_DUST;
		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - 0.4f, 0.f, 0.f);
		_float iRandNum = (rand() % 10 + 10) * 0.1f;
		EffectDesc.vInitScale = _float3(iRandNum, iRandNum, iRandNum);
		EffectDesc.fDeadTime = 0.8f;
		CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	}
}

_uint CMonster::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{
	if (fDamage <= 0 || m_bDead)
		return 0;

	m_tInfo.iCurrentHp -= (int)fDamage;

	if (m_tInfo.iCurrentHp <= 0)
	{
		m_tInfo.iCurrentHp = 0;
		return m_tInfo.iCurrentHp;
	}

	CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Hit.wav"), SOUND_MEFFECT, 0.2f);

	_tchar	sz_SoundMonster[MAX_PATH];
	_float fVolume = 0.5f;
	_uint iNum = rand() % 2 + 1;
	wcscpy_s(sz_SoundMonster, TEXT("LSword_Slash (%d).wav"));
	wsprintf(sz_SoundMonster, sz_SoundMonster, iNum);
	CGameInstance::Get_Instance()->PlaySounds(sz_SoundMonster, SOUND_PEFFECT, 0.2f);
	Make_GetAttacked_Effect(DamageCauser);

	m_bHit = true;
	m_dwHitTime = GetTickCount();

	return m_tInfo.iCurrentHp;
}

void CMonster::Check_Navigation(_float fTimeDelta)
{
	if (m_pNavigationCom == nullptr)
		return;
}

HRESULT CMonster::SetUp_ShaderResources()
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

	if (FAILED(m_pShaderCom->Set_RawValue("g_HitRed", &m_fHitRedColor, sizeof(_float))))
		return E_FAIL;


	if (m_pDissolveTexture != nullptr)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DissolveTexture", m_pDissolveTexture->Get_SRV(0))))
			return E_FAIL;


		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
			return E_FAIL;
	}
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster::SetUp_ShaderID()
{

	return S_OK;
}

void CMonster::Sound_By_State(_float fTimeDelta)
{
}



void CMonster::Free()
{
	__super::Free();


	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_MONSTER, this);
	Safe_Release(m_pDissolveTexture);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSPHERECom);

}