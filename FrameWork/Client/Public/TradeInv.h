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
	bool AddItemToTradeInv(ItemBase* item); // �� ���� ���� ��?
	// �������� ������ �����Ϸ��� �� �� �Ʒ� �Լ��� �� ��
	// ���� �̻��ϸ� ���� �θ� ��
	// �� ��° �Ű� ������ �ڵ尡 �ȸ��� �� �÷��̾�� �������� �Ѱܹ޾Ƽ� �ذ��Ϸ���
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
