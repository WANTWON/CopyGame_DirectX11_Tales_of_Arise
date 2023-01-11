#pragma once
#include "Obj_UI.h"

BEGIN(Client)
class CBaseObj;
class CUIIcon final : public CObj_UI
{
public:
	
	enum ICONNAME {
		ICON_PLAYER, ICON_WARP, ICON_WARP_ON, ICON_DGN, ICON_HOUSE, ICON_TELL, ICON_SHOP,
		ICON_STAIR, ICON_BOSS, ICON_BOX, ICON_BOX_OPEN, ICON_BOOS_DOOR, ICON_LOCK_DOOR, ICON_LOCK_BOX, ICON_CLOSE_DOOR, ICON_END
	};

	typedef struct MiniMapIcontag
	{
		_uint  iTexureNum = ICON_END;
		CBaseObj* pTarget = nullptr;

	}ICONDESC;

private:
	CUIIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIIcon(const CUIIcon& rhs);
	virtual ~CUIIcon() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
 	void Set_TexutureNum(_uint iNum) { m_IconDesc.iTexureNum = iNum; }
	ICONDESC Get_IconDesc() { return m_IconDesc; }

private:
	virtual HRESULT Ready_Components(void * pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources()override;  /* 셰이더 전역변수에 값을 전달한다. */

private:
	ICONDESC		m_IconDesc;
	_bool		m_bShow = true;

public:
	static CUIIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END