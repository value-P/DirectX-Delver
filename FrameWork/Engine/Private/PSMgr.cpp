#include "PSMgr.h"
#include "PSystem.h"
#include "PTrace.h"

PSMgr* PSMgr::m_pInstance{ nullptr };

Engine::PSMgr::PSMgr()
	: Base()
{
}

Engine::PSMgr::~PSMgr()
{
	Free();
}

void Engine::PSMgr::UpdatePSMgr(const float& _fTimeDelta)
{
	list<PSystem*>::iterator iter = m_lstPSystem.begin();
	for (; iter != m_lstPSystem.end();)
	{
		if ((*iter)->IsDead())
		{
			Safe_Release(*iter);
			iter = m_lstPSystem.erase(iter);
		}
		else
		{
			(*iter)->Update(_fTimeDelta);
			iter++;
		}
	}
}

void Engine::PSMgr::LateUpdatePSMgr()
{
	list<PSystem*>::iterator iter{ m_lstPSystem.begin() };
	for (; iter != m_lstPSystem.end(); iter++)
	{
		(*iter)->LateUpdate();
	}
}

void Engine::PSMgr::RenderPSMgr(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	list<PSystem*>::iterator iter{ m_lstPSystem.begin() };
	for (; iter != m_lstPSystem.end(); iter++)
	{
		(*iter)->RenderParticles();
	}
}

void Engine::PSMgr::AddParticles(PSystem* _pParticles)
{
	if (_pParticles)
	{
		m_lstPSystem.push_back(_pParticles);
	}
}

void Engine::PSMgr::PSystem_Clear()
{
	list<PSystem*>::iterator iter{ m_lstPSystem.begin() };
	while (iter != m_lstPSystem.end())
	{
		Safe_Release(*iter);
		iter = m_lstPSystem.erase(iter);
	}
}

void Engine::PSMgr::Free()
{
	PSystem_Clear();
}
