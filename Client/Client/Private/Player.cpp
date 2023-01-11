#include "stdafx.h"
#include "..\Public\Player.h"
#include "PlayerBullet.h"
#include "GameInstance.h"
#include "MonsterBullet.h"
#include "Weapon.h"
#include "PipeLine.h"
#include "Cell.h"
#include "CameraManager.h"
#include "Level_Room.h"
#include "PlayerEffect.h"
#include "ObjectEffect.h"
#include "FightEffect.h"
#include "UIIcon.h"
#include "MonsterEffect.h"
#include "InvenItem.h"
#include "InvenTile.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void * pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(36.3f, m_fStartHeight, 46.8f, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(10.f, 4.2f, 10.f, 1.f));
	
	Compute_CurrentIndex(LEVEL_GAMEPLAY);

	m_fWalkingHeight = m_pNavigationCom->Compute_Height(m_pTransformCom->Get_State(CTransform::STATE_POSITION), (Get_Scale().y * 0.5f));
	m_fStartHeight = m_fWalkingHeight;
	m_fEndHeight = m_fWalkingHeight;

	Set_Scale(_float3(1.2f, 1.2f, 1.2f));
	
	m_tInfo.iMaxHp = 20;
	m_tInfo.iDamage = 20;
	m_tInfo.iCurrentHp = m_tInfo.iMaxHp;

	m_Parts.resize(PARTS_END);
	if (FAILED(Ready_Parts(CPrizeItem::ITEM_END, PARTS_WEAPON)))
		return E_FAIL;

	m_pModelCom->Set_CurrentAnimIndex(m_eState);
	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_PLAYER, this);
	
	CUIIcon::ICONDESC IconDesc;
	IconDesc.iTexureNum = CUIIcon::ICON_PLAYER;
	IconDesc.pTarget = this;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UIIcon"), LEVEL_STATIC, TEXT("UI_ICON"), &IconDesc)))
		return E_FAIL;


	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);
	//	char cName[MAX_PATH];
	//	ZeroMemory(cName, sizeof(char) * MAX_PATH);
	//	pData_Manager->TCtoC(TEXT("Link"), cName);
	//	pData_Manager->Conv_Bin_Model(m_pModelCom, cName, CData_Manager::DATA_ANIM);
	//	//ERR_MSG(TEXT("Save_Bin_Model"));
	//	RELEASE_INSTANCE(CData_Manager);

	//RELEASE_INSTANCE(CGameInstance);


	m_vColor = XMVectorSet(0, 105, 255.f, 255);
	m_eState = WARP_ED;


	return S_OK;
}

int CPlayer::Tick(_float fTimeDelta)
{
	m_iCurrentLevel = (LEVEL)CLevel_Manager::Get_Instance()->Get_CurrentLevelIndex();
	if (m_iCurrentLevel == LEVEL_LOADING)
		return OBJ_NOEVENT;

	if (m_bHit)
	{

		if (!m_bMax)
		{
			m_fHitRedColor += 0.05f;
			if (m_fHitRedColor >= m_fMaxRed)
			{
				m_fHitRedColor = m_fMaxRed;
				m_bMax = true;
			}


		}
		else
		{
			m_fHitRedColor -= 0.05f;
			if (m_fHitRedColor <= m_fMinRed)
			{
				m_bMax = false;
				m_fHitRedColor = m_fMinRed;
			}
		}
	}

	if (m_dwHitTime + 1500 < GetTickCount() && !m_bMax)
	{
		m_bHit = false;
	}


	if (m_bCharge)
	{

		if (!m_bMax)
		{
			m_fColorPercent += 0.05f;
			if (m_fColorPercent >= 1.f)
			{
				m_fColorPercent = 1.f;
				m_bMax = true;
			}


		}
		else
		{
			m_fColorPercent -= 0.05f;
			if (m_fColorPercent <= 0.f)
			{
				m_fColorPercent = 0.f;
				m_bMax = false;
			}
		}
	}


	if (CUI_Manager::Get_Instance()->Get_UI_OpenType() == CUI_Manager::UI_END)
	{
		Key_Input(fTimeDelta);
		Change_Direction(fTimeDelta);
		Check_Navigation(fTimeDelta);
		Get_ProjPosition();
	}

	if (m_eState != m_ePreState)
	{
		m_pModelCom->Set_NextAnimIndex(m_eState);
		m_ePreState = m_eState;
		m_bSoundOnce = false;
	}

	if(!m_bDead)
		Change_Animation(fTimeDelta);

	for (auto& pParts : m_Parts)
	{
		if(pParts != nullptr)
			pParts->Tick(fTimeDelta);
	}

	return OBJ_NOEVENT;
}

void CPlayer::Late_Tick(_float fTimeDelta)
{
	if (m_iCurrentLevel == LEVEL_LOADING)
		return;


	Sound_PlayerVoice_by_State(fTimeDelta);
	Set_PlayerHandItem();

	for (auto& pParts : m_Parts)
	{
		if(pParts != nullptr)
			pParts->Late_Tick(fTimeDelta);
	}
		

	if (nullptr != m_pRendererCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOWDEPTH, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_Parts[PARTS_WEAPON]);

		if (m_Parts[PARTS_ITEM] != nullptr)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, m_Parts[PARTS_ITEM]);
			dynamic_cast<CWeapon*>(m_Parts[PARTS_ITEM])->Set_Scale(_float3(2.f, 2.f, 2.f));
		}

		if (m_Parts[PARTS_EFFECT] != nullptr)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, m_Parts[PARTS_EFFECT]);
			dynamic_cast<CWeapon*>(m_Parts[PARTS_EFFECT])->Set_Scale(_float3(3.f, 3.f, 3.f));
		}
			
#ifdef _DEBUG
		if (m_pAABBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pAABBCom);
		if (m_pOBBCom != nullptr)
			m_pRendererCom->Add_Debug(m_pOBBCom);
		if (m_pSPHERECom != nullptr)
			m_pRendererCom->Add_Debug(m_pSPHERECom);
		if (m_pNavigationCom != nullptr)
			m_pRendererCom->Add_Debug(m_pNavigationCom);
#endif

	}

	CBaseObj* pCollisionBlock = nullptr;
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_BLOCK, m_pOBBCom, &pCollisionBlock))
	{
		if (m_eState == PUSH_LP)
			return;

		_vector vDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pCollisionBlock->Get_TransformState(CTransform::STATE_POSITION);
		if (fabs(XMVectorGetX(vDirection)) > fabs(XMVectorGetZ(vDirection)))
			vDirection = XMVectorSet(XMVectorGetX(vDirection), 0.f, 0.f, 0.f);
		else
			vDirection = XMVectorSet(0.f, 0.f, XMVectorGetZ(vDirection), 0.f);
		m_pTransformCom->Go_PosDir(fTimeDelta, vDirection, m_pNavigationCom);
	}

	CBaseObj* pCollisionTrap = nullptr;
	if ( CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_TRAP, m_pOBBCom, &pCollisionTrap))
	{
		CMonsterBullet::BULLETDESC eBulletDesc;

		eBulletDesc.eOwner = CMonster::MONSTER_CUCCO;
		eBulletDesc.eBulletType = CMonsterBullet::DEFAULT;
		eBulletDesc.vLook = pCollisionTrap->Get_TransformState(CTransform::STATE_LOOK);

		if(m_eState != DMG_B && m_eState != DMG_F)
			Take_Damage(1.f, &eBulletDesc, pCollisionTrap);
	}
		
}

HRESULT CPlayer::Render()
{
	if (m_iCurrentLevel == LEVEL_LOADING)
		return E_FAIL;

	if (nullptr == m_pShaderCom ||
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	//for (_uint i = 0; i < iNumMeshes; ++i)
	//{
	//Render_Model((MESH_NAME)iRenderNum);
	//}

	Render_Model(MESH_HAIR);
	Render_Model(MESH_CLOTHES);
	Render_Model(MESH_EAR);
	Render_Model(MESH_HAT);
	Render_Model(MESH_FACE);
	Render_Model(MESH_SHOES);
	Render_Model(MESH_MOUSE);
	Render_Model(MESH_BELT);


	if (m_eState != EV_TELL_ST && m_eState != EV_TELL_LP && m_eState != EV_TELL_ED &&
		m_eState != IDLE_CARRY && m_eState != ITEM_CARRY && m_eState  != WALK_CARRY)
	{
		Render_Model(m_eLeftHand);
		Render_Model(m_eRightHand);
	}

	

	return S_OK;
}

HRESULT CPlayer::Render_ShadowDepth()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_ShadowLightView(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshContainers();

	if (FAILED(m_pModelCom->Render(m_pShaderCom, MESH_HAIR, SHADER_ANIMSHADOW)))
		return S_OK;
	if (FAILED(m_pModelCom->Render(m_pShaderCom, MESH_CLOTHES, SHADER_ANIMSHADOW)))
		return S_OK;
	if (FAILED(m_pModelCom->Render(m_pShaderCom, MESH_EAR, SHADER_ANIMSHADOW)))
		return S_OK;
	if (FAILED(m_pModelCom->Render(m_pShaderCom, MESH_HAT, SHADER_ANIMSHADOW)))
		return S_OK;
	if (FAILED(m_pModelCom->Render(m_pShaderCom, MESH_SHOES, SHADER_ANIMSHADOW)))
		return S_OK;
	if (FAILED(m_pModelCom->Render(m_pShaderCom, m_eLeftHand, SHADER_ANIMSHADOW)))
		return S_OK;
	if (FAILED(m_pModelCom->Render(m_pShaderCom, m_eRightHand, SHADER_ANIMSHADOW)))
		return S_OK;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_uint CPlayer::Get_PartsItemType()
{
	if (m_Parts[PARTS_ITEM] == nullptr)
		return CWeapon::NONE;

	return dynamic_cast<CWeapon*>(m_Parts[PARTS_ITEM])->Get_PartsType();
}

void CPlayer::Set_AnimState(ANIM eAnim)
{
	if (eAnim == ITEM_GET_ST && m_b2D)
	{
		m_eState = S_ITEM_GET_ST;
	}
	else
		m_eState = eAnim; 

	m_fTime = 0.f;
}

void CPlayer::Set_RecoverHp()
{
	CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_SE_S_HP_GAUGE_UP.wav"), SOUND_SYSTEM, 0.6f);


	 m_tInfo.iCurrentHp += 4; 

	if (m_tInfo.iCurrentHp > m_tInfo.iMaxHp) 
		m_tInfo.iCurrentHp = m_tInfo.iMaxHp; 
}

void CPlayer::Set_RubyAdd(_int iCoin)
{
	 m_tInfo.iCoin += iCoin;
	 CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Rupee_Count_Up.wav"), SOUND_SYSTEM, 0.6f);
}

_bool CPlayer::Set_RubyUse(_int iCoin)
{
	if (m_tInfo.iCoin < iCoin)
		return false;
		
	CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Rupee_Count_Down.wav"), SOUND_SYSTEM, 0.6f);
	m_tInfo.iCoin -= iCoin;
	return true;
}

void CPlayer::Set_2DMode(_bool type)
{
	 m_b2D = type;  
	 m_pNavigationCom->Set_2DNaviGation(type); 
	 m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}



void CPlayer::Compute_CurrentIndex(LEVEL eLevel)
{
	m_pNavigationCom->Compute_CurrentIndex_byDistance(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}

_uint CPlayer::Take_Damage(float fDamage, void * DamageType, CBaseObj * DamageCauser)
{
	if (m_eState == DMG_B || m_eState == DMG_F ||
		m_eState == DMG_PRESS || m_eState == DMG_QUAKE ||
		m_eState == DEAD  || m_bHit)
		return 0;

	if (fDamage <= 0 || m_bDead)
		return 0;

	m_tInfo.iCurrentHp -= (int)fDamage;
	m_bHit = true;
	m_dwHitTime = GetTickCount();

	if (m_tInfo.iCurrentHp <= 0)
	{
		m_tInfo.iCurrentHp = 0;
		m_eState = DEAD;
		return 0;
	}
	

	CGameInstance::Get_Instance()->PlaySounds(TEXT("3_Monster_Hit.wav"), SOUND_PEFFECT, 0.3f);
	if (DamageCauser != nullptr)
	{
		_vector vDir = DamageCauser->Get_TransformState(CTransform::STATE_POSITION) - Get_TransformState(CTransform::STATE_POSITION);;
		_vector PlayerLook = Get_TransformState(CTransform::STATE_LOOK);
		_vector fDot = XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(PlayerLook));
		_float fAngleRadian = acos(XMVectorGetX(fDot));
		_float fAngleDegree = XMConvertToDegrees(fAngleRadian);
		_vector vCross = XMVector3Cross(vDir, PlayerLook);


		if (fAngleDegree > 0.f && fAngleDegree <= 90.f)
			m_eState = CPlayer::DMG_F;
		else if (fAngleDegree > 90.f && fAngleDegree <= 180.f)
			m_eState = CPlayer::DMG_B;
	}
	else if (DamageType != nullptr)
	{
		CMonsterBullet::BULLETDESC BulletDesc;
		memcpy(&BulletDesc, DamageType, sizeof(CMonsterBullet::BULLETDESC));


		if (BulletDesc.eBulletType == CMonsterBullet::DEFAULT && BulletDesc.eOwner == CMonster::MONSTER_ROLA)
			m_eState = DMG_PRESS;
		else 
		{
			_vector BulletLook = BulletDesc.vLook;
			_vector PlayerLook = Get_TransformState(CTransform::STATE_LOOK);
			_vector fDot = XMVector3Dot( XMVector3Normalize(BulletLook), XMVector3Normalize( PlayerLook));
			_float fAngleRadian = acos(XMVectorGetX(fDot));
			_float fAngleDegree = XMConvertToDegrees(fAngleRadian);
			_vector vCross = XMVector3Cross(BulletLook, PlayerLook);

			if (fAngleDegree >= 0.f && fAngleDegree <= 90.f)
				m_eState = CPlayer::DMG_B;
			else if (fAngleDegree > 90.f && fAngleDegree <= 180.f)
				m_eState = CPlayer::DMG_F;

		}
	}	
	else
		m_eState = CPlayer::DMG_F;
			



	return 0;
}

HRESULT CPlayer::Ready_Parts(CPrizeItem::TYPE eType, PARTS PartsIndex)
{
	if (m_Parts[PartsIndex] != nullptr)
		return E_FAIL;

	/* For.Weapon */
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("itemA_L");
	if (nullptr == pSocket)
		return E_FAIL;


	m_WeaponDesc;
	m_WeaponDesc.pSocket = pSocket;
	m_WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	m_WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	switch (eType)
	{
	case Client::CPrizeItem::ARROW:
		m_WeaponDesc.eType = CWeapon::ARROW;
		break;
	case Client::CPrizeItem::DOGFOOD:
		m_WeaponDesc.eType = CWeapon::DOGFOOD;
		break;
	case Client::CPrizeItem::HEART_CONTAINER:
		m_WeaponDesc.eType = CWeapon::HEART_CONTAINER;
		break;
	case Client::CPrizeItem::MAGIC_ROD:
		m_WeaponDesc.eType = CWeapon::MAGIC_ROD;
		break;
	case Client::CPrizeItem::BOW:
		m_WeaponDesc.eType = CWeapon::BOW;
		break;
	case Client::CPrizeItem::TELEPHONE:
		m_WeaponDesc.eType = CWeapon::TELEPHONE;
		break;
	case Client::CPrizeItem::ITEM_END:
		m_WeaponDesc.eType = CWeapon::WEAPON_END;
		break;
	default:
		break;
	}

	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_Parts[PartsIndex] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Weapon"), &m_WeaponDesc);
	if (nullptr == m_Parts[PartsIndex])
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::Ready_Parts_Bullet(CWeapon::TYPE eType, PARTS PartsIndex)
{
	if (m_Parts[PartsIndex] != nullptr)
		return E_FAIL;

	/* For.Weapon */
	CHierarchyNode*		pSocket = m_pModelCom->Get_BonePtr("itemA_L_top_end");
	if (nullptr == pSocket)
		return E_FAIL;


	m_WeaponDesc;
	m_WeaponDesc.pSocket = pSocket;
	m_WeaponDesc.SocketPivotMatrix = m_pModelCom->Get_PivotFloat4x4();
	m_WeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_World4x4Ptr();
	m_WeaponDesc.eType = eType;

	Safe_AddRef(pSocket);

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_Parts[PartsIndex] = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Weapon"), &m_WeaponDesc);
	if (nullptr == m_Parts[PartsIndex])
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}
		

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CPlayer::Set_PlayerHandItem()
{
	CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

	CObj_UI* pInvenTileX = pUI_Manager->Get_EquipItem(CUI_Manager::EQUIP_X);
	CObj_UI* pInvenTileY = pUI_Manager->Get_EquipItem(CUI_Manager::EQUIP_Y);

	if (pInvenTileX == nullptr && pInvenTileY == nullptr)
	{
		m_eRightHand = MESH_SWORD;
		RELEASE_INSTANCE(CUI_Manager);
		return;
	}

	_uint ItemX = 0;
	_uint ItemY = 0;
	if (pInvenTileX != nullptr)
		ItemX = dynamic_cast<CInvenTile*>(pInvenTileX)->Get_TextureNum();
	if (pInvenTileY != nullptr)
		ItemY = dynamic_cast<CInvenTile*>(pInvenTileY)->Get_TextureNum();
	if (ItemX == CInvenItem::ITEM_WAND || ItemY == CInvenItem::ITEM_WAND)
	{
		m_eRightHand = MESH_ROD;
	}
	else
		m_eRightHand = MESH_SWORD;


	RELEASE_INSTANCE(CUI_Manager);
}

void CPlayer::Set_PlayerState_Defaut()
{

	CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
	if (m_iCurrentLevel == LEVEL_GAMEPLAY)
		dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
	if (m_iCurrentLevel == LEVEL_TAILCAVE)
		dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_TERRAIN);
	if (m_iCurrentLevel == LEVEL_ROOM)
		dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_ROOM);
	if (m_eState == ITEM_GET_LP)
		m_eState = ITEM_GET_ED;
	if (m_eState == S_ITEM_GET_LP)
	{
		m_eState = S_ITEM_GET_ED;
		if(m_iCurrentLevel == LEVEL_TOWER)
			dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
	}
		
	if (m_bStop)
		m_bStop = false;

	if (m_eState == EV_TELL_LP && m_eState != EV_TELL_ED)
		m_eState = EV_TELL_ED;
	else if (m_bCarry)
	{
		m_bCarry = false;
		Safe_Release(m_Parts[PARTS_ITEM]);
		m_eState = IDLE;
	}
	CUI_Manager::Get_Instance()->Open_Message(false);
}


void CPlayer::Key_Input(_float fTimeDelta)
{

	if (CUI_Manager::Get_Instance()->Get_Talking() == true)
		return;


	if (m_eState == DMG_B || m_eState == DMG_F || m_eState == FALL_FROMTOP ||
		m_eState == FALL_HOLE || m_eState == FALL_ANTLION || m_eState == KEY_OPEN ||
		m_eState == STAIR_DOWN || m_eState == STAIR_UP || m_bStop || m_eState == LAND 
		|| m_eState == D_LAND || m_eState == SHIELD_HIT || m_eState == WARP_ED || m_eState == WARP_ST|| m_eState == WARP_LP)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bDead)
	{
		if (pGameInstance->Key_Up(DIK_SPACE))
		{
			m_eState = IDLE;
			m_bDead = false;
			m_tInfo.iCurrentHp = m_tInfo.iMaxHp;
		}

		RELEASE_INSTANCE(CGameInstance);
		return;
	}


	if (pGameInstance->Key_Pressing(DIK_0))
		m_tInfo.iCoin++;
	if (pGameInstance->Key_Pressing(DIK_8))
	{
		m_tInfo.iCoin--;
		if(m_tInfo.iCoin < 0)
			m_tInfo.iCoin = 0;
	}


	if (m_eState == ITEM_GET_ST || m_eState == ITEM_GET_LP)
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}


	/* Move Left and Right*/
	if (pGameInstance->Key_Down(DIK_LEFT))
	{
		if (m_eState != DASH_LP)
		{
			m_iDash[DIR_X]--;
			m_dwDashTime = GetTickCount();
		}

	}
	else if (pGameInstance->Key_Down(DIK_RIGHT))
	{
		if (m_eState != DASH_LP)
		{
			m_iDash[DIR_X]++;
			m_dwDashTime = GetTickCount();
		}
	}
	else if (pGameInstance->Key_Pressing(DIK_LEFT))
	{
		m_fTexUV.x -= 0.001f;
		if (m_fTexUV.x <= 0.f)
			m_fTexUV.x = 1.f;
		

		m_eDir[DIR_X] = -1;
	}
	else if (pGameInstance->Key_Pressing(DIK_RIGHT))
	{
		m_fTexUV.x += 0.001f;
		if (m_fTexUV.x >= 1.f)
			m_fTexUV.x = 0.f;

		m_eDir[DIR_X] = 1;
	}
	else
		m_eDir[DIR_X] = 0;


	/* Move Up And Down*/
	if (pGameInstance->Key_Down(DIK_DOWN))
	{
		if (m_eState != DASH_LP)
		{
			m_iDash[DIR_Z]--;
			m_dwDashTime = GetTickCount();
		}
	}
	else if (pGameInstance->Key_Down(DIK_UP))
	{
		if (m_eState != DASH_LP)
		{
			m_iDash[DIR_Z]++;
			m_dwDashTime = GetTickCount();
		}
	}
	if (pGameInstance->Key_Pressing(DIK_DOWN))
	{
		m_fTexUV.y += 0.001f;
		if (m_fTexUV.y >= 1.f)
			m_fTexUV.y = 0.f;

		m_eDir[DIR_Z] = -1;
	}
	else if (pGameInstance->Key_Pressing(DIK_UP))
	{
		m_fTexUV.y -= 0.001f;
		if (m_fTexUV.y <= 0.f)
			m_fTexUV.y = 1.f;

		m_eDir[DIR_Z] = 1;
	}
	else
		m_eDir[DIR_Z] = 0;


	/* Use X Key & Y Key (Attack and Use Item)*/
	if (pGameInstance->Key_Up(DIK_Z))
	{
		/* Special Sword Attack */
		if (m_eState == ANIM::SLASH_HOLD_LP || m_eState == ANIM::SLASH_HOLD_F ||
			m_eState == ANIM::SLASH_HOLD_L || m_eState == ANIM::SLASH_HOLD_B || m_eState == ANIM::SLASH_HOLD_R)
			m_eState = ANIM::SLASH_HOLD_ED;
		else
		{
			if(m_b2D)
				m_eState = S_SLASH;
			else
				m_eState = SLASH;
			m_pModelCom->Set_AnimationReset();
			m_bMakeEffect = false;
			m_bSoundOnce = false;
		}

	
			
		
	}
	else if (pGameInstance->Key_Pressing(DIK_Z))
	{
		if (m_eState == IDLE)
			m_eState = ANIM::SLASH_HOLD_ST;
		else if (m_eState == SLASH_HOLD_B || m_eState == SLASH_HOLD_F || m_eState == SLASH_HOLD_R || m_eState == SLASH_HOLD_L)
			m_eState = SLASH_HOLD_LP;

	}
	else if (pGameInstance->Key_Up(DIK_Y))
	{
		if (m_eState == ANIM::SHIELD_HOLD_LP || m_eState == ANIM::SHIELD_HOLD_F ||
			m_eState == ANIM::SHIELD_HOLD_L || m_eState == ANIM::SHIELD_HOLD_B || m_eState == ANIM::SHIELD_HOLD_R)
			m_eState = ANIM::SHIELD_HOLD_ED;
		else if (m_eState != ANIM::SHIELD)
			m_eState = ANIM::SHIELD;
	}
	else if (pGameInstance->Key_Pressing(DIK_Y))
	{
		if (m_eState == IDLE)
			m_eState = ANIM::SHIELD_HOLD_LP;
		else if (m_eState == SHIELD_HOLD_B || m_eState == SHIELD_HOLD_F || m_eState == SHIELD_HOLD_R || m_eState == SHIELD_HOLD_L)
			m_eState = SHIELD_HOLD_LP;
	}


	/*Jump_Key*/
	if (pGameInstance->Key_Down(DIK_LCONTROL) && m_eState != DASH_LP)
	{
		if (m_eState == JUMP)
		{
			m_eState = ANIM::D_JUMP;
			m_fTime = 0.f;
			m_fDoubleHeight = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		}

		if (m_eState != JUMP && m_eState != D_JUMP && m_eState != D_FALL)
		{
			m_eState = ANIM::JUMP;
			m_fTime = 0.f;
			m_fStartHeight = m_fEndHeight;
		}
	}


	if (m_dwDashTime + 150 < GetTickCount())
	{
		if (m_eState != DASH_ST && m_eState != DASH_LP && m_eState != DASH_ED)
		{
			m_iDash[DIR_Z] = 0;
			m_iDash[DIR_X] = 0;
		}

	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Change_Navigation(LEVEL eLevel)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (eLevel)
	{
	case Client::LEVEL_GAMEPLAY:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Navigation_Field")));
		break;
	case Client::LEVEL_ROOM:
	{
		CUI_Manager::ROOMTYPE eRoomType = CUI_Manager::Get_Instance()->Get_RoomType();
		if (eRoomType == CUI_Manager::MARINHOUSE)
			m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Navigation_Room")));
		else if (eRoomType == CUI_Manager::SHOP)
			m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Navigation_Shop")));
		else if (eRoomType == CUI_Manager::CRANEGAME)
			m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Navigation_CraneGame")));
		else if (eRoomType == CUI_Manager::TELEPHONE)
			m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Navigation_Telephone")));
		break;
	}
	case Client::LEVEL_TAILCAVE:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Navigation_TailCave")));
		break;
	case Client::LEVEL_TOWER:
		m_pNavigationCom = dynamic_cast<CNavigation*>(pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Navigation_Tower")));
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
}


HRESULT CPlayer::Ready_Components(void* pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(1.0f);
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model*/
	if (FAILED(__super::Add_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Link"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_OBB*/
	CCollider::COLLIDERDESC		ColliderDesc;
	ColliderDesc.vScale = _float3(1.f, 1.5f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, XMConvertToRadians(0.0f), 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.7f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_OBB"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;

	/* For.Com_SPHERE */
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	//if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		//return E_FAIL;

		/* For.Com_Navigation */
	CNavigation::NAVIDESC			NaviDesc;
	ZeroMemory(&NaviDesc, sizeof NaviDesc);

	NaviDesc.iCurrentCellIndex = 0;

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_TailCave"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TailCave"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigaitions.push_back(m_pNavigationCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Room"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Room"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigaitions.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Shop"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Shop"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigaitions.push_back(m_pNavigationCom);
	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Field"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Field"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigaitions.push_back(m_pNavigationCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_CraneGame"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_CraneGame"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigaitions.push_back(m_pNavigationCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Tower"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Tower"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigaitions.push_back(m_pNavigationCom);

	if (FAILED(__super::Add_Components(TEXT("Com_Navigation_Telephone"), LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Telephone"), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;
	m_vecNavigaitions.push_back(m_pNavigationCom);

	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderResources()
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

	if (m_bCharge)
	{
	
		if (FAILED(m_pShaderCom->Set_RawValue("g_fColorPercent", &m_fColorPercent, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
			return E_FAIL;
	}
		

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayer::SetUp_ShaderID()
{
	if(m_bCharge)
		m_eShaderID = SHADER_ANIMCHARGE;
	else if (m_bHit)
		m_eShaderID = SHADER_ANIMHIT;
	else
		m_eShaderID = SHADER_ANIMDEFAULT;

	return S_OK;
}


void CPlayer::Render_Model(MESH_NAME eMeshName)
{
	if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_DiffuseTexture", eMeshName, aiTextureType_DIFFUSE)))
		return;

	if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_NormalTexture", eMeshName, aiTextureType_NORMALS)))
		return;

	if (FAILED(m_pModelCom->SetUp_Material(m_pShaderCom, "g_SpecularTexture", eMeshName, aiTextureType_SPECULAR)))
		return;

	if (FAILED(m_pModelCom->Render(m_pShaderCom, eMeshName, m_eShaderID)))
		return;
}

void CPlayer::Sound_PlayerVoice_by_State(_float fTimeDelta)
{

	if (m_bSoundOnce)
		return;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();
	_uint iNum = 0;
	_tchar	sz_SoundPlayer[MAX_PATH];
	_float fVolume = 0.5f;


	

	m_fSoundTime += fTimeDelta;

	switch (m_eState)
	{
	case Client::CPlayer::WALK_CARRY:
	case Client::CPlayer::RUN:
	case Client::CPlayer::SHIELD_HOLD_L:
	case Client::CPlayer::SHIELD_HOLD_R:
	case Client::CPlayer::SHIELD_HOLD_B:
	case Client::CPlayer::SLASH_HOLD_F:
	case Client::CPlayer::SLASH_HOLD_B:
	case Client::CPlayer::SLASH_HOLD_L:
	case Client::CPlayer::SLASH_HOLD_R:
	case Client::CPlayer::SHIELD_HOLD_F:
		m_fSoundEndTime = 0.2f;
		fVolume = 0.1f;
		iNum = rand() % 5 + 1;
		if (iLevel == LEVEL_GAMEPLAY)
		{
			wcscpy_s(sz_SoundPlayer, TEXT("1_Field_FootStep (%d).wav"));
		}
		else
		{
			wcscpy_s(sz_SoundPlayer, TEXT("2_Dgn_FootStep (%d).wav"));
		}
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::DASH_LP:
		m_fSoundEndTime = 0.1f;
		fVolume = 0.1f;
		iNum = rand() % 5 + 1;
		if (iLevel == LEVEL_GAMEPLAY)
		{
			wcscpy_s(sz_SoundPlayer, TEXT("1_Field_FootStep (%d).wav"));
		}
		else
		{
			wcscpy_s(sz_SoundPlayer, TEXT("2_Dgn_FootStep (%d).wav"));
		}
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::LADDER_UP:
		m_fSoundEndTime = 0.2f;
		fVolume = 0.1f;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Ladder.wav"));
		break;
	case Client::CPlayer::ITEM_GET_LP:
	case Client::CPlayer::ITEM_GET_ST:
	case Client::CPlayer::S_ITEM_GET_ST:
		g_fBGMVolume -= 0.01f;
		if (g_fBGMVolume <= 0.05f)
			g_fBGMVolume = 0.05f;
		break;
	case Client::CPlayer::ITEM_GET_ED:
	case Client::CPlayer::S_ITEM_GET_ED:
		g_fBGMVolume += 0.01f;
		if (g_fBGMVolume >= 0.2f)
			g_fBGMVolume = 0.2f;
		break;
	case Client::CPlayer::PULL_LP:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 4 + 1;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Pull.wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::EV_TELL_LP:
		break;
	case Client::CPlayer::SLASH_HOLD_LP:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 4 + 1;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Charge.wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::PUSH_LP:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 4 + 1;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Push.wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::PUSH_WAIT:
		break;
	case Client::CPlayer::JUMP:
	case Client::CPlayer::D_JUMP:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 5 + 1;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Jump (%d).wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::D_FALL:
		break;
	case Client::CPlayer::FALL:
	
		break;
	case Client::CPlayer::SLASH_HOLD_ST:
	case Client::CPlayer::SHIELD:
		m_bSoundOnce = true;
		fVolume = 0.8f;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Shield_On.wav"));
		break;
	case Client::CPlayer::SHIELD_HIT:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 4;
		wcscpy_s(sz_SoundPlayer, TEXT("Guard_Wood_Metal_%d.wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::LAND:
	case Client::CPlayer::D_LAND:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		if (iLevel == LEVEL_GAMEPLAY)
		{
			wcscpy_s(sz_SoundPlayer, TEXT("1_Field_FootStep (3).wav"));
		}
		else
		{
			wcscpy_s(sz_SoundPlayer, TEXT("2_Dgn_FootStep (3).wav"));
		}
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::DMG_PRESS:
	case Client::CPlayer::DMG_B:
	case Client::CPlayer::DMG_F:
	case Client::CPlayer::DMG_QUAKE:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 5 + 1;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Damage (%d).wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::SLASH:
	case Client::CPlayer::S_SLASH:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 3 + 1;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Sword_Swing (%d).wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::SLASH_HOLD_ED:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 3 + 1;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Sword_360 (%d).wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);

		break;
	case Client::CPlayer::SHIELD_HOLD_ED:
		m_bSoundOnce = true;
		fVolume = 0.8f;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Shield_Off.wav"));
		break;
		break;
	case Client::CPlayer::KEY_OPEN:
	case Client::CPlayer::STAIR_UP:
		break;
	case Client::CPlayer::FALL_FROMTOP:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_FallFromTop.wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::FALL_HOLE:
	case Client::CPlayer::FALL_ANTLION:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 4 + 1;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Fall (%d).wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::STAIR_DOWN:
	case Client::CPlayer::LADDER_UP_ED:
		break;
	case Client::CPlayer::ITEM_CARRY:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_itemSet.wav"));
		break;
	case Client::CPlayer::EV_TELL_ST:
		break;
	case Client::CPlayer::EV_TELL_ED:
		break;
	case Client::CPlayer::DEAD:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		iNum = rand() % 3 + 1;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Dead (%d).wav"));
		wsprintf(sz_SoundPlayer, sz_SoundPlayer, iNum);
		break;
	case Client::CPlayer::WARP_ST:
		m_bSoundOnce = true;
		fVolume = 0.5f;
		wcscpy_s(sz_SoundPlayer, TEXT("Link_Warp_Ready.wav"));
		break;
	case Client::CPlayer::WARP_LP:
		break;
	case Client::CPlayer::WARP_ED:
		break;
	default:
		break;
	}

	if (m_fSoundTime > m_fSoundEndTime || m_bSoundOnce)
	{
		pGameInstance->PlaySounds(sz_SoundPlayer, SOUND_PLAYER, fVolume);
		m_fSoundTime = 0.f;
	}
	
	RELEASE_INSTANCE(CGameInstance);

}

void CPlayer::Change_Direction(_float fTimeDelta)
{
	if (m_eState == DMG_B || m_eState == DMG_F || m_eState == DMG_PRESS || m_eState == DMG_QUAKE ||
		m_eState == ITEM_GET_ST || m_eState == ITEM_GET_LP || m_eState == ITEM_GET_ED || m_eState == FALL_FROMTOP || m_eState == FALL_HOLE ||
		m_eState == FALL_ANTLION || m_eState == KEY_OPEN || m_eState == STAIR_DOWN || m_eState == STAIR_UP || m_eState == LADDER_UP_ED ||
		m_eState == ITEM_CARRY || m_bDead  || m_eState == S_ITEM_GET_ST || m_eState == S_ITEM_GET_LP || m_eState == S_ITEM_GET_ED ||
		m_eState == EV_TELL_ST || m_eState == EV_TELL_LP || m_eState == EV_TELL_ED || m_eState == LAND || m_eState == D_LAND || 
		m_eState == SHIELD_HIT || m_eState == DEAD || m_eState == WARP_ED || m_eState == WARP_ST || m_eState == WARP_LP)
		return;

	if (m_eState == SLASH_HOLD_ED || m_eState == DASH_ST || m_eState == DASH_ED)
		return;

	if (m_eState == PUSH_LP || m_eState == PUSH_WAIT || m_eState == PULL_LP)
		SetDirection_Pushing(fTimeDelta);
	else if (m_eState == SLASH_HOLD_LP || m_eState == SLASH_HOLD_ST || m_eState == SLASH_HOLD_B ||
		m_eState == SLASH_HOLD_F || m_eState == SLASH_HOLD_L || m_eState == SLASH_HOLD_R)
		SetDirection_byPosition_Slash(fTimeDelta);
	else if (m_eState == SHIELD_HOLD_LP || m_eState == SHIELD_HOLD_B ||
		m_eState == SHIELD_HOLD_F || m_eState == SHIELD_HOLD_L || m_eState == SHIELD_HOLD_R)
		SetDirection_byPosition_Shield(fTimeDelta);
	else
		SetDirection_byLook(fTimeDelta);
}

void CPlayer::SetDirection_byLook(_float fTimeDelta)
{
	CTransform::TRANSFORMDESC TransformDesc = m_pTransformCom->Get_TransformDesc();
	TransformDesc.fSpeedPerSec = m_eState == DASH_LP ? 7.f : 5.f;
	m_pTransformCom->Set_TransformDesc(TransformDesc);


	__super::Change_Direction();

	if (m_eDir[DIR_X] == 0 && m_eDir[DIR_Z] == 0)
	{
		if (m_bUpDown)
		{
			m_eState = LADDER_WAIT;
			m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, 1.f, 0.f));
		}
		else if (m_bCarry)
		{
			m_eState = IDLE_CARRY;
		}
		else
		{
			if (m_eState == RUN) m_eState = IDLE;
			if (m_eState == DASH_LP) m_eState = DASH_ED;
		}
		
	}
	else
	{
		if (m_eState != JUMP && m_eState != D_JUMP && m_eState != D_FALL)
		{
			if (abs(m_iDash[DIR_X]) > 1 || abs(m_iDash[DIR_Z]) > 1)
			{
				if (m_eState != DASH_LP)
					m_eState = DASH_ST;
			}
			else if (m_bCarry)
				m_eState = WALK_CARRY;
			else
				m_eState = RUN;
		}

		if (m_b2D)
		{
			if (m_bUpDown)
			{
				if (m_eDir[DIR_X] == 0 && m_eDir[DIR_Z] != 0)
				{
					m_eState = LADDER_UP;
					m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, 1.f, 0.f));
				}

				_vector vDir = XMVectorSet(m_eDir[DIR_X], m_eDir[DIR_Z], 0.f, 0.f);
				m_pTransformCom->Go_PosDir(fTimeDelta, vDir, m_pNavigationCom);
			}
			else
				m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom, 0.f);
		}	
		else
			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom, 0.f);
	}

}

void CPlayer::SetDirection_byPosition_Slash(_float fTimeDelta)
{

	CTransform::TRANSFORMDESC TransformDesc = m_pTransformCom->Get_TransformDesc();
	TransformDesc.fSpeedPerSec = 2.5f;
	m_pTransformCom->Set_TransformDesc(TransformDesc);

	///////////////// 이 부분에서 나의 룩벡터와 Direction Vector를 통해 각도로 방향 세팅 예정
	if (m_eDir[DIR_X] > 0)
		m_eState = SLASH_HOLD_R;
	else if (m_eDir[DIR_X] < 0)
		m_eState = SLASH_HOLD_L;

	if (m_eDir[DIR_Z] > 0)
		m_eState = SLASH_HOLD_F;
	else if (m_eDir[DIR_Z] < 0)
		m_eState = SLASH_HOLD_B;

	_vector vDirection = XMVectorSet((_float)m_eDir[DIR_X], 0.f, (_float)m_eDir[DIR_Z], 0.f);
	m_pTransformCom->Go_PosDir(fTimeDelta, vDirection);


}

void CPlayer::SetDirection_byPosition_Shield(_float fTimeDelta)
{
	CTransform::TRANSFORMDESC TransformDesc = m_pTransformCom->Get_TransformDesc();
	TransformDesc.fSpeedPerSec = 2.5f;
	m_pTransformCom->Set_TransformDesc(TransformDesc);

	///////////////// 이 부분에서 나의 룩벡터와 Direction Vector를 통해 각도로 방향 세팅 예정
	if (m_eDir[DIR_X] > 0)
		m_eState = SHIELD_HOLD_R;
	else if (m_eDir[DIR_X] < 0)
		m_eState = SHIELD_HOLD_L;

	if (m_eDir[DIR_Z] > 0)
		m_eState = SHIELD_HOLD_F;
	else if (m_eDir[DIR_Z] < 0)
		m_eState = SHIELD_HOLD_B;

	_vector vDirection = XMVectorSet((_float)m_eDir[DIR_X], 0.f, (_float)m_eDir[DIR_Z], 0.f);
	m_pTransformCom->Go_PosDir(fTimeDelta, vDirection);
}

void CPlayer::SetDirection_Pushing(_float fTimeDelta)
{

	if (m_eDir[DIR_X] == 0 && m_eDir[DIR_Z] == 0)
	{
		m_eState = PUSH_WAIT;
	}
	else
	{
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		if (fabs(XMVectorGetX(vLook)) > fabs(XMVectorGetZ(vLook)))
			vLook = XMVectorGetX(vLook) > 0 ? XMVectorSet(1.f, 0.f, 0.f, 0.f) : XMVectorSet(-1.f, 0.f, 0.f, 0.f);
	
		else
			vLook = XMVectorGetZ(vLook) > 0 ? XMVectorSet(0.f, 0.f, 1.f, 0.f) : XMVectorSet(0.f, 0.f, -1.f, 0.f);

		_vector vNewLook = XMVectorSet(m_eDir[DIR_X], 0.f, m_eDir[DIR_Z], 0.f);

		if (0 > XMVectorGetX(XMVector3Dot(vLook, vNewLook)))
		{
			m_eState = PULL_LP;
			m_pTransformCom->Go_Backward(fTimeDelta*0.1f, m_pNavigationCom);
		}
		else
		{
			m_eState = PUSH_LP;
			m_pTransformCom->Go_Straight(fTimeDelta*0.2f, m_pNavigationCom);
		}
		
	}
}

void CPlayer::Make_GuardEffect(CBaseObj* pTarget)
{
	if (m_bMakeEffect)
		return;

	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CFightEffect::GUARD_FLASH;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK))*0.8f + XMVectorSet(0.f,Get_Scale().y*0.5f,0.f,0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_POSITION) - pTarget->Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.vInitScale = _float3(0.5f, 0.5f, 0.5f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	EffectDesc.eEffectID = CFightEffect::GUARD_RING;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK))*0.8f + XMVectorSet(0.f, Get_Scale().y*0.5f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = Get_TransformState(CTransform::STATE_POSITION) - pTarget->Get_TransformState(CTransform::STATE_POSITION);
	EffectDesc.vInitScale = _float3(0.0f, 0.0f, 0.0f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);


	for (int i = 0; i < 10; ++i)
	{
		EffectDesc.eEffectID = CFightEffect::GUARD_DUST;
		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, Get_Scale().y - 0.4f, 0.f, 0.f);
		_float iRandNum = (rand() % 10 + 5) * 0.1f;
		EffectDesc.vInitScale = _float3(iRandNum, iRandNum, iRandNum);
		EffectDesc.fDeadTime = 0.8f;
		CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AttackEffect"), LEVEL_STATIC, TEXT("Layer_MonsterEffect"), &EffectDesc);

	}

	m_bMakeEffect = true;
}

void CPlayer::Make_SlashEffect()
{
	if (m_bMakeEffect)
		return;

	m_bMakeEffect = true;

	_tchar	sz_SoundEffect[MAX_PATH];
	_float fEffectVolume = 0.3f;
	if (m_eState == SLASH_HOLD_ED)
	{
		fEffectVolume = 0.3f;
		_uint iNum = rand() % 4 + 1;
		wcscpy_s(sz_SoundEffect, TEXT("LSword_AttackCharging%d.wav"));
		wsprintf(sz_SoundEffect, sz_SoundEffect, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_SoundEffect, SOUND_PEFFECT, fEffectVolume);
	}
	else
	{
		fEffectVolume = 0.3f;
		_uint iNum = rand() % 5 + 1;
		wcscpy_s(sz_SoundEffect, TEXT("LSword_Swing%d.wav"));
		wsprintf(sz_SoundEffect, sz_SoundEffect, iNum);
		CGameInstance::Get_Instance()->PlaySounds(sz_SoundEffect, SOUND_PEFFECT, fEffectVolume);
	}


	if (m_eRightHand == MESH_ROD)
	{
		CPlayerBullet::BULLETDESC BulletDesc;
		BulletDesc.eBulletType = CPlayerBullet::WAND;
		BulletDesc.fDeadTime = 2.f;
		BulletDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK)) + XMVectorSet(0.f,1.f,0.f,0.f);
		BulletDesc.vLook = XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK));

		CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_PlayerBullet"), LEVEL_STATIC, TEXT("Layer_PlayerBullet"), &BulletDesc);
		return;
	}

	
	m_BulletLook = XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK));
	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.pTarget = this;

	EffectDesc.eEffectType = CEffect::MODEL;
	EffectDesc.eEffectID = CPlayerEffect::SLASH;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 0.45f, 0.f, 0.f);
	EffectDesc.vLook = m_BulletLook;
	EffectDesc.vInitScale = _float3(3.f, 3.f, 3.f);
	EffectDesc.fDeadTime = 0.5f;
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_PlayerEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

	
	EffectDesc.eEffectID = CPlayerEffect::SWISH;
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) +XMVectorSet(0.f, 0.5f, 0.f, 0.f);
	EffectDesc.fDeadTime = 0.5f;
	EffectDesc.vLook = m_BulletLook;
	EffectDesc.vInitScale = _float3(3.f, 3.f, 3.f);
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_PlayerEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

}

void CPlayer::Make_ChargeEffect()
{
	CEffect::EFFECTDESC EffectDesc;
	EffectDesc.pTarget = this;
	EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
	EffectDesc.eEffectID = CPlayerEffect::RIPPLE;
	EffectDesc.vLook = XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK));
	EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 0.2f, 0.f, 0.f);
	EffectDesc.iTextureNum = 2;
	EffectDesc.vInitScale = _float3(5.f, 5.f, 5.f);
	EffectDesc.fDeadTime = 1.5f;
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_PlayerEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
	CGameInstance::Get_Instance()->PlaySounds(TEXT("Link_ChargeEffect.wav"), SOUND_PEFFECT, 0.5f);
	
	m_bCharge = true;
}

void CPlayer::Make_DefaultEffect(_float fTimeDelta, ANIM eState)
{
	m_fEffectTime += fTimeDelta;
	CEffect::EFFECTDESC EffectDesc;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (eState)
	{
	case Client::CPlayer::WALK_CARRY:
	case Client::CPlayer::RUN:
		m_fEffectEndTime = 0.2f;
		if (m_fEffectTime > m_fEffectEndTime)
		{
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CObjectEffect::SMOKE;
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 0.6f, 0.f, 0.f);
			EffectDesc.fDeadTime = 0.2f;
			EffectDesc.vColor = XMVectorSet(255, 255, 255, 255);
			EffectDesc.vInitScale = _float3(1.f, 1.f, 1.f);
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
			m_fEffectTime = 0.f;
		}
		break;
	case Client::CPlayer::DASH_LP:
		m_fEffectEndTime = 0.1f;
		if (m_fEffectTime > m_fEffectEndTime)
		{
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CObjectEffect::SMOKE;
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.f, 0.5f, 0.f, 0.f);
			EffectDesc.fDeadTime = 0.1f;
			EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
			EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
			m_fEffectTime = 0.f;

			if (pGameInstance->Get_CurrentLevelIndex() != LEVEL_GAMEPLAY)
				break;

			for (int i = 0; i < 3; ++i)
			{
				EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
				EffectDesc.eEffectID = CObjectEffect::GRASS_TEX;
				EffectDesc.vInitScale = _float3(1.f, 1.f, 1.f);
				EffectDesc.fDeadTime = 0.5f;
				EffectDesc.iTextureNum = rand() % 3;
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);
			}
		}
		break;
	case Client::CPlayer::SLASH_HOLD_F:
	case Client::CPlayer::SLASH_HOLD_B:
	case Client::CPlayer::SLASH_HOLD_L:
	case Client::CPlayer::SLASH_HOLD_R:
	case Client::CPlayer::SLASH_HOLD_LP:
	case Client::CPlayer::SHIELD_HOLD_F:
	case Client::CPlayer::SHIELD_HOLD_LP:
	case Client::CPlayer::SLASH_HOLD_ST:
		m_fEffectEndTime = 0.5f;
		if (m_fEffectTime > m_fEffectEndTime)
		{
			EffectDesc.pTarget = this;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CPlayerEffect::CROSS;
			EffectDesc.vLook = XMVector3Normalize(Get_TransformState(CTransform::STATE_LOOK));
			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + EffectDesc.vLook*1.8f + XMVectorSet(0.f, 1.f, 0.f, 0.f);
			EffectDesc.iTextureNum = 1;
			EffectDesc.fDeadTime = 1.0f;

			if (!m_bCharge)
				EffectDesc.vInitScale = _float3(1.2f, 1.2f, 0.f);
			else
				EffectDesc.vInitScale = _float3(2.f, 2.f, 0.f);

			pGameInstance->PlaySounds(TEXT("Link_ChargeCross.wav"), SOUND_OBJECT, 0.3f);
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PlayerEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
			m_fEffectTime = 0.f;
		}
		break;
	case Client::CPlayer::LAND:
	case Client::CPlayer::D_LAND:
		EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
		EffectDesc.eEffectID = CObjectEffect::SMOKE;
		EffectDesc.fDeadTime = 0.1f;
		EffectDesc.vColor = XMVectorSet(214, 201, 187, 255);
		EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);

		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.5f, 0.5f, 0.5f, 0.f);
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(0.5f, 0.5f, -0.5f, 0.f);
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-0.5f, 0.5f, 0.5f, 0.f);
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

		EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-0.5f, 0.5f, -0.5f, 0.f);
		pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

		for (int i = 0; i < 3; ++i)
		{
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CObjectEffect::GRASS_TEX;
			EffectDesc.vInitScale = _float3(1.f, 1.f, 1.f);
			EffectDesc.fDeadTime = 0.5f;
			EffectDesc.iTextureNum = rand() % 3;
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ObjectEffect"), LEVEL_STATIC, TEXT("Layer_ObjectEffect"), &EffectDesc);

		}
		break;
	case Client::CPlayer::WARP_ST:
		m_fEffectEndTime = 0.1f;

		if (m_fEffectTime > m_fEffectEndTime)
		{
			CEffect::EFFECTDESC EffectDesc;
			EffectDesc.eEffectType = CEffect::VIBUFFER_RECT;
			EffectDesc.eEffectID = CMonsterEffect::ROLA_SMOKE;
			EffectDesc.fDeadTime = 0.5f;
			EffectDesc.vColor = XMVectorSet(255, 255, 255, 255);
			EffectDesc.vInitScale = _float3(2.f, 2.f, 0.f);

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(1.f, 0.5f, 1.f, 0.f);
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(1.f, 0.5f, -1.f, 0.f);
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-1.f, 0.5f, 1.f, 0.f);
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);

			EffectDesc.vInitPositon = Get_TransformState(CTransform::STATE_POSITION) + XMVectorSet(-1.f, 0.5f, -1.f, 0.f);
			pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MonsterEffect"), LEVEL_STATIC, TEXT("Layer_PlayerEffect"), &EffectDesc);
			m_fEffectTime = 0.f;
		}
		break;
	default:
		break;
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Change_Animation(_float fTimeDelta)
{
	switch (m_eState)
	{
	case Client::CPlayer::IDLE:
	case Client::CPlayer::IDLE_CARRY:
		m_bMakeEffect = false;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		break;
	case Client::CPlayer::WALK_CARRY:
	case Client::CPlayer::RUN:
		Make_DefaultEffect(fTimeDelta, m_eState);
		m_eAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		break;
	case Client::CPlayer::DASH_LP:
		Make_DefaultEffect(fTimeDelta, m_eState);
		m_eAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		break;
	case Client::CPlayer::LADDER_UP:
	case Client::CPlayer::LADDER_WAIT:
	case Client::CPlayer::ITEM_GET_LP:
	case Client::CPlayer::PULL_LP:
	case Client::CPlayer::EV_TELL_LP:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		break;
	case Client::CPlayer::SLASH_HOLD_F:
	case Client::CPlayer::SLASH_HOLD_B:
	case Client::CPlayer::SLASH_HOLD_L:
	case Client::CPlayer::SLASH_HOLD_R:
	case Client::CPlayer::SLASH_HOLD_LP:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		m_fChargeTime += fTimeDelta;
		Make_DefaultEffect(fTimeDelta, m_eState);
		if (m_fChargeTime > 2.f)
		{
			Make_ChargeEffect();
			m_fChargeTime = 0.f;
		}
		break;
	case Client::CPlayer::SHIELD_HOLD_L:
	case Client::CPlayer::SHIELD_HOLD_R:
	case Client::CPlayer::SHIELD_HOLD_B:
	case Client::CPlayer::SHIELD_HOLD_F:
	case Client::CPlayer::SHIELD_HOLD_LP:
		m_eAnimSpeed = 4.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		break;
	case Client::CPlayer::PUSH_LP:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		break;
	case Client::CPlayer::PUSH_WAIT:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		break;
	case Client::CPlayer::JUMP:
	{
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		m_fTime += 0.1f;
		m_pTransformCom->Jump(m_fTime, 3.f, 2.0f, m_fStartHeight, m_fEndHeight);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_eState = LAND;
			_vector vPosition = XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_fEndHeight);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		}
		break;
	}
	case Client::CPlayer::D_JUMP:
	{
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		m_fTime += 0.1f;
		m_pTransformCom->Jump(m_fTime, 3.f, 2.0f, m_fDoubleHeight, m_fEndHeight);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
			m_eState = D_FALL;
		break;
	}
	case Client::CPlayer::D_FALL:
	{
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		m_fTime += 0.1f;
		m_pTransformCom->Jump(m_fTime, 3.f, 2.0f, m_fStartHeight, m_fEndHeight);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
			m_eState = D_LAND;
		if (XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION)) <= m_fEndHeight)
			m_eState = D_LAND;
		break;
	}
	case Client::CPlayer::FALL:
	{
		m_eAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		m_pTransformCom->Go_PosDir(fTimeDelta, XMVectorSet(0.f, -1.f, 0.f, 0.f), m_pNavigationCom);
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		break;
	}
	case Client::CPlayer::SLASH_HOLD_ST:
		m_bIsLoop = false;
		m_eAnimSpeed = 2.f;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_eState = SLASH_HOLD_LP;
			m_bMakeEffect = false;
		}	
		break;
	case Client::CPlayer::SHIELD:
	case Client::CPlayer::SHIELD_HIT:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_bMakeEffect = false;
			m_eState = SHIELD_HOLD_LP;
			m_fChargeTime = 1.5f;
			m_bSoundOnce = false;
		}			
		break;
	case Client::CPlayer::LAND:
	case Client::CPlayer::D_LAND:
		if (!m_bFirst)
		{
			Make_DefaultEffect(fTimeDelta, m_eState);
			m_bFirst = true;
		}
		m_eAnimSpeed = 4.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_eState = IDLE;
			m_bFirst = false;
		}
		break;
	case Client::CPlayer::DMG_PRESS:
		m_eAnimSpeed = 2.f;
		if (m_fPressedScale > 0.1f && !m_bPressed)
		{
			m_fPressedScale -= 0.1f;
			Set_Scale(_float3(1.f, m_fPressedScale, 1.f));

			if (m_fPressedScale <= 0.1f)
			{
				m_bPressed = true;
				m_dwPressedTime = GetTickCount();
			}
		}
		if (m_bPressed && m_dwPressedTime + 1000 < GetTickCount())
		{
			if (m_fPressedScale < 1.f)
			{
				m_fPressedScale += 0.1f;
				Set_Scale(_float3(1.f, m_fPressedScale, 1.f));
			}
		}
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_fPressedScale = 1.f;
			m_bPressed = false;
			m_eState = IDLE;
			Set_Scale(_float3(1.f, m_fPressedScale, 1.f));
		}
		break;
	case Client::CPlayer::DMG_B:
		m_eAnimSpeed = 1.5f;
		m_bIsLoop = false;
		m_pTransformCom->Go_Straight(fTimeDelta*0.4f);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
			m_eState = IDLE;
		break;
	case Client::CPlayer::DMG_F:
	case Client::CPlayer::DMG_QUAKE:
		m_eAnimSpeed = 1.5f;
		m_bIsLoop = false;
		m_pTransformCom->Go_Backward(fTimeDelta*0.1f);
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
			m_eState = IDLE;
		break;
	case Client::CPlayer::SLASH:
	case Client::CPlayer::S_SLASH:
	case Client::CPlayer::SLASH_HOLD_ED:
		Make_SlashEffect();
		m_eAnimSpeed = 2.5f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			if (m_Parts[PARTS_EFFECT] != nullptr)
				Safe_Release(m_Parts[PARTS_EFFECT]);
			m_eState = IDLE;
			m_bMakeEffect = false;
			m_bCharge = false;
		}
		break;
	case Client::CPlayer::DASH_ST:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_eState = DASH_LP;
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Link_Dash_Start.wav"), SOUND_PLAYER, 0.5f);
		}
			
		break;
	case Client::CPlayer::ITEM_GET_ST:
	{
		CCamera* pCamera =  CCameraManager::Get_Instance()->Get_CurrentCamera();
		dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_ITEMGET);
		m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, -1.f ,0.f));
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
			m_eState = ITEM_GET_LP;
		break;
	}
	case Client::CPlayer::S_ITEM_GET_ST:
	{
		CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
		dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_ITEMGET);
		m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, -1.f, 0.f));
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
			m_eState = S_ITEM_GET_LP;
		break;
	}
	case Client::CPlayer::DASH_ED:
		m_eAnimSpeed = 3.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_eState = IDLE;
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Link_Dash_End.wav"), SOUND_PLAYER, 0.5f);
		}		
		break;
	case Client::CPlayer::SHIELD_HOLD_ED:
		m_eAnimSpeed = 3.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
			m_eState = IDLE;
		break;
	case Client::CPlayer::ITEM_GET_ED:
	case Client::CPlayer::S_ITEM_GET_ED:
	case Client::CPlayer::KEY_OPEN:
	case Client::CPlayer::STAIR_UP:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
			m_eState = IDLE;
		break;
	case Client::CPlayer::FALL_FROMTOP:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		
		if (m_fEndHeight < XMVectorGetY(Get_TransformState(CTransform::STATE_POSITION)))
			m_pTransformCom->Go_PosDir(fTimeDelta*3, XMVectorSet(0.f, -1.f, 0.f, 0.f));

		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
			m_eState = IDLE;
		break;
	case Client::CPlayer::FALL_HOLE:
	case Client::CPlayer::FALL_ANTLION:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_eState = FALL_FROMTOP;
			_vector vPosition = Get_TransformState(CTransform::STATE_POSITION);
			vPosition = XMVectorSetY(vPosition, 15.f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
			m_pTransformCom->Go_Backward(fTimeDelta * 25, m_pNavigationCom);
		}
		break;
	case Client::CPlayer::STAIR_DOWN:
	case Client::CPlayer::LADDER_UP_ED:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			_vector vPortalPos = XMLoadFloat3(&m_vPortalPos);
			vPortalPos = XMVectorSetW(vPortalPos, 1.f);

			if (!m_b2D)
			{
				m_eState = FALL_FROMTOP;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPortalPos);
				if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_DYNAMIC)
				{
					CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_DYNAMIC);
					CCamera* pCamera = CCameraManager::Get_Instance()->Get_CurrentCamera();
					dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_Position(vPortalPos);
				}
					
				m_pNavigationCom->Set_2DNaviGation(false);
				m_b2D = false;
			}
			else
			{
				m_eState = IDLE;
				vPortalPos = XMVectorSetY(vPortalPos, XMVectorGetY(vPortalPos) - 1.f);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPortalPos);
				CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_2D);
				m_pNavigationCom->Set_2DNaviGation(true);
				m_pNavigationCom->Compute_CurrentIndex_byDistance(vPortalPos);
			}	
		}
		break;
	case Client::CPlayer::ITEM_CARRY:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_eState = IDLE_CARRY;
			m_bCarry = true;
		}
		break;
	case Client::CPlayer::EV_TELL_ST:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_eState = EV_TELL_LP;
			m_bCarry = true;
		}
		break;
	case Client::CPlayer::EV_TELL_ED:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			Set_PlayerState_Defaut();
			CLevel* pLevel =  CGameInstance::Get_Instance()->Get_CurrentLevel();
			dynamic_cast<CLevel_Room*>(pLevel)->Ready_Layer_TelephoneObject(TEXT("Layer_Object"));
		}
		break;
	case Client::CPlayer::DEAD:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_bDead = true;
		}
		break;
	case Client::CPlayer::WARP_ST:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		Make_DefaultEffect(fTimeDelta, m_eState);
		m_fTime += fTimeDelta;
		if (m_fTime > 2.f && !m_bFirst)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Link_Warp_Start Up.wav"), SOUND_PEFFECT, 0.5f);
			m_bFirst = true;
		}
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_fTime = 0.f;
			m_fEffectTime = 0.f;
			m_eState = WARP_LP;
			m_bFirst = false;
		}
		break;
	case Client::CPlayer::WARP_LP:
		m_eAnimSpeed = 2.f;
		m_bIsLoop = true;
		m_fTime += fTimeDelta;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		m_vWarpPos = XMVectorSetY(m_vWarpPos, XMVectorGetY(Get_TransformState(CTransform::STATE_POSITION)));
		if (m_pTransformCom->Go_PosLerp(m_fTime, m_vWarpPos, 0.4f))
		{
			m_pNavigationCom->Compute_CurrentIndex_byDistance(Get_TransformState(CTransform::STATE_POSITION));
			m_eState = WARP_ED;
			m_fTime = 0.f;
		}
		break;
	case Client::CPlayer::WARP_ED:
		m_fTime += fTimeDelta;
		if (m_fTime > 0.7f && !m_bFirst)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Link_FallFromTop.wav"), SOUND_PEFFECT, 0.5f);
			m_bFirst = true;
		}

		m_eAnimSpeed = 2.f;
		m_bIsLoop = false;
		if (m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop))
		{
			m_fTime = 0.f;
			m_bFirst = false;
			m_eState = IDLE;
		}
		break;
	default:
		m_bIsLoop = true;
		m_pModelCom->Play_Animation(fTimeDelta*m_eAnimSpeed, m_bIsLoop);
		break;
	}

	m_eAnimSpeed = 1.f;
}

void CPlayer::Check_Navigation(_float fTimeDelta)
{
	if (  m_eState == DEAD || m_eState == LADDER_UP_ED
		|| m_eState == STAIR_DOWN || m_bDead)
	{
		return;
	}

	if (m_pNavigationCom->Get_CurrentCellIndex() == -1)
	{
		m_pNavigationCom->Compute_CurrentIndex_byDistance(Get_TransformState(CTransform::STATE_POSITION));
		if (m_pNavigationCom->Get_CurrentCellIndex() == -1)
			return;
	}



	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::DROP)
	{
		if (/*m_eState != FALL_ANTLION &&*/ m_eState != JUMP && m_eState != D_JUMP && m_eState != D_FALL && m_eState != LAND && m_eState != D_LAND)
		{
			if(m_eState != FALL_ANTLION)
				m_pTransformCom->Go_Straight(fTimeDelta * 10, m_pNavigationCom, 0.f);
			m_eState = FALL_ANTLION;
		}
			
	}
	else if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ACCESSIBLE)
	{
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);
		
		if(m_eState != D_FALL &&  m_eState != JUMP)
				m_fStartHeight = m_fWalkingHeight;
		m_fEndHeight = m_fWalkingHeight;

		if (m_eState != JUMP && m_eState != D_JUMP && m_eState != D_FALL && m_eState != LAND && m_eState != D_LAND && m_eState != FALL_FROMTOP)
		{
			if (m_fWalkingHeight < XMVectorGetY(vPosition) && m_b2D)
				m_eState = FALL;
			else if (m_fWalkingHeight < XMVectorGetY(vPosition))
			{
				vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
			}

		}
	
		if (m_fWalkingHeight > XMVectorGetY(vPosition))
		{
			if (m_eState == FALL)
				m_eState = LAND;
			vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		}

	}
	else if (m_pNavigationCom->Get_CurrentCelltype() == CCell::ONLYJUMP)
	{
		
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);
		
		m_fStartHeight = m_fWalkingHeight;
		m_fEndHeight = m_fWalkingHeight -2;

		
		if (m_eState == LAND)
			m_eState = FALL;

		if (m_eState != JUMP && m_eState != D_JUMP && m_eState != D_FALL && m_eState != LAND && m_eState != D_LAND)
		{
			if (m_fWalkingHeight <= XMVectorGetY(vPosition))
				m_eState = FALL;

		}
		
		if (m_fWalkingHeight > XMVectorGetY(vPosition))
		{
			m_pNavigationCom->Compute_CurrentIndex_byHeight(vPosition);
		}
	}


	if (m_pNavigationCom->Get_CurrentCelltype() == CCell::UPDOWN)
	{
		m_bUpDown = true;

		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		m_pNavigationCom->Compute_CurrentIndex_byHeight(vPosition);
		_float m_fWalkingHeight = m_pNavigationCom->Compute_Height(vPosition, 0.f);
		m_fStartHeight = m_fWalkingHeight;
		m_fEndHeight = m_fWalkingHeight;

		
		if (m_iCurrentLevel == LEVEL_TOWER)
		{
			
			if (m_fWalkingHeight > XMVectorGetY(vPosition))
			{
				if (XMVectorGetY(vPosition)< 5.6)
				{
					vPosition = XMVectorSetY(vPosition, 5.4f);
					m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
				}
				else
				{
					m_pNavigationCom->Compute_CurrentIndex_byHeight(vPosition);
				}

			}
			 if (XMVectorGetY(vPosition) > 16.1)
			{
				m_fWalkingHeight = 16.1f;
				vPosition = XMVectorSetY(vPosition, m_fWalkingHeight);
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
			}
			
		}
		else 
		{
			if (m_fWalkingHeight > XMVectorGetY(vPosition))
				m_pNavigationCom->Compute_CurrentIndex_byHeight(vPosition);
		}

	}	
	else
		m_bUpDown = false;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer*	pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();


	for (auto& iter : m_Parts)
		Safe_Release(iter);

	m_Parts.clear();

	Safe_Release(m_WeaponDesc.pSocket);

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSPHERECom);

	for (auto& iter : m_vecNavigaitions)
		Safe_Release(iter);
	m_vecNavigaitions.clear();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModelCom);


}
