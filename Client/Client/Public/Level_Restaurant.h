#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CCamera_Dynamic;
class CCollision_Manager;
class CLevel_Restaurant final : public CLevel
{
private:
	CLevel_Restaurant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Restaurant() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	_bool Get_MiniGameStart() { return m_bMinigameStart; }
	void Set_MiniGameStart(_bool tof);

	_int Get_Score() { return m_iScore; }
	_float Get_TotalTime() {return m_fTotalTime	;}
	void Increase_Score(_int iScore)
	{
		m_iScore += iScore;

		if (0 > m_iScore)
			m_iScore = 0;
	}

private:
	CCollision_Manager* m_pCollision_Manager = nullptr;
	CCamera_Dynamic*	m_pCamera = nullptr;
	_float	   m_fMinLength = MAXDISTANCE;

	_float		m_fBlurTimer = 0.f;
	_bool		m_bMinigameStart = false;

	vector<class CEffect*> m_SnowParticles1;
	vector<class CEffect*> m_SnowParticles2;

	_float m_fTotalTime = 0.f;
	_float m_fLimitTime = 30.f;
	_float m_fCreateTime[4] = { 0.f, };
	_float m_fSpwanTime = 5.f;
	_int m_iScore = 0;

	_bool m_bIsSpwan[4][3] = { { false, }, { false, }, { false, }, { false, } };

public:
	static CLevel_Restaurant* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

