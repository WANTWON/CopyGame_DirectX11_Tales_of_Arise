#pragma once
#include "Client_Defines.h"
#include "Effect.h"
#include "GameInstance.h"



BEGIN(Client)
class CObjectEffect final : public CEffect
{
public:
	enum TYPE { 
		/*For Grass */
		GRASS, LAWN, GRASS_TEX,

		/*For Item Get */
		ITEM_GET_FLASH, ITEM_GET_GLOW , HORIZONTAL_GLOW, RAINBOW_RING ,
		GRAD_RING, RAINBOW_HALO, GLITTER , RAY,
	
		/*For Smoke*/
		SMOKE

	};



protected:
	CObjectEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObjectEffect(const CObjectEffect& rhs);
	virtual ~CObjectEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Target(CBaseObj* pObject) { m_pTarget = pObject; }
	void Set_EndMode(_bool type) { m_bEndMode = type; }

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderID();
	virtual void Change_Animation(_float fTimeDelta);
	virtual void Change_Texture(_float fTimeDelta) override;

private:
	void Tick_Grass(_float fTimeDelta);
	void Tick_Smoke(_float fTimeDelta);
	void Tick_Composs(_float fTimeDelta);

	/* For Get Item*/
	void Tick_GlowEffect(_float fTimeDelta);
	void Tick_FlashEffect(_float fTimeDelta);
	void Tick_HorizontalGlowEffect(_float fTimeDelta);
	void Tick_RingEffect(_float fTimeDelta);
	void Tick_HaloEffect(_float fTimeDelta);
	void Tick_GlitterEffect(_float fTimeDelta);
	void Tick_Ray(_float fTimeDelta);

private:
	_float	m_fAngle = 0.f;
	_float	m_fColorTime = 0.f;
	_float	m_fSpeed = 0.f;

	vector<_vector> m_vecColor;
	_int			m_iColorIndex = 0;
	_bool			m_bColorChaged = false;
	_bool			m_bEndMode = false;
	_bool			m_bMax = false;
	_float3			m_vMaxScale = _float3(2.f, 0.5f, 0.f);
	_float			m_fZoomValue = 0.f;

public:
	static CObjectEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END