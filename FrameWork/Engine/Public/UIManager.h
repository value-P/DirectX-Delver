#pragma once

#include "Engine_Define.h"
#include "UIBase.h"

namespace Engine
{
	// 언제든지 추가 삭제 가능, 이름은 알아서 수정하도록
	enum UITYPE
	{
		HUD, // 항상 화면에 떠 있는 UI ex) HP bar, minimap 같은 거
		MENU, // 포켓몬 도감, 인벤토리 등을 고를 수 있는 메뉴
		POKEDEX, // 도감
		INVENTORY, // 장비, 소비 아이템 등이 들어감
		INVENTORYTAB, // 장비, 소비 아이템 등이 들어가 있는 탭들

		UILAYER_END,
	};

	class ENGINE_DLL UIManager : public Base
	{
		DECLARE_SINGLETON(UIManager)

	private:
		explicit UIManager();
		virtual ~UIManager();

	public:
		HRESULT Init_UI(const wstring& _wstrUITag, UIBase* _pUIBase); // Init을 통해 UI Create
		_int Update_UI(const _float& fTimeDelta, HWND hWnd);
		void LateUpdate_UI();
		void Render_UI(); // enum 순서대로 HUD -> MENU -> ... -> INVENTORYTAB

	public:		// Class Function
		void Activate_UI(UIBase* _pUI); // 현재 UI 활성화
		void Activate_UI(const wstring& _wstrUITag);
		bool Key_Lock_UI(const wstring& _wstrUITag);
		void Return_UI(); // 뒤로가기
		void Clear_UI(); // 전체 UI 종료
		void SelectDelete_UI(const wstring& _wstrUITag);

	public:		// Get Function
		UIBase* GetParentUI();
		UIBase* GetParentUI(const wstring& wstrKey);
		UIBase* GetHUD() { return m_pHUD; }
		UIBase* GetCurUI() { return m_pCurUI; }
		vector<UIBase*>& GetCurUIVec() { return m_vecUI; }
		bool	GetPlayerHittable() { return m_bPlayerHittable; }

	public:		// Set Function
		void	SetPlayerHittable(bool _bHittable);

	private:
		UIBase* Find_UI(const wstring _wstrUITag);

	private:
		void Key_Input(HWND hWnd);

	private:
		virtual void Free();

	private:
		// 실시간으로 활성화된 UI들을 관리할 container
		vector<UIBase*> m_vecUI;
		UIBase* m_pCurUI = nullptr; // 현재 활성화 되어 있는 UI
		UIBase* m_pHUD = nullptr; // 월드 상에 떠 있을 가장 기본적인 UI

		// UI Create 할 때는 이름을 키값으로 저장 (무조건 영어로)
		unordered_map<wstring, UIBase*> m_UImap;

		_matrix m_matOrthoProj;

		bool m_bPlayerHittable{ true };
	};
}
