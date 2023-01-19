#pragma once

#include "BaseObj.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CModel;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)
class CEffect final : public CBaseObj
{
public:
	enum EFFECT_TYPE 
	{ 
		TYPE_TEXTURE,	
		TYPE_MESH, 
		TYPE_PARTICLE, 
		TYPE_END 
	};

	typedef struct tagEffectDescription
	{
		EFFECT_TYPE eType = TYPE_END;

		_tchar wcFileName[MAX_PATH] = TEXT("");				// "Effect.dds" ~ "Effect.png" ~ "Effect.fbx" 
		_tchar wcFilePath[MAX_PATH] = TEXT("");				// "../../../Bin/Resources/Textures/Effect/*" ~ "../../../Bin/Resources/Meshes/Effect/*"

		_tchar wcTexturePrototypeId[MAX_PATH] = TEXT("");
		_tchar wcModelPrototypeId[MAX_PATH] = TEXT("");		
	
		_float fSpeed = 0.f;

		/*vector<_float4> ScaleCurves;
		vector<_float4> ColorCurves;*/
	} EFFECTDESC;

	EFFECTDESC Get_EffectDesc() { return m_tEffectDesc; }
	/*vector<_float4> Get_ScaleCurves() { return m_tEffectDesc.ScaleCurves; }
	vector<_float4> Get_ColorCurves() { return m_tEffectDesc.ColorCurves; }
	void Set_ScaleCurves(vector<_float4> ScaleCurves) { m_tEffectDesc.ScaleCurves = ScaleCurves; }
	void Set_ColorCurves(vector<_float4> ColorCurves) { m_tEffectDesc.ColorCurves = ColorCurves; }*/

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

//public:
//	void Add_ScaleCurve(_float4 ScaleCurve);
//	void Add_ColorCurve(_float4 ColorCurve);

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override; 

private:
	EFFECTDESC m_tEffectDesc;

	/* Sprite Effect */
	CVIBuffer_Rect*	m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

	/* Mesh Effect */
	CModel* m_pModelCom = nullptr;

	/* Particle Effect */
	CVIBuffer_Point_Instance* m_pVIBufferPointInstanceCom = nullptr;

public:
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END