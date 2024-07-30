#include "KeyMgr.h"

IMPLEMENT_SINGLETON(KeyMgr)

Engine::KeyMgr::KeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}

Engine::KeyMgr::~KeyMgr()
{
}

bool Engine::KeyMgr::Key_Down(int _iKey)
{
	if ((!m_bKeyState[_iKey]) && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}
/*
bool Engine::KeyMgr::Mouse_Pressing()
{
	//if (WM_LBUTTONDOWN && )
	//return false;

}
*/
void Engine::KeyMgr::Free()
{
}
