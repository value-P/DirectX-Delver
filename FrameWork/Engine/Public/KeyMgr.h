#pragma once
#include "Base.h"
#include "Engine_Define.h"

namespace Engine {

	class ENGINE_DLL KeyMgr : public Base
	{
		DECLARE_SINGLETON(KeyMgr)

	private:
		explicit KeyMgr();
		virtual ~KeyMgr();

	public:
		bool Key_Down(int _iKey);
		//bool Mouse_Pressing();

	private:
		bool m_bKeyState[VK_MAX];

	public:
		virtual	void Free();
	};


}