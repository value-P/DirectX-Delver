#pragma once

#include "Base.h"
#include "Engine_Define.h"

#pragma region BitFlag
// ���� �ʿ������ Ȥ�� ������ ���� ���ܵ�
enum UIFlag
{
	UIFIRST = 1 << 0,
	UISECOND = 1 << 1,
	UITHIRD = 1 << 2,
	UIFORTH = 1 << 3,
	UIFITH = 1 << 4,
	UISIXTH = 1 << 5,
	UISEVENT = 1 << 6,
	UIEIGHT = 1 << 7,
};

#pragma endregion

namespace Engine
{
	class VIBuffer;

	class ENGINE_DLL UIBase : public Base
	{
	protected:
		explicit UIBase(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual ~UIBase();

	public:
		virtual HRESULT Init_UI();
		virtual _int Update_UI(const _float& fTimeDelta);
		virtual void LateUpdate_UI();
		virtual void Render_UI();

	public:
		virtual UIBase* GetParentUI() { return nullptr; };
		//_ubyte GetLayerFlag() { return m_bLayerFlag; }

#pragma region For Bit Flag
	/*public:
		bool AddLayerFlag(UIFlag flag);
		void RemoveLayerFlag(UIFlag flag) { m_bLayerFlag &= ~flag; }
		bool IsLayerOn(UIFlag flag);*/
#pragma endregion

	public:
		virtual VIBuffer* Get_VIBuffer() { return nullptr; }

	protected:
		virtual void OnMouseClicked(_uint index) abstract; // pure�� ����

	protected:
		virtual void Free();

	protected:
		LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
		// ���� �ʿ���� �� ������ Ȥ�� ���� ���ܵ�
		/*
		_bool m_bIsActivated = false;

		// Ȥ�� ���� ���ܵ�
		_ubyte m_bLayerFlag = 0; // ��Ʈ �÷��� �����
		*/
	};
}
