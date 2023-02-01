#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"

BEGIN(Engine)
class CModel;
class CHierarchyNode;
END

BEGIN(Client)

class CWeapon final : public CBaseObj
{
public:
	typedef struct tagWeaponDesc
	{
		CHierarchyNode*		pSocket = nullptr;
		_float4x4			SocketPivotMatrix;
		const _float4x4*	pParentWorldMatrix;
		char				pModeltag[MAX_PATH] = "";
		CBaseObj*			pOwner = nullptr;

	}WEAPONDESC;
private:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	_float4x4 Get_CombinedWorldMatrix() { return m_CombinedWorldMatrix; }
	CBaseObj* Get_Owner() { return m_WeaponDesc.pOwner; }
	void On_Collider(void) { m_isCollider = true; }
	void Off_Collider(void) { m_isCollider = false; }
	void Set_WeaponDesc(WEAPONDESC tWeaponDesc); 

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CModel*					m_pModelCom = nullptr;

private:
	WEAPONDESC				m_WeaponDesc;
	_float4x4				m_CombinedWorldMatrix;


private:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_bool m_isCollider = false;


public:
	static CWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END