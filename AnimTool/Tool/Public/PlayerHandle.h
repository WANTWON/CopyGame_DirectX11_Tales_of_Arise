#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CModel;
class CHierarchyNode;
class CCollider;
END

BEGIN(Client)

class CPlayerHandle final : public CGameObject
{
public:
	typedef struct tagHandleDesc
	{
		CHierarchyNode* pSocket = nullptr;
		_float4x4 SocketPivotMatrix;
		const _float4x4* pParentWorldMatrix;
		const _tchar* pModelTag = nullptr;
	}HANDLEDESC;

private:
	CPlayerHandle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerHandle(const CPlayerHandle& rhs);
	virtual ~CPlayerHandle() = default;

public:
	HANDLEDESC Get_HandleDesc(void) const { return m_tHandleDesc; }
	void Change_HandleDesc(HANDLEDESC tHandleDesc) { memcpy(&m_tHandleDesc, &tHandleDesc, sizeof(HANDLEDESC)); }

public:
	void Collision(void);

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render(void);

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CCollider* m_pOBBCom = nullptr;

private:
	HANDLEDESC m_tHandleDesc;
	_float4x4 m_CombinedWorldMatrix;

private:
	HRESULT Ready_Components(void);
	HRESULT SetUp_ShaderResources(void);

public:
	static CPlayerHandle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};

END