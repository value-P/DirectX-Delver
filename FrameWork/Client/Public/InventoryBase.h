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
	// 순서 교체가 필요할 경우, 필요한가? 필요하다~ 그런가???
	void SwapItem(ItemBase** src, ItemBase** dst);

	// 인벤토리 안에 이미 아이템이 있는지 확인(ID를 통해 인벤토리 안에 중복된 아이템이 있는가?), 개수 등 늘리기 위해서(현재까지는)
	ItemBase* FindItem(ItemBase* item);
	_int FindIndexOfItem(ItemBase* item);

	bool AddItem(ItemBase* item); // 인벤토리에 아이템 같은 게 추가 될 경우 호출
	bool AddItem(_uint index, ItemBase* item); // 인덱스로 추가

	ItemBase* MoveItem(ItemBase* item);
	ItemBase* MoveItem(_uint index); // 아이템 옮길 때

	// 아이템을 사용할 경우 필요한데, 아래 두 함수 수정 필요
	_int SubtractItem(_uint index); // 장착, 사용, 버리기 등 모든 상황에서 먼저 아이템의 개수를 줄이려고 시도
	_int SubtractItem(ItemBase* item); // Overloading for TEST

	bool RemoveItem(_uint index); // 아이템의 개수가 1보다 작을 경우 아이템의 삭제
	bool RemoveItem(ItemBase* item); // Overloading for TEST

public:
	static	InventoryBase* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	Component* Clone() override;

protected:
	virtual void Free() override;

protected:
	// 기본적인 공간, 각 탭 마다 크기가 다르므로 따로 들고 있어야 하나?
	// 문제1 : 겹치는 소비아이템 같은 경우에는 어떻게 할 것인가?

	// 임의접근 및 검색에 용이, 순회에는 그다지 좋지 않음, Swap하려면 Icon에서 키값의 변경이 아마 필요
	//unordered_map<wstring, ItemBase*> m_itemMap;

	// Swap 할 때 매우 편함(Icon에 연결된 주소만 교체), 순회에는 그다지 좋지 않음, 검색할 때 순회 필요, 임의 접근 불가능
	//list<ItemBase*> m_itemList;

	// 임의접근 가능, 순회는 나머지보다 나음, 검색하려면 순회해야함
	vector<ItemBase*> m_itemVec;

	// Temp Code, 추후 수정하거나 삭제 예정
	_uint iExtra;
};
