#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CCamera_Dynamic;
class CCollision_Manager;
class CLevel_BattleZone final : public CLevel
{
private:
	CLevel_BattleZone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_BattleZone() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Interact_Object(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Instancing(const _tchar* pLayerTag);
	HRESULT Ready_Layer_DecoObject(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Battle_UI(const _tchar* pLayerTag);

private:
	CCollision_Manager* m_pCollision_Manager = nullptr;
	CCamera_Dynamic*	m_pCamera = nullptr;
	_float	   m_fMinLength = MAXDISTANCE;
	_bool	   m_bZumIn = false;

public:
	static CLevel_BattleZone* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

