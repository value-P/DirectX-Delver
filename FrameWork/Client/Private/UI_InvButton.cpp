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
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::Update_UI(fTimeDelta);
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
	return 0;
}

void UI_InvButton::LateUpdate_UI()
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::LateUpdate_UI();
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
}

void UI_InvButton::Render_UI()
{
	// �� Ŭ������ ���������� ǥ���ɶ� ������Ʈ ������Ұ� �ִٸ� ���⿡
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
	// �� Ŭ������ �θ�μ� ǥ���ɶ� Ŭ�������� �Ҹ��� �ִٸ� ���⿡
	UI::OnMouseClicked(index);
}

void UI_InvButton::OnIconClicked(_uint index)
{
	// �� Ŭ������ ���������� ǥ���ɶ� Ŭ�������� �Ҹ��� �ִٸ� ���⿡
	//Engine::Activate_UI(this);	// �ڱ��ڽ��� �������μ� UIMgr�� m_CurUI�� �ڽ��� ��
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
