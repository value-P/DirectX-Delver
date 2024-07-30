#pragma once

#include "Base.h"
#include "Engine_Define.h"

#pragma region BitFlag
// 현재 필요없지만 혹시 나중을 위해 남겨둠
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
		virtual void OnMouseClicked(_uint index) abstract; // pure와 동일

	protected:
		virtual void Free();

	protected:
		LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
		// 딱히 필요없을 거 같지만 혹시 몰라 남겨둠
		/*
		_bool m_bIsActivated = false;

		// 혹시 몰라 남겨둠
		_ubyte m_bLayerFlag = 0; // 비트 플래그 연산용
		*/
	};
}
