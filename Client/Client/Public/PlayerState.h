#pragma once
#include "Obj_UI.h"

BEGIN(Client)

class CPlayerState final : public CObj_UI
{
public:
	enum STATE_TYPE { HP, KEY, RUBY, NUMBER, TYPE_END };
	enum HP_TEXNUM { HP100, HP75, HP50, HP25, HP0 };
	enum NUM_TEXNUM { NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};

	typedef struct Statetag
	{
		STATE_TYPE m_eType = TYPE_END;
		_float2 fPosition = _float2(500, 500);
	}STATEDESC;


private:
	CPlayerState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerState(const CPlayerState& rhs);
	virtual ~CPlayerState() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_TextureNum(_uint iNum) { m_iTextureNum = iNum; }
	_uint Get_TextureNum() { return m_iTextureNum; }

private:
	_uint m_iTextureNum = 0;
	STATEDESC m_StateDesc;

private:
	virtual HRESULT Ready_Components(void * pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources()override;  /* 셰이더 전역변수에 값을 전달한다. */

public:
	static CPlayerState* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END