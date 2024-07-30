#pragma once

#include "Base.h"
#include "Component.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL ProtoMgr : public Base
{
	DECLARE_SINGLETON(ProtoMgr)

private:
	explicit ProtoMgr();
	virtual ~ProtoMgr();

public:
	HRESULT			Init_Proto(const wstring pProtoTag, Component* pComponent);
	Component*		Clone_Proto(const wstring pProtoTag);

private:
	Component*		Find_Prototype(const wstring pProtoTag);

private:
	map<const wstring, Component*>		m_mapProto;

public:
	virtual void	Free();
};

END