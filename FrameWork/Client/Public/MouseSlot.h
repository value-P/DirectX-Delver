#pragma once

#include "Component.h"

class ItemBase;

class MouseSlot : public Component
{
	using Super = Component;

protected:
	explicit MouseSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit MouseSlot(const MouseSlot& rhs);
	virtual ~MouseSlot();

public:
	virtual	HRESULT		Init_QuickSlot();
	virtual	_int		Update_Component(const _float& fTimeDelta) override;
	virtual	void		LateUpdate_Component() override;

public: // Get
	const vector<ItemBase*>& GetItemVec() { return m_itemVec; }
	SLOTTYPE_ID Get_SlotType() { return m_eSlotType; }
	_uint Get_ItemIndex() { return m_iIndex; }

public:
	void Set_BroughtItem(SLOTTYPE_ID _eSlotType, _uint _iIndex);

public:
	void Item_Slot_Swap();
	bool AddItem(ItemBase* item);
	ItemBase* MoveItemFromSlot();
	bool IsSlotEmpty();

	// Need To Test, 아이템 버리기
	ItemBase* RemoveItemFromSlot();

public:
	static	MouseSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	Component* Clone() override;

protected:
	virtual void Free() override;

protected:
	vector<ItemBase*> m_itemVec;

private:
	SLOTTYPE_ID m_eSlotType = ST_END;
	_uint m_iIndex;

};
