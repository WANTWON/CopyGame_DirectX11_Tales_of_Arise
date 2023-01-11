#pragma once
#include "Client_Defines.h"
#include "Npc.h"
#include "GameInstance.h"
#include "Cell.h"


BEGIN(Client)
class CShopNpc final : public CNpc
{
public:
	enum STATE {LASER, LASER_ST, TALK, IDLE};

	enum MSGTEX_SHOPTALK { TALK_DEFAULT, BOW, MAGIC_ROD, HEART_CONTAINER, DOG_FOOD, ARROW, GIVE_MONEY, THANKYOU, SORRY };

private:
	CShopNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShopNpc(const CShopNpc& rhs);
	virtual ~CShopNpc() = default;

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
	void	ReMake_Item();

private:
	STATE m_eState = IDLE;
	_int  m_iCoin = 0;

public:
	virtual void Free() override;
	static CShopNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
};
END