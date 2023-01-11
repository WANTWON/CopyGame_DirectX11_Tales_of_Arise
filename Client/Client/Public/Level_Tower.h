#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Collision_Manger.h"

BEGIN(Client)

class CLevel_Tower final : public CLevel
{
private:
	CLevel_Tower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Tower() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);


private:
	CCollision_Manager* m_pCollision_Manager = nullptr;


public:
	static CLevel_Tower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

