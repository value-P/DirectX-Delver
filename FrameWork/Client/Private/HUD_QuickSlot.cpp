#include "stdafx.h"
#include "HUD_QuickSlot.h"

#include "QuickSlot.h"
#include "ItemBase.h"
#include "InventoryBase.h"
#include "MouseSlot.h"
#include "EquipmentSlot.h"

HUD_QuickSlot::HUD_QuickSlot(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

HUD_QuickSlot::~HUD_QuickSlot()
{
}

HUD_QuickSlot* HUD_QuickSlot::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	HUD_QuickSlot* pInstance = new HUD_QuickSlot(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT HUD_QuickSlot::Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
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

_int HUD_QuickSlot::Update_UI(const _float& fTimeDelta)
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::Update_UI(fTimeDelta);
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
	return 0;
}

void HUD_QuickSlot::LateUpdate_UI()
{
	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::LateUpdate_UI();
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
}

void HUD_QuickSlot::Render_UI()
{
	// �� Ŭ������ ���������� ǥ���ɶ� ������Ʈ ������Ұ� �ִٸ� ���⿡
}

void HUD_QuickSlot::Update_Icon(const _float& fTimeDelta)
{
	
}

void HUD_QuickSlot::LateUpdate_Icon()
{
}

void HUD_QuickSlot::Render_Icon()
{
	UI::Render_Icon();
}

void HUD_QuickSlot::OnMouseClicked(_uint index)
{
	// �� Ŭ������ �θ�μ� ǥ���ɶ� Ŭ�������� �Ҹ��� �ִٸ� ���⿡
	
}

void HUD_QuickSlot::OnIconClicked(_uint index)
{
	// �� Ŭ������ ���������� ǥ���ɶ� Ŭ�������� �Ҹ��� �ִٸ� ���⿡
	//Engine::Activate_UI(this);	// �ڱ��ڽ��� �������μ� UIMgr�� m_CurUI�� �ڽ��� ��
	/*
	QuickSlot* quickSlot = dynamic_cast<QuickSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerQuickSlot", ID_DYNAMIC));

	ItemBase* clickedItem = quickSlot->MoveItemFromSlot(index);
	if (clickedItem)
	{
		// ItemType�� ���� ó���Ұ� ���߿�... TODO
		InventoryBase* inventory = dynamic_cast<InventoryBase*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerInventory", ID_DYNAMIC));
		if (inventory)
		{
			if (inventory->AddItem(clickedItem))
			{

			}
			else
				quickSlot->AddItemToSlot(index, clickedItem);
		}
	}
	*/
}

void HUD_QuickSlot::OnIconClicking(_uint index)
{
	MouseSlot* mouseSlot = dynamic_cast<MouseSlot*>(Engine::Get_Component(L"GameObject", L"Mouse", L"Proto_MouseSlot", ID_DYNAMIC));
	QuickSlot* quickSlot = dynamic_cast<QuickSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerQuickSlot", ID_DYNAMIC));
	ItemBase* clickedItem = nullptr;

	bool bIsMouseSlot_Empty = mouseSlot->IsSlotEmpty();

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80) // 1) ���� ���콺 Ŭ�� ����!
	{
		// ���콺 ���� ���� ����
		if (bIsMouseSlot_Empty) // �� ���� ���콺 ������ ����ִٸ�!
		{
			clickedItem = quickSlot->MoveItemFromSlot(index); // �÷��̾��� ���â ���� �ش� ������ �ε����� �ش��ϴ� ������ ��ȯ. 

			if (mouseSlot->AddItem(clickedItem)) // �̸� �Űܵ� �κ��丮 �������� ���콺 �������� �ű��.
			{
				mouseSlot->Set_BroughtItem(ST_QUICK, index); // �̶�, � ���Կ��� ������ �Դ��� ����Ѵ�.
			}
			else // �ű��� ���Ѵٸ�, �̸� �Űܵ� �κ��丮 �������� �ٽ� �κ��丮�� �����Ѵ�.
				quickSlot->AddItemToSlot(index, clickedItem);
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
		// ���콺 ���� ���� ����
		if (bIsMouseSlot_Empty) // �� ���� ���콺 ������ ����ִٸ�!
		{
			
				//clickedItem = quickSlot->MoveItemFromSlot(index); // �÷��̾��� �κ��丮���� �ش� ������ �ε����� �ش��ϴ� ������ ��ȯ. 
				//if(clickedItem)
				//{
				//	if (clickedItem->Get_ItemType() >= ITEM_HEAD && clickedItem->Get_ItemType() <= ITEM_RING) // ���⸦ ������ ��� ���������� Ȯ���Ѵ�.
				//	{	// ��� â�� �����Ѵ�.
				//		EquipmentSlot* equipmentSlot = dynamic_cast<EquipmentSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerEquipmentSlot", ID_DYNAMIC));
				//		// ��� â ���Կ� ������ Ÿ�԰� �µ��� �����Ѵ�.
				//		if (equipmentSlot->EquipItem(clickedItem->Get_ItemType(), clickedItem))
				//		{
				//			;
				//		}
				//		// �������� ���ߴٸ�, �ش� �������� �ٽ� �κ��丮�� �̵���Ų��.
				//		else
				//			quickSlot->AddItemToSlot(index, clickedItem);
				//	}

				//	else // ������ Ÿ���� ��� â ���� Ÿ���� �ƴ϶�� �ش� �������� ����Ѵ�.
				//	{
				//		;
				//	}
				//}
			;
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
			clickedItem = quickSlot->MoveItemFromSlot(index); // �÷��̾��� �κ��丮���� �ش� ������ �ε����� �ش��ϴ� ������ ��ȯ. 
			quickSlot->AddItemToSlot(index, mouseSlot->MoveItemFromSlot()); // ���콺 ���Կ��� �������� �����Ͽ�, �÷��̾� �κ��丮�� ����.
			mouseSlot->AddItem(clickedItem); // �κ��丮���� ������ ��������, ���콺 ���Կ� �߰�.
			if (clickedItem)
				mouseSlot->Item_Slot_Swap(); // ���� ���콺 ������ �������� �浿 ������ �����۰� ��ȯ�ϰ�, ��ȯ�� ���콺 ������ �������� ���� ��ġ�� �����϶�.
		}
	}


}

HRESULT HUD_QuickSlot::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	pComponent = Clone_Proto(L"Proto_ItemSlot_Texture");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureIcon = dynamic_cast<Texture*>(pComponent);

	return S_OK;
}

HRESULT HUD_QuickSlot::Add_UI()
{
	return S_OK;
}

void HUD_QuickSlot::Free()
{
	UI::Free();
}
