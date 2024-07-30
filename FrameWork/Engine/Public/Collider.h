#pragma once

#include "Component.h"

namespace Engine
{
	class Transform;

	// 각 객체에 마다 이것을 컴포넌트에 넣고 SetTransform, SetSize, SetColLayer, SetThisPointer 를 설정해 줘야 자동으로 돌아감
	class ENGINE_DLL Collider : public Component
	{
	private:
		explicit Collider(LPDIRECT3DDEVICE9 _pGraphicDevice);
		explicit Collider(const Collider& _rhs);
		virtual ~Collider();
		
	public:
		static Collider* Create(LPDIRECT3DDEVICE9 _pGraphicDevice);
		virtual Component* Clone();

	public:
		HRESULT Init_Collider();

	public:
		virtual	int	Update_Component(const _float& fTimeDelta);		// Transform 수정 시 Collider 의 업데이트 부분도 수정할 것임
		virtual	void LateUpdate_Component();

	public:		// Get Function
		const CINFO& GetCInfo()					{ return m_tInfo; }
		const D3DXVECTOR3& GetInfo(INFO _eType) { return m_tInfo.m_vInfo[_eType]; }
		const D3DXVECTOR3& GetSize()			{ return m_tInfo.m_vSize; }
		const float& GetRadius()				{ return m_tInfo.m_fRadius; }
		const D3DXMATRIX& GetWorld()			{ return m_tInfo.m_matWorld; }
		void* GetThisPointer()					{ return m_tInfo.m_pThis; }
		int	GetDamage()							{ return m_tInfo.m_iDamage; }
		const size_t& GetColSphere()			{ return m_iColSphere; }
		const size_t& GetColOBB()				{ return m_iColOBB; }
		const size_t& GetColType()				{ return m_iColType; }
		const size_t& GetColLayer()				{ return m_tInfo.m_iColLayer; }
		const list<pair<CINFO, D3DXVECTOR3>>& GetColSphere_Info(COLLAYER _eType);
		const list<pair<CINFO, D3DXVECTOR3>>& GetColOBB_Info(COLLAYER _eType);

	public:		// Set Function
		void SetTransform(Transform* _pTransform);	// 각 객체에서 꼭 Collider를 추가한 후 Transform에 대한 포인터를 넣어줘야함 그래야 편해짐
		void SetInfo(INFO _eType, const D3DXVECTOR3& _vInfo);
		void SetSize(const float& _fX, const float& _fY, const float& _fZ);	// 충돌 사이즈가 1,1,1 이 아닌 이상 무조건 각 객체마다 자신이 갖을 사이즈를 넣어 줘야함
		void SetSize(const D3DXVECTOR3& _vSize);							// 충돌 사이즈가 1,1,1 이 아닌 이상 무조건 각 객체마다 자신이 갖을 사이즈를 넣어 줘야함
		void SetColLayer(COLLAYER _eColLayer);
		void SetThisPointer(void* _pThis);
		void SetDamage(const int& _iDamgae);

	public:		// Class Function
		void AddColSphere(const size_t& _iColType, const CINFO& _ColInfo, const D3DXVECTOR3& _vNear);
		void AddColOBB(const size_t& _iColType, const CINFO& _ColInfo, const D3DXVECTOR3& _vNear);
		void AddColType(const size_t& _iColType);
		void ReBuild();				// 임시 함수
		void SettingCInfo();
		void ClearInfo();
		void SelfBuilding();	// Transform 까지 자신이 직접 관리해야 사용할 수 있음
		void SelfBuilding(const D3DXVECTOR3& _vRight, const D3DXVECTOR3& _vUp, const D3DXVECTOR3& _vLook, const D3DXVECTOR3& _vPos);

	private:
		void SettingRadius();

	private:
		Transform* m_pTransform{ nullptr };
		CINFO m_tInfo;
		list<pair<CINFO, D3DXVECTOR3>>	m_lstColSphere_Info[COL_END];			// 어떤 레이어와 구 충돌을 하였는가
		list<pair<CINFO, D3DXVECTOR3>>	m_lstColOBB_Info[COL_END];				// 어떤 레이어와 OBB 충돌을 하였는가
		size_t m_iColSphere{ 0 }, m_iColOBB{ 0 };
		size_t  m_iColType{ 0 };		// ColLayer는 각 객체가 갖는 자신의 COLLAYER 값이고, ColType은 그 객체가 어떤 객체와 충돌이 되었다를 가지고 있음

	private:
		virtual void Free();
	};
}

