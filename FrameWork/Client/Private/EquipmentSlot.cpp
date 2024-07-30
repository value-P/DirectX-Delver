#include "stdafx.h"
#include "EquipmentSlot.h"
#include "ItemBase.h"
#include "InventoryBase.h"

#include "Export_Utility.h"

EquipmentSlot::EquipmentSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{

}

EquipmentSlot::EquipmentSlot(const EquipmentSlot& rhs)
	: Super(rhs), m_itemVec(rhs.m_itemVec)
{

}

EquipmentSlot::~EquipmentSlot()
{

}

HRESULT EquipmentSlot::Init_EquipmentSlot()
{
	m_itemVec.resize(6, nullptr);

	return S_OK;
}

_int EquipmentSlot::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

void EquipmentSlot::LateUpdate_Component()
{
}

bool EquipmentSlot::EquipItem(_uint slotIdx, ItemBase* equipment)
{
	if (slotIdx != static_cast<_uint>(equipment->Get_ItemType()))
		return false;

	if (m_itemVec[slotIdx] == nullptr)
	{
		m_itemVec[slotIdx] = equipment;

		return true;
	}

	ItemBase* outItem = m_itemVec[slotIdx];
	m_itemVec[slotIdx] = equipment;

	InventoryBase* inventory = dynamic_cast<InventoryBase*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerInventory", ID_DYNAMIC));
	const vector<ItemBase*> itemVec = inventory->GetItemVec();
	for (size_t i = 0; i < itemVec.size(); ++i)
	{
		if (itemVec[i] == nullptr)
		{
			inventory->AddItem(i, outItem);

			return true;
		}
	}

	m_itemVec[slotIdx] = outItem;

	return false;
}

ItemBase* EquipmentSlot::UnEquipItem(_uint slotIdx)
{
	if (m_itemVec[slotIdx] == nullptr)
		return false;

	ItemBase* outItem = m_itemVec[slotIdx];
	m_itemVec[slotIdx] = nullptr;

	return outItem;
}

EquipmentSlot* EquipmentSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	EquipmentSlot* pInstance = new EquipmentSlot(pGraphicDev);
	if (FAILED(pInstance->Init_EquipmentSlot()))
	{
		Safe_Release(pInstance);

		MSG_BOX("EquipmentSlot Created Failed");
		return nullptr;
	}

	return pInstance;
}

Component* EquipmentSlot::Clone()
{
	return new EquipmentSlot(*this);
}

void EquipmentSlot::Free()
{
	for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* src){
		if (src)
		{
			Safe_Release(src);
		}
		});

	m_itemVec.clear();
	m_itemVec.shrink_to_fit();

	Super::Free();
}
