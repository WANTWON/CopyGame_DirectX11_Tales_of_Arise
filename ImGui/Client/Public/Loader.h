#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* ���������� ���� �����͸� �غ��Ѵ�. */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	LEVEL Get_NextLevelID() const {
		return m_eNextLevel;
	}

	CRITICAL_SECTION Get_CriticalSection() {
		return m_CriticalSection;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

	const _tchar* Get_LoadingText() const {
		return m_szLoadingText;
	}

public:
	HRESULT Initialize(LEVEL eNextLevel);

	HRESULT Loading_ForLogoLevel();
	HRESULT Loading_ForClient();

	HRESULT Loading_ForMaptoolSnowFieldModel();
	HRESULT Loading_ForMaptoolBossRoomModel();
	HRESULT Loading_ForMaptoolVillageModel();
	HRESULT Loading_ForMaptoolCityModel();
	HRESULT Loading_ForMaptoolInteriorModel();
	HRESULT Loading_ForMaptoolKitchenModel();
	
	HRESULT Loading_ForActor();
	HRESULT Loading_ForEffect();
	HRESULT Loading_ForEffectTexture();

private:
	LEVEL					m_eNextLevel = LEVEL_END;
	_bool					m_isFinished = false;
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

private:
	HANDLE					m_hThread = 0;
	CRITICAL_SECTION		m_CriticalSection;			


public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel);
	virtual void Free();
};

END