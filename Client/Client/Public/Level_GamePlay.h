#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Collision_Manger.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
public:
	enum MUSIC { FIELD, MYSTERY};

private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	void Setting_Light();

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Object(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Portal(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Npc(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Warp(const _tchar* pLayerTag);

public:
	MUSIC Get_FieldType() { return m_eMusic; }
private:
	CCollision_Manager* m_pCollision_Manager = nullptr;
	MUSIC m_eMusic = FIELD;
	_bool m_bMakeLight = false;

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

