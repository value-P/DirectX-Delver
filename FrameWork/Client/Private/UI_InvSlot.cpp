#include "stdafx.h"

#include "UI_InvSlot.h"
#include "InventoryBase.h"
#include "QuickSlot.h"
#include "MouseSlot.h"
#include "ItemBase.h"
#include "EquipmentSlot.h"

UI_InvSlot::UI_InvSlot(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_InvSlot::~UI_InvSlot()
{
}

UI_InvSlot* UI_InvSlot::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_InvSlot* pInstance = new UI_InvSlot(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT UI_InvSlot::Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
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

_int UI_InvSlot::Update_UI(const _float& fTimeDelta)
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::Update_UI(fTimeDelta);
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
	return 0;
}

void UI_InvSlot::LateUpdate_UI()
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::LateUpdate_UI();
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
}

void UI_InvSlot::Render_UI()
{
	// �� Ŭ������ ���������� ǥ���ɶ� ������Ʈ ������Ұ� �ִٸ� ���⿡
}

void UI_InvSlot::Update_Icon(const _float& fTimeDelta)
{
}

void UI_InvSlot::LateUpdate_Icon()
{
}

void UI_InvSlot::Render_Icon()
{
	UI::Render_Icon();
}

void UI_InvSlot::OnMouseClicked(_uint index)
{
	// �� Ŭ������ �θ�μ� ǥ���ɶ� Ŭ�������� �Ҹ��� �ִٸ� ���⿡
	UI::OnMouseClicked(index);
}

void UI_InvSlot::OnIconClicked(_uint index)
{
	;
}

void UI_InvSlot::OnIconClicking(_uint index)
{
	MouseSlot* mouseSlot = dynamic_cast<MouseSlot*>(Engine::Get_Component(L"GameObject", L"Mouse", L"Proto_MouseSlot", ID_DYNAMIC));
	InventoryBase* inventory = dynamic_cast<InventoryBase*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerInventory", ID_DYNAMIC));
	ItemBase* clickedItem = nullptr;
	
	bool bIsMouseSlot_Empty = mouseSlot->IsSlotEmpty();

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80) // 1) ���� ���콺 Ŭ�� ����!
	{
		// ���콺 ���� ���� ����
		if (bIsMouseSlot_Empty) // �� ���� ���콺 ������ ����ִٸ�!
		{
			clickedItem = inventory->MoveItem(index); // �÷��̾��� �κ��丮���� �ش� ������ �ε����� �ش��ϴ� ������ ��ȯ. 
			if (clickedItem)
			{
				if (mouseSlot->AddItem(clickedItem)) // �̸� �Űܵ� �κ��丮 �������� ���콺 �������� �ű��.
				{
					mouseSlot->Set_BroughtItem(ST_INVENTORY, index); // �̶�, � ���Կ��� ������ �Դ��� ����Ѵ�.
				}
				else // �ű��� ���Ѵٸ�, �̸� �Űܵ� �κ��丮 �������� �ٽ� �κ��丮�� �����Ѵ�.
					inventory->AddItem(index, clickedItem);
			}
		}

		else // �� ���� ���콺 ������ ���ִٸ�!
		{
			;
			/*
			inventory->AddItem(index, mouseSlot->MoveItemFromSlot()); // �κ��丮 ���Կ� ���콺 ������ �������� �ű��
			mouseSlot->AddItem(clickedItem); // �̸� �Űܵ� �κ��丮 �������� ���콺 �������� �ű��.
			mouseSlot->Item_Slot_Swap(); // ���콺 ������ ������ �� �������� �������� �̵���Ų��.
			*/
		}
	}

	else if (Engine::Get_DIMouseState(DIM_RB) & 0x80) // 2) ������ ���콺 Ŭ�� ����!
	{
		// ���콺 ���� ���� ����
		if (bIsMouseSlot_Empty) // �� ���� ���콺 ������ ����ִٸ�!
		{
			
				clickedItem = inventory->MoveItem(index); // �÷��̾��� �κ��丮���� �ش� ������ �ε����� �ش��ϴ� ������ ��ȯ. 
				if (clickedItem)
				{
					if (clickedItem->Get_ItemType() >= ITEM_HEAD && clickedItem->Get_ItemType() <= ITEM_NECKLACE) // ���⸦ ������ ��� ���������� Ȯ���Ѵ�.
					{	// ��� â�� �����Ѵ�.
						EquipmentSlot* equipmentSlot = dynamic_cast<EquipmentSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerEquipmentSlot", ID_DYNAMIC));
						// ��� â ���Կ� ������ Ÿ�԰� �µ��� �����Ѵ�.
						if (equipmentSlot->EquipItem(clickedItem->Get_ItemType(), clickedItem))
						{
							;
						}
						// �������� ���ߴٸ�, �ش� �������� �ٽ� �κ��丮�� �̵���Ų��.
						else
							inventory->AddItem(index, clickedItem);
					}

					else // ������ Ÿ���� ��� â ���� Ÿ���� �ƴ϶�� �ش� �������� ����Ѵ�.
					{
						inventory->AddItem(index, clickedItem);
					}
				}
			
		}
		else // �� ���� ���콺 ������ ���ִٸ�!
			; // LB ��ư ���콺 �巡�� ���̴�, ����� �������� ����!
	}

	else // 3) ���콺 Ŭ���� ���� ����!
	{
		// ���콺 ���� ���� ����
		if (bIsMouseSlot_Empty) // �� ���� ���콺 ������ ����ִٸ�
			; // �ش� ������ ���� �����츦 ����϶�.
		else // �� ���� ���콺 ������ ���ִٸ�!
		{
			// if (mouseSlot->Get_SlotType() == ST_EQUIP)
			// if (mouseSlot->Get_ItemIndex() == clickedItem->Get_ItemType())

			clickedItem = inventory->MoveItem(index); // �÷��̾��� �κ��丮���� �ش� ������ �ε����� �ش��ϴ� ������ ��ȯ. 
			if (clickedItem != nullptr)
			{
				if (mouseSlot->Get_SlotType() == ST_EQUIP) // ���콺 ������ �������� ��� â ���Կ��� ������ �Դµ�,
					if (mouseSlot->Get_ItemIndex() != clickedItem->Get_ItemType()) // ��ü�Ϸ��� �������� ������ ������ �ƴ϶��
					{
						mouseSlot->Item_Slot_Swap();
						inventory->AddItem(index, clickedItem);
						return;
					}
			}
			
			inventory->AddItem(index, mouseSlot->MoveItemFromSlot()); // ���콺 ���Կ��� �������� �����Ͽ�, �÷��̾� �κ��丮�� ����.
			mouseSlot->AddItem(clickedItem); // �κ��丮���� ������ ��������, ���콺 ���Կ� �߰�.

			if (clickedItem)
				mouseSlot->Item_Slot_Swap(); // ���� ���콺 ������ �������� �浹 ������ �����۰� ��ȯ�ϰ�, ��ȯ�� ���콺 ������ �������� ���� ��ġ�� �����϶�.
		}
	}
}

HRESULT UI_InvSlot::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	pComponent = Clone_Proto(L"Proto_ItemSlot_Texture");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureIcon = dynamic_cast<Texture*>(pComponent);

	return S_OK;
}

HRESULT UI_InvSlot::Add_UI()
{
	return S_OK;
}

void UI_InvSlot::Free()
{
	UI::Free();
}
