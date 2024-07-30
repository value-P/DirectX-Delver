#include "stdafx.h"
#include "MouseSlot.h"
#include "ItemBase.h"
#include "InventoryBase.h"
#include "QuickSlot.h"
#include "EquipmentSlot.h"
#include "Export_Utility.h" 

MouseSlot::MouseSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

MouseSlot::MouseSlot(const MouseSlot& rhs)
	: Super(rhs)
{
	m_itemVec = rhs.m_itemVec;
}

MouseSlot::~MouseSlot()
{
}

HRESULT MouseSlot::Init_QuickSlot()
{
	m_itemVec.resize(1, nullptr);

	return S_OK;
}

_int MouseSlot::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

void MouseSlot::LateUpdate_Component()
{
}

void MouseSlot::Set_BroughtItem(SLOTTYPE_ID _eSlotType, _uint _iIndex)
{
	m_eSlotType = _eSlotType;
	m_iIndex = _iIndex;
}

void MouseSlot::Item_Slot_Swap()
{
	switch (m_eSlotType)
	{
	case ST_QUICK:
	{
		QuickSlot* quickSlot = dynamic_cast<QuickSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerQuickSlot", ID_DYNAMIC));
		quickSlot->AddItemToSlot(m_iIndex, MoveItemFromSlot());
		m_eSlotType = ST_END;
		break;
	}
	case ST_INVENTORY:
	{
		InventoryBase* inventory = dynamic_cast<InventoryBase*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerInventory", ID_DYNAMIC));
		inventory->AddItem(m_iIndex, MoveItemFromSlot());
		m_eSlotType = ST_END;
		break;
	}
	case ST_EQUIP:
	{
		EquipmentSlot* equipmentSlot = dynamic_cast<EquipmentSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerEquipmentSlot", ID_DYNAMIC));
		equipmentSlot->EquipItem(m_iIndex, MoveItemFromSlot());
		m_eSlotType = ST_END;
		break;
	}
	}
}

bool MouseSlot::AddItem(ItemBase* item)
{
	if (m_itemVec[0] == nullptr)
	{
		m_itemVec[0] = item;

		return true;
	}

	return false;
}

ItemBase* MouseSlot::MoveItemFromSlot()
{
	if (m_itemVec[0])
	{
		ItemBase* outItem = m_itemVec[0];
		m_itemVec[0] = nullptr;
		return outItem;
	}

	return nullptr;
}

bool MouseSlot::IsSlotEmpty()
{
	if (m_itemVec[0] == nullptr)
		return true;

	return false;
}

ItemBase* MouseSlot::RemoveItemFromSlot()
{
	if (m_itemVec[0])
	{
		ItemBase* outItem = m_itemVec[0]->Clone();

		Safe_Release(m_itemVec[0]);
		m_itemVec[0] = nullptr;

		return outItem;
	}

	return nullptr;
}

MouseSlot* MouseSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	MouseSlot* pInstance = new MouseSlot(pGraphicDev);
	if (FAILED(pInstance->Init_QuickSlot()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MouseSlot Create Failed");
		return nullptr;
	}

	return pInstance;
}

Component* MouseSlot::Clone()
{
	return new MouseSlot(*this);
}

void MouseSlot::Free()
{
	for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* src) {
		if (src)
			Safe_Release(src);
		});

	m_itemVec.clear();
	m_itemVec.shrink_to_fit();

	Super::Free();
}
