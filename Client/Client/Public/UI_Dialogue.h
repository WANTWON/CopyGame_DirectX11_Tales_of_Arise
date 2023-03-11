#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Dialogue final : public CUI_Base
{
private:
	CUI_Dialogue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Dialogue(const CUI_Dialogue& rhs);
	virtual ~CUI_Dialogue() = default;


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
	static CUI_Dialogue* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }

	//wchar_t* CharToWChar(const char* pstrSrc);

public:
	void Read_TextFiles_for_dialogue();
	void Read_TextFiles_for_Quest1Clear();
	void Read_TextFiles_for_Quest2Strat();
	void Read_TextFiles_for_Quest2Clear();
	void Read_TextFiles_for_Quest3Start();
	void Read_TextFiles_for_Quest3Clear();
	void Read_TextFiles_for_Stage1Last();
	void Read_TextFiles_for_Quest4Start();
	void Read_TextFiles_for_Minigame1start();
	void Read_TextFiles_for_Minigame1end();
	void Read_TextFiles_for_Minigame2start();
	void Read_TextFiles_for_Minigame2end();
	void Read_TextFiles_for_LawBossBattleStart();
	void Read_TextFiles_for_LawBossBattleEvent();

	void Read_TextFiles_for_LawBossBattleEnd();
	void Read_TextFiles_for_LastQuestStart();

	wchar_t* ConverCtoWC(char* str);

	void Render_Fonts(_uint index);

	void Open_Dialogue(_uint index);// { m_iVectorIndex = index; m_btick = true; }

private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*               m_pTextureCom2 = nullptr;
	CTexture*               m_pTextureCom3 = nullptr;
	_bool m_bfadein = false;
	_bool m_bfadeout = false;

	_bool m_bmoveleft = true;

	_float m_fFade = -20.f;
	_float m_fFontsize = 0.9f;
	_float m_fFontOffsetY = 35.f;

	_float m_fFlowMAX = 100.f;
	_float m_fFlowCurrent = 100.f;
	_float m_fCursorpingpong = 0.f;
	
	_float2					m_fFontPos = { 0.f,0.f };
	_float m_FontR, m_FontG, m_FontB = 0.f;

	_tchar					m_szTXT[MAX_PATH] = TEXT("");
	char fuck[256];
	vector<_tchar*> m_vDialogue[8];  // m_vDialogue[0] m_vDialogue[1]

	vector<_tchar*> m_vDialouge1[7]; // quest 1 clear

	vector<_tchar*> m_vDialouge2[7]; // quest 2 start

	vector<_tchar*> m_vDialouge3[6]; // quest 2 clear

	vector<_tchar*> m_vDialouge4[6]; // quest 3 start

	vector<_tchar*> m_vDialouge5[7]; // quest 3 clear

	vector<_tchar*> m_vDialouge6[5]; // stage1 last

	vector<_tchar*> m_vDialouge7[6]; // quest 4 start;

	vector<_tchar*> m_vDialogue8[3]; // minigame 1 start;

	vector<_tchar*> m_vDialogue9[6]; // minigame 1 end;

	vector<_tchar*> m_vDialogue10[4]; // minigame 1 start;

	vector<_tchar*> m_vDialogue11[6]; // minigame 1 end;

	vector<_tchar*> m_vDialogue12[7]; // LawBossbattle start;

	vector<_tchar*> m_vDialogue13[5]; // LawBossbattle EVENT;

	vector<_tchar*> m_vDialogue14[5]; // LawBossbattleEnd

	vector<_tchar*> m_vDialogue15[17]; // LASTQUESTSTART

	vector<vector<vector<_tchar*>>> m_vCurrentDialogue;
	/*m_vCurrentDialogue.reszie(10);
	m_vCurrentDialogue[0] = asdf*/
	//m_vCurrentDialogue[0][0]
	_uint m_iVectorIndex = 0;

	_uint m_iDialogueindex = 0;

	_uint m_iPortraitnum = 0;

	_bool m_bpingdown = true;
	_float m_fbrightpos = 0.f;

	_uint vectorsize = 0;

	_bool m_btick = false;

	_bool m_bSexyEventEnd = false;
	_float m_fSexyOffTimer = 0.f;
	//vector<vector>
};

END