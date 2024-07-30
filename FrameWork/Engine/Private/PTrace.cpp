#include "PTrace.h"

Engine::PTrace::PTrace(LPDIRECT3DDEVICE9 _pGraphicDeivce)
	: PSystem(_pGraphicDeivce)
{
}

Engine::PTrace::~PTrace()
{
}

PTrace* Engine::PTrace::Create(LPDIRECT3DDEVICE9 _pGraphicDeivce)
{
	PTrace* pInstance = new PTrace(_pGraphicDeivce);

	if (FAILED(pInstance->Init_PTrace()))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_PTrace Failed", L"PTrace::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Engine::PTrace::Init_PTrace()
{
	m_fSize = 0.2f;
	m_vbSize = 256;
	m_vbBatchSize = 256;

	if (FAILED(PSystem::Init_Particles()))
	{
		return E_FAIL;
	}

	return S_OK;
}

int Engine::PTrace::Update(const float& _fTimeDelta)
{
	list<Attribute>::iterator iter{ m_lstParticles.begin() };

	for (; iter != m_lstParticles.end(); iter++)
	{
		if (iter->bAlive)
		{
			iter->fAge += _fTimeDelta;
			if (iter->fAge > iter->fLifeTime)
			{
				iter->bAlive = false;
			}
		}
	}

	return 0;
}

void Engine::PTrace::LateUpdate()
{
}

void Engine::PTrace::PreRenderParticles()
{
	PSystem::PreRenderParticles();
}

void Engine::PTrace::PostRenderParticles()
{
	PSystem::PostRenderParticles();
}

void Engine::PTrace::ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor)
{
	_pAttribute->bAlive = true;
	_pAttribute->vPos.x = _vPos.x + GetRandomFloat(-0.4f, 0.4f);
	_pAttribute->vPos.y = _vPos.y + GetRandomFloat(-0.4f, 0.4f);
	_pAttribute->vPos.z = _vPos.z + GetRandomFloat(-0.4f, 0.4f);
	_pAttribute->fLifeTime = 2.0f;
	_pAttribute->fAge = 0.0f;
	_pAttribute->dwColor = _vColor;
	_pAttribute->fSize = 1.0f;
}

void Engine::PTrace::Free()
{
	PSystem::Free();
}
