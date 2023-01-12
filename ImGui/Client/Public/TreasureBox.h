#pragma once

#include "BaseObj.h"
BEGIN(Engine)
class CModel;
END


BEGIN(Client)

class CTreasureBox final : public CBaseObj
{
public:
	enum STATE { APPEAR, CLOSE, CLOSE_WAIT, NOT_OPEN, OPEN, OPEN_WAIT };
	enum ITEMTYPE { COMPASS, MAP, SMALL_KEY, BOSS_KEY, FEATHER, HEART, RUBY };

	typedef struct TreasureBoxTag
	{
		ITEMTYPE eItemType = COMPASS;
		_float3 vPosition = _float3(0.f, 0.f, 0.f);
		_bool	bVisible = true;
	}BOXTAG;
	

private:
	CTreasureBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTreasureBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	BOXTAG	Get_BoxDesc() { return m_eTreasureBoxDesc; }
	void	Set_BoxDesc(BOXTAG BoxDesc) { memcpy(&m_eTreasureBoxDesc, &BoxDesc, sizeof(BOXTAG)); }
	void	Set_Picked();

private:
	void Change_Animation(_float fTimeDelta); 
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;

private:
	CModel*					m_pModelCom = nullptr;
	BOXTAG					m_eTreasureBoxDesc;
	_bool					m_bGet = false;

public:
	static CTreasureBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END