#pragma once
#include "Client_Defines.h"
#include "Npc.h"
#include "GameInstance.h"
#include "Cell.h"


BEGIN(Client)
class CFieldNpc final : public CNpc
{
public:
	enum STATE_CUCCOKEEPER { HANG_MOVE, TALK_CUCCO, IDLE_CUCCO };
	enum STATE_MADAM { KISS, LAUGH, SCREAM, TALK_MADAM, IDLE_MADAM };
	enum STATE_GRANDMA { ITEM_GET_LP, TALK_GRANDMA, TALK_BROOM, TALK_BROOM_ED, TALK_BROOM_ST, IDLE_GRANDMA, IDLE_GRANDMA_BROOM};
	enum STATE_CHILD { APPEAL, CATCH, PANIC, PANIC2, RUN, SHOW, STANDBY, TALK_CHILD, TALK_CATCHBALL, TEASE, THROW, IDLE_CHILD, IDLE_CHILD_CATCHBALL };
	enum STATE_MOTHER { TALK_MOTHER, IDLE_MOTHER };


	enum MSGTEX_SHOPTALK { TALKMSG_CUCCO, TALKMSG_MADAM, TALKMSG_GRANDMA, TALKMSG_CHILD, TALKMSG_MOM, THANKYOU_MADAM, THANKYOU_MOM, THISISFORYOU };


private:
	CFieldNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFieldNpc(const CFieldNpc& rhs);
	virtual ~CFieldNpc() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Check_Navigation(_float fTimeDelta);
	virtual void GiveItemMode() override;

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID();
	virtual void Change_Animation(_float fTimeDelta);
	void	Change_Message();

	int		CuccoKeeper_Tick(_float fTimeDelta);
	int		Madam_Tick(_float fTimeDelta);
	int		Grandma_Tick(_float fTimeDelta);
	int		Child_Tick(_float fTimeDelta);

	void	Make_ChildEffect(_float fTimeDelta);
	
private:
	_uint	m_eState = IDLE_CUCCO;
	_uint	m_eIdleState = IDLE_CUCCO;
	_uint	m_eTalkState = IDLE_CUCCO;

	_float					m_fEffectTime = 0.f;
	_float					m_fEffectEndTime = 0.f;
	

	_float4 m_vTargetPos = _float4(0.f, 0.f, 0.f, 1.f);

	DWORD	m_dwIdleTime = GetTickCount();
	DWORD	m_dwWalkTime = GetTickCount();

public:
	virtual void Free() override;
	static CFieldNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
};
END