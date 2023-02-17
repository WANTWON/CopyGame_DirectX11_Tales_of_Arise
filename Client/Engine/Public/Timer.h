#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{	
private:
	CTimer();
	virtual ~CTimer() = default;

public:
	float Get_TimeDelta(void) { 
		return m_fTimeDelta*m_fTimeOffset; }

	void Set_TimeSpeedOffset(_float fOffset) { m_fTimeOffset = fOffset; }
	float Get_TimeSpeedOffset() { return m_fTimeOffset;}
public:
	HRESULT Initialize();
	void Update(void);

private:
	LARGE_INTEGER		m_CurrentTime;
	LARGE_INTEGER		m_OldTime;
	LARGE_INTEGER		m_OriginTime;
	LARGE_INTEGER		m_CpuTick;

	_float				m_fTimeDelta;
	_float				m_fTimeOffset = 1.f;


public:
	static CTimer* Create();
	virtual void Free() override;
};

END