#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "VIBuffer_Terrain.h"
#include "Terrain.h"

BEGIN(Client)


class CTerrain_Manager final : public CBase
{
	DECLARE_SINGLETON(CTerrain_Manager)

public:
	enum TERRAIN_DEBUG_TYPE { DEBUG_SOILD, DEBUG_WIRE, DEBUG_NONE };

	typedef struct TerrainTagDesc
	{
		CVIBuffer_Terrain::TERRAINDESC TerrainDesc;

		_bool	m_bShowWireFrame = true;
		TERRAIN_DEBUG_TYPE	m_eDebugTerrain = DEBUG_NONE;

	}TERRAINDESC;

	typedef struct TerrainShapingTagDesc
	{
		_float  fHeight = 0;
		_float	fRadius = 0.f;
		_float	fSharp = 0.f;

	}TERRAINSHAPEDESC;


private:
	CTerrain_Manager();
	virtual ~CTerrain_Manager() = default;

public:
	void Add_DebugTerrain(class CTerrain* pTerrain) { m_pTerrain = pTerrain; }
	CVIBuffer_Terrain* Create_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, HANDLE hFile, _ulong& dwByte);
	CVIBuffer_Terrain* Create_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, HANDLE hFile, _ulong& dwByte);
	void Out_DebugTerrain();

public:
	void Set_TerrainDesc(TERRAINDESC* eTerrainDesc) { memcpy(&m_pTerrainDesc, eTerrainDesc, sizeof(TERRAINDESC)); }
	void Set_TerrainShapeDesc(TERRAINSHAPEDESC* eTerrainDesc) { memcpy(&m_TerrainShapeDesc, eTerrainDesc, sizeof(TERRAINSHAPEDESC)); }
	void Set_bWireFrame(_bool type) { m_pTerrainDesc.m_bShowWireFrame = type; }
	void Set_TerrainShow(_bool type) { m_bTerrainShow = type; }
	void Set_MoveOffset(_int iOffset ) { m_iMoveOffset = iOffset; }
	void Set_PickingWorldPos(_float3 vPickingPos) {m_vMousePickPos = vPickingPos;}

public:
	TERRAINDESC Get_TerrainDesc() const { return m_pTerrainDesc; }
	TERRAINSHAPEDESC Get_TerrainShapeDesc() const { return m_TerrainShapeDesc; }
	_bool Get_TerrainShow() { return m_bTerrainShow; }
	_float3 Get_PickingPos() { return m_vMousePickPos; }
	_int Get_MoveOffset() { return m_iMoveOffset; }


public:
	HRESULT Create_Terrain(LEVEL eLevel, const _tchar* pLayerTag);

private:
	class CTerrain*		m_pTerrain =  nullptr;
	TERRAINDESC		m_pTerrainDesc;
	TERRAINSHAPEDESC m_TerrainShapeDesc;
	_bool m_bTerrainShow = true;
	_float3 m_vMousePickPos = _float3(0,0,0);
	_int m_iMoveOffset = 1;

public:
	void Free() override;
};

END