#pragma once
#include "InteractObject.h"


BEGIN(Client)
class CApple final : public CInteractObject
{


protected:
	CApple(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CApple(const CApple& rhs);
	virtual ~CApple() = default;

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
	
private:
	_bool	m_bIsGain = false;
	

public:
	static CApple* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END