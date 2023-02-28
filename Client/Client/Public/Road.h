#pragma once

#include "InteractObject.h"

BEGIN(Client)
class CRoad : public CInteractObject
{
protected:
	CRoad(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRoad(const CRoad& rhs);
	virtual ~CRoad() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_EdgeDetection() override;

protected:
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual _bool Is_AnimationLoop(_uint eAnimId) override;
	virtual HRESULT SetUp_ShaderID() override;

private:
	NONANIMDESC				m_ModelDesc;

public:
	static CRoad* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free(void) override;
};
END