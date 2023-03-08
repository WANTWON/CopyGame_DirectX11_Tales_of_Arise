#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Dialogue_Caption final : public CUI_Base
{
private:
	CUI_Dialogue_Caption(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Dialogue_Caption(const CUI_Dialogue_Caption& rhs);
	virtual ~CUI_Dialogue_Caption() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */


public:
	static CUI_Dialogue_Caption* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void moveleft() { m_fPosition.x -= 10.f; }

	//wchar_t* CharToWChar(const char* pstrSrc);

public:
	void Read_TextFiles_for_BossRinwell();
	void Read_TextFiles_for_Strike_RinwellLaw();


	void Read_TextFiles_for_Quest2Strat();
	void Read_TextFiles_for_Quest2Clear();
	void Read_TextFiles_for_Quest3Start();
	void Read_TextFiles_for_Quest3Clear();

	wchar_t* ConverCtoWC(char* str);

	void Render_Fonts(_uint index);

	void Open_Dialogue(_uint index);// { m_iVectorIndex = index; m_btick = true; }

private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*               m_pTextureCom2 = nullptr;
         
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
	vector<_tchar*> m_vDialogue[5];  // m_vDialogue[0] m_vDialogue[1]

	vector<_tchar*> m_vDialouge1[4]; // quest 1 clear

	vector<_tchar*> m_vDialouge2[7]; // quest 2 start

	vector<_tchar*> m_vDialouge3[6]; // quest 2 clear

	vector<_tchar*> m_vDialouge4[6]; // quest 3 start

	vector<_tchar*> m_vDialouge5[6]; // quest 3 clear


	vector<vector<vector<_tchar*>>> m_vCurrentDialogue;
	/*m_vCurrentDialogue.reszie(10);
	m_vCurrentDialogue[0] = asdf*/
	//m_vCurrentDialogue[0][0]
	_float m_fTimer = 0.f;

	_uint m_iVectorIndex = 0;

	_uint m_iDialogueindex = 0;



	_bool m_bpingdown = true;
	_float m_fbrightpos = 0.f;

	_uint vectorsize = 0;

	_bool m_btick = false;
	//vector<vector>
};

END