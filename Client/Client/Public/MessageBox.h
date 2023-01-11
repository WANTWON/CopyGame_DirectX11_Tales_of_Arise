#pragma once
#include "Obj_UI.h"

BEGIN(Client)

class CMessageBox final : public CObj_UI
{
public:
	enum MSG_TYPE {GET_ITEM, SHOP_TALK, MARIN_TALK , CRANEGAME_TALK, FIELDNPC_TALK, 
		ALBATOSS_TALK, TARIN_TALK, TELEPHONE_TALK };

	typedef struct MessageBoxtag
	{
		_float2		fPosition = _float2(g_iWinSizeX * 0.5f, 600);
		MSG_TYPE	m_eMsgType = GET_ITEM;

	}MSGDESC;

private:
	CMessageBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMessageBox(const CMessageBox& rhs);
	virtual ~CMessageBox() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources()override;  /* 셰이더 전역변수에 값을 전달한다. */

private:
	vector<_int> m_vecTex;

	_uint		m_iTextureNum = 0;
	_bool		m_bRender = false;
	_float		m_fAlpha = 0.0f;
	_bool		m_bClosing = false;
	MSGDESC		m_MsgDesc;



public:
	static CMessageBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END