#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CCamera_Dynamic;
class CCollision_Manager;
class CLevel_LawBattle final : public CLevel
{
private:
	CLevel_LawBattle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_LawBattle() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

	void BattleTick(_float fTimeDelta);
	void NotBattleTick(_float fTimeDelta);

	void BattleLateTick(_float fTimeDelta);
	void NotBattleLateTick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Battle_UI(const _tchar* pLayerTag);

private:
	CCollision_Manager* m_pCollision_Manager = nullptr;
	CCamera_Dynamic*	m_pCamera = nullptr;
	_float	   m_fMinLength = MAXDISTANCE;
	_bool	   m_bZumIn = false;

	_float m_fBlurTimer = 0.f;

public:
	static CLevel_LawBattle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

