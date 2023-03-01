#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_InstancingForPooling(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Npc(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Deco_SnowField(const _tchar* pLayerTag);
	HRESULT Ready_Layer_SnowDecoObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BossMapObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_RestaurantObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_CityMapObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_WorkToolMapObject(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Interact_SnowField(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Interact_Restaurant(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Interact_WorkTool(const _tchar* pLayerTag);


private:
	class CPlayerCreater*	m_pLoader = nullptr;
	class CPlayerCreater*	m_pPlayerLoader = nullptr;
	class CPlayerCreater*	m_pMonsterLoader = nullptr;
	
public:
	static CLevel_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

