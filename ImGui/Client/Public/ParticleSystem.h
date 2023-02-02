#pragma once

#include "Effect.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)
class CParticleSystem : public CEffect
{
public:
	typedef struct tagParticleDesc
	{
		_tchar wcPrototypeId[MAX_PATH] = TEXT("");	/* "Spark.dds > Spark" */
		_uint m_eSpawnType = 0;
		_bool m_bBillboard = false;
		_float3 m_vColor = _float3(0.f, 0.f, 0.f);
		_float m_fAlpha = 1.f;
		_float m_fAlphaDiscard = 0.f;
		_int m_iMaxParticles = 1000.f;
		_float m_fParticlesLifetime = 1.f;
		_float m_fParticlesPerSecond = 1.f;
		_float m_fParticleDeviationX = 0.f;
		_float m_fParticleDeviationY = 0.f;
		_float m_fParticleDeviationZ = 0.f;
		_float3 m_vParticleDirection = _float3(0.f, 0.f, 0.f);
		_bool m_bRandomDirectionX = false; 
		_bool m_bRandomDirectionY = false;
		_bool m_bRandomDirectionZ = false;
		_float m_fParticleVelocity = 1.f;
		_float m_fParticleVelocityVariation = 0.f;
		_float m_fParticleSize = 1.f;
		_float m_fParticleSizeVariation = 0.f;
	} PARTICLEDESC;

private:
	struct ParticleType 
	{
		_float fPositionX = 0.f, fPositionY = 0.f, fPositionZ = 0.f;
		_float3 vDirection = _float3(0.f, 0.f, 0.f);
		_float fRed = 0.f, fGreen = 0.f, fBlue = 0.f;
		_float fInitialAlpha = 0.f, fAlpha = 0.f;
		_float fInitialVelocity = 0.f, fVelocity = 0.f;
		_float fInitialSize = 0.f, fSize = 0.f;
		_float fLife = 0.f;
		_bool bActive = false;
	};

	struct VertexType
	{
		_float3 vPosition = _float3(0.f, 0.f, 0.f);
		_float2 vTexture = _float2(0.f, 0.f);
		_float fAlpha = 1.f;
		_float4 vColor = _float4(0.f, 0.f, 0.f, 1.f);
	};

public: /* Getters & Setters */
	virtual _tchar* Get_PrototypeId() { return m_tParticleDesc.wcPrototypeId; }
	PARTICLEDESC Get_ParticleDesc() { return m_tParticleDesc; }
	void Set_ParticleDesc(PARTICLEDESC tParticleDesc) { m_tParticleDesc = tParticleDesc; }

public:
	HRESULT ResetParticleSystem();

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

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;

private:
	HRESULT InitializeParticleSystem();
	HRESULT InitializeBuffers();

	void EmitParticles(_float fTimeDelta);
	void UpdateParticles(_float fTimeDelta);
	void SortParticles();
	
	void VelocityLerp(_uint iParticleIndex);
	void SizeLerp(_uint iParticleIndex);
	void AlphaLerp(_uint iParticleIndex);

	void KillParticles();

	HRESULT UpdateBuffers();
	void RenderBuffers();

private:
	/* Particle System Properties. */
	PARTICLEDESC m_tParticleDesc;

	_int m_fCurrentParticleCount;
	_float m_fAccumulatedTime;
	_bool m_bDidBurst = false;

	/* Buffer Properties. */
	VertexType* m_Vertices = nullptr;
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	_int m_iVertexCount = 0;
	_int m_iIndexCount = 0;

	/* List of Particles. */
	ParticleType* m_Particles = nullptr; 

	CTexture* m_pTextureCom = nullptr; 

public:
	static CParticleSystem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END