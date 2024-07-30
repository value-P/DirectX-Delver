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
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	return 0;
}

void HUD_QuickSlot::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void HUD_QuickSlot::Render_UI()
{
	// 이 클래스가 아이콘으로 표현될때 업데이트 해줘야할게 있다면 여기에
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
	// 이 클래스가 부모로서 표현될때 클릭했을때 불릴게 있다면 여기에
	
}

void HUD_QuickSlot::OnIconClicked(_uint index)
{
	// 이 클래스가 아이콘으로 표현될때 클릭했을때 불릴게 있다면 여기에
	//Engine::Activate_UI(this);	// 자기자신을 넣음으로서 UIMgr의 m_CurUI가 자신이 됨
	/*
	QuickSlot* quickSlot = dynamic_cast<QuickSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerQuickSlot", ID_DYNAMIC));

	ItemBase* clickedItem = quickSlot->MoveItemFromSlot(index);
	if (clickedItem)
	{
		// ItemType에 따라 처리할거 나중에... TODO
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

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80) // 1) 왼쪽 마우스 클릭 상태!
	{
		// 마우스 슬롯 상태 구분
		if (bIsMouseSlot_Empty) // ① 현재 마우스 슬롯이 비어있다면!
		{
			clickedItem = quickSlot->MoveItemFromSlot(index); // 플레이어의 장비창 에서 해당 슬롯의 인덱스에 해당하는 아이템 반환. 

			if (mouseSlot->AddItem(clickedItem)) // 미리 옮겨둔 인벤토리 아이템을 마우스 슬롯으로 옮긴다.
			{
				mouseSlot->Set_BroughtItem(ST_QUICK, index); // 이때, 어떤 슬롯에서 가지고 왔는지 기록한다.
			}
			else // 옮기지 못한다면, 미리 옮겨둔 인벤토리 아이템을 다시 인벤토리에 삽입한다.
				quickSlot->AddItemToSlot(index, clickedItem);
		}

		else // ② 현재 마우스 슬롯이 차있다면!
		{
			;
			/*
			equipmentSlot->EquipItem(index, mouseSlot->MoveItemFromSlot()); // 인벤토리 슬롯에 마우스 슬롯의 아이템을 옮기고
			mouseSlot->AddItem(clickedItem); // 미리 옮겨둔 인벤토리 아이템을 마우스 슬롯으로 옮긴다.
			mouseSlot->Item_Slot_Swap(); // 마우스 슬롯이 가지고 온 슬롯으로 아이템을 이동시킨다.
			*/
		}
	}

	else if (Engine::Get_DIMouseState(DIM_RB) & 0x80) // 2) 오른쪽 마우스 클릭 상태은 기능이 없다.
	{
		// 마우스 슬롯 상태 구분
		if (bIsMouseSlot_Empty) // ① 현재 마우스 슬롯이 비어있다면!
		{
			
				//clickedItem = quickSlot->MoveItemFromSlot(index); // 플레이어의 인벤토리에서 해당 슬롯의 인덱스에 해당하는 아이템 반환. 
				//if(clickedItem)
				//{
				//	if (clickedItem->Get_ItemType() >= ITEM_HEAD && clickedItem->Get_ItemType() <= ITEM_RING) // 무기를 제외한 장비 아이템인지 확인한다.
				//	{	// 장비 창을 개방한다.
				//		EquipmentSlot* equipmentSlot = dynamic_cast<EquipmentSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerEquipmentSlot", ID_DYNAMIC));
				//		// 장비 창 슬롯에 아이템 타입과 맞도록 삽입한다.
				//		if (equipmentSlot->EquipItem(clickedItem->Get_ItemType(), clickedItem))
				//		{
				//			;
				//		}
				//		// 삽입하지 못했다면, 해당 아이템을 다시 인벤토리로 이동시킨다.
				//		else
				//			quickSlot->AddItemToSlot(index, clickedItem);
				//	}

				//	else // 아이템 타입이 장비 창 슬롯 타입이 아니라면 해당 아이템을 사용한다.
				//	{
				//		;
				//	}
				//}
			;
		}
		else // ② 현재 마우스 슬롯이 차있다면!
			; // LB 버튼 마우스 드래그 중이니, 기능을 수행하지 마라!
	}

	else // 3) 마우스 클릭이 없는 상태!
	{
		// 마우스 슬롯 상태 구분
		if (bIsMouseSlot_Empty) // ① 현재 마우스 슬롯이 비어있다면
			; // 해당 슬롯의 설명 윈도우를 출력하라.
		else // ② 현재 마우스 슬롯이 차있다면!
		{
			clickedItem = quickSlot->MoveItemFromSlot(index); // 플레이어의 인벤토리에서 해당 슬롯의 인덱스에 해당하는 아이템 반환. 
			quickSlot->AddItemToSlot(index, mouseSlot->MoveItemFromSlot()); // 마우스 슬롯에서 아이템을 추출하여, 플레이어 인벤토리에 저장.
			mouseSlot->AddItem(clickedItem); // 인벤토리에서 추출한 아이템을, 마우스 슬롯에 추가.
			if (clickedItem)
				mouseSlot->Item_Slot_Swap(); // 현재 마우스 슬롯의 아이템을 충동 슬롯의 아이템과 교환하고, 교환된 마우스 슬롯의 아이템을 이전 위치로 삽입하라.
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
