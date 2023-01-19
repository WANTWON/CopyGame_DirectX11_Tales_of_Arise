#pragma once

#include "Base.h"
#include "Effect.h"

BEGIN(Client)
class CEffect_Manager final : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

public:
	list<class CEffect*> Get_InstancedEffects() { return m_InstancedEffects; }

public:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT Create_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, void* pArg = nullptr);

private:
	list<class CEffect*> m_InstancedEffects;

public:
	virtual void Free() override;
};
END