#include "..\..\Header\Frame.h"

Frame::Frame() : m_fCallLimit(0.f), m_fAccTimeDelta(0.f)
{
}

Frame::~Frame()
{
}

_bool Frame::IsPermit_Call(const _float & fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta;

	if (m_fAccTimeDelta >= m_fCallLimit)
	{
		m_fAccTimeDelta = 0.f;
		return true;
	}

	return false;
}

HRESULT Frame::Init_Frame(const _float & fCallLimit)
{
	m_fCallLimit = 1.f / fCallLimit;

	return S_OK;
}

Frame * Frame::Create(const _float & fCallLimit)
{
	Frame *		pInstance = new Frame;

	if (FAILED(pInstance->Init_Frame(fCallLimit)))
	{
		MSG_BOX("Frame Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Frame::Free()
{
}
