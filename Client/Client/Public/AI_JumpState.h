#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAI_JumpState final : public CAIState
{
public:
	enum JUMPTYPE { JUMP_BATTLE, JUMP_END };

public:
	CAI_JumpState(class CPlayer* pPlayer, STATETYPE eType, _bool useskill = false , _float fTime = 0.f);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta);
	virtual CAIState* EventInput(void);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_bool Check_JumpEnd(_float fOffset);

private:
	void Move(_float fTimeDelta);
	_float m_fStartHeight = 0.f;
	_float m_fTime = 0.f;

	_bool m_bIsJump = false;
	_bool m_bIsDrop = false;
	_bool m_bIsInput = false;

private:
	DIRID m_eDirection = DIRID::DIR_END;
	JUMPTYPE m_eJumpType = JUMP_END;
	_bool m_bUseskill = false;
};
END
END