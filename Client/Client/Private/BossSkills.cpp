#include "stdafx.h"

#include "BossSkills.h"
#include "AstralDoubt.h"
#include "Player.h"

CBossSkills::CBossSkills(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBullet(pDevice, pContext)
{
}

CBossSkills::CBossSkills(const CBossSkills & rhs)
	: CBullet(rhs)
{
}

HRESULT CBossSkills::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossSkills::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	CAstralDoubt* pAstralDoubt = static_cast<CAstralDoubt*>(m_BulletDesc.pOwner);

	_float4x4 PivotMatrix = pAstralDoubt->Get_Model()->Get_PivotFloat4x4();
	_matrix ParentWorldMatrix = pAstralDoubt->Get_Transform()->Get_WorldMatrix();

	CHierarchyNode* pBone;

	switch (m_BulletDesc.eBulletType)
	{
		case BULLET_SPEAR_MULTI_1:
		{
			pBone = pAstralDoubt->Get_Model()->Get_BonePtr("HMIDDLE3_3_R");
			_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Bullet.dat"), SocketMatrix);
			m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, m_pEffects.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
			break;
		}
		case BULLET_SPEAR_MULTI_2:
		{
			pBone = pAstralDoubt->Get_Model()->Get_BonePtr("HMIDDLE2_3_L");
			_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Bullet.dat"), SocketMatrix);
			m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, m_pEffects.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
			break;
		}
		case BULLET_SPEAR_MULTI_3:
		{
			pBone = pAstralDoubt->Get_Model()->Get_BonePtr("HMIDDLE1_3_R");
			_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Bullet.dat"), SocketMatrix);
			m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, m_pEffects.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
			break;
		}
		case BULLET_SPEAR_MULTI_4:
		{
			pBone = pAstralDoubt->Get_Model()->Get_BonePtr("HMIDDLE3_3_L");
			_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Bullet.dat"), SocketMatrix);
			m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, m_pEffects.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
			break;
		}
		case BULLET_SPEAR_MULTI_5:
		{
			pBone = pAstralDoubt->Get_Model()->Get_BonePtr("HMIDDLE2_3_R");
			_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Bullet.dat"), SocketMatrix);
			m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, m_pEffects.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
			break;
		}
		case BULLET_SPEAR_MULTI_6:
		{
			pBone = pAstralDoubt->Get_Model()->Get_BonePtr("HMIDDLE1_3_L");
			_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Bullet.dat"), SocketMatrix);
			m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, m_pEffects.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
			break;
		}
		case BULLET_SPEAR_DYNAMIC:
		{
			//..

			break;
		}
		case BULLET_LASER:
		{
			pBone = pAstralDoubt->Get_Model()->Get_BonePtr("HEAD1_C");
			_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

			_matrix mWorldMatrix = XMMatrixIdentity();
			mWorldMatrix.r[3] = SocketMatrix.r[3];

			m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Head_Beam.dat"), mWorldMatrix);
			m_pTransformCom->Set_State(CTransform::STATE::STATE_TRANSLATION, m_pEffects.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));

			/* Populate Focus Points. */
			FOCUSPOINTDESC tFocusPointDesc;
			ZeroMemory(&tFocusPointDesc, sizeof(FOCUSPOINTDESC));

			_vector vOwnerPosition = pAstralDoubt->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
			_vector vOwnerRight = pAstralDoubt->Get_TransformState(CTransform::STATE::STATE_RIGHT);
			_vector vOwnerLook = pAstralDoubt->Get_TransformState(CTransform::STATE::STATE_LOOK);

			XMStoreFloat3(&tFocusPointDesc.vFocusPosition, (vOwnerPosition + XMVector4Normalize(vOwnerLook) * 50.f));
			tFocusPointDesc.vFocusPosition.y = 0.f;
			m_LaserFocusPoints.push_back(tFocusPointDesc);

			XMStoreFloat3(&tFocusPointDesc.vFocusPosition, (vOwnerPosition + (XMVector4Normalize(vOwnerLook) * 35.f) + (XMVector4Normalize(vOwnerRight) * 35.f)));
			tFocusPointDesc.vFocusPosition.y = 0.f;
			m_LaserFocusPoints.push_back(tFocusPointDesc);

			XMStoreFloat3(&tFocusPointDesc.vFocusPosition, (vOwnerPosition + (XMVector4Normalize(vOwnerLook) * 20.f) + (XMVector4Normalize(-vOwnerRight) * 20.f)));
			tFocusPointDesc.vFocusPosition.y = 0.f;
			m_LaserFocusPoints.push_back(tFocusPointDesc);

			XMStoreFloat3(&tFocusPointDesc.vFocusPosition, (vOwnerPosition + (XMVector4Normalize(vOwnerLook) * 10.f) + (XMVector4Normalize(+vOwnerRight) * 10.f)));
			tFocusPointDesc.vFocusPosition.y = 0.f;
			m_LaserFocusPoints.push_back(tFocusPointDesc);

			m_vFocusPoint = m_LaserFocusPoints[m_iCurrentFocusPoint].vFocusPosition;
			m_vNextFocusPoint = m_LaserFocusPoints[m_iCurrentFocusPoint + 1].vFocusPosition;

			break;
		}
	}

	return S_OK;
}

int CBossSkills::Tick(_float fTimeDelta)
{
	_int iSuperTick = __super::Tick(fTimeDelta);

	if (iSuperTick == OBJ_DEAD)
	{
		Dead_Effect();
		return OBJ_DEAD;
	}

	if (iSuperTick == OBJ_NOSHOW)
		return OBJ_NOSHOW;

	_float fTime = CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));

	switch (m_BulletDesc.eBulletType)
	{
		case BULLET_SPEAR_MULTI_1:
		case BULLET_SPEAR_MULTI_2:
		case BULLET_SPEAR_MULTI_3:
		case BULLET_SPEAR_MULTI_4:
		case BULLET_SPEAR_MULTI_5:
		case BULLET_SPEAR_MULTI_6:
		{
			Tick_SpearMulti(fTime);
			break;
		}
		case BULLET_LASER:
		{
			Tick_Laser(fTime);
			break;
		}
	}

	if (m_pAABBCom != nullptr)
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pSPHERECom != nullptr)
		m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pOBBCom != nullptr)
		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CBossSkills::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	switch (m_BulletDesc.eBulletType)
	{
		case BULLET_LASER:
		{
			LateTick_Laser(fTimeDelta);
			break;
		}
	}
}

void CBossSkills::Collision_Check()
{
	if (Check_Exception() == false)
		return;

	switch (m_BulletDesc.eBulletType)
	{
		case BULLET_SPEAR_MULTI_1:
		case BULLET_SPEAR_MULTI_2:
		case BULLET_SPEAR_MULTI_3:
		case BULLET_SPEAR_MULTI_4:
		case BULLET_SPEAR_MULTI_5:
		case BULLET_SPEAR_MULTI_6:
		{
			CBaseObj* pCollisionTarget = nullptr;
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pSPHERECom, &pCollisionTarget))
			{
				static_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, false);
				m_bCollided = true;
				m_bDead = true;
			}

			break;
		}
		case BULLET_LASER:
		{
			CBaseObj* pCollisionTarget = nullptr;
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pOBBCom, &pCollisionTarget))
				static_cast<CPlayer*>(pCollisionTarget)->Take_Damage(m_BulletDesc.iDamage, m_BulletDesc.pOwner, false);

			break;
		}
	}
}

void CBossSkills::Tick_SpearMulti(_float fTimeDelta)
{
	if (m_fBulletTimer > m_BulletDesc.fDeadTime)
	{
		Stop_Effect();
		m_bDead = true;
	}
	else
		m_fBulletTimer += fTimeDelta;

	m_pTransformCom->Go_PosDir(fTimeDelta, m_BulletDesc.vTargetDir);

	for (auto& pEffect : m_pEffects)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;

		if (pEffect)
			pEffect->Set_State(CTransform::STATE_TRANSLATION, Get_TransformState(CTransform::STATE_TRANSLATION));
	}
}

void CBossSkills::Tick_Laser(_float fTimeDelta)
{
	if (m_fBulletTimer > m_BulletDesc.fDeadTime)
	{
		Stop_Effect();
		m_bDead = true;
	}
	else
		m_fBulletTimer += fTimeDelta * 2.f;

	/* Focus Points Logic */
	_float fLerpTime = m_BulletDesc.fDeadTime / (m_LaserFocusPoints.size() - 1);
	if (m_fFocusTimer > fLerpTime)
	{
		m_iCurrentFocusPoint++;

		m_vFocusPoint = m_LaserFocusPoints[m_iCurrentFocusPoint].vFocusPosition;
		m_vNextFocusPoint = m_LaserFocusPoints[m_iCurrentFocusPoint + 1].vFocusPosition;

		m_fFocusTimer = 0.f;
	}
	else
		m_fFocusTimer += fTimeDelta * 2.f;
	
	/* Lerping to Next Focus Point */
	_float fInterpFactor = m_fFocusTimer / fLerpTime;
	_vector vLerpPosition = XMLoadFloat3(&m_vFocusPoint) + fInterpFactor * (XMLoadFloat3(&m_vNextFocusPoint) - XMLoadFloat3(&m_vFocusPoint));

	vLerpPosition = XMVectorSetW(vLerpPosition, 1.f);

	m_pTransformCom->LookAt(vLerpPosition);
}

void CBossSkills::LateTick_Laser(_float fTimeDelta)
{
	for (auto& pEffect : m_pEffects)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;

		if (!pEffect)
			continue;

		if (!wcscmp(pEffect->Get_PrototypeId(), TEXT("Cylinder_ko_01")))
		{
			_matrix mEffectWorldMatrix = pEffect->Get_Transform()->Get_WorldMatrix();
			
			_vector vScale, vRotationQuaternion, vTranslation;
			XMMatrixDecompose(&vScale, &vRotationQuaternion, &vTranslation, mEffectWorldMatrix);

			_matrix mScalingMatrix = XMMatrixScalingFromVector(vScale);
			_matrix mTranslationMatrix = XMMatrixTranslationFromVector(vTranslation);

			_matrix mBulletWorldMatrix = m_pTransformCom->Get_WorldMatrix();
			_vector vScaleBullet, vRotationBullet, vTranslationBullet;
			XMMatrixDecompose(&vScaleBullet, &vRotationBullet, &vTranslationBullet, mBulletWorldMatrix);

			_matrix mRotationMatrix = XMMatrixRotationQuaternion(vRotationBullet);

			mEffectWorldMatrix = XMMatrixIdentity();
			mEffectWorldMatrix = XMMatrixMultiply(mEffectWorldMatrix, mScalingMatrix);
			mEffectWorldMatrix = XMMatrixMultiply(mEffectWorldMatrix, mRotationMatrix);
			mEffectWorldMatrix = XMMatrixMultiply(mEffectWorldMatrix, mTranslationMatrix);

			pEffect->Get_Transform()->Set_WorldMatrix(mEffectWorldMatrix);
		}
	}
}

void CBossSkills::Dead_Effect()
{
	for (auto& pEffect : m_pEffects)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;
	}

	switch (m_BulletDesc.eBulletType)
	{
		case BULLET_SPEAR_MULTI_1:
		case BULLET_SPEAR_MULTI_2:
		case BULLET_SPEAR_MULTI_3:
		case BULLET_SPEAR_MULTI_4:
		case BULLET_SPEAR_MULTI_5:
		case BULLET_SPEAR_MULTI_6:
		{
			if (m_bCollided)
			{
				_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
				CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Bullet_Explosion.dat"), mWorldMatrix);
			}

			break;
		}
	}
}

void CBossSkills::Stop_Effect()
{
	for (auto& pEffect : m_pEffects)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;

		if (!pEffect)
			continue;

		CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(pEffect);
		if (pParticleSystem)
			pParticleSystem->Set_Stop(true);
	}
}

HRESULT CBossSkills::Ready_Components(void * pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	switch (m_BulletDesc.eBulletType)
	{
		case BULLET_SPEAR_MULTI_1:
		case BULLET_SPEAR_MULTI_2:
		case BULLET_SPEAR_MULTI_3:
		case BULLET_SPEAR_MULTI_4:
		case BULLET_SPEAR_MULTI_5:
		case BULLET_SPEAR_MULTI_6:
		{
			/* For.Com_SPHERE */
			CCollider::COLLIDERDESC ColliderDesc;
			ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

			ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
			ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
			ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

			if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
				return E_FAIL;

			break;
		}
		case BULLET_LASER:
		{
			/* For.Com_OBB */

			CCollider::COLLIDERDESC ColliderDesc;
			ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

			ColliderDesc.vScale = _float3(2.5f, 1.0f, 80.1f);
			ColliderDesc.vPosition = _float3(0.f, 0.f, 40.f);

			if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
				return E_FAIL;

			break;
		}
	}

	return S_OK;
}

CBossSkills * CBossSkills::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBossSkills* pInstance = new CBossSkills(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Create: CBossSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBossSkills::Clone(void * pArg)
{
	CBossSkills* pInstance = new CBossSkills(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Clone: CBossSkills"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossSkills::Free()
{
	__super::Free();
}