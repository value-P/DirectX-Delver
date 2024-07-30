#include "..\..\Header\Timer.h"

Timer::Timer() : m_fTimeDelta(0.f)
{
}

Timer::~Timer()
{
}

HRESULT Timer::Init_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void Timer::Update_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = float(m_FrameTime.QuadPart - m_LastTime.QuadPart) / m_CpuTick.QuadPart;

	m_LastTime = m_FrameTime;

}

Timer* Timer::Create(void)
{
	Timer*		pInstance = new Timer;

	if (FAILED(pInstance->Init_Timer()))
	{
		MSG_BOX("Timer Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Timer::Free()
{
}
