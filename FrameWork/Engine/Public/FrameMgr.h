#pragma once

#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL FrameMgr : public Base
{
	DECLARE_SINGLETON(FrameMgr)

private:
	explicit FrameMgr();
	virtual ~FrameMgr();

public:
	_bool			IsPermit_Call(const wstring pFrameTag, const _float& fTimeDelta);
	HRESULT			Init_Frame(const wstring pFrameTag, const _float& fCallLimit);
	
private:
	Frame*			Find_Frame(const wstring pFrameTag);

private:
	map<const wstring, Frame*>			m_mapFrame;

private:
	virtual void		Free();
};

END