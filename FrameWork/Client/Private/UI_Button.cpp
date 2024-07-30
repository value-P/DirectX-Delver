#include "stdafx.h"
#include "UI_Button.h"
#include "MouseSlot.h"

UI_Button::UI_Button(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_Button::~UI_Button()
{
}

UI_Button* UI_Button::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, UIBUTTON_TYPE _eUIButton_Type, vector<wstring>& _strProtoKeyVec, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_Button* pInstance = new UI_Button(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_eUIButton_Type, _strProtoKeyVec, _fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT UI_Button::Init_UI(UIBUTTON_TYPE _eUIButton_Type, vector<wstring>& _strProtoKeyVec, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

	m_eUIButton_Type = _eUIButton_Type;
	//m_eUITYPE = UT_BUTTON;
	m_strProtoKeyVec = _strProtoKeyVec;

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	if (FAILED(Add_UI()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int UI_Button::Update_UI(const _float& fTimeDelta)
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::Update_UI(fTimeDelta);
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
	return 0;
}

void UI_Button::LateUpdate_UI()
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::LateUpdate_UI();
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
}

void UI_Button::Render_UI()
{
	// �� Ŭ������ ���������� ǥ���ɶ� ������Ʈ ������Ұ� �ִٸ� ���⿡
}

void UI_Button::Update_Icon(const _float& fTimeDelta)
{
}

void UI_Button::LateUpdate_Icon()
{
}

void UI_Button::Render_Icon()
{
	UI::Render_Icon();
}

void UI_Button::OnMouseClicked(_uint index)
{
	// �� Ŭ������ ���������� ǥ���ɶ� Ŭ�������� �Ҹ��� �ִٸ� ���⿡
}

void UI_Button::OnIconClicked(_uint index)
{
}
void UI_Button::OnIconClicking(_uint index)
{
	//MouseSlot* mouseSlot = dynamic_cast<MouseSlot*>(Engine::Get_Component(L"GameObject", L"Mouse", L"Proto_MouseSlot", ID_DYNAMIC));
	//bool bIsMouseSlot_Empty = mouseSlot->IsSlotEmpty();

	if (m_eUIButton_Type == UBT_CLOSE)
	{
		if (Engine::Get_DIMouseState(DIM_LB) & 0x80) // 1) ���� ���콺 Ŭ�� ����!
		{
			// ���콺 ���� ���� ����

			if (m_bMouse_Lock == false)
			{
				m_bMouse_Lock = true;
				Safe_Release(m_pTextureIcon);
				m_pTextureIcon = dynamic_cast<Texture*>(Clone_Proto(m_strProtoKeyVec[1]));
			}
		}

		else if (Engine::Get_DIMouseState(DIM_RB) & 0x80) // 2) ������ ���콺 Ŭ�� ����!
		{
			// ���� ����Դϴ�.
		}

		else // 3) ���콺 Ŭ���� ���� ����!
		{
			// ���콺 ���� ���� ����

			if (m_bMouse_Lock == true)
			{
				m_bMouse_Lock = false;
				m_bIs_ButtonClicked = true;
			}
		}
	}

}

HRESULT UI_Button::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	pComponent = Clone_Proto(m_strProtoKeyVec[0]);
	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureIcon = dynamic_cast<Texture*>(pComponent);

	return S_OK;
}

HRESULT UI_Button::Add_UI()
{
	return S_OK;
}

void UI_Button::Free()
{
	UI::Free();
}
