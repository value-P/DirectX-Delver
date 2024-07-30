#include "stdafx.h"
#include "UI_EquipSlot.h"
#include "EquipmentSlot.h"
#include "InventoryBase.h"
#include "MouseSlot.h"
#include "ItemBase.h"

UI_EquipSlot::UI_EquipSlot(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_EquipSlot::~UI_EquipSlot()
{
}

UI_EquipSlot* UI_EquipSlot::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, EQUIPSLOT_ID _eEquipType, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_EquipSlot* pInstance = new UI_EquipSlot(_pGraphicDevice);

	pInstance->Set_EquipType(_eEquipType);

	if (FAILED(pInstance->Init_UI(_fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT UI_EquipSlot::Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);
	//m_eUITYPE = UT_SLOT;

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

_int UI_EquipSlot::Update_UI(const _float& fTimeDelta)
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::Update_UI(fTimeDelta);
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
	return 0;
}

void UI_EquipSlot::LateUpdate_UI()
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::LateUpdate_UI();
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
}

void UI_EquipSlot::Render_UI()
{
	// �� Ŭ������ ���������� ǥ���ɶ� ������Ʈ ������Ұ� �ִٸ� ���⿡
}

void UI_EquipSlot::Update_Icon(const _float& fTimeDelta)
{
}

void UI_EquipSlot::LateUpdate_Icon()
{
}

void UI_EquipSlot::Render_Icon()
{
	UI::Render_Icon();
}

void UI_EquipSlot::OnMouseClicked(_uint index)
{
	UI::OnMouseClicked(index);
}

void UI_EquipSlot::OnIconClicked(_uint index)
{

}

void UI_EquipSlot::OnIconClicking(_uint index)
{
	MouseSlot* mouseSlot = dynamic_cast<MouseSlot*>(Engine::Get_Component(L"GameObject", L"Mouse", L"Proto_MouseSlot", ID_DYNAMIC));
	EquipmentSlot* equipmentSlot = dynamic_cast<EquipmentSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerEquipmentSlot", ID_DYNAMIC));
	ItemBase* clickedItem = nullptr;
	
	bool bIsMouseSlot_Empty = mouseSlot->IsSlotEmpty();

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80) // 1) ���� ���콺 Ŭ�� ����!
	{
		// ���콺 ���� ���� ����
		if (bIsMouseSlot_Empty) // �� ���� ���콺 ������ ����ִٸ�!
		{
			clickedItem = equipmentSlot->UnEquipItem(index); // �÷��̾��� ���â ���� �ش� ������ �ε����� �ش��ϴ� ������ ��ȯ. 

			if (mouseSlot->AddItem(clickedItem)) // �̸� �Űܵ� �κ��丮 �������� ���콺 �������� �ű��.
			{
				mouseSlot->Set_BroughtItem(ST_EQUIP, index); // �̶�, � ���Կ��� ������ �Դ��� ����Ѵ�.
			}
			else // �ű��� ���Ѵٸ�, �̸� �Űܵ� �κ��丮 �������� �ٽ� �κ��丮�� �����Ѵ�.
				equipmentSlot->EquipItem(index, clickedItem);
		}

		else // �� ���� ���콺 ������ ���ִٸ�!
		{
			;
			/*
			equipmentSlot->EquipItem(index, mouseSlot->MoveItemFromSlot()); // �κ��丮 ���Կ� ���콺 ������ �������� �ű��
			mouseSlot->AddItem(clickedItem); // �̸� �Űܵ� �κ��丮 �������� ���콺 �������� �ű��.
			mouseSlot->Item_Slot_Swap(); // ���콺 ������ ������ �� �������� �������� �̵���Ų��.
			*/
		}
	}

	else if (Engine::Get_DIMouseState(DIM_RB) & 0x80) // 2) ������ ���콺 Ŭ�� ������ ����� ����.
	{
		;
	}

	else // 3) ���콺 Ŭ���� ���� ����!
	{
		// ���콺 ���� ���� ����
		if (bIsMouseSlot_Empty) // �� ���� ���콺 ������ ����ִٸ�
			; // �ش� ������ ���� �����츦 ����϶�.
		else // �� ���� ���콺 ������ ���ִٸ�!
		{
			clickedItem = equipmentSlot->UnEquipItem(index); // �÷��̾��� �κ��丮���� �ش� ������ �ε����� �ش��ϴ� ������ ��ȯ. 
			equipmentSlot->EquipItem(index, mouseSlot->MoveItemFromSlot()); // ���콺 ���Կ��� �������� �����Ͽ�, �÷��̾� �κ��丮�� ����.
			mouseSlot->AddItem(clickedItem); // �κ��丮���� ������ ��������, ���콺 ���Կ� �߰�.
			if (clickedItem)
				mouseSlot->Item_Slot_Swap(); // ���� ���콺 ������ �������� �浿 ������ �����۰� ��ȯ�ϰ�, ��ȯ�� ���콺 ������ �������� ���� ��ġ�� �����϶�.
		}
	}
}

HRESULT UI_EquipSlot::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	switch (m_eEquipType) {
	case ES_HEAD:
		pComponent = Clone_Proto(L"Proto_HeadSlot_Texture");
		break;

	case ES_CHEST:
		pComponent = Clone_Proto(L"Proto_ChestSlot_Texture");
		break;

	case ES_PANTS:
		pComponent = Clone_Proto(L"Proto_PantsSlot_Texture");
		break;

	case ES_HANDS:
		pComponent = Clone_Proto(L"Proto_HandsSlot_Texture");
		break;

	case ES_RING:
		pComponent = Clone_Proto(L"Proto_RingSlot_Texture");
		break;

	case ES_AMULET:
		pComponent = Clone_Proto(L"Proto_AmuletSlot_Texture");
		break;
	}

	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureIcon = dynamic_cast<Texture*>(pComponent);

	return S_OK;
}

HRESULT UI_EquipSlot::Add_UI()
{
	return S_OK;
}

void UI_EquipSlot::Free()
{
	UI::Free();
}
