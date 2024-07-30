#include "PCharging.h"

Engine::PCharging::PCharging(LPDIRECT3DDEVICE9 _pGraphicDeivce)
    : PSystem(_pGraphicDeivce)
{
}

Engine::PCharging::~PCharging()
{
}

PCharging* Engine::PCharging::Create(LPDIRECT3DDEVICE9 _pGraphicDeivce, const D3DXVECTOR3& _vOrigin)
{
    PCharging* pInstance = new PCharging(_pGraphicDeivce);

	if (FAILED(pInstance->Init_PCharging(_vOrigin)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_PCharging Failed", L"PCharging::Create", MB_OK);
		return nullptr;
	}

    return pInstance;
}

HRESULT Engine::PCharging::Init_PCharging(const D3DXVECTOR3& _vOrigin)
{
    m_fSize = 0.5f;
    m_vbSize = 128;
    m_vbBatchSize = 128;
    m_iMaxParticles = 128;
    m_vOrigin = _vOrigin;
    if (FAILED(PSystem::Init_Particles()))
    {
        return E_FAIL;
    }

    return S_OK;
}

int Engine::PCharging::Update(const float& _fTimeDelta)
{
    list<Attribute>::iterator iter{ m_lstParticles.begin() };

    for (; iter != m_lstParticles.end(); iter++)
    {
        if (iter->bAlive)
        {
            iter->vPos += iter->vVelocity * _fTimeDelta;
            iter->fAge += _fTimeDelta;
            if (iter->fAge > iter->fLifeTime)
            {
                iter->bAlive = false;
            }
            if (D3DXVec3Length(&(m_vOrigin - iter->vPos)) < 0.5f)
            {
                iter->bAlive = false;
            }
        }
    }

    return 0;
}

void Engine::PCharging::LateUpdate()
{
	if ((size_t)m_iMaxParticles < m_lstParticles.size())
    {
		RemoveDeadParticles();
    }
}

void Engine::PCharging::PreRenderParticles()
{
    PSystem::PreRenderParticles();
}

void Engine::PCharging::PostRenderParticles()
{
    PSystem::PostRenderParticles();
}

void Engine::PCharging::ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor, const float& _fSpeed)
{
    _pAttribute->bAlive = true;
    _pAttribute->vPos.x = _vPos.x + GetRandomFloat(-0.4f, 0.4f);
    _pAttribute->vPos.y = _vPos.y + GetRandomFloat(-0.4f, 0.4f);
    _pAttribute->vPos.z = _vPos.z + GetRandomFloat(-0.4f, 0.4f);
	D3DXVec3Normalize(&_pAttribute->vVelocity, &(m_vOrigin - _pAttribute->vPos));
    _pAttribute->vVelocity *= _fSpeed;
    _pAttribute->fLifeTime = 20.0f;
    _pAttribute->fAge = 0.0f;
    _pAttribute->dwColor = _vColor;
    _pAttribute->fSize = 2.0f;
}

void Engine::PCharging::Free()
{
    PSystem::Free();
}
