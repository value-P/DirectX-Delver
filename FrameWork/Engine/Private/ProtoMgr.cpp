#include "..\..\Header\ProtoMgr.h"

IMPLEMENT_SINGLETON(ProtoMgr)

ProtoMgr::ProtoMgr()
{
}

ProtoMgr::~ProtoMgr()
{
	Free();
}

HRESULT ProtoMgr::Init_Proto(const wstring pProtoTag, Component * pComponent)
{
	Component*		pPrototype = Find_Prototype(pProtoTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapProto.insert({ pProtoTag, pComponent });

	return S_OK;
}

Component * ProtoMgr::Clone_Proto(const wstring pProtoTag)
{
	Component*	pPrototype = Find_Prototype(pProtoTag);

	NULL_CHECK_RETURN(pPrototype, nullptr);

	return pPrototype->Clone();
}

Component * ProtoMgr::Find_Prototype(const wstring pProtoTag)
{
	auto	iter = m_mapProto.find(pProtoTag);

	if (iter == m_mapProto.end())
		return nullptr;

	return iter->second;
}

void ProtoMgr::Free()
{
	for_each(m_mapProto.begin(), m_mapProto.end(), CDeleteMap());
	m_mapProto.clear();
}
