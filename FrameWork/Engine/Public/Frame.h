#pragma once
#include "Base.h"

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL Frame : public Base
{
private:
	explicit Frame();
	virtual ~Frame();

public:
	_bool		IsPermit_Call(const _float& fTimeDelta);

public:
	HRESULT		Init_Frame(const _float& fCallLimit);

private:
	_float				m_fCallLimit;
	_float				m_fAccTimeDelta;

public:
	static Frame*		Create(const _float& fCallLimit);

private:
	virtual void	Free();
};

END