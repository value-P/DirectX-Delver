#pragma once
#include "Base.h"
#include "Engine_Define.h"

namespace Engine
{
    class ENGINE_DLL RandomMgr :
        public Base
    {
        DECLARE_SINGLETON(RandomMgr)

    private:
        explicit RandomMgr();
        virtual ~RandomMgr();

    public:
        HRESULT		Init_RandomMgr();

    public:
        int        Rand_Num(int iMin, int iMax);
        _float      Rand_Num(_float fMin, _float fMax);

    private:
        random_device   m_RandomDev;

    private:
        virtual void		Free();
    };
}

