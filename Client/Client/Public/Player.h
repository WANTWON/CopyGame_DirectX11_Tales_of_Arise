#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"


BEGIN(Engine)

class CNavigation;
class CModel;

END


BEGIN(Client)

class CPlayer final : public CBaseObj
{	
public:
	enum ANIM {
		IDLE, RUN, WALK, STAIR_DOWN, STAIR_UP, FALL, LADDER_UP, LADDER_UP_ED, LADDER_WAIT, IDLE_CARRY, ITEM_CARRY, WALK_CARRY,
		DEAD, SHIELD_HOLD_B, SHIELD_HOLD_F, SHIELD_HOLD_L, SHIELD_HOLD_R, S_ITEM_GET_ED, S_ITEM_GET_LP, S_ITEM_GET_ST, EV_TELL_ED, EV_TELL_LP, EV_TELL_ST,
		WARP_ED, WARP_LP, WARP_ST, D_FALL, D_JUMP, D_LAND, JUMP, LAND, S_SLASH, SLASH, SLASH_HOLD_B, SLASH_HOLD_ED, SLASH_HOLD_F,
		SLASH_HOLD_L, SLASH_HOLD_LP, SLASH_HOLD_R, SLASH_HOLD_ST, SHIELD_HOLD_ED, SHIELD_HOLD_LP, SHIELD, SHIELD_HIT,
		BOW_ED, BOW_ST, DASH_ED, DASH_LP, DASH_ST, DMG_B, DMG_F, DMG_PRESS, DMG_QUAKE, ITEM_GET_ED, ITEM_GET_LP, ITEM_GET_ST,
		KEY_OPEN, FALL_ANTLION, FALL_FROMTOP, FALL_HOLE, PULL_LP, PUSH_LP, PUSH_WAIT
	};

	enum PARTS { PARTS_WEAPON, PARTS_END };
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() override;

public: /*For Navigation*/
	void	Change_Navigation(LEVEL eLevel);
	void	Check_Navigation();
	void	Compute_CurrentIndex(LEVEL eLevel);

private:
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	vector<CNavigation*>	m_vecNavigaitions;
	_uint					m_eState = IDLE;
	_uint					m_ePreState = D_LAND;

private:
	vector<class CGameObject*>			m_Parts;

private:
	HRESULT Ready_Parts();
	HRESULT Ready_Components(void* pArg);
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END