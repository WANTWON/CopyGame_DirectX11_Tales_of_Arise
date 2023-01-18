#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Point_Instance;
END

BEGIN(Client)
class CParticle_Point final : public CGameObject
{
private:
	CParticle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Point(const CParticle_Point& rhs);
	virtual ~CParticle_Point() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader* m_pShaderCom = nullptr;
	CTexture* m_pTextureCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform*	m_pTransformCom = nullptr;
	CVIBuffer_Point_Instance* m_pVIBufferCom = nullptr;

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources();

public:
	static CParticle_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END