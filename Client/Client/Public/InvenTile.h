#pragma once
#include "Obj_UI.h"

BEGIN(Client)

class CInvenItem;
class CInvenTile final : public CObj_UI
{
public:
	enum TILE_TYPE { INVEN_TILE, EQUIP_TILE };
	enum EQUIP_KEY { EQUIP_X, EQUIP_Y, EQUIP_NONE };
	enum TILE_STATE { STATE_DEFAULT, STATE_EQUIP };

	typedef struct InvenTiletag
	{
		TILE_TYPE eTileType = INVEN_TILE;
		EQUIP_KEY eEquipKey = EQUIP_NONE;
		TILE_STATE eState = STATE_DEFAULT;
		_float2 vPosition = _float2(0.f, 0.f);

	}INVENDESC;

private:
	CInvenTile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInvenTile(const CInvenTile& rhs);
	virtual ~CInvenTile() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_SelectTile(_bool type) { m_bSelected = type; }
	void Set_TileState(TILE_STATE eState) {m_InvenDesc.eState = eState;}
	void Set_TextureNum(_uint iNum);
	_uint Get_TextureNum();
	class CInvenItem* Get_Item() { return m_pItem; }

private:
	virtual HRESULT Ready_Components(void * pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources()override;  /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() override;

private:
	class CInvenItem* m_pItem = nullptr;
	INVENDESC  m_InvenDesc;
	_bool m_bSelected = false;

public:
	static CInvenTile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END