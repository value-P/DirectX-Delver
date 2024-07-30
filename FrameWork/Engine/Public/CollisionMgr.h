#pragma once

#include "Base.h"
#include "Engine_Define.h"

namespace Engine
{
	class Collider;
	class ColliderFrame;

	class ENGINE_DLL CollisionMgr : public Base
	{
	public:
		static CollisionMgr* GetInstance()
		{
			if (nullptr == m_pInstance)
			{
				m_pInstance = new CollisionMgr();
				m_pInstance->Init_ColMgr();
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
		static CollisionMgr* m_pInstance;

	private:
		explicit CollisionMgr();
		virtual ~CollisionMgr();

	private:
		void Init_ColMgr();

	public:		// 이걸로 계산할 Collider를 넣어줌 내부에서 자체적으로 분류할거임
		void AddCollider(Collider* _pCollider);
		void ComputeCollision();

	public:		// ColliderFrame 을 랜더함
		void RenderColFrame(LPDIRECT3DDEVICE9 _pGraphicDevice);

	public:
		void ClearColMgr();

	private:	// Class Function 이 함수들로 계산함
		bool ComputeColSphere(Collider* _Collider1, Collider* _Collider2, D3DXVECTOR3* _vNear);
		bool ComputeColOBB(Collider* _pCollider1, Collider* _pCollider2, D3DXVECTOR3* _vNear);
		bool ComputeColOBB_Sphere(Collider* _pOBB, Collider* _pSphere, D3DXVECTOR3* _vNear);

	private:
		void ColSettingFunction();

	private:
		list<Collider*> m_lstCollider[COL_END];
		list<pair<Collider*, Collider*>> m_lstOBBCollider;		// OBB 충돌을 비교할 Collider 들
		list<pair<Collider*, Collider*>> m_lstOSCollider;		// OBB와 Sphere 충돌을 비교할 Collider들
		size_t m_ColLayer[COL_END];
		size_t m_OBBOBBColLayer[COL_END];
		size_t m_OBBSPColLayer[COL_END];
		size_t m_SPOBBColLayer[COL_END];
		ColliderFrame* m_pColBuffer{ nullptr };

	private:
		virtual void Free();
	};
}



