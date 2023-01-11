#pragma once
#include "Client_Defines.h"
#include "Effect.h"
#include "GameInstance.h"

BEGIN(Engine)
class CVIBuffer_SwordTrail;

END

BEGIN(Client)
class CSwordTrail final : public CEffect
{
protected:
	CSwordTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSwordTrail(const CSwordTrail& rhs);
	virtual ~CSwordTrail() = default;

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
	CVIBuffer_SwordTrail*		m_pVIBufferCom = nullptr;
	
	_float3			m_vPointDown;
	_float3			m_vPoint_Up;
	_float4x4		m_WeaponMatrix;

public:
	static CSwordTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END