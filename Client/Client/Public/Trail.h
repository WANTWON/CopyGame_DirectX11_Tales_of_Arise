#pragma once
#include "Client_Defines.h"
#include "BaseObj.h"
#include "GameInstance.h"

BEGIN(Engine)
class CVIBuffer_Trail;
class CTexture;
END

BEGIN(Client)
class CTrail final : public CBaseObj
{
protected:
	CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail(const CTrail& rhs);
	virtual ~CTrail() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void Update_Trail_Point(_float3 vPointDown, _float3 vPointUp, _fmatrix vWeaponMatrix);

public:
	void Set_Target(CBaseObj* pObject) { m_pTarget = pObject; }

private:
	virtual HRESULT Ready_Components(void* pArg = nullptr);
	virtual HRESULT SetUp_ShaderID();
	virtual HRESULT SetUp_ShaderResources();

private:


private:
	CVIBuffer_Trail*		m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CBaseObj*				m_pTarget = nullptr;
	_float3			m_vPointDown;
	_float3			m_vPoint_Up;
	_float4x4		m_WeaponMatrix;

public:
	static CTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END