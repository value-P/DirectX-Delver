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

	// �� �����Կ��� �ű� �� ���� ���̴� �͵� �ʿ��Ѱ�??
	bool AddItemToSlot(ItemBase* item);
	bool AddItemToSlot(_uint index, ItemBase* item);

	ItemBase* MoveItemFromSlot(_uint index); // �����Կ��� ������ ��

	// Remove �ϴ� ���� �� ��

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
