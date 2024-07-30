#pragma once

#include "Component.h"

namespace Engine
{
	class Transform;

	// �� ��ü�� ���� �̰��� ������Ʈ�� �ְ� SetTransform, SetSize, SetColLayer, SetThisPointer �� ������ ��� �ڵ����� ���ư�
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
		virtual	int	Update_Component(const _float& fTimeDelta);		// Transform ���� �� Collider �� ������Ʈ �κе� ������ ����
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
		void SetTransform(Transform* _pTransform);	// �� ��ü���� �� Collider�� �߰��� �� Transform�� ���� �����͸� �־������ �׷��� ������
		void SetInfo(INFO _eType, const D3DXVECTOR3& _vInfo);
		void SetSize(const float& _fX, const float& _fY, const float& _fZ);	// �浹 ����� 1,1,1 �� �ƴ� �̻� ������ �� ��ü���� �ڽ��� ���� ����� �־� �����
		void SetSize(const D3DXVECTOR3& _vSize);							// �浹 ����� 1,1,1 �� �ƴ� �̻� ������ �� ��ü���� �ڽ��� ���� ����� �־� �����
		void SetColLayer(COLLAYER _eColLayer);
		void SetThisPointer(void* _pThis);
		void SetDamage(const int& _iDamgae);

	public:		// Class Function
		void AddColSphere(const size_t& _iColType, const CINFO& _ColInfo, const D3DXVECTOR3& _vNear);
		void AddColOBB(const size_t& _iColType, const CINFO& _ColInfo, const D3DXVECTOR3& _vNear);
		void AddColType(const size_t& _iColType);
		void ReBuild();				// �ӽ� �Լ�
		void SettingCInfo();
		void ClearInfo();
		void SelfBuilding();	// Transform ���� �ڽ��� ���� �����ؾ� ����� �� ����
		void SelfBuilding(const D3DXVECTOR3& _vRight, const D3DXVECTOR3& _vUp, const D3DXVECTOR3& _vLook, const D3DXVECTOR3& _vPos);

	private:
		void SettingRadius();

	private:
		Transform* m_pTransform{ nullptr };
		CINFO m_tInfo;
		list<pair<CINFO, D3DXVECTOR3>>	m_lstColSphere_Info[COL_END];			// � ���̾�� �� �浹�� �Ͽ��°�
		list<pair<CINFO, D3DXVECTOR3>>	m_lstColOBB_Info[COL_END];				// � ���̾�� OBB �浹�� �Ͽ��°�
		size_t m_iColSphere{ 0 }, m_iColOBB{ 0 };
		size_t  m_iColType{ 0 };		// ColLayer�� �� ��ü�� ���� �ڽ��� COLLAYER ���̰�, ColType�� �� ��ü�� � ��ü�� �浹�� �Ǿ��ٸ� ������ ����

	private:
		virtual void Free();
	};
}

