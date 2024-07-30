#pragma once

#include "Engine_Define.h"
#include "UIBase.h"

namespace Engine
{
	// �������� �߰� ���� ����, �̸��� �˾Ƽ� �����ϵ���
	enum UITYPE
	{
		HUD, // �׻� ȭ�鿡 �� �ִ� UI ex) HP bar, minimap ���� ��
		MENU, // ���ϸ� ����, �κ��丮 ���� �� �� �ִ� �޴�
		POKEDEX, // ����
		INVENTORY, // ���, �Һ� ������ ���� ��
		INVENTORYTAB, // ���, �Һ� ������ ���� �� �ִ� �ǵ�

		UILAYER_END,
	};

	class ENGINE_DLL UIManager : public Base
	{
		DECLARE_SINGLETON(UIManager)

	private:
		explicit UIManager();
		virtual ~UIManager();

	public:
		HRESULT Init_UI(const wstring& _wstrUITag, UIBase* _pUIBase); // Init�� ���� UI Create
		_int Update_UI(const _float& fTimeDelta, HWND hWnd);
		void LateUpdate_UI();
		void Render_UI(); // enum ������� HUD -> MENU -> ... -> INVENTORYTAB

	public:		// Class Function
		void Activate_UI(UIBase* _pUI); // ���� UI Ȱ��ȭ
		void Activate_UI(const wstring& _wstrUITag);
		bool Key_Lock_UI(const wstring& _wstrUITag);
		void Return_UI(); // �ڷΰ���
		void Clear_UI(); // ��ü UI ����
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
		// �ǽð����� Ȱ��ȭ�� UI���� ������ container
		vector<UIBase*> m_vecUI;
		UIBase* m_pCurUI = nullptr; // ���� Ȱ��ȭ �Ǿ� �ִ� UI
		UIBase* m_pHUD = nullptr; // ���� �� �� ���� ���� �⺻���� UI

		// UI Create �� ���� �̸��� Ű������ ���� (������ �����)
		unordered_map<wstring, UIBase*> m_UImap;

		_matrix m_matOrthoProj;

		bool m_bPlayerHittable{ true };
	};
}
