#pragma once
#include "InteractObject.h"


BEGIN(Client)
class CTreasureBox final : public CInteractObject
{
public:
	enum ANIM {
		Close1,
		Close2,
		Open1,
		Open2,
		Default,
	};

protected:
	CTreasureBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTreasureBox(const CTreasureBox& rhs);
	virtual ~CTreasureBox() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();


protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	

protected: /* For.Components */
	//CModel*					m_pModelCom = nullptr;
	//CTexture*				m_pDissolveTexture = nullptr;
	_bool					m_bOpen = false;
	_bool					m_bOpenFinish = false;

public:
	static CTreasureBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END