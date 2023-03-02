#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CCollision_Manager;
class CLevel_SnowField final : public CLevel
{
private:
	CLevel_SnowField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_SnowField() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Deco_SnowField(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Trigger(const _tchar* pLayerTag);


private:
	CCollision_Manager* m_pCollision_Manager = nullptr;

private:
	_float m_fBlurTimer = 0.f;
	 
	vector<class CEffect*> m_SnowParticles1;
	vector<class CEffect*> m_SnowParticles2;
	vector<class CEffect*> m_WindParticles;

public:
	static CLevel_SnowField* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

