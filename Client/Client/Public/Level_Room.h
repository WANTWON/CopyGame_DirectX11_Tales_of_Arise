#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Collision_Manger.h"

BEGIN(Client)

class CLevel_Room final : public CLevel
{
private:
	CLevel_Room(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Room() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Portal(const _tchar* pLayerTag);

	HRESULT Ready_Layer_ShopObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MarinObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_CraneGameObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_TelephoneObject(const _tchar* pLayerTag);

private:
	CCollision_Manager* m_pCollision_Manager = nullptr;


public:
	static CLevel_Room* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

