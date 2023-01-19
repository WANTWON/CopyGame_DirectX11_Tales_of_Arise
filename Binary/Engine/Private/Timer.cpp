#include "..\Public\Timer.h"

CTimer::CTimer()
	: m_fTimeDelta(0.f)
{
	ZeroMemory(&m_CurrentTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
}

HRESULT CTimer::Initialize(void)
{
	QueryPerformanceCounter(&m_CurrentTime);
	QueryPerformanceCounter(&m_OldTime);
	
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void CTimer::Tick(void)
{
	QueryPerformanceCounter(&m_CurrentTime);

	m_fTimeDelta = float(m_CurrentTime.QuadPart - m_OldTime.QuadPart) / m_CpuTick.QuadPart;

	m_OldTime = m_CurrentTime;
}

CTimer * CTimer::Create(void)
{
	CTimer* pInstance = new CTimer();

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CTimer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimer::Free(void)
{
}
