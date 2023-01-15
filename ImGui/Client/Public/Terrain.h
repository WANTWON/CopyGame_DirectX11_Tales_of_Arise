#pragma once

#include "BaseObj.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CTexture;

END


BEGIN(Client)

class CTerrain final : public CBaseObj
{
public:
	enum ShADERID { DEFAULT , WIRE, PICKED, SPLATTING };
	enum TERRAIN_DEBUG_TYPE { DEBUG_SOILD, DEBUG_WIRE, DEBUG_NONE };
	enum TEXTURE { TYPE_DIFFUSE, TYPE_BRUSH, TYPE_FILTER, TYPE_END };
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual int Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

public:
	virtual _bool Picking(_float3* PickingPoint);
	virtual void PickingTrue();

private:
	virtual HRESULT Ready_Components(void* pArg)override;
	virtual HRESULT SetUp_ShaderID()override;
	virtual HRESULT SetUp_ShaderResources()override;
	void Set_Terrain_Shape();
	void Set_Picked();


private:
	CTexture*				m_pTextureCom[TYPE_END] = { nullptr };
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;

	_bool	m_bWireFrame = false;
	_bool	m_bDebugShow = true;
	TERRAIN_DEBUG_TYPE m_eDebugtype = DEBUG_NONE;

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END