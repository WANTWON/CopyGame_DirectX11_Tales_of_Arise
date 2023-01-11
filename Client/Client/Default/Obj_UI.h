#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;

END


BEGIN(Client)

class CObj_UI final : public CGameObject
{
protected:
	CObj_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObj_UI(const CObj_UI& rhs);
	virtual ~CObj_UI() = default;


public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta) = 0;
	virtual void Late_Tick(_float fTimeDelta) = 0;
	virtual HRESULT Render() = 0;

protected:
	CShader*				m_pShaderCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

protected:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;

protected:
	virtual HRESULT Ready_Components() = 0;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

public:
	virtual void Free() override;
};

END