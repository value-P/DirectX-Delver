#pragma once

#include "Base.h"
#include "Engine_Define.h"

namespace Engine
{
    class PSystem;

    class ENGINE_DLL PSMgr : public Base
    {
    public:
        static PSMgr* GetInstance()
        {
			if (nullptr == m_pInstance)
            {
                m_pInstance = new PSMgr();
            }
            return m_pInstance;
        }
        static void DestroyInstance()
        {
            if (nullptr != m_pInstance)
            {
                delete m_pInstance;
                m_pInstance = nullptr;
            }
        }
    private:
        static PSMgr* m_pInstance;
        
    private:
        explicit PSMgr();
        virtual ~PSMgr();

    public:
        void UpdatePSMgr(const float& _fTimeDelta);
        void LateUpdatePSMgr();
		void RenderPSMgr(LPDIRECT3DDEVICE9 _pGraphicDevice);
        
    public:     // 생성하고 꼭 넣어줄 것
        void AddParticles(PSystem* _pParticles);

    public:
        void PSystem_Clear();

    private:
        list<PSystem*> m_lstPSystem;

    private:
        virtual void Free();
    };
}


