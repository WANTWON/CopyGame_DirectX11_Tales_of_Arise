#pragma once

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)

private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	ID3D11DepthStencilView*	Get_DepthStencilView() { return m_pOldDSV; }
	ID3D11ShaderResourceView* Get_BackBufferCopySRV() { return m_pBackBufferSRV; }

public:
	HRESULT Ready_ShadowDepthStencilRenderTargetView(ID3D11Device * pDevice, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferCopyTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWinCX, _uint iWinCY);

	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, const _float4* pColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT Begin_ShadowMRT(ID3D11DeviceContext* pContext, const _tchar* pMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* pContext);
	
	HRESULT Bind_ShaderResource(const _tchar* pTargetTag, class CShader* pShader, const char* pConstantName);
	HRESULT Copy_BackBufferTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _tchar* pMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

private:
	/* Render Targets. */
	map<const _tchar*, class CRenderTarget*> m_RenderTargets;
	typedef map<const _tchar*, class CRenderTarget*> RENDERTARGETS;

	/* Multiple Rendering Targets. */
	map<const _tchar*, list<class CRenderTarget*>> m_MRTs;
	typedef map<const _tchar*, list<class CRenderTarget*>> MRTS;

private:
	ID3D11RenderTargetView*	m_pOldRTV = nullptr;
	ID3D11DepthStencilView*	m_pOldDSV = nullptr;
	ID3D11DepthStencilView*	m_pShadowDeptheStencil = nullptr;
	ID3D11Texture2D* m_pBackBufferTextureCopy = nullptr;
	ID3D11ShaderResourceView* m_pBackBufferSRV = nullptr;

private:
	class CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};
END