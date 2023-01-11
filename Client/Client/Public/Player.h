#pragma once

#include "BaseObj.h"
#include "PrizeItem.h"
#include "Weapon.h"

BEGIN(Engine)
class CModel;
class CNavigation;
END


BEGIN(Client)

class CPlayer final : public CBaseObj
{
public:
	//ST : Start,  LP : Loop, ED : End
	enum ANIM {
		IDLE, RUN, WALK, STAIR_DOWN, STAIR_UP,FALL, LADDER_UP, LADDER_UP_ED, LADDER_WAIT, IDLE_CARRY, ITEM_CARRY, WALK_CARRY, 
		DEAD, SHIELD_HOLD_B, SHIELD_HOLD_F, SHIELD_HOLD_L, SHIELD_HOLD_R, S_ITEM_GET_ED, S_ITEM_GET_LP, S_ITEM_GET_ST, EV_TELL_ED, EV_TELL_LP, EV_TELL_ST,
		WARP_ED, WARP_LP, WARP_ST, D_FALL, D_JUMP, D_LAND, JUMP, LAND, S_SLASH, SLASH, SLASH_HOLD_B, SLASH_HOLD_ED, SLASH_HOLD_F,
		SLASH_HOLD_L, SLASH_HOLD_LP, SLASH_HOLD_R, SLASH_HOLD_ST, SHIELD_HOLD_ED, SHIELD_HOLD_LP, SHIELD, SHIELD_HIT,
		BOW_ED, BOW_ST, DASH_ED, DASH_LP, DASH_ST, DMG_B, DMG_F, DMG_PRESS, DMG_QUAKE, ITEM_GET_ED, ITEM_GET_LP, ITEM_GET_ST,
		KEY_OPEN, FALL_ANTLION, FALL_FROMTOP, FALL_HOLE, PULL_LP, PUSH_LP, PUSH_WAIT
	};

	enum MESH_NAME {
		MESH_HAT, MESH_RING1, MESH_SHEILD2, MESH_EAR, MESH_HAIR, MESH_ROD, MESH_BELT, MESH_HOOK, MESH_SWORD, MESH_FACE, MESH_CLOTHES, MESH_RING2,
		MESH_OCARINA, MESH_SHEILD, MESH_MOUSE, MESH_SWORDHAND , MESH_SWORD2, MESH_SHEILD2_DECO, MESH_SHOES, MESH_FILPPER, MESH_NONE
	};


	enum PARTS { PARTS_WEAPON, PARTS_ITEM, PARTS_EFFECT, PARTS_END };


private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public: /* Getter & Setter */
	ANIM	Get_AnimState() { return m_eState; }
	OBJINFO Get_Info() { return m_tInfo; }
	_uint	Get_PartsItemType();
	CWeapon::WEAPONDESC Get_WeaponDesc() { return m_WeaponDesc; }
	_bool	Get_2DMode() { return m_b2D; }
	_float2 Get_PlayerTexUV() { return m_fTexUV; }
	MESH_NAME Get_RightHandItem() { return m_eRightHand; }


	void	Set_Info(OBJINFO Info) { m_tInfo = Info; }
	void	Set_AnimState(ANIM eAnim);
	void	Set_Carry(_bool type) { m_bCarry = type; }
	void	Set_RecoverHp();
	void	Set_MaxHp() { m_tInfo.iMaxHp += 4;  m_tInfo.iCurrentHp = m_tInfo.iMaxHp; }
	void	Set_RubyAdd(_int iCoin = 1);
	_bool	Set_RubyUse(_int iCoin);
	void	Set_JumpingHeight(_float fHeight) { m_fStartHeight = fHeight; m_fEndHeight = fHeight; }
	void	Set_NextPortal(_float3 vPosition, _bool is2D) { m_vPortalPos = vPosition; m_b2D = is2D;}
	void	Set_2DMode(_bool type);
	void	Set_WarpPosition(_vector vPosition) { m_vWarpPos = vPosition; }


public:
	void	Change_Navigation(LEVEL eLevel);
	void	Compute_CurrentIndex(LEVEL eLevel);
	virtual _uint Take_Damage(float fDamage, void* DamageType, CBaseObj* DamageCauser) override;
	HRESULT Ready_Parts(CPrizeItem::TYPE eType, PARTS PartsIndex);
	HRESULT Ready_Parts_Bullet(CWeapon::TYPE eType, PARTS PartsIndex);
	void	Set_PlayerState_Defaut();
	void	Set_PlayerHandItem();
	
public:
	void Make_GuardEffect(CBaseObj* pTarget = nullptr);
	void Make_SlashEffect();
	void Make_ChargeEffect();
	void Make_DefaultEffect(_float fTimeDelta, ANIM eState);

private:
	void Key_Input(_float fTimeDelta);
	void Change_Direction(_float fTimeDelta);
	void Change_Animation(_float fTimeDelta);
	void Check_Navigation(_float fTimeDelta);
	void Render_Model(MESH_NAME eMeshName);
	void Sound_PlayerVoice_by_State(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT SetUp_ShaderResources() override; /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() override;

	void SetDirection_byLook(_float fTimeDelta);
	void SetDirection_byPosition_Slash(_float fTimeDelta);
	void SetDirection_byPosition_Shield(_float fTimeDelta);
	void SetDirection_Pushing(_float fTimeDelta);



private:
	vector<class CGameObject*>			m_Parts;
	vector<CNavigation*>				m_vecNavigaitions;
	CNavigation*						m_pNavigationCom =  nullptr ;
	CModel*								m_pModelCom = nullptr;

private:
	OBJINFO					m_tInfo;
	
	/*For Anim State*/
	ANIM					m_eState = IDLE;
	ANIM					m_ePreState = IDLE;
	_float					m_eAnimSpeed = 1.f;

	_int					m_iDash[DIR_END] = { 0 };
	_bool					m_bIsLoop = true;
	_bool					m_bFirst = false;
	_bool					m_bUpDown = false;
	_bool					m_bCarry = false;

	//For Jump
	_float					m_fStartHeight = 2.2f;
	_float					m_fDoubleHeight = 2.2f;
	_float					m_fEndHeight = 2.2f;


	//For Hit State
	_bool					m_bHit = false;
	_bool					m_bPressed = false;
	_float					m_fPressedScale = 1.f;
	_float					m_fHitRedColor = 0.5f;
	_float					m_fMaxRed = 1.f;
	_float					m_fMinRed = 0.4f;
	_bool					m_bMax = false;
	_bool					m_bMakeEffect = false;

	_bool					m_bCharge = false;
	_vector					m_vColor = { 0.f,0.f,0.f,0.f };
	_float					m_fColorPercent = 0.f;


	/* For Item&Weapon */
	//LeftHand : MESH_SHEILD, MESH_OCARINA 
	//RightHand : MESH_SWORD MESH_SWORD2 MESH_HOOK, MESH_SHOVEL, MESH_WAND, MESH_FILPPER
	CWeapon::WEAPONDESC		m_WeaponDesc;
	MESH_NAME				m_eLeftHand = MESH_SHEILD;
	MESH_NAME				m_eRightHand = MESH_SWORD;
	_vector					m_BulletLook = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	
	/* For State Timer */
	_float					m_fTime = 0.f;
	_float					m_fEffectTime = 0.f;
	_float					m_fEffectEndTime = 0.f;
	_float					m_fChargeTime = 0.f;
	DWORD					m_dwDashTime = GetTickCount();
	DWORD					m_dwPressedTime = GetTickCount();
	DWORD					m_dwHitTime = GetTickCount();
	DWORD					m_dwChargeTime = GetTickCount();


	/* For Portal */
	_float3					m_vPortalPos = _float3(0.f, 0.f, 0.f);
	_vector					m_vWarpPos = { 0.f, 0.f, 0.f , 0.f};
	_bool					m_b2D = false;
	

	_float2		  m_fTexUV = _float2(0.f, 0.f);
	
public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END