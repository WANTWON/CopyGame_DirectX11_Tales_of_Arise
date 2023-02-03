#pragma once

#include "Effect.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEffectTexture final : public CEffect
{
public:
	typedef struct tagTextureEffectDescription
	{
		_tchar wcPrototypeId[MAX_PATH] = TEXT("");

		_float3 vColorInitial = _float3(1.f, 1.f, 1.f);
		_float3 vColor = _float3(1.f, 1.f, 1.f);
		_float fAlphaInitial = 1.f;
		_float fAlpha = 1.f;
		_float m_fAlphaDiscard = 0.f;
		_bool m_bGlow = false;
		_float3 vGlowColor = _float3(1.f, 1.f, 1.f);
		_bool m_bDistortion = false;
		_float fInitialSize = 1.f;
		_float fSize = 1.f;
		_float fLifetime = 0.f;
		_float fNoiseSpeed = 0.f;
		_float fNoisePowerInitial = 0.f;
		_float fNoisePower = 10.f;
	} TEXTUREEFFECTDESC;

public:
	virtual _tchar* Get_PrototypeId() { return m_tTextureEffectDesc.wcPrototypeId; }

	TEXTUREEFFECTDESC Get_TextureEffectDesc() { return m_tTextureEffectDesc; }
	void Set_TextureEffectDesc(TEXTUREEFFECTDESC tTextureDesc) { m_tTextureEffectDesc = tTextureDesc; }

public:
	CEffectTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffectTexture(const CEffectTexture& rhs);
	virtual ~CEffectTexture() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Glow() override;

public:
	void ColorLerp();
	void SizeLerp();
	void AlphaLerp();

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;

private:
	TEXTUREEFFECTDESC m_tTextureEffectDesc;

	/* Texture Effect */
	CVIBuffer_Rect*	m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

	_float m_fTimer = 0.f;

public:
	static CEffectTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END