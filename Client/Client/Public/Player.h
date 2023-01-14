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
		ANIM_ADVENT, ANIM_ARISE_B, ANIM_ARISE_F, ANIM_ATTACK_AKIZAME, ANIM_ATTACK_BAKUSAIZAN, ANIM_ATTACK_DANKUUKEN, ANIM_ATTACK_ENGETU, ANIM_ATTACK_GOUKABAKUENNZIN_END, 
		ANIM_ATTACK_GOUKABAKUENNZIN_LOOP, ANIM_ATTACK_GOUKABAKUENNZIN_START, ANIM_ATTACK_GOUZHAN, ANIM_ATTACK_HADOUMEPPU_END, ANIM_ATTACK_HADOUMEPPU_LOOP, ANIM_ATTACK_HADOUMEPPU_START, 
		ANIM_ATTACK_HIENZIN, ANIM_ATTACK_HITENSYUOUKU_END, ANIM_ATTACK_HITENSYUOUKU_LOOP, ANIM_ATTACK_HITENSYUOUKU_START, ANIM_ATTACK_HOUOUTENKU_END, ANIM_ATTACK_HOUOUTENKU_LOOP, 
		ANIM_ATTACK_HOUOUTENKU_START, ANIM_ATTACK_HOUSYUTIGAKUZIN, ANIM_ATTACK_KAGEROU_END, ANIM_ATTACK_KAGEROU_LOOP, ANIM_ATTACK_KAGEROU_START, ANIM_ATTACK_MAKINZEN, ANIM_ATTACK_NORMAL_0,
		ANIM_ATTACK_NORMAL_1, ANIM_ATTACK_NORMAL_6, ANIM_ATTACK_NORMAL_7, ANIM_ATTACK_NORMAL_8, ANIM_ATTACK_NORMAL_AIR_0, ANIM_ATTACK_NORMAL_AIR_1, ANIM_ATTACK_NORMAL_AIR_2, ANIM_ATTACK_NORMAL_AIR_3,
		ANIM_ATTACK_NORMAL_AIR_4, ANIM_ATTACK_RAIGAISEN, ANIM_ATTACK_RESYOUSEN_END, ANIM_ATTACK_RESYOUSEN_LOOP, ANIM_ATTACK_RESYOUSEN_START, ANIM_ATTACK_RYUUSEIZIN, ANIM_ATTACK_SENKUSYOUREPA,
		ANIM_ATTACK_SINKUSOUENZIN, ANIM_ATTACK_SOURYUSATU, ANIM_ATTACK_STRIKE, ANIM_ATTACK_STRIKE_AIR, ANIM_BATTLE_IDLE_LONG, ANIM_BATTLE_JUMP, ANIM_BATTLE_LAND, ANIM_BATTLE_MOVE_BREAK, 
		ANIM_BATTLE_MOVE_IDLE, ANIM_BATTLE_MOVE_RUN, ANIM_BATTLE_MOVE_WALK_B, ANIM_BATTLE_MOVE_WALK_F, ANIM_BATTLE_MOVE_WALK_L, ANIM_BATTLE_MOVE_WALK_R, ANIM_DAMAGE_AIR_LARGE_B, ANIM_DAMAGE_AIR_LARGE_F,
		ANIM_DAMAGE_AIR_LARGE_L, ANIM_DAMAGE_AIR_LARGE_R, ANIM_DAMAGE_AIR_LOOP, ANIM_DAMAGE_AIR_SMALL_B, ANIM_DAMAGE_AIR_SMALL_F, ANIM_DAMAGE_AIR_SMALL_L, ANIM_DAMAGE_AIR_SMALL_R, 
		ANIM_DAMAGE_LARGE_B, ANIM_DAMAGE_LARGE_F, ANIM_DAMAGE_LARGE_L, ANIM_DAMAGE_LARGE_R, ANIM_DAMAGE_LOOP, ANIM_DAMAGE_SMALL_B, ANIM_DAMAGE_SMALL_F, ANIM_DAMAGE_SMALL_L, ANIM_DAMAGE_SMALL_R,
		ANIM_DASH, ANIM_DASH_BRAKE_LEFT_END, ANIM_DASH_BRAKE_LEFT_START, ANIM_DEAD, ANIM_DOWN_B, ANIM_DOWN_F, ANIM_GUARD_HIT, ANIM_GUARD_LOOP, ANIM_GUARD_START, ANIM_GUARD_WALK_B,
		ANIM_GUARD_WALK_F, ANIM_GUARD_WALK_L, ANIM_GUARD_WALK_R, ANIM_IDLE, ANIM_IDLE_SIDE_LONG, ANIM_IDLE_SIDE, ANIM_IDLE_TO_IDLE_SIDE, ANIM_IDLE_TO_IDLE_SIDE_LONG, ANIM_IDLE_TO_WALK, ANIM_JUMP_LAND,
		ANIM_JUMP_LAND_HIGH, ANIM_JUMP_LOOP, ANIM_JUMP_RUN_END, ANIM_JUMP_RUN_LAND, ANIM_JUMP_RUN_LOOP, ANIM_JUMP_RUN_START, ANIM_JUMP_START, ANIM_LADDER_DOWN_END, ANIM_LADDER_DOWN_LOOP, 
		ANIM_LADDER_DOWN_START, ANIM_LADDER_IDLE, ANIM_LADDER_TO_JUMP_RUN, ANIM_LADDER_UP_END, ANIM_LADDER_UP_LOOP, ANIM_LADDER_UP_START, ANIM_OPEN_DOOR, ANIM_OPEN_TREASURE, ANIM_REACT, ANIM_RUN,
		ANIM_RUN_BRAKE_END, ANIM_RUN_BRAKE_START, ANIM_USE_ITEM, ANIM_WALK, ANIM_WALK_TO_IDLE_SIDE_END, ANIM_WALK_TO_IDLE_SIDE_START, ANIM_END
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
	void Change_Navigation(LEVEL eLevel);
	void Check_Navigation();
	void Compute_CurrentIndex(LEVEL eLevel);

private:
	CModel* m_pModelCom = nullptr;
	CNavigation* m_pNavigationCom = nullptr;
	vector<CNavigation*> m_vecNavigations;

private:
	vector<class CGameObject*> m_Parts;

private:
	HRESULT Ready_Parts();
	HRESULT Ready_Components(void* pArg);
	HRESULT SetUp_ShaderResources();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END