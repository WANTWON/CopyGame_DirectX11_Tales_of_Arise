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


private:
	CCollision_Manager* m_pCollision_Manager = nullptr;
	CCamera_Dynamic*	m_pCamera = nullptr;
	_float	   m_fMinLength = MAXDISTANCE;

	_float		m_fBlurTimer = 0.f;
	_bool		m_bMinigameStart = false;

	_float3		m_vItemPosition[8];

	_int m_iScore = 0;


public:
	static CLevel_WorkTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

