#pragma once

#include "Component.h"

class ItemBase;
class Player;

class TradeInv : public Component
{
	using Super = Component;

protected:
	explicit TradeInv(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit TradeInv(const TradeInv& rhs);
	virtual ~TradeInv();

public:
	virtual	HRESULT		Init_TradeInv();
	virtual	_int		Update_Component(const _float& fTimeDelta) override;
	virtual	void		LateUpdate_Component() override;

public:
	_int Get_PlayerMoney();
	void Set_PlayerMoney(_int _iPlayerMoney);

public:
	vector<ItemBase*>* Get_ShopItemList() { return &m_itemVec; }
	bool FindItemFromTradeInv(ItemBase* item);
	bool AddItemToTradeInv(ItemBase* item); // 쓸 일이 있을 까?
	// 상점에서 뭔가를 구입하려고 할 때 아래 함수를 쓸 것
	// 뭔가 이상하면 필히 부를 것
	// 두 번째 매개 변수는 코드가 안먹힐 시 플레이어에서 소지금을 넘겨받아서 해결하려고
	ItemBase* MoveItemFromTradeInv(_uint index, _int* iMoney = nullptr);
	ItemBase* MoveItemFromTradeInv(ItemBase* item);

public:
	static	TradeInv* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual	Component* Clone() override;

protected:
	virtual void Free() override;

protected:
	vector<ItemBase*> m_itemVec;

private:
	Player* m_pPlayer = nullptr;
};
