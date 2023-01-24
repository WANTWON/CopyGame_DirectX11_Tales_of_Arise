#include "stdafx.h"
#include "..\Public\TreasureBox.h"
#include "Player.h"
#include "Level_Manager.h"
#include "CameraManager.h"

CTreasureBox::CTreasureBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInteractObject(pDevice, pContext)
{
}

CTreasureBox::CTreasureBox(const CTreasureBox & rhs)
	: CInteractObject(rhs)
{

}

HRESULT CTreasureBox::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreasureBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_INTERACT, this);
	
	_vector vPosition = *(_vector*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);

	m_pModelCom->Set_CurrentAnimIndex(CTreasureBox::ANIM::Close2);
	m_pModelCom->Set_NextAnimIndex(CTreasureBox::ANIM::Open2);
	m_pModelCom->Play_Animation(0.3f, false);

	return S_OK;
}

int CTreasureBox::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);
	

	if (m_bCollision && 0 == m_iCollisiongCount)
	{
		m_bIsAnimationFinished =  m_pModelCom->Play_Animation(fTimeDelta, false);
		if (m_bIsAnimationFinished)
		{
			m_iCollisiongCount = 1;
			m_bOpen = true;
		}

	}

	if (false == m_bCollision && false == m_bOpen)
	{
		m_pModelCom->Play_Animation(0.f, false);

	}
	//else if(false == m_bCollision && false == m_bOpen)
	//{
	//	m_pModelCom->Play_Animation(0.f, false);

	//}
	
	//if (m_bIsAnimationFinished)
	//	m_iCollisiongCount = 1;

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CTreasureBox::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CGameObject* pGameObject = pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameObject);
	if (!pPlayer)
		return;


	
	m_bCollision = m_pSPHERECom->Collision(pPlayer->Get_Collider());
	
		
	/*if (m_bIsAnimationFinished)
	{
		m_iCollisiongCount = 1;
		m_bIsAnimationFinished = m_pModelCom->Play_Animation(1, Is_AnimationLoop(m_pModelCom->Get_CurrentAnimIndex()));
	}
*/
	




}

HRESULT CTreasureBox::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	//_bool bGlow = true;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool))))
	//	return E_FAIL;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		//if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
		//	return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_eShaderID)))
			return E_FAIL;
	}

	//bGlow = false;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_bGlow", &bGlow, sizeof(_bool))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CTreasureBox::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_ShadowLightView(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	//for(_uint i = 0; i < iNumMeshes; ++i)
	//{
	//	if (FAILED(m_pModelCom->Render(m_pShaderCom, i, SHADER_ANIMSHADOW)))
	//		return S_OK;
	//}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CTreasureBox * CTreasureBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTreasureBox* pInstance = new CTreasureBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTreasureBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTreasureBox::Clone(void * pArg)
{
	CTreasureBox* pInstance = new CTreasureBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTreasureBox"));
		Safe_Release(pInstance);
	}



	return pInstance;
}

void CTreasureBox::Free()
{
	__super::Free();

}

HRESULT CTreasureBox::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_BATTLE, TEXT("TreasureBox02"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f, 1.f, 1.2f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_BATTLE, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	///* For.Com_AABB */
	//CCollider::COLLIDERDESC AABB_ColliderDesc;
	//ZeroMemory(&AABB_ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	//AABB_ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	//AABB_ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	//AABB_ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_AABB"), LEVEL_SNOWFIELD, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom, &AABB_ColliderDesc)))
	//	return E_FAIL;
	
		

	return S_OK;
}

_bool CTreasureBox::Is_AnimationLoop(_uint eAnimId)
{
	return false;
}
