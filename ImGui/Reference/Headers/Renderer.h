#pragma once

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP { RENDER_PRIORITY, RENDER_SHADOWDEPTH, RENDER_NONALPHABLEND, RENDER_NONLIGHT, RENDER_ALPHABLEND, RENDER_UI_FRONT, RENDER_UI_BACK, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;

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
	class CShader* m_pShader = nullptr;
	_float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	HRESULT Render_Priority();
	HRESULT Render_ShadowDepth();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_Lights();
	HRESULT Render_Glow();
	HRESULT Render_Blend();
	HRESULT Render_NonLight();
	HRESULT Render_AlphaBlend();
	HRESULT Render_UI();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
	bool m_bRenderDebug = false;
	bool m_bRenderComponentDebug = false;
#endif // _DEBUG

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void Free();
};
END