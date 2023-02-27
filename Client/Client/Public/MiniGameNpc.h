#pragma once

#include "Client_Defines.h"
#include "Npc.h"


BEGIN(Client)

class CMiniGameNpc final : public CNpc
{
public:
	enum NPCTYPE { NPC_NFC_SLV_000, NPC_NFC_SLV_000_2th, NPC_NFM_SLV_000, 
		NPC_NFY_FIA_000, NPC_NMM_BEF_000
	, NPC_NMM_BLS_000, NPC_NMM_DIM_000,
		NPC_NMM_MHB_000, NPC_NMM_SLV_000, MAN_GLD, MAN_PLC};


private:
	CMiniGameNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiniGameNpc(const CMiniGameNpc& rhs);
	virtual ~CMiniGameNpc() = default;

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
	static CMiniGameNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END