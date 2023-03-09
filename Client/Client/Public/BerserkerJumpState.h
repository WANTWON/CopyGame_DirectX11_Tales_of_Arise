#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)

class CBerserkerJumpState final : public CBerserkerState
{
public:
	CBerserkerJumpState(class CBerserker* pBerserker, _float fTime);
	
	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

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