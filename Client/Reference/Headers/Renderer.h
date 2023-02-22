#pragma once

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_SHADOWDEPTH, RENDER_NONALPHABLEND, RENDER_ALPHABLENDLIGHTS, RENDER_GLOW, RENDER_NONLIGHT, RENDER_ALPHABLEND, RENDER_DISTORTION, RENDER_EDGE_DETECTION, RENDER_UI_FRONT, RENDER_UI_BACK, RENDER_UI_GLOW, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;

public:
	class CShader* Get_ShaderPostProcessing() { return m_pShaderPostProcessing; }
	void Set_Fog(_bool bFog) { m_bFog = bFog; }
	void Set_Distort(_bool bDistort) { m_bDistort = bDistort; }
	void Set_ZoomBlur(_bool bZoomBlur, _float fFocusPower = 3.f, _uint iFocusDetail = 0); 
	void Set_Saturation(_bool bSaturation, _float fSaturationPower = 1.f);

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Render_GameObjects();

#ifdef _DEBUG
public:
	HRESULT Add_Debug(class CComponent* pDebugCom);
	void Debug_Clear();
#endif // _DEBUG

private:
	list<class CGameObject*> m_GameObjects[RENDER_END];
	typedef list<class CGameObject*> GAMEOBJECTS;

#ifdef _DEBUG
private:
	list<class CComponent*> m_DebugComponents;
#endif

private:
	class CTarget_Manager* m_pTarget_Manager = nullptr;
	class CLight_Manager* m_pLight_Manager = nullptr;

	class CVIBuffer_Rect* m_pVIBuffer = nullptr;
	class CShader* m_pShaderDeferred = nullptr;
	class CShader* m_pShaderPostProcessing = nullptr;

	_float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	HRESULT Render_Priority();
	HRESULT Render_ShadowDepth();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_AlphaBlendLights();
	HRESULT Render_Lights();
	HRESULT Render_Glow();
	HRESULT Render_Blend();
	HRESULT Render_NonLight();
	HRESULT Render_AlphaBlend();
	HRESULT Render_Distortion();
	HRESULT Render_EdgeDetection();

	HRESULT Render_PostProcessing();

	HRESULT Render_UI();
	HRESULT Render_UI_Glow();

	HRESULT Render_UI_PostProcessing();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
	bool m_bRenderDebug = false;
	bool m_bRenderComponentDebug = false;
#endif // _DEBUG

	/* Distortion Objects */
	class CTexture* m_pDistortionTexture = nullptr;
	class CTexture* m_pDistortionNoiseTexture = nullptr;
	_float m_fDistortionTimer = 0.f;
	
	/* Fog (Post Processing) */
	_bool m_bFog = false;

	/* Distortion (Post Processing) */
	_bool m_bDistort = false;
	class CTexture* m_pScreenDistortionTexture = nullptr;
	_float m_fScreenDistortionTimer = 0.f;

	/* Zoom Blur (Post Processing) */
	_bool m_bZoomBlur = false;
	_float m_fFocusPower = 3.f;
	_int m_iFocusDetail = 1;

	/* Saturation */
	_bool m_bSaturation = false;
	_float m_fSaturationPower = 1.f;

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void Free();
};
END