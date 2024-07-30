#include "PSpread.h"

Engine::PSpread::PSpread(LPDIRECT3DDEVICE9 _pGraphicDeivce)
    : PSystem(_pGraphicDeivce)
{
}

Engine::PSpread::~PSpread()
{
}

PSpread* Engine::PSpread::Create(LPDIRECT3DDEVICE9 _pGraphicDeivce)
{
    PSpread* pInstance = new PSpread(_pGraphicDeivce);

    if (FAILED(pInstance->Init_PSpread()))
    {
        Safe_Release(pInstance);
        MessageBox(nullptr, L"Init_PSpread Failed", L"PSpread::Create", MB_OK);
        return nullptr;
    }

    return pInstance;
}

HRESULT Engine::PSpread::Init_PSpread()
{
    m_fSize = 0.2f;
    m_vbSize = 64;
    m_vbBatchSize = 64;

    if (FAILED(PSystem::Init_Particles()))
    {
        return E_FAIL;
    }

    return S_OK;
}

int Engine::PSpread::Update(const float& _fTimeDelta)
{
    list<Attribute>::iterator iter{ m_lstParticles.begin() };

    for (; iter != m_lstParticles.end(); iter++)
    {
        if (iter->bAlive)
        {
            iter->vPos += iter->vVelocity * _fTimeDelta;
			iter->vVelocity += iter->vAccel * _fTimeDelta;
            iter->fAge += _fTimeDelta;
            if (iter->fAge > iter->fLifeTime)
            {
                iter->bAlive = false;
            }
        }
    }

    return 0;
}

void Engine::PSpread::LateUpdate()
{
}

void Engine::PSpread::PreRenderParticles()
{
    PSystem::PreRenderParticles();
}

void Engine::PSpread::PostRenderParticles()
{
    PSystem::PostRenderParticles();
}

void Engine::PSpread::ResetParticle(Attribute* _pAttribute, const D3DXVECTOR3& _vPos, const D3DCOLOR& _vColor)
{
    _pAttribute->bAlive = true;
    _pAttribute->vPos = _vPos;
	D3DXVECTOR3 vMin{ -1.0f,0.0f,-1.0f }, vMax{ 1.0f,0.5f,1.0f };
    GetRandomVector(&_pAttribute->vVelocity, &vMin, &vMax);
    D3DXVec3Normalize(&_pAttribute->vVelocity, &_pAttribute->vVelocity);
    _pAttribute->vVelocity *= 2.0f;
	_pAttribute->vAccel = { -_pAttribute->vVelocity.x * 0.2f,-2.5f,-_pAttribute->vVelocity.z * 0.2f };
    _pAttribute->fLifeTime = 3.0f;
    _pAttribute->fAge = 0.0f;
    _pAttribute->dwColor = _vColor;
    _pAttribute->fSize = 2.0f;
}

void Engine::PSpread::Free()
{
    PSystem::Free();
}
