#pragma once

#include "Client_Defines.h"
#include "Npc.h"


BEGIN(Client)

class CBattleNpc final : public CNpc
{
public:
	enum NPCTYPE { NPC_LAW };


private:
	CBattleNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattleNpc(const CBattleNpc& rhs);
	virtual ~CBattleNpc() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public: /*For.State*/
	virtual _bool	Is_AnimationLoop(_uint eAnimId);
	void			Tick_State(_float fTimeDelta);
	void			LateTick_State(_float fTimeDelta);
	void			Talk_with_Npc();

private:
	virtual HRESULT Ready_Components(void* pArg);
	_uint m_eState = 0;

public:
	static CBattleNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END