#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)
class CRunState final : public CIceWolfState
{
public:
	CRunState(class CIce_Wolf* pIceWolf, DIRID eDir);

	virtual CIceWolfState* AI_Behaviour() override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Move(_float fTimeDelta);

private:
	DIRID m_eDirection = DIRID::DIR_END;
};
END
END