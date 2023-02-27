#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 다음레벨을 위한 데이터를 준비한다. */

BEGIN(Client)

class CPlayerCreater final : public CBase
{

private:
	CPlayerCreater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPlayerCreater() = default;

public:
	THREAD_ID Get_CreateActorType() const {
		return m_eCloneType;
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
	HRESULT Initialize(THREAD_ID eNextLevel);

	HRESULT Cloning_ForPlayer();
	HRESULT Cloning_ForNpc();
	HRESULT Cloning_ForMonster();
	HRESULT Cloning_ForNonAnim();

	HRESULT Loading_MonsterAtFirst();
	


private: /* For NonAnim */
	HRESULT Ready_InstancingForPooling(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Npc(const _tchar* pLayerTag);
	HRESULT Ready_Layer_DecoObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SnowDecoObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BossMapObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_RestaurantObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_CityMapObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_WorkToolMapObject(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Interact_SnowField(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Interact_Restaurant(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Interact_WorkTool(const _tchar* pLayerTag);

private:
	THREAD_ID				m_eCloneType = THREAD_END;
	_bool					m_isFinished = false;
	_tchar					m_szLoadingText[MAX_PATH] = TEXT("");
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

private:
	HANDLE					m_hThread = 0;
	CRITICAL_SECTION		m_CriticalSection;


public:
	static CPlayerCreater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, THREAD_ID eNextLevel);
	virtual void Free();
};

END