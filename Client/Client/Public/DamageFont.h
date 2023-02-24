#pragma once

#include "UIBase.h"
#include "BaseObj.h"


BEGIN(Client)

class CDamageFont final : public CUI_Base
{
private:
	CDamageFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDamageFont(const CDamageFont& rhs);
	virtual ~CDamageFont() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void ReUse_Setting(void* pArg);

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

private:
	CTexture*				m_pTextureCom1 = nullptr;


public:
	static CDamageFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:

	_uint m_itexnum = 0;
	_uint m_iCurrentDamage = 0;
	_bool m_bfadein = true;

	_float m_fStart_timer = 0.f;


	_float m_fbrightpos_hp[4] = { 0.f , -0.5f, -1.f, -1.5f };

	_float2 m_fTargetPos = { 0.f , 0.f };

	_float m_fScaler = 2.f;

	_bool m_bfadeout = false;
	_float m_fYFadeout = 0.f;
	_float m_fNext = 25.f;

	_float2 m_fRandomOffset = { 0.f , 0.f };

	_bool m_bplusminus = false;

	_bool m_bnorend = true;

public:
	typedef struct tagdamagefont
	{
		//_float2 position = { 0.f,0.f };
		_uint iDamage = 0;
		//_float2 fposition = { 0.f,0.f };
		CBaseObj* pPointer = nullptr;

	}DMGDESC;

	DMGDESC m_damagedesc;


};

END