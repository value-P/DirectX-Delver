#include "UIBase.h"

Engine::UIBase::UIBase(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::UIBase::~UIBase()
{

}

HRESULT Engine::UIBase::Init_UI()
{


	return S_OK;
}

_int Engine::UIBase::Update_UI(const _float& fTimeDelta)
{

	return 0;
}

void Engine::UIBase::LateUpdate_UI()
{

}

void Engine::UIBase::Render_UI()
{

}

void Engine::UIBase::Free()
{
	Safe_Release(m_pGraphicDev);
}
