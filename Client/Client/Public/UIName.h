#pragma once
#include "Obj_UI.h"

BEGIN(Client)

class CUIName final : public CObj_UI
{
public:
	enum TEX { NAME_ALBATROSS, NAME_FIELD, NAME_TAILBOSS, NAME_TAILCAVE, NAME_TOWER,
	NAME_MARINHOUSE, NAME_SHOP, NAME_TELLROOM, NAME_CRANEGAME, NAME_FOREST};

	typedef struct InvenTiletag
	{
		_uint			iTexNum = NAME_FIELD;
		_float2			vPosition = _float2(0.f, 0.f);

	}NAMEDESC;

private:
	CUIName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIName(const CUIName& rhs);
	virtual ~CUIName() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	void Set_Visible(_bool type) { m_bShow = type; }
	void Set_TexType(_uint iNum) { m_ButtonDesc.iTexNum = iNum; }
	_uint Get_TextureType() { return m_ButtonDesc.iTexNum; }
	_bool Get_Visible() { return m_bShow; }

private:
	virtual HRESULT Ready_Components(void * pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources()override;  /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() override;

private:
	NAMEDESC  m_ButtonDesc;
	_bool		m_bShow = true;
	_float		m_fAlpha = 0.0f;

public:
	static CUIName* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END