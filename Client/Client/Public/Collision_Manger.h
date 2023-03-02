#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "Collider.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CBaseObj;
class CCollision_Manager : public CBase
{
	DECLARE_SINGLETON(CCollision_Manager)

public:
	enum COLLSIONGROUP { COLLISION_PLAYER, COLLISION_MONSTER, COLLISION_PBULLET, COLLISION_MBULLET, COLLISION_INTERACT,COLLISION_TRIGGER, COLLISION_MINIGAME1, COLLISION_MINIGAME2, COLLISION_END };

	enum COLLIDERTYPE { COLLIDER_AABB, COLLIDER_OBB, COLLIDER_SPHERE, COLLIDER_END };

private:
	explicit CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	void	Add_CollisionGroup(COLLSIONGROUP CollisionGroup, CBaseObj* pGameObject);
	void	Add_CollisionGroupCollider(COLLSIONGROUP CollisionGroup, CCollider* pCollider, CBaseObj* pGameObject);
	void	Out_CollisionGroup(COLLSIONGROUP CollisionGroup, CBaseObj* pGameObject);
	void	Out_CollisionGroupCollider(COLLSIONGROUP CollisionGroup, CCollider* pCollider, CBaseObj* pGameObject);
	void	Clear_CollisionGroup(COLLSIONGROUP CollisionGroup);
	void	Clear_CollisionGroupExpect(COLLSIONGROUP CollisionGroup);
	void	Clear_AllCollisionGroup();
public:
	_bool	CollisionwithGroup(COLLSIONGROUP CollisionGroup, CCollider* pCollider, CBaseObj** pOut = nullptr, COLLIDERTYPE eType = COLLIDER_END);
	_bool	CollisionwithGroupCollider(COLLSIONGROUP CollisionGroup, CCollider* pCollider, CBaseObj** pOut = nullptr, COLLIDERTYPE eType = COLLIDER_END);
	_bool	CollisionwithGroup(COLLSIONGROUP SourGroup, COLLSIONGROUP DestGroup, CBaseObj** outSour, CBaseObj** outDest);
	void	CollisionwithBullet();
	void	Update_Collider();
	void	Update_ColliderGroup(COLLSIONGROUP SourGroup);

public:
	CCollider* Reuse_Collider(CCollider::TYPE eType, _uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
	void Collect_Collider(CCollider::TYPE eType, CCollider* pCollider);

private:
	_float  Calculate_DmgDirection(CBaseObj* Sour, CBaseObj* Dest, _float* vCross = nullptr);

private:
	list<CBaseObj*>		m_GameObjects[COLLISION_END];

	list<CBaseObj*>		m_ColliderGameObjects[COLLISION_END];
	list<CCollider*>	m_Colliders[COLLISION_END];

	list<CCollider*> m_ColliderPool[COLLIDER_END];

public:
	virtual void Free() override;
};
END

