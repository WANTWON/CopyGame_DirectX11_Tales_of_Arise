#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_QuestClear final : public CUI_Base
{
private:
	CUI_QuestClear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_QuestClear(const CUI_QuestClear& rhs);
	virtual ~CUI_QuestClear() = default;


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
	static CUI_QuestClear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }

	//wchar_t* CharToWChar(const char* pstrSrc);

public:


	void Open_sysmsg(_uint index);//, _uint index1);l
								  //void Close_sysmsg() { m_bInterect = false; }


private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*               m_pTextureCom2 = nullptr;
	CTexture*				m_pTextureCom3 = nullptr;
	CTexture*               m_pTextureCom4 = nullptr;
	CTexture*				m_pTextureCom5 = nullptr;
	CTexture*				m_pTextureCom6 = nullptr;
	CTexture*				m_pTextureCom7 = nullptr;
	CTexture*				m_pTextureCom8 = nullptr;
	CTexture*				m_pTextureCom9 = nullptr;
	

	_bool m_bfadein = false;
	_bool m_bfadein1 = false;
	_bool m_bfadeout = false;

	_bool m_bmoveleft = true;

	_float m_fFadeX = -50.f;
	_float m_fFadeY = -20.f;
	_float m_fFade1X = -50.f;
	_float m_fFade1Y = -20.f;



	_float m_fFontsize = 0.9f;
	_float m_fFontOffsetY = 35.f;



	_float2					m_fFontPos = { 0.f,0.f };
	_float2                 m_fFontPos1 = { 0.f , 0.f };


	_uint m_iDialogueindex = 0;

	_uint m_iVectorIndex = 0;
	_uint m_iVectorIndex1 = 1;

	_bool m_bgoup = false;
	_float m_fAlpha1 = 0.f;
	_bool m_bseconton = false;
	//vector<vector>
	_float timer = 0.f;
	_float m_fDietimer = 0.f;
	_bool m_bfirstarrived = false;
	_bool m_bDeadtimeron = false;

	_uint msgtype = 0;


	_float m_fAlpharune = 0.f;
	_bool m_bAlpharuneup = false;

	_float m_fAlphaMiddle = 0.f;

	_float m_fAlphaLight = 0.f;

	_float m_fAlphaScreen = 0.f;
	_bool m_bAlphascreenon = true;

	_float m_fAlphabottomletter = 0.f;
	_bool m_bAlphabottomletter = false;

	_float m_fAlphaGald = 0.f;
	_bool m_bAlphaGald = false;

	_float m_fLineXOffset = -1320.f;
	_bool m_bLinemove = true;
	_float m_fLightEffectXOffset = -500.f;
	_bool m_bLightEffectmove = false;

	_float m_fStartYScale = 0.f;
	_float m_fNameYScale = 0.f;
	_bool m_bNameYScalemove = false;
	_bool m_bStastYScalemove = false;
	_bool m_bLightreverse = false;

	_float m_fPlusgaldAlpha = 0.f;
	_float m_fPlusgaldOffsetY = 0.f;
	_bool m_bPlusgald = false;

	_bool m_bGaldup = false;

	_uint m_bUpgaldcount = 0;

	_float m_fTimer = 0.f;
	_uint m_igaldnum = 0;



public:
		typedef struct tagquestclear
		{
			//_float2 position = { 0.f,0.f };
			_uint iGaingald = 0;
			_uint iRewardItemcount = 0;
			ITEM_NAME eName1 = ITEMNAME_END;
			ITEM_NAME eName2 = ITEMNAME_END;
			ITEM_NAME eName3 = ITEMNAME_END;
			//	ITEM_TYPE eType = ITEMTYPE_END;

		}QUESTCLEARDESC;

		QUESTCLEARDESC m_questdesc;
};

END