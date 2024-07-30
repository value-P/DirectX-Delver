#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL Timer : public Base
{
private:
	explicit Timer();
	virtual ~Timer();

public:
	_float			Get_TimeDelta() { return m_fTimeDelta; }

public:
	HRESULT		Init_Timer(void);
	void		Update_TimeDelta(void);

private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;

	_float					m_fTimeDelta;

public:
	static Timer*	Create(void);

private:
	virtual void	Free();
};

END