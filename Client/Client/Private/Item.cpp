#include "stdafx.h"

#include "Item.h"
#include "Player.h"
#include "Level_Manager.h"
#include "PlayerManager.h"
#include "UI_Get_item_Popup.h"
#include "UI_InterectMsg.h"
#include "UI_Questmsg.h"
#include "Effect.h"
#include "ParticleSystem.h"

CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CInteractObject(pDevice, pContext)
{
}

CItem::CItem(const CItem & rhs)
	: CInteractObject(rhs)
{

}

HRESULT CItem::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	if (pArg != nullptr)
		memcpy(&m_ItemDesc, pArg, sizeof(ITEMDESC));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_ItemDesc.m_ModelDesc.WorldMatrix));
		_vector vPosition = XMLoadFloat3(&m_ItemDesc.m_ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_pTransformCom->Set_Rotation(m_ItemDesc.m_ModelDesc.vRotation);
		Set_Scale(m_ItemDesc.m_ModelDesc.vScale);

		if (m_ItemDesc.m_ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_ItemDesc.m_ModelDesc.m_fAngle));

	}

	return S_OK;
}

int CItem::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	
	if (m_bDead)
	{
		dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();
		return OBJ_DEAD;
	}

	__super::Tick(fTimeDelta);

	/* Dissolve */
	if (m_bIsGain)
	{
		if (m_fDissolveTimer > m_fDissolveLifespan)
			m_bDead = true;
		else
			m_fDissolveTimer += fTimeDelta;
	}
	
	Spawn_Particles();

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CItem::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;

	__super::Late_Tick(fTimeDelta);

	/* Particles Remove */
	for (auto& pPickupFlare : m_pPickupFlares)
	{
		if (pPickupFlare && pPickupFlare->Get_PreDead())
			pPickupFlare = nullptr;	
	}

	if (m_ItemDesc.etype == ITEMTYPE::CRYSTAL)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GLOW, this);

	if (!m_bIsGain)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EDGE_DETECTION, this);

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	if (!pPlayer)
		return;

	m_bCollision = m_pSPHERECom->Collision(pPlayer->Get_Collider());
	
	if (m_bCollision)
	{
		dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Open_sysmsg(1);
		m_bfirst = true;
		if (CGameInstance::Get_Instance()->Key_Up(DIK_E) && !m_bIsGain)
		{
			dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();
			m_bIsGain = true;
			
			_vector vOffset = XMVectorSet(0.f, m_fRadius * 2, 0.f, 0.f);
			_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

			_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation;

			m_pGetItem = CEffect::PlayEffectAtLocation(TEXT("Get_Item.dat"), mWorldMatrix);

			if (!m_pPickupFlares.empty())
			{
				for (auto& pPickupFlare : m_pPickupFlares)
				{
					CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(pPickupFlare);
					if (pPickupFlare)
						pParticleSystem->Set_Stop(true);
				}
			}

			pPlayer->Set_PlayerCollectState();

			switch (m_ItemDesc.etype)
			{
				case APPLE:
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_APPLE, ITEMTYPE_FRUIT,true , false);
					break;
				case LETTUCE:
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LETTUCE, ITEMTYPE_VEGITABLE, true, false);
				
					/* Quest Handling */
					if (CUI_Manager::Get_Instance()->Get_QuestIndex() == 1 && CUI_Manager::Get_Instance()->Get_QuestComplete(0) == false)
					{
						CUI_Questmsg::QUESTMSGDESC questmsgdesc;
						ZeroMemory(&questmsgdesc, sizeof(CUI_Questmsg::QUESTMSGDESC));
						questmsgdesc.maxcount = 3;
						questmsgdesc.eName = QUEST_LETTUCE;
						CUI_Manager::Get_Instance()->Plus_Quest1_Lettuce();
						questmsgdesc.currentcount = CUI_Manager::Get_Instance()->Get_Quest1_Lettuce();

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTMESSAGE"), LEVEL_STATIC, TEXT("QMSG"), &questmsgdesc)))
							return;
					}
					break;
				case PLANT:
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_HERB, ITEMTYPE_MATERIAL, true, false);
				
					/* Quest Handling */
					if (CUI_Manager::Get_Instance()->Get_QuestIndex() == 1 && CUI_Manager::Get_Instance()->Get_QuestComplete(0) == false)
					{
						CUI_Questmsg::QUESTMSGDESC questmsgdesc;
						ZeroMemory(&questmsgdesc, sizeof(CUI_Questmsg::QUESTMSGDESC));
						questmsgdesc.maxcount = 3;
						questmsgdesc.eName = QUEST_HERB;
						CUI_Manager::Get_Instance()->Plus_Quest1_Herb();

						questmsgdesc.currentcount = CUI_Manager::Get_Instance()->Get_Quest1_Herb();

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTMESSAGE"), LEVEL_STATIC, TEXT("QMSG"), &questmsgdesc)))
							return;
					}
					break;
				case MUSHROOM:
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_PYOGOMUSHROOM, ITEMTYPE_MATERIAL, true, false); 
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_SONG2MUSHROOM, ITEMTYPE_MATERIAL, true, false);
					break;
				case BOX:
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_ARSORSWORD, ITEMTYPE_SWORD,false,true);
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_HWANGJELLY, ITEMTYPE_JELLY, true, false);
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_GRAPEJELLY, ITEMTYPE_JELLY, true, false);
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_GRAPEJELLY, ITEMTYPE_JELLY, true, false);
					break;
				case JEWEL:
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_SALTROCK, ITEMTYPE_MATERIAL, true, false);
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_SALTROCK, ITEMTYPE_MATERIAL, true, false);
					CUI_Manager::Get_Instance()->AddItem(ITEMNAME_SALTROCK, ITEMTYPE_MATERIAL, true, false);
					break;
			}
			
			m_fTimeDeltaAcc += fTimeDelta;

			_bool m_bSoundStart = false;

			if (m_bIsGain)
			{
				if (!m_bSoundStart)
				{
					if (m_fTimeDeltaAcc > 0.04f)
						CGameInstance::Get_Instance()->StopSound(SOUND_OBJECT);

					CGameInstance::Get_Instance()->PlaySounds(TEXT("ItemGain.wav"), SOUND_OBJECT, 0.4f);
					m_bSoundStart = true;
				}
			}
		}
	}
	
	if(m_bfirst && !m_bCollision)
	{
		m_bfirst = false;
		dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();
	}
}

HRESULT CItem::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CItem::Render_Glow()
{
	if (!m_pShaderCom || !m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_bool bUseDiffuseColor = false;
	if (FAILED(m_pShaderCom->Set_RawValue("g_bUseDiffuseColor", &bUseDiffuseColor, sizeof(_bool))))
		return E_FAIL;
	_float3 vGlowColor = _float3(0.f, 1.f, 1.f);
	if (FAILED(m_pShaderCom->Set_RawValue("g_vGlowColor", &vGlowColor, sizeof(_float3))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshContainers();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GlowTexture", nullptr)))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;
		if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_GlowTexture", i, aiTextureType_EMISSIVE)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_bIsGain ? 9 : 8)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CItem::Render_EdgeDetection()
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
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 7)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CItem::Ready_Components(void * pArg)
{
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	switch (m_ItemDesc.etype)
	{
	case JEWEL:
	case CRYSTAL:
		ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		break;
	default:
		ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
		ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
		ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
		break;
	}
	/* For.Com_SPHERE */
	
	
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_ItemDesc.m_ModelDesc.pModeltag, (int)strlen(m_ItemDesc.m_ModelDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem::SetUp_ShaderResources()
{
	if (FAILED(__super::SetUp_ShaderResources()))
		E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveTimer", &m_fDissolveTimer, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveLifespan", &m_fDissolveLifespan, sizeof(_float))))
		return E_FAIL;
	_float4 vDissolveColor = _float4(0.85f, 0.9f, 1.f, 1.f);
	if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveColor", &vDissolveColor, sizeof(_float4))))
		return E_FAIL;
	_float4 vDissolveHighlight = _float4(0.55f, 0.9f, 1.f, 1.f);
	if (FAILED(m_pShaderCom->Set_RawValue("g_DissolveHighlight", &vDissolveHighlight, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem::SetUp_ShaderID()
{
	if (false == m_bIsGain)
		m_eShaderID = SHADER_NONANIMDEFAULT;
	else
		m_eShaderID = SHADER_NONANIM_DISSOLVE;

	return S_OK;
}

void CItem::Spawn_Particles()
{
	if (m_pPickupFlares.empty())
	{
		_vector vOffset;
		_tchar wcParticlesName[MAX_PATH] = TEXT("");
		switch (m_ItemDesc.etype)
		{
			case ITEMTYPE::APPLE:
				vOffset = XMVectorSet(0.f, 0.f, 0.f, 0.f);
				wcscpy_s(wcParticlesName, MAX_PATH, TEXT("Pickup_Flares.dat"));
				break;
			case ITEMTYPE::BOX:
				vOffset = XMVectorSet(0.f, 2.f, 0.f, 0.f);
				wcscpy_s(wcParticlesName, MAX_PATH, TEXT("Pickup_Flares_Big.dat"));
				break;
			case ITEMTYPE::CRYSTAL:
				vOffset = XMVectorSet(0.f, 4.f, 0.f, 0.f);
				wcscpy_s(wcParticlesName, MAX_PATH, TEXT("Pickup_Flares_Big.dat"));
				break;
			case ITEMTYPE::JEWEL:
				vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
				wcscpy_s(wcParticlesName, MAX_PATH, TEXT("Pickup_Flares_Big.dat"));
				break;
			case ITEMTYPE::LETTUCE:
				vOffset = XMVectorSet(0.f, 2.f, 0.f, 0.f);
				wcscpy_s(wcParticlesName, MAX_PATH, TEXT("Pickup_Flares.dat"));
				break;
			case ITEMTYPE::MUSHROOM:
				vOffset = XMVectorSet(0.f, 2.f, 0.f, 0.f);
				wcscpy_s(wcParticlesName, MAX_PATH, TEXT("Pickup_Flares.dat"));
				break;
			case ITEMTYPE::PLANT:
				vOffset = XMVectorSet(0.f, 2.f, 0.f, 0.f);
				wcscpy_s(wcParticlesName, MAX_PATH, TEXT("Pickup_Flares.dat"));
				break;
		}

		_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;

		_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;

		m_pPickupFlares = CEffect::PlayEffectAtLocation(wcParticlesName, mWorldMatrix);
	}
}

CItem * CItem::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CItem* pInstance = new CItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Create: CItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone(void * pArg)
{
	CItem* pInstance = new CItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Clone: CItem"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem::Free()
{
	__super::Free();	
}