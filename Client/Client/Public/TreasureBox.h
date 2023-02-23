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

	typedef struct TreasureBoxTag
	{
		_uint eItemType = 0;
		XMFLOAT3	vPosition = XMFLOAT3(0.f, 0.f, 0.f);
		XMFLOAT3	vScale = XMFLOAT3(1.f, 1.f, 1.f);
		XMFLOAT3	vRotation = XMFLOAT3(0.f, 0.f, 0.f);
		float		m_fAngle = 0.f;
	}BOXDESC;

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
	virtual HRESULT Render_EdgeDetection();

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	

protected: /* For.Components */
	BOXDESC					m_Boxtag;
	_bool					m_bOpen = false;
	_bool					m_bOpenFinish = false;
	_float					m_fTimeDeltaAcc = 0.f;
public:
	static CTreasureBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END