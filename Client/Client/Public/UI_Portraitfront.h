
#pragma once

#include "UIBase.h"
#include "PlayerManager.h"
#include "Player.h"


BEGIN(Client)

class CUI_Portraitfront abstract : public CUI_Base
{
protected:
	CUI_Portraitfront(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Portraitfront(const CUI_Portraitfront& rhs);
	virtual ~CUI_Portraitfront() = default;

	
public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	/*static CUI_Portraitfront* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);*/
	virtual void Free() override;

protected:
	void moveright() { m_fPosition.x += 12.f; }
//
public:
	void UpdateShaderID() { if (m_fCurrentBoost >= m_fMaxBoost) m_bSmash = true; if (!m_bSmash)m_eShaderID = UI_POTRAIT_DARK; else if (m_bSmash)m_eShaderID = 0; }

	HRESULT RenderBoostGuage();

protected:
	CTexture*				m_pTextureCom1 = nullptr;
	_bool m_bfadein = false;
	_bool m_bfadeout = false;

	_bool m_bmoveright = true;
	_bool m_bArrived = false;

	_bool m_bSmash = false;
	_uint m_itexnum = 0;
	_bool m_bfirst = true;
	_bool m_bfirst1 = true;

	_bool m_bportraiton = false;
	_bool m_bbackoff = false;

	_float m_fAlpha_p = 1.f;

	_float m_fMaxBoost = 100.f;
	_float m_fCurrentBoost = 20.f;
	_float m_fGlowTimer = 0.f;

	//_bool m_bminus = 

	/*_float m_fPlayer1_Alpha = 0.f;
	_float m_fPlayer2_Alpha = 0.f;
	_float m_fPlayer3_Alpha = 0.f;
	_float m_fPlayer4_Alpha = 0.f;*/
};

END