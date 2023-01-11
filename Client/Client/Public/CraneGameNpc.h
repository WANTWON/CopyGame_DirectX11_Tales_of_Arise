#pragma once
#include "Client_Defines.h"
#include "Npc.h"
#include "GameInstance.h"
#include "Cell.h"


BEGIN(Client)
class CCraneGameNpc final : public CNpc
{
public:
	enum STATE {FALL, FALL_ED, FALL_ED_LP, RAGE, TALK, IDLE};

	enum MSGTEX_SHOPTALK { TALK_DEFAULT, HOW_TO_PLAY, LETS_GO, ONE_MORE_TIME, FIGHTING, GOODBYE , SORRY };

private:
	CCraneGameNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCraneGameNpc(const CCraneGameNpc& rhs);
	virtual ~CCraneGameNpc() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Check_Navigation(_float fTimeDelta);
	virtual void Send_Answer_toNPC(_uint iTextureNum) override;
	_bool	Get_GameStart() { return m_bGameStart; };

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderResources();
	virtual HRESULT SetUp_ShaderID();
	virtual void Change_Animation(_float fTimeDelta);
	void	Change_Message();


private:
	STATE m_eState = IDLE;
	_int  m_iCoin = 0;
	_bool m_bGameStart = false;


public:
	virtual void Free() override;
	static CCraneGameNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
};
END