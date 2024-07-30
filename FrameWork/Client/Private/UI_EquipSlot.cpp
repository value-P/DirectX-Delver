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
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	return 0;
}

void UI_EquipSlot::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void UI_EquipSlot::Render_UI()
{
	// 이 클래스가 아이콘으로 표현될때 업데이트 해줘야할게 있다면 여기에
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

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80) // 1) 왼쪽 마우스 클릭 상태!
	{
		// 마우스 슬롯 상태 구분
		if (bIsMouseSlot_Empty) // ① 현재 마우스 슬롯이 비어있다면!
		{
			clickedItem = equipmentSlot->UnEquipItem(index); // 플레이어의 장비창 에서 해당 슬롯의 인덱스에 해당하는 아이템 반환. 

			if (mouseSlot->AddItem(clickedItem)) // 미리 옮겨둔 인벤토리 아이템을 마우스 슬롯으로 옮긴다.
			{
				mouseSlot->Set_BroughtItem(ST_EQUIP, index); // 이때, 어떤 슬롯에서 가지고 왔는지 기록한다.
			}
			else // 옮기지 못한다면, 미리 옮겨둔 인벤토리 아이템을 다시 인벤토리에 삽입한다.
				equipmentSlot->EquipItem(index, clickedItem);
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
		;
	}

	else // 3) 마우스 클릭이 없는 상태!
	{
		// 마우스 슬롯 상태 구분
		if (bIsMouseSlot_Empty) // ① 현재 마우스 슬롯이 비어있다면
			; // 해당 슬롯의 설명 윈도우를 출력하라.
		else // ② 현재 마우스 슬롯이 차있다면!
		{
			clickedItem = equipmentSlot->UnEquipItem(index); // 플레이어의 인벤토리에서 해당 슬롯의 인덱스에 해당하는 아이템 반환. 
			equipmentSlot->EquipItem(index, mouseSlot->MoveItemFromSlot()); // 마우스 슬롯에서 아이템을 추출하여, 플레이어 인벤토리에 저장.
			mouseSlot->AddItem(clickedItem); // 인벤토리에서 추출한 아이템을, 마우스 슬롯에 추가.
			if (clickedItem)
				mouseSlot->Item_Slot_Swap(); // 현재 마우스 슬롯의 아이템을 충동 슬롯의 아이템과 교환하고, 교환된 마우스 슬롯의 아이템을 이전 위치로 삽입하라.
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
