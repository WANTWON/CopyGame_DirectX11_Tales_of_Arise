#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CChaseState : public CIceWolfState
{
public:
	CChaseState(class CIce_Wolf* pIceWolf);

	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;
	
private:
	_bool m_bTriggerTurn = false;
	_float m_fRandTime = 0.f;
	
//private:
//	void Find_Target_InField(void);
};

END
END