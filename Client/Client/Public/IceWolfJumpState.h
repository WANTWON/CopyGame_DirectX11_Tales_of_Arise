#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CIceWolfJumpState final : public CIceWolfState
{
public:
	CIceWolfJumpState(class CIce_Wolf* pIceWolf, _float fTime);

	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fTime = 0.f;
	_float m_fStartHeight;

private:
	void Move(_float fTimeDelta);
};

END
END