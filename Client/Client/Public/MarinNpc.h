#pragma once
#include "Client_Defines.h"
#include "Npc.h"
#include "GameInstance.h"
#include "Cell.h"


BEGIN(Client)
class CMarinNpc final : public CNpc
{
public:
	enum MSGTEX_MARINTALK {
		MARIN_TALK, MARIN_Q1_PERFUME, MARIN_A1_COMPLETE, MARIN_A1_FAIL, MARIN_Q2_CHANGE, MARIN_A2_FAIL, MARIN_A2_COMPLETE,
		MARIN_Q3_VECTOR, MARIN_Q3_VECTOR_FAIL, MARIN_Q3_FAIL, MARIN_Q3_DOT, MARIN_Q3_DOT_FAIL, MARIN_Q3_COMPLETE, 
		MARIN_Q4_CHARM, MARIN_A4_COMPLETE, MARIN_Q5_FINAL, MARIN_A5_COMPLETE, MARIN_A5_FAIL, MARIN_LOVEUFAIL, MARIN_TALKEND_FAIL, MARIN_TALKEND_COMPLETE};

	enum TALIKING_MODE { PERFUME, CHANGE, CROSS_DOT, CHARM, FINAL};

	enum STATE {DASH_LP, JUMP, LAND, RUN, IDLE, WALK, DEPRESS, EXCITING, SCOLD_LP, SCOLD_ST, SURPRISE_LP, SURPRISE_ST,
	TALK, TALK2, TREMBLE, MARIN_GET, MARIN_GETED};

private:
	CMarinNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMarinNpc(const CMarinNpc& rhs);
	virtual ~CMarinNpc() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool	Get_IsGet() { return m_bGet; }
	void	Set_GetMode();
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
	_int	m_iCoin = 0;
	_int	m_iHeart = 0;
	_uint	m_eTalkingMode = PERFUME;
	_bool	m_bEnd = false;
	_bool	m_bGet = false;
	_float4  m_vLastPosition;
	_float3  m_vLastLook;

	DWORD	m_dwIdleTime = GetTickCount();
	DWORD	m_dwWalkTime = GetTickCount();
public:
	virtual void Free() override;
	static CMarinNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
};
END