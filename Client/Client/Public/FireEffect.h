#pragma once
#include "Client_Defines.h"
#include "Effect.h"
#include "GameInstance.h"



BEGIN(Client)
class CFireEffect final : public CEffect
{
public:
	enum TYPE { FIRE_SMALL, FIRE_FORME};
	enum SHADER {FIRE};


protected:
	CFireEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFireEffect(const CFireEffect& rhs);
	virtual ~CFireEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Target(CBaseObj* pObject) { m_pTarget = pObject; }
	
private:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderID();
	virtual HRESULT SetUp_ShaderResources() override;
	virtual void Change_Animation(_float fTimeDelta);
	
private:
	void Tick_Fire(_float fTimeDelta);

private:
	_float	m_fAngle = 0.f;
	_float	m_fColorTime = 0.f;
	_float	m_fSpeed = 0.f;
	_float	m_fTime = 0.f;

	vector<_vector> m_vecColor;
	_int			m_iColorIndex = 0;

	_float2 distortion1 = _float2(0.1f, 0.2f);
	_float2 distortion2 = _float2(0.f, 0.2f);
	_float2 distortion3 = _float2(0.1f, 0.1f);
	float  distortionScale = 0.0f;
	float  distortionBias = 0.5f;

private:
	CTexture* m_pNoiseTexture = nullptr;
	CTexture* m_pAlphaTexture = nullptr;


public:
	static CFireEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END