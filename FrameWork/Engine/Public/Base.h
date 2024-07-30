#pragma once

class _declspec(dllexport) Base
{
protected:
	inline	explicit Base();
	inline virtual ~Base();

public:
	inline	unsigned long	AddRef();
	inline unsigned long	Release();

private:
	unsigned long		m_dwRefCnt;

public:
	inline virtual void		Free()	= 0;
};

#include "Base.inl"