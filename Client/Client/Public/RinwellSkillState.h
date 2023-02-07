#pragma once

#include "RinwellState.h"

BEGIN(Client)
class CEffect;
BEGIN(AiRinwell)

class CSkillState final : public CRinwellState
{
public:
	CSkillState(class CAiRinwell* pPlayer, _uint eType);

	virtual CRinwellState* Tick(_float fTimeDelta) override;
	virtual CRinwellState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_vector m_vTargetPosition = {0.f,0.f,0.f,0.f};
	_int	m_iChangeCount = 0;
	_bool	m_ChangedDir = false;
	_bool	m_bAirMove = false;
	vector<CEffect*> m_pEffects;
	vector<CEffect*> m_pBlastEffects;
};

END
END