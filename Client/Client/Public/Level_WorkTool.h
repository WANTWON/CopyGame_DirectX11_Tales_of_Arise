#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CCamera_Dynamic;
class CCollision_Manager;
class CLevel_WorkTool final : public CLevel
{
private:
	CLevel_WorkTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_WorkTool() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	void Set_MiniGameStart(_bool type);
	_bool Get_MiniGameStart() { return m_bMinigameStart; }


	void Increase_Score(_int iScore) { m_iScore += iScore; }
	_int Get_Score() { return m_iScore; }

	_float Get_LimitTime() { return m_fLimitTime; }
	void   Set_LimitTime(_float time) { m_fLimitTime = time; }

	void Set_MiniGameUI(_bool tof) { m_bMinigamseStartUI = tof; }

	_int Get_Phase() { return m_iPhase; }
	void Plus_Phase(_int phase) { m_iPhase += phase; }

	_bool Get_Finished() { return m_bGameFinish; }

private:
	CCollision_Manager* m_pCollision_Manager = nullptr;
	CCamera_Dynamic*	m_pCamera = nullptr;
	_float	   m_fMinLength = MAXDISTANCE;

	_float		m_fBlurTimer = 0.f;
	_bool		m_bMinigameStart = false;
	_bool       m_bMinigamseStartUI = true;
	_float3		m_vItemPosition[8];


	DWORD m_dwTime = GetTickCount();

	_int m_iScore = 0;

	_int m_iPhase = 1;

	
	_float m_fLimitTime = 20.f;

	_float m_fSpawnTimer = 0.f;

	_bool m_bGameFinish = false;

	_float m_fSpawnTime = 0.f;

public:
	static CLevel_WorkTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

