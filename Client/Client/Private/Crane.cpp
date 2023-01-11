#include "stdafx.h"
#include "..\Public\Crane.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_Manager.h"
#include "CollapseTile.h"

CCrane::CCrane(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNonAnim(pDevice, pContext)
{
}

CCrane::CCrane(const CCrane & rhs)
	: CNonAnim(rhs)
{
}

HRESULT CCrane::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCrane::Initialize(void * pArg)
{
	
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	m_eObjectID = OBJ_KEY;

	if (pArg != nullptr)
	{
		_vector vecPostion = XMLoadFloat3((_float3*)pArg);
		vecPostion = XMVectorSetY(vecPostion, 4.f);
		vecPostion = XMVectorSetW(vecPostion, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);
	}

	Set_Scale(_float3(3.f, 3.f, 3.f));
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);

	return S_OK;
}

int CCrane::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);
		return OBJ_DEAD;
	}

	if (m_IsMoved[DIR_X] == true && m_IsMoved[DIR_Z] == true)
	{
		_float fPositionY = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		_float fPositionX = XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		_float fPositionZ = XMVectorGetZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

		m_fMoveSoundTime += fTimeDelta;

		if (m_bUp && fPositionY < 4)
			m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		else if (m_bUp && fPositionX > -4.5f)
			m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(-1.f, 0.f, 0.f, 0.f));
		else if (m_bUp && fPositionZ > -3.f)
			m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, 0.f, -1.f, 0.f));
		else if (fPositionZ <= -3.f)
		{
			if (!m_bFinished)
				CUI_Manager::Get_Instance()->Set_PlayGame(false);
			m_bFinished = true;
			CGameInstance::Get_Instance()->StopSound(SOUND_OBJECT);
			
		}
			

		if (fPositionY > 1 && !m_bUp)
			m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, -1.f, 0.f, 0.f));
		else
			m_bUp = true;
		

		if (m_fMoveSoundTime > 3.f && !m_bFinished && m_bUp)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Crane_Move.wav"), SOUND_OBJECT, 0.6f);
			m_fMoveSoundTime = 0.f;
		}
		else if (!m_bUp && !m_bMove)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Crane_Move2.wav"), SOUND_OBJECT, 0.4f);
			m_bMove = true;
		}

	}
	else
	{
		if (m_fVelocity > 0)
		{
			m_fVelocity -= 0.005f;

			_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector vDir = XMVectorSet(m_eDir[DIR_X], 0.f, m_eDir[DIR_Z], 0.f);
			vPosition = vPosition + vDir*m_fVelocity;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

			if (!m_bMove)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Crane_Move.wav"), SOUND_OBJECT, 0.4f);
				m_bMove = true;
			}

		}
		if (m_fVelocity <= 0)
		{
			CGameInstance::Get_Instance()->StopSound(SOUND_OBJECT);
			m_IsMoved[m_eInputDir] = true;
			m_fVelocity = 0.f;
			m_bMove = false;
		}


	}

	return OBJ_NOEVENT;
}

void CCrane::Late_Tick(_float fTimeDelta)
{
	if (nullptr != m_pRendererCom)
	{
#ifdef _DEBUG
		if (m_pAABBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pAABBCom);
		if (m_pOBBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pOBBCom);
		if (m_pSPHERECom != nullptr)
			m_pRendererCom->Add_Debug(m_pSPHERECom);
#endif

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	}
		

	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

		

	if (m_pCollisionObj == nullptr && CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_ITEM, m_pSPHERECom, &m_pCollisionObj))
	{
		if (m_pCollisionObj != nullptr)
		{
			if (m_pCollisionObj->Get_Dead())
				m_pCollisionObj = nullptr;
			else
			{
				_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_vector vItemPosition = m_pCollisionObj->Get_TransformState(CTransform::STATE_POSITION);
				_vector vDirection = XMVector3Normalize(vPosition - vItemPosition);

				vItemPosition += fTimeDelta*vDirection*3.f;

				m_pCollisionObj->Set_State(CTransform::STATE_POSITION, vItemPosition);
			}
		}
	}

	if (m_pCollisionObj != nullptr)
	{
		if (m_pCollisionObj->Get_Dead())
			m_pCollisionObj = nullptr;
		else
		{
			if (m_bFinished)
			{
				_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				vPosition = XMVectorSetY(vPosition,1.f);
				m_pCollisionObj->Set_State(CTransform::STATE_POSITION, vPosition);
				m_pCollisionObj = nullptr;
				
			}
			else
			{
				_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				_vector vItemPosition = m_pCollisionObj->Get_TransformState(CTransform::STATE_POSITION);
				_vector vDirection = XMVector3Normalize(vPosition - vItemPosition);

				vItemPosition += fTimeDelta*vDirection*3.f;

				m_pCollisionObj->Set_State(CTransform::STATE_POSITION, vItemPosition);
			}
			
		}
		
	}

	CBaseObj* pColiisionTile = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_TRAP, m_pSPHERECom, &pColiisionTile))
	{
		if (m_pCollisionObj == nullptr)
		{
			m_pCollisionObj = dynamic_cast<CCollapseTile*>(pColiisionTile)->Get_CollisionObj();
			_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			m_pCollisionObj->Set_State(CTransform::STATE_POSITION, vPosition);
			dynamic_cast<CCollapseTile*>(pColiisionTile)->Set_Check(true);
		}
			
	}

}

HRESULT CCrane::Render()
{
	__super::Render();

	return S_OK;

}

void CCrane::Input_Dir(DIRINPUT eDIr)
{
	m_eInputDir = eDIr;

	m_eDir[DIR_X] = 0.f; 	m_eDir[DIR_Z] = 0.f;
	m_eDir[eDIr] = 1.f; 
	m_fVelocity += 0.01f; 
	
}

void CCrane::Setting_Default()
{
	m_eInputDir = DIR_END;
	m_bFinished = false;  m_bUp = false;
	m_pCollisionObj = nullptr;
	for (_uint i = 0; i < DIR_END; ++i)
	{
		m_IsMoved[i] = false;
		m_eDir[i] = 0.f;
	}
		
	
}


HRESULT CCrane::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_ROOM, TEXT("Prototype_Component_Model_Crane"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_SPHERE*/
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}


CCrane * CCrane::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCrane*	pInstance = new CCrane(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCrane"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCrane::Clone(void * pArg)
{
	CCrane*	pInstance = new CCrane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCrane"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrane::Free()
{
	__super::Free();
	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);
}
