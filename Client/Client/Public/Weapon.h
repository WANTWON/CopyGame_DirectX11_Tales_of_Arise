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
	enum TYPE { NONE, BOW, ARROW, DOGFOOD, HEART_CONTAINER, MAGIC_ROD, TELEPHONE, WEAPON_END
	};


	typedef struct tagWeaponDesc
	{
		CHierarchyNode*		pSocket = nullptr;
		_float4x4			SocketPivotMatrix;
		const _float4x4*	pParentWorldMatrix;
		TYPE				eType = BOW;

	}WEAPONDESC;

private:
	CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

	void TarilWeapon_Update(_fmatrix fSocketMatirx);

public:
	TYPE Get_PartsType() { return m_WeaponDesc.eType; }

private:
	CModel*					m_pModelCom = nullptr;
	_bool					m_bFirst = false;
	_vector					m_BulletLook = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	_float					m_Ypos = 0.f;

private:
	WEAPONDESC				m_WeaponDesc;
	_float4x4				m_CombinedWorldMatrix;

	class CGameObject*		m_pTrailWeapon_Effect = nullptr;
	_float					m_fEffectTurm = 0.f;

	_float3					m_vPointUp;
	_float3					m_vPointDown;

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	HRESULT Ready_Effect();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END