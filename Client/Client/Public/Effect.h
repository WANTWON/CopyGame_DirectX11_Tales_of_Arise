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
	
public:
	vector<array<_float, 5>> Get_ColorCurves() { return m_ColorCurves; }
	vector<_float3> Get_VelocityCurves() { return m_VelocityCurves; }
	vector<_float3> Get_SizeCurves() { return m_SizeCurves; }
	vector<array<_float, 5>> Get_ScaleCurves() { return m_ScaleCurves; }
	vector<_float3> Get_AlphaCurves() { return m_AlphaCurves; }
	vector<_float3> Get_TurnVelocityCurves() { return m_TurnVelocityCurves; }
	vector<_float3> Get_NoisePowerCurves() { return m_NoisePowerCurves; }
	void Set_ColorCurves(vector<array<_float, 5>> ColorCurves) { m_ColorCurves = ColorCurves; }
	void Set_VelocityCurves(vector<_float3> VelocityCurves) { m_VelocityCurves = VelocityCurves; }
	void Set_SizeCurves(vector<_float3> SizeCurves) { m_SizeCurves = SizeCurves; }
	void Set_ScaleCurves(vector<array<_float, 5>> ScaleCurves) { m_ScaleCurves = ScaleCurves; }
	void Set_AlphaCurves(vector<_float3> AlphaCurves) { m_AlphaCurves = AlphaCurves; }
	void Set_TurnVelocityCurves(vector<_float3> TurnVelocityCurves) { m_TurnVelocityCurves = TurnVelocityCurves; }
	void Set_NoisePowerCurves(vector<_float3> NoisePowerCurves) { m_NoisePowerCurves = NoisePowerCurves; }
	EFFECT_TYPE Get_EffectType() { return m_eType; }
	virtual _tchar* Get_PrototypeId() PURE;
	void Set_EffectType(EFFECT_TYPE eType) { m_eType = eType; }
	void Set_ShaderId(_uint iShaderId) { m_eShaderID = iShaderId; }

public:
	void Add_ColorCurve(array<_float, 5> ColorCurve) { m_ColorCurves.push_back(ColorCurve); }
	void Add_VelocityCurve(_float3 VelocityCurve) { m_VelocityCurves.push_back(VelocityCurve); }
	void Add_SizeCurve(_float3 SizeCurve) { m_SizeCurves.push_back(SizeCurve); }
	void Add_ScaleCurve(array<_float, 5> ScaleCurve) { m_ScaleCurves.push_back(ScaleCurve); }
	void Add_AlphaCurve(_float3 AlphaCurve) { m_AlphaCurves.push_back(AlphaCurve); }
	void Add_TurnVelocityCurve(_float3 TurnVelocityCurve) { m_TurnVelocityCurves.push_back(TurnVelocityCurve); }
	void Add_NoisePowerCurve(_float3 NoisePowerCurve) { m_NoisePowerCurves.push_back(NoisePowerCurve); }

	void Remove_ColorCurve(_uint iIndex) { m_ColorCurves.erase(m_ColorCurves.begin() + iIndex); }
	void Remove_VelocityCurve(_uint iIndex) { m_VelocityCurves.erase(m_VelocityCurves.begin() + iIndex); };
	void Remove_SizeCurve(_uint iIndex) { m_SizeCurves.erase(m_SizeCurves.begin() + iIndex); };
	void Remove_ScaleCurve(_uint iIndex) { m_ScaleCurves.erase(m_ScaleCurves.begin() + iIndex); }
	void Remove_AlphaCurve(_uint iIndex) { m_AlphaCurves.erase(m_AlphaCurves.begin() + iIndex); };
	void Remove_TurnVelocityCurve(_uint iIndex) { m_TurnVelocityCurves.erase(m_TurnVelocityCurves.begin() + iIndex); }
	void Remove_NoisePowerCurve(_uint iIndex) { m_NoisePowerCurves.erase(m_NoisePowerCurves.begin() + iIndex); }

	virtual void Add_MaskTexture() {};
	virtual void Add_NoiseTexture() {};

	void Set_EffectName(_tchar* pEffectName) { wcscpy_s(m_wcEffectName, MAX_PATH, pEffectName); }
	void Set_AttachObject(CGameObject* pGameObject) { m_pAttachObject = pGameObject; }

	static vector<class CEffect*> PlayEffectAtLocation(_tchar* wcEffectName, _vector vLocation);

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
	
	/* X = Value, Y = Start, Z = End */
	vector<array<_float, 5>> m_ColorCurves;
	vector<_float3> m_VelocityCurves;
	vector<_float3> m_SizeCurves;				/* "Size" is uniform Scaling (same value for X, Y and Z Axis). Used for Texture and Particles Effects. */
	vector<array<_float, 5>> m_ScaleCurves;		/* "Scale" is standard Scaling (different values for X, Y, and Z Axis). Used for Mesh Effects. */
	vector<_float3> m_AlphaCurves;
	vector<_float3> m_TurnVelocityCurves;
	vector<_float3> m_NoisePowerCurves;

	CTexture* m_pMaskTexture = nullptr;
	CTexture* m_pNoiseTexture = nullptr;

	/* If != nullptr, Effect should stay attached to GameObject Position. */
	CGameObject* m_pAttachObject = nullptr;

	_tchar m_wcEffectName[MAX_PATH] = TEXT("");

public:
	virtual void Free() override;
};
END