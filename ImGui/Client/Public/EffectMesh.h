#pragma once

#include "Effect.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CEffectMesh final : public CEffect
{
public:
	typedef struct tagMeshEffectDescription
	{
		_float3 vColor = _float3(1.f, 1.f, 1.f);
		_float fAlphaInitial = 1.f;
		_float fAlpha = 1.f;
		_float fLifetime = 0.f;

		_float3 vTurn = _float3(0.f, 0.f, 0.f);
		_float fTurnVelocityInitial = 0.f;
		_float fTurnVelocity = 0.f;

		_tchar wcPrototypeId[MAX_PATH] = TEXT("");	/* "Spark.dds > Spark" */
		_tchar wcMaskTexture[MAX_PATH] = TEXT("");
		_tchar wcNoiseTexture[MAX_PATH] = TEXT("");
		_tchar wcDissolveTexture[MAX_PATH] = TEXT("");

		_float fNoiseSpeed = 0.f;
		_float fNoisePowerInitial = 0.f;
		_float fNoisePower = 10.f;
		
	} MESHEFFECTDESC;

public:
	virtual _tchar* Get_PrototypeId() { return m_tMeshEffectDesc.wcPrototypeId; }

	MESHEFFECTDESC Get_MeshEffectDesc() { return m_tMeshEffectDesc; }
	void Set_MeshEffectDesc(MESHEFFECTDESC tMeshDesc) { m_tMeshEffectDesc = tMeshDesc; }
	
public:
	CEffectMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffectMesh(const CEffectMesh& rhs);
	virtual ~CEffectMesh() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Add_MaskTexture() override;
	virtual void Add_NoiseTexture() override;
	virtual void Add_DissolveTexture() override;
	
	void VelocityLerp();
	void SizeLerp();
	void AlphaLerp();
	void TurnVelocityLerp();
	void NoisePowerLerp();

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;

private:
	MESHEFFECTDESC  m_tMeshEffectDesc;

	/* Mesh Effect */
	CModel* m_pModelCom = nullptr;

	_float m_fTimer = 0.f;

public:
	static CEffectMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END