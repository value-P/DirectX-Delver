#pragma once

#include "Component.h"

class ItemBase;

class InventoryBase : public Component
{
	using Super = Component;

protected:
	explicit InventoryBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit InventoryBase(const InventoryBase& rhs);
	virtual ~InventoryBase();

public:
	virtual	HRESULT		Init_Inventory();
	virtual	_int		Update_Component(const _float& fTimeDelta) override;
	virtual	void		LateUpdate_Component() override;

public: // Get, Set
	const vector<ItemBase*>& GetItemVec() { return m_itemVec; }

public:
	// ���� ��ü�� �ʿ��� ���, �ʿ��Ѱ�? �ʿ��ϴ�~ �׷���???
	void SwapItem(ItemBase** src, ItemBase** dst);

	// �κ��丮 �ȿ� �̹� �������� �ִ��� Ȯ��(ID�� ���� �κ��丮 �ȿ� �ߺ��� �������� �ִ°�?), ���� �� �ø��� ���ؼ�(���������)
	ItemBase* FindItem(ItemBase* item);
	_int FindIndexOfItem(ItemBase* item);

	bool AddItem(ItemBase* item); // �κ��丮�� ������ ���� �� �߰� �� ��� ȣ��
	bool AddItem(_uint index, ItemBase* item); // �ε����� �߰�

	ItemBase* MoveItem(ItemBase* item);
	ItemBase* MoveItem(_uint index); // ������ �ű� ��

	// �������� ����� ��� �ʿ��ѵ�, �Ʒ� �� �Լ� ���� �ʿ�
	_int SubtractItem(_uint index); // ����, ���, ������ �� ��� ��Ȳ���� ���� �������� ������ ���̷��� �õ�
	_int SubtractItem(ItemBase* item); // Overloading for TEST

	bool RemoveItem(_uint index); // �������� ������ 1���� ���� ��� �������� ����
	bool RemoveItem(ItemBase* item); // Overloading for TEST

public:
	static	InventoryBase* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	Component* Clone() override;

protected:
	virtual void Free() override;

protected:
	// �⺻���� ����, �� �� ���� ũ�Ⱑ �ٸ��Ƿ� ���� ��� �־�� �ϳ�?
	// ����1 : ��ġ�� �Һ������ ���� ��쿡�� ��� �� ���ΰ�?

	// �������� �� �˻��� ����, ��ȸ���� �״��� ���� ����, Swap�Ϸ��� Icon���� Ű���� ������ �Ƹ� �ʿ�
	//unordered_map<wstring, ItemBase*> m_itemMap;

	// Swap �� �� �ſ� ����(Icon�� ����� �ּҸ� ��ü), ��ȸ���� �״��� ���� ����, �˻��� �� ��ȸ �ʿ�, ���� ���� �Ұ���
	//list<ItemBase*> m_itemList;

	// �������� ����, ��ȸ�� ���������� ����, �˻��Ϸ��� ��ȸ�ؾ���
	vector<ItemBase*> m_itemVec;

	// Temp Code, ���� �����ϰų� ���� ����
	_uint iExtra;
};
