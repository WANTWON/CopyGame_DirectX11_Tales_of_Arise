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
	HRESULT Initialize_Load(const _tchar * VIBufferTag, void * pArg);
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
	

public:
	void Set_Terrain_Shape();
	void Set_Picked();
	void Save_Terrain(HANDLE hFile, _ulong* dwByte);
	HRESULT Create_FilterTexture();

private:
	CTexture*							m_pTextureCom[TYPE_END] = { nullptr };
	CVIBuffer_Terrain*					m_pVIBufferCom = nullptr;
	ID3D11ShaderResourceView*			m_pFilterTexture = nullptr;


	_bool	m_bWireFrame = false;
	_bool	m_bDebugShow = true;
	TERRAIN_DEBUG_TYPE m_eDebugtype = DEBUG_NONE;

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual CGameObject* Clone_Load(const _tchar* VIBufferTag, void* pArg = nullptr) override;
	virtual void Free() override;
};

END