#pragma once

#include "Component.h"

class ItemBase;

class EquipmentSlot : public Component
{
	using Super = Component;

protected:
	explicit EquipmentSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit EquipmentSlot(const EquipmentSlot& rhs);
	virtual ~EquipmentSlot();

public:
	virtual	HRESULT		Init_EquipmentSlot();
	virtual	_int		Update_Component(const _float& fTimeDelta) override;
	virtual	void		LateUpdate_Component() override;

public:
	bool IsActivated() { return m_bIsActivated; }
	void SetIsActivated(bool bIsActivated) { m_bIsActivated = bIsActivated; }

	ItemBase* GetLeftHandItem() { return m_itemVec[static_cast<_uint>(ITEM_LEFTHAND)]; }

	const vector<ItemBase*>& GetItemVec() { return m_itemVec; }

public:
	bool EquipItem(_uint slotIdx, ItemBase* equipment);
	ItemBase* UnEquipItem(_uint slotIdx);

public:
	static	EquipmentSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	Component* Clone() override;

protected:
	virtual void Free() override;

protected:
	vector<ItemBase*> m_itemVec;

	_bool m_bIsActivated = false;
};
