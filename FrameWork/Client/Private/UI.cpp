#include "stdafx.h"
#include "UI.h"

UI::UI(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UIBase(_pGraphicDevice), m_pParentUI(this)
{
	D3DXMatrixIdentity(&m_matIcon);
	D3DXMatrixIdentity(&m_matBG);
}

UI::~UI()
{
}

HRESULT UI::Init_UI(const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	m_fX = _fX;
	m_fY = _fY;
	m_fSizeX = _fSizeX;
	m_fSizeY = _fSizeY;
	SettingUI();
	return S_OK;
}

_int UI::Update_UI(const _float& fTimeDelta)
{
	// 무언가 UI 업데이트를 해줘야 할게 있으면 생성한 클래스에서 해주면됨
	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [&](UI* Temp)->void {Temp->Update_Icon(fTimeDelta); });
	return 0;
}

void UI::LateUpdate_UI()
{
	// 무언가 UI 업데이트를 해줘야 할게 있으면 생성한 클래스에서 해주면됨
	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [](UI* Temp)->void {Temp->LateUpdate_Icon(); });
}

void UI::Render_UI()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matBG);
	m_pTextureBG->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [](UI* Temp)->void {Temp->Render_Icon(); });
}

void UI::Render_Icon()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matIcon);

	if (m_pTextureIcon != nullptr)
	{
		m_pTextureIcon->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
	
	if (m_pTextureItem != nullptr)
	{
		m_pTextureItem->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
}

void UI::OnMouseClicked(_uint index)
{
	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [index](UI* Temp)->void { Temp->OnIconClicked(index); });
}

void UI::Add_ChildUI_Group(vector<UI*>* _vecChildUI)
{
	m_vecChildUI_Group.push_back(_vecChildUI);
}

void UI::Add_ChildUI(UI* _pUI)
{
	m_vecChildUI.push_back(_pUI);
	_pUI->m_pParentUI = this;
}

void UI::SetState_Button(_int _iIndex, bool _bActive_Button)
{
	m_vecChildUI[_iIndex]->m_bActive_Button = _bActive_Button;
}



void UI::SettingUI()
{
	m_matIcon._11 = m_fSizeX * 0.5f;
	m_matIcon._22 = m_fSizeY * 0.5f;
	m_matIcon._41 = m_fX - float(WINCX >> 1) + m_fSizeX * 0.5f;
	m_matIcon._42 = -m_fY + float(WINCY >> 1) - m_fSizeY * 0.5f;
	m_matIcon._43 = 1.f;
	// 특별한 경우가 아니면 배경은 전체화면에 띄우기 때문에 기본값으로 넣어줌 ( 화면 정중앙 )
	m_matBG._11 =  400.0f;
	m_matBG._22 = 300.0f;
}

_int UI::Check_State_Button()
{
	_int iFind_Index = 0;
	vector<UI*>::iterator iter;
	for (iter = m_vecChildUI.begin(); iter != m_vecChildUI.end(); ++iter)
	{
		if ((*iter)->GetState_Button() == true)
			return iFind_Index;
		++iFind_Index;
	}
	if (iter == m_vecChildUI.end())
		return -1;
}

void UI::Free()
{
	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [](UI* Temp)->void {Safe_Release(Temp); });
	m_vecChildUI.clear();

	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureIcon);

	Safe_Release(m_pTextureBG);
	UIBase::Free();
}
