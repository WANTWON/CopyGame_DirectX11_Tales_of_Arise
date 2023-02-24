#include "stdafx.h"
#include "..\Public\TreasureBox.h"
#include "Player.h"
#include "Level_Manager.h"
#include "CameraManager.h"
#include "UI_InterectMsg.h"

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
	
	if (pArg != nullptr)
		memcpy(&m_Boxtag, pArg, sizeof(BOXDESC));

	m_pModelCom->Set_CurrentAnimIndex(CTreasureBox::ANIM::Close2);
	m_pModelCom->Set_CurrentAnimIndex(CTreasureBox::ANIM::Open2);
	m_pModelCom->Play_Animation(0.3f, false);

	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&m_Boxtag.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(m_Boxtag.vScale);

		if (m_Boxtag.m_fAngle != 0)
			m_pTransformCom->Rotation(XMLoadFloat3(&m_Boxtag.vRotation), XMConvertToRadians(m_Boxtag.m_fAngle));
	}

	return S_OK;
}

int CTreasureBox::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || Check_IsinFrustum(2.f) == false)
		return OBJ_NOEVENT;
	__super::Tick(fTimeDelta);

	if (m_bOpenFinish)
		return OBJ_NOEVENT;
	

	if (m_bOpen)
	{
		m_bIsAnimationFinished = m_pModelCom->Play_Animation(fTimeDelta, false);
		m_fTimeDeltaAcc += fTimeDelta;

		if (m_bIsAnimationFinished)
		{
			
			m_bOpen = false;
			m_bOpenFinish = true;
			CUI_Manager::Get_Instance()->AddItem(ITEMNAME_OMEGAELIXIR, ITEMTYPE_POTION, true, true);
			CUI_Manager::Get_Instance()->AddItem(ITEMNAME_HWANGJELLY, ITEMTYPE_JELLY, true, false);
			CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LEMONJELLY, ITEMTYPE_JELLY, true, false);
			//CUI_Manager::Get_Instance()->AddItem(ITEMNAME_GRAPEJELLY, ITEMTYPE_JELLY, true, false);
		}
		if (m_fTimeDeltaAcc > 0.02f)
			CGameInstance::Get_Instance()->StopSound(SOUND_OBJECT);

	}
	else
	{
		m_pModelCom->Play_Animation(0.f, false);
		m_bOpenFinish = false;
		
	}
		

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CTreasureBox::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick() || Check_IsinFrustum(2.f) == false)
		return ;
	__super::Late_Tick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EDGE_DETECTION, this);

	if (m_bOpenFinish)
		return;

	
	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	if (!pPlayer)
		return;

	m_bCollision = m_pSPHERECom->Collision(pPlayer->Get_Collider());

	if (m_bCollision)
	{
		if (GetKeyState('E') < 0)
			pPlayer->Set_PlayerCollectState(this);
		dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Open_sysmsg(1);
		
		/*if (!m_bOpen)
		{
			CUI_Manager::Get_Instance()->AddItem(ITEMNAME_ARSORSWORD, ITEMTYPE_SWORD, true, false);
			CUI_Manager::Get_Instance()->AddItem(ITEMNAME_HWANGJELLY, ITEMTYPE_JELLY, false, true);
			CUI_Manager::Get_Instance()->AddItem(ITEMNAME_GRAPEJELLY, ITEMTYPE_JELLY, false, true);
			CUI_Manager::Get_Instance()->AddItem(ITEMNAME_GRAPEJELLY, ITEMTYPE_JELLY, false, true);
		}*/

		if (m_bIsInteract)
		{
			m_bOpen = true;
			
			//Sound

			_bool m_bSoundStart = false;

			if (!m_bSoundStart)
			{


				CGameInstance::Get_Instance()->PlaySounds(TEXT("BoxOpen.wav"), SOUND_OBJECT, 0.4f);
				m_bSoundStart = true;
			}

			
		}
	}
	dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();
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

HRESULT CTreasureBox::Render_EdgeDetection()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 5)))
			return E_FAIL;
	}
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
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("TreasureBox02"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f, 1.f, 1.2f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.5f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), /*LEVEL_SNOWFIELD*/LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
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
