#pragma once
#include "Client_Defines.h"
#include "Npc.h"
#include "GameInstance.h"
#include "Cell.h"


BEGIN(Client)
class CTarinNpc final : public CNpc
{
public:
	enum MSGTEX_TARINTALK { DEFAULT, THANK_YOU, THISISFORYOU};

	enum STATE { IDLE, WOW, TALK};

private:
	CTarinNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTarinNpc(const CTarinNpc& rhs);
	virtual ~CTarinNpc() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Check_Navigation(_float fTimeDelta);
	virtual void Send_Answer_toNPC(_uint iTextureNum) override;
	virtual void GiveItemMode() override;
private:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID();
	virtual void Change_Animation(_float fTimeDelta);
	void	Change_Message();


private:
	STATE	m_eState = IDLE;
	_float4  m_vLastPosition;
	_float3  m_vLastLook;


public:
	virtual void Free() override;
	static CTarinNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
};
END