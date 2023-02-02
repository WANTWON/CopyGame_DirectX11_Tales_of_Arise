#pragma once

#include "BaseObj.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CModel;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)
class CEffect abstract : public CBaseObj
{
public:
	enum EFFECT_TYPE
	{
		TYPE_TEXTURE,
		TYPE_MESH,
		TYPE_PARTICLE,
		TYPE_END
	};

	/* Struct used to Save/Load Data. */
	typedef struct tagEffectDescription
	{
		EFFECT_TYPE eType = TYPE_END;
		_uint iVelocityCurvesCount = 0;
		_uint iSizeCurvesCount = 0;
		_uint iAlphaCurvesCount = 0;
	} EFFECTDESC;

	_bool m_bIsSelected = true;

public:
	vector<_float3> Get_VelocityCurves() { return m_VelocityCurves; }
	vector<_float3> Get_SizeCurves() { return m_SizeCurves; }
	vector<_float3> Get_AlphaCurves() { return m_AlphaCurves; }
	vector<_float3> Get_TurnVelocityCurves() { return m_TurnVelocityCurves; }
	vector<_float3> Get_NoisePowerCurves() { return m_NoisePowerCurves; }
	void Set_VelocityCurves(vector<_float3> VelocityCurves) { m_VelocityCurves = VelocityCurves; }
	void Set_SizeCurves(vector<_float3> SizeCurves) { m_SizeCurves = SizeCurves; }
	void Set_AlphaCurves(vector<_float3> AlphaCurves) { m_AlphaCurves = AlphaCurves; }
	void Set_TurnVelocityCurves(vector<_float3> TurnVelocityCurves) { m_TurnVelocityCurves = TurnVelocityCurves; }
	void Set_NoisePowerCurves(vector<_float3> NoisePowerCurves) { m_NoisePowerCurves = NoisePowerCurves; }
	EFFECT_TYPE Get_EffectType() { return m_eType; }
	virtual _tchar* Get_PrototypeId() PURE;
	_bool Get_Play() { return m_bPlay; }
	void Set_EffectType(EFFECT_TYPE eType) { m_eType = eType; }
	void Set_Play(_bool bPlay) { m_bPlay = bPlay; }
	void Set_ShaderId(_uint iShaderId) { m_eShaderID = iShaderId; }

public:
	void Add_VelocityCurve(_float3 VelocityCurve) { m_VelocityCurves.push_back(VelocityCurve); }
	void Add_SizeCurve(_float3 SizeCurve) { m_SizeCurves.push_back(SizeCurve); }
	void Add_AlphaCurve(_float3 AlphaCurve) { m_AlphaCurves.push_back(AlphaCurve); }
	void Add_TurnVelocityCurve(_float3 TurnVelocityCurve) { m_TurnVelocityCurves.push_back(TurnVelocityCurve); }
	void Add_NoisePowerCurve(_float3 NoisePowerCurve) { m_NoisePowerCurves.push_back(NoisePowerCurve); }

	void Remove_VelocityCurve(_uint iIndex) { m_VelocityCurves.erase(m_VelocityCurves.begin() + iIndex); };
	void Remove_SizeCurve(_uint iIndex) { m_SizeCurves.erase(m_SizeCurves.begin() + iIndex); };
	void Remove_AlphaCurve(_uint iIndex) { m_AlphaCurves.erase(m_AlphaCurves.begin() + iIndex); };
	void Remove_TurnVelocityCurve(_uint iIndex) { m_TurnVelocityCurves.erase(m_TurnVelocityCurves.begin() + iIndex); }
	void Remove_NoisePowerCurve(_uint iIndex) { m_NoisePowerCurves.erase(m_NoisePowerCurves.begin() + iIndex); }

	virtual void Add_MaskTexture() {};
	virtual void Add_NoiseTexture() {};
	virtual void Add_DissolveTexture() {};

public:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override; 

protected:
	EFFECT_TYPE m_eType = TYPE_END;

	_bool m_bPlay = false;

	/* X = Value, Y = Start, Z = End */
	vector<_float3> m_VelocityCurves;
	vector<_float3> m_SizeCurves;
	vector<_float3> m_AlphaCurves;
	vector<_float3> m_TurnVelocityCurves;
	vector<_float3> m_NoisePowerCurves;

	CTexture* m_pMaskTexture = nullptr;
	CTexture* m_pNoiseTexture = nullptr;
	CTexture* m_pDissolveTexture = nullptr;

public:
	virtual void Free() override;
};
END