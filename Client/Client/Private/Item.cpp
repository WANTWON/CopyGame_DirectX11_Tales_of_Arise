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
		_vector vPosition = XMLoadFloat3(&m_ItemDesc.ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(m_ItemDesc.ModelDesc.vScale);

		if (m_ItemDesc.ModelDesc.m_fAngle != 0)
			m_pTransformCom->Rotation(XMLoadFloat3(&m_ItemDesc.ModelDesc.vRotation), XMConvertToRadians(m_ItemDesc.ModelDesc.m_fAngle));
	}

	switch (m_ItemDesc.etype)
	{
		case ITEMTYPE::LETTUCE:
		case ITEMTYPE::PLANT:
		case ITEMTYPE::MUSHROOM:
		{
			m_fRadius = 1.f;
			break;
		}
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

	if (m_pPickupFlares.empty())
	{
		switch (m_ItemDesc.etype)
		{
			case ITEMTYPE::APPLE:
			case ITEMTYPE::LETTUCE:
			case ITEMTYPE::JEWEL:
			case ITEMTYPE::PLANT:
			case ITEMTYPE::MUSHROOM:
			{
				/*_vector vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
				_vector vLocation = m_pTransformCom->Get_State(CTransform::STATE::STATE_TRANSLATION) + vOffset;
				
				_matrix mWorldMatrix = m_pTransformCom->Get_WorldMatrix();
				mWorldMatrix.r[3] = vLocation;

				m_pPickupFlares = CEffect::PlayEffectAtLocation(TEXT("Pickup_Flares.dat"), mWorldMatrix);*/
				break;
			}
		}
	}

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	return OBJ_NOEVENT;
}

void CItem::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return;
	__super::Late_Tick(fTimeDelta);

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	if (!pPlayer)
		return;

	m_bCollision = m_pSPHERECom->Collision(pPlayer->Get_Collider());
	
	if (m_bCollision)
	{
		dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Open_sysmsg(1);
		m_bfirst = true;
		if (CGameInstance::Get_Instance()->Key_Up(DIK_E)&&!m_bIsGain)
		{
			dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();//¤·¤§
			m_bIsGain = true;
			
			/*_vector vOffset = XMVectorSet(0.f, m_fRadius, 0.f, 0.f);
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
			}*/

			//ITEMINFO*  itempointer = new  ITEMINFO;
			switch (m_ItemDesc.etype)
			{
			case APPLE:
				CUI_Manager::Get_Instance()->AddItem(ITEMNAME_APPLE, ITEMTYPE_FRUIT,true , false);
				break;
			case LETTUCE:
				CUI_Manager::Get_Instance()->AddItem(ITEMNAME_LETTUCE, ITEMTYPE_VEGITABLE, true, false);
				
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

			case SLIMPLANT:
				CUI_Manager::Get_Instance()->AddItem(ITEMNAME_SMALLHERB, ITEMTYPE_MATERIAL, true, false);
				break;

			case MUSHROOM:
			//	_uint random = rand() % 2;
			//		if(random == 0)
						CUI_Manager::Get_Instance()->AddItem(ITEMNAME_PYOGOMUSHROOM, ITEMTYPE_MATERIAL, true, false); 
		//			else
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

				
		//		CUI_Manager::Get_Instance()->AddItem(testdesc.eName, testdesc.eType);

			

			}
			


		}

	}
	
	
	if(m_bfirst && !m_bCollision)
	{
		m_bfirst = false;
		dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();
	}
		   //COLLIDE
		
	

	if (m_bIsGain)
	{
		m_DissolveAlpha += fTimeDelta; //AFTER EAT

		if (1 < m_DissolveAlpha)
		{
			m_DissolveAlpha = 1.f;
			m_bDead = true;
		}
	}
}

HRESULT CItem::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CItem::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

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
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_SPHERE */
	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Model*/
	_tchar szModeltag[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, m_ItemDesc.ModelDesc.pModeltag, (int)strlen(m_ItemDesc.ModelDesc.pModeltag), szModeltag, MAX_PATH);
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), iLevel, szModeltag, (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

_bool CItem::Is_AnimationLoop(_uint eAnimId)
{
	return false;
}

HRESULT CItem::SetUp_ShaderID()
{
	if (false == m_bIsGain)
		m_eShaderID = SHADER_NONANIMDEFAULT;
	else
		m_eShaderID = SHADER_NONANIM_DISSOLVE;

	return S_OK;
}

CItem * CItem::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CItem* pInstance = new CItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CItem"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CItem::Clone(void * pArg)
{
	CItem* pInstance = new CItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CItem"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CItem::Free()
{
	__super::Free();
}