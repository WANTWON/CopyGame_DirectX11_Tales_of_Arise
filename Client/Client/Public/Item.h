#pragma once

#include "InteractObject.h"

BEGIN(Client)
class CItem final : public CInteractObject
{
public:
	enum ITEMTYPE { APPLE, JEWEL, MUSHROOM, LETTUCE, PLANT, BOX,PIZZA , BREAD, STEAK , TABACO , CHESS, CRYSTAL };

	typedef struct ItemTag
	{
		ITEMTYPE etype = APPLE;
		NONANIMDESC m_ModelDesc;
	}ITEMDESC;

protected:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CItem& rhs);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Glow() override;
	virtual HRESULT Render_EdgeDetection() override;

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual _bool Is_AnimationLoop(_uint eAnimId) override { return true; }
	virtual HRESULT SetUp_ShaderResources() override;
	virtual HRESULT SetUp_ShaderID() override;

private:
	void Spawn_Particles();

private:
	ITEMDESC m_ItemDesc;
	_bool m_bIsGain = false;

	_bool m_bfirst = false;
	_float m_fTimeDeltaAcc = 0.f;

	vector<class CEffect*> m_pPickupFlares;
	vector<class CEffect*> m_pGetItem;

	_float m_fDissolveTimer = 0.f;
	_float m_fDissolveLifespan = 3.f;

public:
	static CItem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END