#include "..\..\Header\TimerMgr.h"

IMPLEMENT_SINGLETON(TimerMgr)

TimerMgr::TimerMgr()
{
}

TimerMgr::~TimerMgr()
{
	Free();
}

_float TimerMgr::Get_TimeDelta(const wstring pTimerTag) 
{
	Timer* pTimer = Find_Timer(pTimerTag);
	NULL_CHECK_RETURN(pTimer, 0.f);

	return pTimer->Get_TimeDelta();
}

void TimerMgr::Update_TimeDelta(const wstring pTimerTag)
{
	Timer* pTimer = Find_Timer(pTimerTag);
	NULL_CHECK(pTimer);

	pTimer->Update_TimeDelta();
}

HRESULT TimerMgr::Init_Timer(const wstring pTimerTag)
{
	Timer*		pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = Timer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	m_mapTimers.insert({pTimerTag, pTimer});

	return S_OK;
}

Timer * TimerMgr::Find_Timer(const wstring pTimerTag)
{
	auto		iter = m_mapTimers.find(pTimerTag);

	if (iter == m_mapTimers.end())
		return nullptr;
	
	return iter->second;
}

void TimerMgr::Free(void)
{
	for_each(m_mapTimers.begin(), m_mapTimers.end(), CDeleteMap());
	m_mapTimers.clear();
}
