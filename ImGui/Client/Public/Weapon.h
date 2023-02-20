#pragma once

#include "BaseObj.h"

BEGIN(Engine)
class CModel;
class CHierarchyNode;
END

BEGIN(Client)
class CWeapon final : public CBaseObj
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
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	HANDLEDESC Get_HandleDesc(void) const { return m_tHandleDesc; }
	void Change_HandleDesc(HANDLEDESC tHandleDesc) { memcpy(&m_tHandleDesc, &tHandleDesc, sizeof(HANDLEDESC)); }

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render(void);

private:
	CModel* m_pModelCom = nullptr;

private:
	HANDLEDESC m_tHandleDesc;
	_float4x4 m_CombinedWorldMatrix;

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT SetUp_ShaderResources(void) override;

public:
	static CWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};
END