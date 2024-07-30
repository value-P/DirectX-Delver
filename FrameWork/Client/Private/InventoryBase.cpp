#include "stdafx.h"
#include "InventoryBase.h"
#include "ItemBase.h"

InventoryBase::InventoryBase(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev), iExtra(0)
{
	
}

InventoryBase::InventoryBase(const InventoryBase& rhs)
	: Super(rhs), iExtra(rhs.iExtra)
{
	m_itemVec = rhs.m_itemVec; // 흠 이게 된다고 하던데 말이지
}

InventoryBase::~InventoryBase()
{
	
}

HRESULT InventoryBase::Init_Inventory()
{
	m_itemVec.resize(18, nullptr);

	return S_OK;
}

Engine::_int InventoryBase::Update_Component(const _float& fTimeDelta)
{
	

	return 0;
}

void InventoryBase::LateUpdate_Component()
{
	
}

void InventoryBase::SwapItem(ItemBase** src, ItemBase** dst)
{
	ItemBase* temp;
	temp = *src;

	*src = *dst;
	*dst = temp;
}

_int InventoryBase::FindIndexOfItem(ItemBase* item)
{
	for (size_t i = 0; i < m_itemVec.size(); ++i)
	{
		if (m_itemVec[i]->Get_ItemID() == item->Get_ItemID())
		{
			return i;
		}
	}

	return -1;
}

bool InventoryBase::AddItem(ItemBase* item)
{
	// Stackable일 경우 따로 처리 필요

	if (item == nullptr)
		return false;

	for (_uint i = 0; i < m_itemVec.size(); ++i)
	{
		if (m_itemVec[i] == nullptr)
		{
			m_itemVec[i] = item;

			return true;
		}
	}

	return false;
}

bool InventoryBase::AddItem(_uint index, ItemBase* item)
{
	if (item == nullptr)
		return false;

	// vector로 만들 경우
	if (m_itemVec[index] == nullptr)
	{
		m_itemVec[index] = item;

		return true;
	}

	if (m_itemVec[index]->Get_ItemID() == item->Get_ItemID())
	{
		if (m_itemVec[index]->IsStackable())
		{
			m_itemVec[index]->Increase_ItemCount();

			return true;
		}

		return false;
	}

	return false;
}

ItemBase* InventoryBase::MoveItem(ItemBase* item)
{
	ItemBase* found = FindItem(item);
	if (found != nullptr)
	{
		int index = FindIndexOfItem(found);

		if (SubtractItem(found) == -1)
			return nullptr;

		else if (SubtractItem(found) == 0)
		{
			m_itemVec[index] = nullptr;

			return found;
		}
		else
		{
			return found;
		}
	}

	return nullptr;
}

ItemBase* InventoryBase::MoveItem(_uint index)
{
	if (m_itemVec[index] == nullptr)
		return nullptr;

	if (m_itemVec[index]->IsStackable())
	{
		// 추후 따로 처리
	}

	ItemBase* itemOut = m_itemVec[index];
	m_itemVec[index] = nullptr;

	return itemOut;
}

_int InventoryBase::SubtractItem(_uint index)
{
	ItemBase* temp = m_itemVec[index];

	if (temp == nullptr)
		return -1;

	if (temp->Decrease_ItemCount()) // Subtracted ItemCount Successfully
	{
		return temp->Get_ItemCount();
	}

	return 0;

	// ItemCount below 1, need to RemoveItem
	/*if (RemoveItem(index))
		return 0;

	return -1;*/
}

_int InventoryBase::SubtractItem(ItemBase* item)
{
	ItemBase* tempItem = FindItem(item);

	if (tempItem == nullptr)
		return -1;

	if (tempItem->Decrease_ItemCount()) // Subtracted ItemCount Successfully
	{
		return tempItem->Get_ItemCount();
	}

	return 0;

	// ItemCount below 1, need to RemoveItem
	/*if (RemoveItem(tempItem))
		return 0;*/
}

bool InventoryBase::RemoveItem(_uint index)
{
	auto it = find(m_itemVec.begin(), m_itemVec.end(), m_itemVec[index]);
	if (it != m_itemVec.end())
	{
		Safe_Release(m_itemVec[index]);
		m_itemVec[index] = nullptr;
		m_itemVec.erase(it);

		return true;
	}

	return false;
}

bool InventoryBase::RemoveItem(ItemBase* item)
{
	auto it = find_if(m_itemVec.begin(), m_itemVec.end(), [item](ItemBase* temp)->bool {
		if (item->Get_ItemID() == temp->Get_ItemID())
			return true;

		return false;
		});

	if (it != m_itemVec.end()) // 아이템을 찾았다~
	{
		Safe_Release(*it);
		*it = nullptr;
		m_itemVec.erase(it);

		return true;
	}

	return false;
}

ItemBase* InventoryBase::FindItem(ItemBase* item)
{
	auto it = find_if(m_itemVec.begin(), m_itemVec.end(), [item](ItemBase* temp)->bool {
		if (item->Get_ItemID() == temp->Get_ItemID())
			return true;

		return false;
		});

	if (it != m_itemVec.end()) // 아이템을 찾았다~
	{
		return *it;
	}

	return nullptr;
}

InventoryBase* InventoryBase::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	InventoryBase* pInstance = new InventoryBase(pGraphicDev);

	if (FAILED(pInstance->Init_Inventory()))
	{
		Safe_Release(pInstance);

		MSG_BOX("InventoryBase Create Failed");
		return nullptr;
	}

	return pInstance;
}

Engine::Component* InventoryBase::Clone()
{
	return new InventoryBase(*this);
}

void InventoryBase::Free()
{
	for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* src) {
		if (src)
			Safe_Release(src);
		});

	Super::Free();
}
