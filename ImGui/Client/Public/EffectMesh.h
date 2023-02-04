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
		_tchar wcPrototypeId[MAX_PATH] = TEXT("");
		_tchar wcMaskTexture[MAX_PATH] = TEXT("");
		_tchar wcNoiseTexture[MAX_PATH] = TEXT("");
		_float3 vColorInitial = _float3(1.f, 1.f, 1.f);
		_float3 vColor = _float3(1.f, 1.f, 1.f);
		_float fAlphaInitial = 1.f;
		_float fAlpha = 1.f;
		_bool bGlow = false;
		_float3 vGlowColor = _float3(1.f, 1.f, 1.f);
		_float fGlowPower = 1.f;
		_float fLifetime = 0.f;
		_float fStartAfter = 0.f;
		_float3 vPosition = _float3(0.f, 0.f, 0.f);
		_float3 vRotation = _float3(0.f, 0.f, 0.f);
		_float3 vScaleInitial = _float3(1.f, 1.f, 1.f);
		_float3 vScale = _float3(1.f, 1.f, 1.f);
		_float3 vTurn = _float3(0.f, 0.f, 0.f);
		_float fTurnVelocityInitial = 0.f;
		_float fTurnVelocity = 0.f;
		_float fMaskSpeed = 0.f;
		_float fMaskDirectionX = 0.f;
		_float fMaskDirectionY = 0.f;
		_float fNoiseSpeed = 0.f;
		_float fNoiseDirectionX = 0.f;
		_float fNoiseDirectionY = 0.f;
		_float fNoisePowerInitial = 0.f;
		_float fNoisePower = 10.f;
	} MESHEFFECTDESC;

public:
	virtual _tchar* Get_PrototypeId() { return m_tMeshEffectDesc.wcPrototypeId; }

	MESHEFFECTDESC Get_MeshEffectDesc() { return m_tMeshEffectDesc; }
	void Set_MeshEffectDesc(MESHEFFECTDESC tMeshDesc) { m_tMeshEffectDesc = tMeshDesc; }
	MESHEFFECTDESC Get_MeshEffectDescTool () { return m_tMeshEffectDescTool; }
	void Set_MeshEffectDescTool(MESHEFFECTDESC tMeshDesc) { m_tMeshEffectDescTool = tMeshDesc; }
	
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
	virtual HRESULT Render_Glow() override;

public:
	virtual void Add_MaskTexture() override;
	virtual void Add_NoiseTexture() override;
	
	void Reset_Initial();
	void ColorLerp();
	void AlphaLerp();
	void ScaleLerp();
	void TurnVelocityLerp();
	void NoisePowerLerp();

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;

private:
	MESHEFFECTDESC m_tMeshEffectDesc;
	MESHEFFECTDESC m_tMeshEffectDescTool;

	/* Mesh Effect */
	CModel* m_pModelCom = nullptr;

	_float m_fTimer = 0.f;
	_bool m_bCanStart = false;

public:
	static CEffectMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END