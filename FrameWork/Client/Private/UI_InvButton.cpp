#include "stdafx.h"
#include "UI_InvButton.h"

UI_InvButton::UI_InvButton(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_InvButton::~UI_InvButton()
{
}

UI_InvButton* UI_InvButton::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, InvButton_ID _eInvButtonType, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_InvButton* pInstance = new UI_InvButton(_pGraphicDevice);

	pInstance->Set_InvButtonType(_eInvButtonType);

	if (FAILED(pInstance->Init_UI(_fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT UI_InvButton::Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

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

_int UI_InvButton::Update_UI(const _float& fTimeDelta)
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	return 0;
}

void UI_InvButton::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void UI_InvButton::Render_UI()
{
	// 이 클래스가 아이콘으로 표현될때 업데이트 해줘야할게 있다면 여기에
}

void UI_InvButton::Update_Icon(const _float& fTimeDelta)
{
}

void UI_InvButton::LateUpdate_Icon()
{
}

void UI_InvButton::Render_Icon()
{
	UI::Render_Icon();
}

void UI_InvButton::OnIconClicking(_uint index)
{
}

void UI_InvButton::OnMouseClicked(_uint index)
{
	// 이 클래스가 부모로서 표현될때 클릭했을때 불릴게 있다면 여기에
	UI::OnMouseClicked(index);
}

void UI_InvButton::OnIconClicked(_uint index)
{
	// 이 클래스가 아이콘으로 표현될때 클릭했을때 불릴게 있다면 여기에
	//Engine::Activate_UI(this);	// 자기자신을 넣음으로서 UIMgr의 m_CurUI가 자신이 됨
}

HRESULT UI_InvButton::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	switch (m_eInvButtonType) {
	case IB_INV_ACTIV:
		pComponent = Clone_Proto(L"Proto_Inventory_Activate_Texture");
		break;

	case IB_INV_INACTIV:
		pComponent = Clone_Proto(L"Proto_Inventory_Inactivate_Texture");
		break;

	case IB_STAT_ACTIV:
		pComponent = Clone_Proto(L"Proto_Status_Activate_Texture");
		break;

	case IB_STAT_INACTIV:
		pComponent = Clone_Proto(L"Proto_Status_Inactivate_Texture");
		break;
	}

	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureIcon = dynamic_cast<Texture*>(pComponent);

	return S_OK;
}

HRESULT UI_InvButton::Add_UI()
{
	return S_OK;
}

void UI_InvButton::Free()
{
	UI::Free();
}
