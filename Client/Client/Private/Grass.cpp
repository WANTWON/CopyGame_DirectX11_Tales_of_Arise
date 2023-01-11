#include "stdafx.h"
#include "..\Public\Grass.h"
#include "GameInstance.h"
#include "Player.h"
#include "UI_Manager.h"
#include "InvenItem.h"
#include "ObjectEffect.h"

CGrass::CGrass(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CNonAnim(pDevice, pContext)
{
}

CGrass::CGrass(const CGrass & rhs)
	: CNonAnim(rhs)
{
}


HRESULT CGrass::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGrass::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_GrassDesc, pArg, sizeof(GRASSDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	//CPickingMgr::Get_Instance()->Add_PickingGroup(this);

	m_eObjectID = OBJ_KEY;

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&m_GrassDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}

	Set_Scale(_float3(1.2f, 1.2f, 1.2f));
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);

	//char cName[MAX_PATH];
	//switch (m_GrassDesc.eType)
	//{
	//	case LAWN:
	//		/* For.Com_Model*/
	//		pData_Manager->TCtoC(TEXT("Lawn"), cName);
	//		pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_NONANIM);
	//		//ERR_MSG(TEXT("Save_Bin_Model"));
	//		
	//		break;
	//	
	//	case GRASS:
	//		/* For.Com_Model*/
	//		pData_Manager->TCtoC(TEXT("Grass"), cName);
	//		pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_NONANIM);
	//		//ERR_MSG(TEXT("Save_Bin_Model"));
	//		
	//		break;

	//	case GRASS2x2:
	//		pData_Manager->TCtoC(TEXT("Grass2x2"), cName);
	//		pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_NONANIM);
	//		//ERR_MSG(TEXT("Save_Bin_Model"));

	//		break;
	//default:
	//	break;
	//}

	//RELEASE_INSTANCE(CData_Manager);
	//RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

int CGrass::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);

		return OBJ_DEAD;
	}
		
	return OBJ_NOEVENT;
}

void CGrass::Late_Tick(_float fTimeDelta)
{
	if (Check_IsinFrustum() == false)
		return;

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		
#ifdef _DEBUG
		if (m_pAABBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pAABBCom);
		if (m_pOBBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pOBBCom);
		if (m_pSPHERECom != nullptr)
			m_pRendererCom->Add_Debug(m_pSPHERECom);
#endif
	}
		
	Compute_CamDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	
	if (!m_bCut && CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PBULLET, m_pSPHERECom))
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		CPlayer::ANIM ePlayerState = pPlayer->Get_AnimState();

		if (ePlayerState == CPlayer::SLASH)
		{
			
			m_bCut = true;

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			pGameInstance->PlaySounds(TEXT("5_Obj_GrassCut.wav"), SOUND_OBJECT, g_fObjVolume);

			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::MODEL;

			if(m_GrassDesc.eType == GRASS || m_GrassDesc.eType == GRASS2x2)
				EffectDesc.eEffectID = CObjectEffect::GRASS;
			else 
				EffectDesc.eEffectID = CObjectEffect::LAWN;
			for (int i = 0; i < 4; ++i)
			{
				
				EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - 0.4f, 0.f, 0.f);
				EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
				EffectDesc.vLook = XMVectorSet((rand() % 20 - 10) * 0.1f, /*(rand() % 20 -10) * 0.1f*/ 1.f, (rand() % 20 - 10)* 0.1f, 0.f);
				EffectDesc.fDeadTime = 0.5f;
				pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);

			}

			RELEASE_INSTANCE(CGameInstance);
		}
			
	}

	//if(m_GrassDesc.eType == LAWN)
		//SetUp_BillBoard();

}

HRESULT CGrass::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	if (m_bCut)
	{
		switch (m_GrassDesc.eType)
		{
		case LAWN:
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", 1, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", 1, aiTextureType_NORMALS)))
				return E_FAIL;
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", 1, aiTextureType_SPECULAR)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Render(m_pShaderCom, 1, m_eShaderID)))
				return E_FAIL;
			break;
		case GRASS:
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", 0, aiTextureType_DIFFUSE)))
				return E_FAIL;
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", 0, aiTextureType_NORMALS)))
				return E_FAIL;
			if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", 0, aiTextureType_SPECULAR)))
				return E_FAIL;
			if (FAILED(m_pModelCom->Render(m_pShaderCom, 0, m_eShaderID)))
				return E_FAIL;
			break;
		case GRASS2x2:
			for (_uint i = 0; i < iNumMeshes; ++i)
			{
				if (i % 2 == 1)
					continue;

				if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
					return E_FAIL;
				if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
					return E_FAIL;
				if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", i, aiTextureType_SPECULAR)))
					return E_FAIL;
				if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
					return E_FAIL;
			}
			break;
		default:
			break;
		}

			
	}
	else
	{
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
	}
	

	return S_OK;

}


HRESULT CGrass::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 4.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	switch (m_GrassDesc.eType)
	{
	case LAWN:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Lawn"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case GRASS:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Grass"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	case GRASS2x2:
		/* For.Com_Model*/
		if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Grass2x2"), (CComponent**)&m_pModelCom)))
			return E_FAIL;
		break;
	default:
		break;
	}

	CCollider::COLLIDERDESC		ColliderDesc;

	/* For.Com_SPHERE */
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}



CGrass * CGrass::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CGrass*	pInstance = new CGrass(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CGrass"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CGrass::Clone(void * pArg)
{
	CGrass*	pInstance = new CGrass(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CGrass"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrass::Free()
{
	__super::Free();
}
