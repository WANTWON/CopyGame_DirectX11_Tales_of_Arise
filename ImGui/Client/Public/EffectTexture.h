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
	typedef struct tagEffectDescription
	{
		EFFECT_TYPE eType = TYPE_END;

		_tchar wcFileName[MAX_PATH] = TEXT("");				// "Effect.dds" ~ "Effect.png" ~ "Effect.fbx" 
		_tchar wcFilePath[MAX_PATH] = TEXT("");				// "../../../Bin/Resources/Textures/Effect/*" ~ "../../../Bin/Resources/Meshes/Effect/*"

		_tchar wcTexturePrototypeId[MAX_PATH] = TEXT("");
		_tchar wcModelPrototypeId[MAX_PATH] = TEXT("");
	} EFFECTDESC;

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

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;

private:
	/* Texture Effect */
	CVIBuffer_Rect*	m_pVIBufferCom = nullptr;
	CTexture* m_pTextureCom = nullptr;

public:
	static CEffectTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END