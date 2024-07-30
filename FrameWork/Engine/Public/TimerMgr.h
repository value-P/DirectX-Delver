#pragma once
#include "Base.h"
#include "Timer.h"

BEGIN(Engine)

class ENGINE_DLL TimerMgr : public Base
{

	DECLARE_SINGLETON(TimerMgr)
private:
	explicit TimerMgr();
	virtual ~TimerMgr();

public:
	_float		Get_TimeDelta(const wstring pTimerTag);
	void		Update_TimeDelta(const wstring pTimerTag);

public:
	HRESULT		Init_Timer(const wstring pTimerTag);

private:
	Timer*		Find_Timer(const wstring pTimerTag);

private:
	map<const wstring, Timer*>		m_mapTimers;

private:
	virtual void		Free(void);
};

END