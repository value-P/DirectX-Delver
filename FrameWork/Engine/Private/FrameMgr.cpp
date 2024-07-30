#include "..\..\Header\FrameMgr.h"

IMPLEMENT_SINGLETON(FrameMgr)


FrameMgr::FrameMgr()
{
}


FrameMgr::~FrameMgr()
{
	Free();
}

_bool FrameMgr::IsPermit_Call(const wstring pFrameTag, const _float & fTimeDelta)
{
	Frame*		pFrame = Find_Frame(pFrameTag);
	NULL_CHECK_RETURN(pFrame, false);

	return pFrame->IsPermit_Call(fTimeDelta);
}

HRESULT FrameMgr::Init_Frame(const wstring pFrameTag, const _float & fCallLimit)
{
	Frame*		pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = Frame::Create(fCallLimit);
	NULL_CHECK_RETURN(pFrame, E_FAIL);

	m_mapFrame.insert({ pFrameTag, pFrame });

	return S_OK;
}

Frame * FrameMgr::Find_Frame(const wstring pFrameTag)
{
	auto	iter = m_mapFrame.find(pFrameTag);

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

void FrameMgr::Free()
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
	m_mapFrame.clear();
}
