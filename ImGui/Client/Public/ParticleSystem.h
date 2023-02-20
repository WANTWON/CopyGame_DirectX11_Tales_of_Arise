#pragma once

#include "Effect.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)
class CParticleSystem : public CEffect
{
public: /* Getters & Setters */
	virtual _tchar* Get_PrototypeId() { return m_tParticleDesc.wcPrototypeId; }
	PARTICLEDESC Get_ParticleDesc() { return m_tParticleDesc; }
	void Set_ParticleDesc(PARTICLEDESC tParticleDesc) { m_tParticleDesc = tParticleDesc; }

public:
	CParticleSystem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticleSystem(const CParticleSystem& rhs);
	virtual ~CParticleSystem() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Glow() override;

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;

public:
	HRESULT ResetParticleSystem();

private:
	HRESULT InitializeParticleSystem();

	void EmitParticles(_float fTimeDelta);
	void UpdateParticles(_float fTimeDelta);
	void SortParticles();
	
	void ColorLerp(_uint iParticleIndex);
	void VelocityLerp(_uint iParticleIndex);
	void SizeLerp(_uint iParticleIndex);
	void AlphaLerp(_uint iParticleIndex);

	void KillParticles();

private:
	/* Particle System Properties. */
	PARTICLEDESC m_tParticleDesc; 

	_int m_iCurrentParticleCount;
	_float m_fAccumulatedTime;
	_bool m_bCanStart = false;
	_bool m_bDidBurst = false;

	/* Individual Particles Properties. */
	Particle* m_Particles = nullptr; 

	CTexture* m_pTextureCom = nullptr; 
	CVIBuffer_Point_Instance* m_pVIBufferPointInstance = nullptr;

public:
	static CParticleSystem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END