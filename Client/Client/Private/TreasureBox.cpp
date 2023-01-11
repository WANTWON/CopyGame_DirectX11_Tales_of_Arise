#include "stdafx.h"
#include "..\Public\TreasureBox.h"
#include "GameInstance.h"
#include "Player.h"
#include "UIButton.h"
#include  "PrizeItem.h"
#include "Monster.h"
#include "FootSwitch.h"
#include "InvenItem.h"
#include "TreasureBoxEffect.h"
#include "ObjectEffect.h"
#include "UIIcon.h"

CTreasureBox::CTreasureBox(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

HRESULT CTreasureBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTreasureBox::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_eTreasureBoxDesc, pArg, sizeof(BOXTAG));

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_eObjectID = OBJID::OBJ_BLOCK;
	Set_Scale(_float3(1.3f, 1.3f, 1.3f));

	_vector vecPostion = XMLoadFloat3(&m_eTreasureBoxDesc.vPosition);
	vecPostion = XMVectorSetW(vecPostion, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vecPostion);


	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_BOX, this);

	CUIIcon::ICONDESC IconDesc;
	IconDesc.iTexureNum = CUIIcon::ICON_BOX;
	IconDesc.pTarget = this;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_TAILCAVE, TEXT("UI_ICON"), &IconDesc)))
		return E_FAIL;
	return S_OK;
}

int CTreasureBox::Tick(_float fTimeDelta)
{
	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
	}

	Change_Animation(fTimeDelta);

	return OBJ_NOEVENT;
}

void CTreasureBox::Late_Tick(_float fTimeDelta)
{

	CGameInstance*  pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f) == false)
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	__super::Late_Tick(fTimeDelta);

	CBaseObj*		pTarget = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	if (m_eTreasureBoxDesc.bVisible == false)
	{

		_float fDistance = XMVectorGetX(XMVector3Length(Get_TransformState(CTransform::STATE_POSITION) - pTarget->Get_TransformState(CTransform::STATE_POSITION)));
		CInvenItem* pInvenItem = dynamic_cast<CInvenItem*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Compass")));
		
		if (!m_bCheck)
		{
			if (fDistance < 13.f)
			{
				if (pInvenItem != nullptr)
					pInvenItem->Set_CompassOn(true);
			}
			else
			{
				if (pInvenItem != nullptr)
					pInvenItem->Set_CompassOn(false);
			}


			if (!m_bEntranceEffect)
				m_bEntranceEffect = Check_Visible();
			else
			{
				Make_EntranceEffect();
				dynamic_cast<CPlayer*>(pTarget)->Set_Stop(true);
				m_bCheck = true;

				if (pInvenItem != nullptr)
					pInvenItem->Set_CompassOn(false);
			}
				
		}
		

		if (m_eTreasureBoxDesc.bVisible == false)
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
			
	}
		

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
	SetUp_ShaderID();

	
	CUIButton*		pButton = dynamic_cast<CUIButton*>(CUI_Manager::Get_Instance()->Get_Button());
	LEVEL			iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();


	if (!m_bGet && m_pOBBCom->Collision(pTarget->Get_Collider()))
	{
		pButton->Set_Visible(true);
		pButton->Set_TexType(CUIButton::OPEN);
		_float2 fPosition = pTarget->Get_ProjPosition();
		fPosition.y = g_iWinSizeY - fPosition.y;
		fPosition.x += 50.f;
		fPosition.y -= 30.f;
		pButton->Set_Position(fPosition);
		if (pGameInstance->Key_Down(DIK_A))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Unlocking Treasure Box.wav"), SOUND_OBJECT, 0.4f);
			pButton->Set_Visible(false);
			if (m_eState == CLOSE_WAIT)
				m_eState = OPEN;
			else if (m_eState == OPEN_WAIT)
				m_eState = CLOSE;
		}
	}
	else
		pButton->Set_Visible(false);



	if (m_bOpen && m_dwOpenTime + 1500 < GetTickCount())
	{
		OpenBox();
		m_bOpen = false;
	}


	if (m_eState == OPEN_WAIT &&  dynamic_cast<CPlayer*>(pTarget)->Get_AnimState() == CPlayer::ITEM_GET_ST)
	{
		list<CGameObject*>* pEffectList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_TreasureBox"));
		if (pEffectList != nullptr)
		{
			for (auto& iter : *pEffectList)
			{
				if (iter != nullptr)
					dynamic_cast<CTreasureBoxEffect*>(iter)->Set_EndMode(true);
			}

		}
	}


	RELEASE_INSTANCE(CGameInstance);

}

HRESULT CTreasureBox::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
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

void CTreasureBox::Set_Visible(_bool type)
{
	m_eTreasureBoxDesc.bVisible = type;

	if (type == true)
		CGameInstance::Get_Instance()->StopSound(SOUND_MEFFECT);
}

void CTreasureBox::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CTreasureBox::APPEAR:
		break;
	case Client::CTreasureBox::CLOSE:
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
			m_eState = CLOSE_WAIT;
		break;
	case Client::CTreasureBox::CLOSE_WAIT:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta * 2, m_bIsLoop);
		break;
	case Client::CTreasureBox::NOT_OPEN:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta * 2, m_bIsLoop);
		break;
	case Client::CTreasureBox::OPEN:
		Make_OpenEffect();
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta, m_bIsLoop))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_Unlock Treasure Box Magic Sound Effect.wav"), SOUND_OBJECT, 0.4f);
			m_eState = OPEN_WAIT;
			m_bGet = true;
			m_bOpen = true;
			m_dwOpenTime = GetTickCount();
		}

		break;
	case Client::CTreasureBox::OPEN_WAIT:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta * 2, m_bIsLoop);
		break;
	default:
		break;
	}
}

HRESULT CTreasureBox::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 2.0f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_TreasureBox"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC		ColliderDesc;


	/* For.Com_OBB*/
	ColliderDesc.vScale = _float3(1.8f, 1.8f, 1.8f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.0f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreasureBox::SetUp_ShaderResources()
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

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_bool CTreasureBox::Check_Visible()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// 조건 1 스위치가 없거나 스위치가 눌려 있을 경우
	list<CGameObject*>* pSwitches = pGameInstance->Get_ObjectList(LEVEL_TAILCAVE, TEXT("Layer_FootSwitch"));
	for (auto& iter : *pSwitches)
	{
		if (dynamic_cast<CBaseObj*>(iter)->Check_IsinFrustum() == false)
			continue;

		if (dynamic_cast<CFootSwitch*>(iter)->Get_IsBoxMade() == false)
			goto RETURN_UNVISIBLE;

	}

	// 조건 2 .화면 내에 있는 몬스터들이 없거나 모두 죽었다면 
	list<CGameObject*>* pMonsters = pGameInstance->Get_ObjectList(LEVEL_TAILCAVE, TEXT("Layer_Monster"));

	for (auto& iter : *pMonsters)
	{
		if (iter == nullptr)
			continue;

		if (dynamic_cast<CBaseObj*>(iter)->Check_IsinFrustum() == false)
			continue;

		if (iter->Get_Dead() == false)
			goto RETURN_UNVISIBLE;
	}

	CInvenItem* pInvenItem = dynamic_cast<CInvenItem*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Compass")));
	if (pInvenItem != nullptr)
		pInvenItem->Set_CompassOn(false);
	
		pGameInstance->PlaySounds(TEXT("5_Obj_TreasureBox_AppearGlow.wav"), SOUND_ACTOR, 0.4f);

	RELEASE_INSTANCE(CGameInstance);
	return true;



RETURN_UNVISIBLE:
	RELEASE_INSTANCE(CGameInstance);
	return false;

}

void CTreasureBox::OpenBox()
{


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	pPlayer->Set_AnimState(CPlayer::ITEM_GET_ST);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	CPrizeItem::ITEMDESC ItemDesc;
	XMStoreFloat3(&ItemDesc.vPosition, pPlayer->Get_TransformState(CTransform::STATE_POSITION));

	ItemDesc.eInteractType = CPrizeItem::PRIZE;

	switch (m_eTreasureBoxDesc.eItemType)
	{
	case COMPASS:
		ItemDesc.eType = CPrizeItem::COMPASS;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), iLevel, TEXT("PrizeItem"), &ItemDesc);
		break;
	case MAP:
		ItemDesc.eType = CPrizeItem::MAP;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), iLevel, TEXT("PrizeItem"), &ItemDesc);
		break;
	case BOSS_KEY:
		ItemDesc.eType = CPrizeItem::BOSS_KEY;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), iLevel, TEXT("PrizeItem"), &ItemDesc);
		break;
	case SMALL_KEY:
		ItemDesc.eType = CPrizeItem::SMALL_KEY;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), iLevel, TEXT("PrizeItem"), &ItemDesc);
		break;
	case HEART:
		ItemDesc.eType = CPrizeItem::HEART_RECOVERY;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), iLevel, TEXT("PrizeItem"), &ItemDesc);
		break;
	case RUBY:
		ItemDesc.eType = CPrizeItem::RUBY_GREEN;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), iLevel, TEXT("PrizeItem"), &ItemDesc);
		break;
	case FEATHER:
		ItemDesc.eType = CPrizeItem::FEATHER;
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), iLevel, TEXT("PrizeItem"), &ItemDesc);
		break;
	default:
		break;
	}



	RELEASE_INSTANCE(CGameInstance);

}

void CTreasureBox::Make_OpenEffect()
{
	if (m_bMakeEffect)
		return;

	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect::EFFECTDESC EffectDesc;

	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.pTarget = this;

	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CTreasureBoxEffect::TREASURE_BEAM;
	EffectDesc.fDeadTime = 2.f;
	EffectDesc.vInitScale = _float3(3.f, 3.f, 3.0f);
	EffectDesc.vDistance = XMVectorSet(0.0f, 0.f, -1.f, 0.f);
	EffectDesc.vColor = XMVectorSet(247, 255, 139, 255);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TreasureBoxEffect"), LEVEL_STATIC, TEXT("Layer_TreasureBox"), &EffectDesc);

	EffectDesc.eEffectID = CTreasureBoxEffect::TREASURE_HALO;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.fStartTime = 0.f;
	EffectDesc.fDeadTime = 1.f;
	EffectDesc.vInitScale = _float3(5.f, 5.f, 5.f);
	EffectDesc.vColor = XMVectorSet(255, 255, 255, 255);
	EffectDesc.vDistance = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TreasureBoxEffect"), LEVEL_STATIC, TEXT("Layer_TreasureBox"), &EffectDesc);


	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.eEffectID = CTreasureBoxEffect::TREASURE_GLOW;
	EffectDesc.iTextureNum = 0;
	EffectDesc.fStartTime = 0.3f;
	EffectDesc.vColor = XMVectorSet(247, 255, 139, 255);;
	EffectDesc.fDeadTime = EffectDesc.fStartTime + 2.5f;
	EffectDesc.vDistance = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(0.5f, 0.5f, 0.5f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TreasureBoxEffect"), LEVEL_STATIC, TEXT("Layer_TreasureBox"), &EffectDesc);

	EffectDesc.eEffectID = CTreasureBoxEffect::TREASURE_GLOW;
	EffectDesc.iTextureNum = 0;
	EffectDesc.fStartTime = 0.2f;
	EffectDesc.vColor = XMVectorSet(255, 137, 105, 255);;
	EffectDesc.fDeadTime = EffectDesc.fStartTime + 2.5f;
	EffectDesc.vDistance = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(1.f, 1.f, 1.0f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TreasureBoxEffect"), LEVEL_STATIC, TEXT("Layer_TreasureBox"), &EffectDesc);


	EffectDesc.eEffectID = CObjectEffect::ITEM_GET_FLASH;
	EffectDesc.iTextureNum = 6;
	EffectDesc.fStartTime = 0.3f;
	EffectDesc.fDeadTime = 0.6f;
	EffectDesc.vInitScale = _float3(0.f, 0.f, 0.0f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_TreasureBox"), &EffectDesc);


	LIGHTDESC			LightDesc;

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPosition, EffectDesc.vInitPositon + EffectDesc.vDistance);
	LightDesc.fRange = 2.f;	
	LightDesc.vDiffuse = _float4(1.f, 1.f, 0.5f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 0.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return;


	RELEASE_INSTANCE(CGameInstance);

	m_bMakeEffect = true; 
}

void CTreasureBox::Make_EntranceEffect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("5_Obj_TreasureBox_AppearGlow3.wav"), SOUND_MEFFECT, 0.4f);
	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 5.f, 0.f, 0.f);
	EffectDesc.pTarget = this;

	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.eEffectID = CTreasureBoxEffect::TREASURE_CROSS;
	EffectDesc.iTextureNum = 6;
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = XMVectorSet(0.f, -1.f, 0.f, 0.f);
	EffectDesc.vInitScale = _float3(0.5f, 0.5f, 0.5f);
	pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TreasureBoxEffect"), LEVEL_STATIC, TEXT("Layer_TreasureBox"), &EffectDesc);
	RELEASE_INSTANCE(CGameInstance);
}

CTreasureBox * CTreasureBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTreasureBox*	pInstance = new CTreasureBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTreasureBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTreasureBox::Clone(void * pArg)
{
	CTreasureBox*	pInstance = new CTreasureBox(*this);

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

	Safe_Release(m_pModelCom);
}
