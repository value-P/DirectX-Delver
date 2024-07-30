#include "..\..\Header\Component.h"

Component::Component()
	: m_pGraphicDev(nullptr), m_bClone(false)
{
}

Component::Component(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_bClone(false)
{
	m_pGraphicDev->AddRef();
}

Component::Component(const Component& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_bClone(true)
{
	m_pGraphicDev->AddRef();
}

Component::~Component()
{
}

void Component::Free()
{
	Safe_Release(m_pGraphicDev);
}
