#include "RandomMgr.h"

IMPLEMENT_SINGLETON(RandomMgr)

Engine::RandomMgr::RandomMgr()
{
}

Engine::RandomMgr::~RandomMgr()
{
    Free();
}

HRESULT Engine::RandomMgr::Init_RandomMgr()
{
    return S_OK;
}

int Engine::RandomMgr::Rand_Num(int iMin, int iMax)
{
    mt19937 gen(m_RandomDev());
    uniform_int_distribution<int> dis(iMin,iMax);

    int iResult = dis(gen);

    return iResult;
}

_float Engine::RandomMgr::Rand_Num(_float fMin, _float fMax)
{
    mt19937 gen(m_RandomDev());
    uniform_real_distribution<float> dis(fMin, fMax);

    _float iResult = dis(gen);

    return iResult;
}

void Engine::RandomMgr::Free()
{
}
