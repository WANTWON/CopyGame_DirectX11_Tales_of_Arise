#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)

class CAstralDoubt_TeleportState final : public CAstralDoubt_State
{
public:
	CAstralDoubt_TeleportState(class CAstralDoubt* pAstral_Doubt);
	
	virtual CAstralDoubt_State* AI_Behaviour(_float fTimeDelta) override;
	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fTime = 0.f;
};

END
END