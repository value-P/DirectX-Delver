#pragma once

#include "Component.h"

class ItemBase;

class QuickSlot : public Component
{
	using Super = Component;

protected:
	explicit QuickSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit QuickSlot(const QuickSlot& rhs);
	virtual ~QuickSlot();

public:
	virtual	HRESULT		Init_QuickSlot();
	virtual	_int		Update_Component(const _float& fTimeDelta) override;
	virtual	void		LateUpdate_Component() override;

public:
	const vector<ItemBase*>& GetItemVec() { return m_itemVec; }
	ItemBase* GetActivatedItem() { return m_pActivatedItem; }
	_uint GetHealAmount() { return m_iHealAmount; }
	void ResetHealAmount() { m_iHealAmount = 0; }

public:
	ItemBase* FindItemFromSlot(ItemBase* item);

	// 흠 퀵슬롯에서 옮길 때 개수 줄이는 것도 필요한가??
	bool AddItemToSlot(ItemBase* item);
	bool AddItemToSlot(_uint index, ItemBase* item);

	ItemBase* MoveItemFromSlot(_uint index); // 퀵슬롯에서 내보낼 때

	// Remove 일단 쓰지 말 것

public:
	static	QuickSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	Component* Clone() override;

protected:
	void ActivateItem();

protected:
	virtual void Free() override;

protected:
	vector<ItemBase*>	m_itemVec;
	ItemBase*			m_pActivatedItem{ nullptr };
	_uint				m_iHealAmount = 0;
};
