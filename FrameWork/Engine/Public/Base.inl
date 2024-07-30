#include "Base.h"
Base::Base()
	: m_dwRefCnt(0)
{

}

Base::~Base()
{

}

inline unsigned long Base::AddRef()
{
	return ++m_dwRefCnt;
}

inline unsigned long Base::Release()
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}

	return m_dwRefCnt--;
}

