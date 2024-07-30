#include "UIManager.h"
#include "Renderer.h"
#include "RcTex.h"
#include "Ray.h"
#include "RayPickingMgr.h"

IMPLEMENT_SINGLETON(UIManager)

Engine::UIManager::UIManager()
{

}

Engine::UIManager::~UIManager()
{
	Free();
}

HRESULT Engine::UIManager::Init_UI(const wstring& _wstrUITag, UIBase* _pUIBase)
{
	UIBase* pUIBase = Find_UI(_wstrUITag);
	// 기존 포켓몬 UI의 경우 같은 Key값을 같은 UI더라도 호출 순서가 다르면 중복 생성을 허용했다.
	// 하지만 델버의 경우 이를 막아야한다. 중복 생성 방지!
	//if (nullptr != pUIBase) { return E_FAIL; }
	if (nullptr != pUIBase)
	{
		_pUIBase->Release();
		return E_FAIL;
	}
	if (nullptr != _pUIBase) { m_UImap.insert({ _wstrUITag, _pUIBase }); }
	if (nullptr == m_pHUD) { m_pHUD = _pUIBase; }	// 처음 넣어주는 UI를 HUD로 쓰겠다 아니면 따로 함수 만들어서 직접 넣어줘도 됨

	return S_OK;
}

_int Engine::UIManager::Update_UI(const _float& fTimeDelta, HWND hWnd)
{
	Key_Input(hWnd);

	if (m_pCurUI)
	{
		if (m_pCurUI != m_pHUD)
			m_pHUD->Update_UI(fTimeDelta);

		m_pCurUI->Update_UI(fTimeDelta);
	}

	return 0;
}

void Engine::UIManager::LateUpdate_UI()
{
	if (m_pCurUI)
	{
		if (m_pCurUI != m_pHUD)
			m_pHUD->LateUpdate_UI();

		m_pCurUI->LateUpdate_UI();
	}
}

void Engine::UIManager::Render_UI()
{
	/*
	if (m_pHUD)
	{
		m_pHUD->Render_UI();
	}
	*/
	if (m_pCurUI)
	{
		if (m_pCurUI != m_pHUD)
			m_pHUD->Render_UI();

		m_pCurUI->Render_UI();
		
	}
	
}

void Engine::UIManager::Activate_UI(UIBase* _pUI)
{
	if (nullptr != _pUI)
	{
		m_pCurUI = _pUI;
		m_vecUI.push_back(m_pCurUI);
	}
}

// [수정] 오버로딩 : 키값으로 UI 활성화 및 같은 UI 다중 활성화 방지.
void Engine::UIManager::Activate_UI(const wstring& _wstrUITag)
{
	UIBase* pUI = Find_UI(_wstrUITag);
	if (nullptr != pUI)
	{
		if(m_vecUI.back() != pUI)
		{
			m_pCurUI = pUI;
			m_vecUI.push_back(m_pCurUI);
		}
	}
}

bool Engine::UIManager::Key_Lock_UI(const wstring& _wstrUITag)
{
	UIBase* pUI = Find_UI(_wstrUITag);
	if (nullptr != pUI)
	{
		if (m_pCurUI != pUI)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void Engine::UIManager::Return_UI()
{
	m_vecUI.pop_back();

	if (!m_vecUI.empty())
	{
		m_pCurUI = m_vecUI.back();
	}
	else
	{
		m_pCurUI = nullptr;
	}
}

void Engine::UIManager::Clear_UI()
{
	m_pCurUI = nullptr;
	m_vecUI.clear();
}

void Engine::UIManager::SelectDelete_UI(const wstring& _wstrUITag)
{
	UIBase* pUIBase = Find_UI(_wstrUITag);
	m_UImap.erase(_wstrUITag);
	Safe_Release(pUIBase);

	return;
}

UIBase* Engine::UIManager::GetParentUI()
{
	return m_pCurUI->GetParentUI();
}

UIBase* Engine::UIManager::GetParentUI(const wstring& wstrKey)
{
	unordered_map<wstring, UIBase*>::iterator iter = find_if(m_UImap.begin(), m_UImap.end(),
		[&](pair<wstring, UIBase*> Temp)->bool {return wstrKey == Temp.first; });
	
	if (m_UImap.end() == iter)
	{
		return nullptr;
	}

	return (*iter).second->GetParentUI();
}

void Engine::UIManager::SetPlayerHittable(bool _bHittable)
{
	m_bPlayerHittable = _bHittable;
}

UIBase* Engine::UIManager::Find_UI(const wstring _wstrUITag)
{
	unordered_map<wstring, UIBase*>::iterator iter = m_UImap.find(_wstrUITag);

	if (m_UImap.end() == iter)
	{
		return nullptr;
	}

	return (*iter).second;
}

void Engine::UIManager::Key_Input(HWND hWnd)
{
	
}

void Engine::UIManager::Free()
{
	for_each(m_UImap.begin(), m_UImap.end(), [](pair<wstring, UIBase*> Temp)->void
		{
			if (nullptr != Temp.second)
			{
				Temp.second->Release();
				Temp.second = nullptr;
			}
		});
	m_UImap.clear();
	m_pCurUI = nullptr;
	m_pHUD = nullptr;
}
