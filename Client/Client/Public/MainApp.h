#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "UI_Manager.h"
#include "Object_Pool_Manager.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*			m_pGameInstance = nullptr;
	CUI_Manager*			m_pUI_Manager = nullptr;
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	CRenderer*				m_pRenderer = nullptr;

private:
	_uint					m_iNumRender = 0;
	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	_float					m_fTimeAcc = 0.f;

private:
	HRESULT Open_Level(LEVEL eLevel);
	HRESULT Ready_Prototype_Component();	
	_bool		m_bImgui = false;


public:
	static CMainApp* Create();
	virtual void Free();
};

END

