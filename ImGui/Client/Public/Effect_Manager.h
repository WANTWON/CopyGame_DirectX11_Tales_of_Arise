#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)
class CEffect_Manager final : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)

private:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	vector<class CEffect*> Get_InstancedEffects() { return m_InstancedEffects; }

public:
	void Add_Effect(class CEffect * pEffect);
	void Remove_Effect(class CEffect * pEffect);

private:
	vector<class CEffect*> m_InstancedEffects;

public:
	virtual void Free() override;
};
END