#include "stdafx.h"
#include "TradeInv.h"
#include "ItemBase.h"
#include "Player.h"

#include "Export_Utility.h"

TradeInv::TradeInv(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{

}

TradeInv::TradeInv(const TradeInv& rhs)
	: Super(rhs)
{

}

TradeInv::~TradeInv()
{

}

HRESULT TradeInv::Init_TradeInv()
{
	

	return S_OK;
}

_int TradeInv::Update_Component(const _float& fTimeDelta)
{
	m_itemVec.reserve(10);
	vector<GameObject*>* gameObject = Management::GetInstance()->Get_GameObject(L"GameObject", L"Player");
	m_pPlayer = dynamic_cast<Player*>((*gameObject)[0]);
	return 0;
}

void TradeInv::LateUpdate_Component()
{

}

_int TradeInv::Get_PlayerMoney() { return m_pPlayer->Get_PlayerMoney(); }
void TradeInv::Set_PlayerMoney(_int _iPlayerMoney) { m_pPlayer->Set_PlayerMoney(_iPlayerMoney); }

bool TradeInv::FindItemFromTradeInv(ItemBase* item)
{
	for_each(m_itemVec.begin(), m_itemVec.end(), [item](ItemBase* src)->bool{
		if (item->Get_ItemID() == src->Get_ItemID())
			return true;

		return false;
		});

	return false;
}

bool TradeInv::AddItemToTradeInv(ItemBase* item)
{
	if (item == nullptr)
		return false;

	if (FindItemFromTradeInv(item) == false)
	{
		m_itemVec.push_back(item);

		return true;
	}

	return false;
}




ItemBase* TradeInv::MoveItemFromTradeInv(_uint index, _int* iMoney)
{
	if (index >= m_itemVec.size())
		return nullptr;

	if (m_itemVec[index] == nullptr)
		return nullptr;

#pragma region 첫 번째 코드

	// 이 코드 nullptr인지 꼭 확인할 것
	vector<GameObject*>* gameObject = Engine::Get_GameObject(L"GameObject", L"Player");
	if (gameObject == nullptr)
		return nullptr;

	// 이 코드도 nullptr인지 꼭 확인할 것
	Player* player = dynamic_cast<Player*>((*gameObject)[0]);
	if (player == nullptr)
		return nullptr;

	if (player->Get_PlayerMoney() >= m_itemVec[index]->Get_ItemPrice())
	{
		ItemBase* outItem = m_itemVec[index]->Clone();
		outItem->Set_ItemStat(m_itemVec[index]->Get_ItemStat());

		return outItem;
	}

#pragma endregion

#pragma region 첫 번째 코드가 안 먹힐 시

	/*if (iMoney != nullptr)
	{
		if ((*iMoney) >= m_itemVec[index]->Get_ItemPrice())
		{
			ItemBase* outItem = m_itemVec[index]->Clone();
			outItem->Set_ItemStat(m_itemVec[index]->Get_ItemStat());

			return outItem;
		}
	}*/
#pragma endregion

	return nullptr;
}

ItemBase* TradeInv::MoveItemFromTradeInv(ItemBase* item)
{
	ItemBase* outItem = item->Clone();
	return outItem;	
}

TradeInv* TradeInv::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	TradeInv* pInstance = new TradeInv(pGraphicDev);
	if (FAILED(pInstance->Init_TradeInv()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TradeInv Create Failed");
		return nullptr;
	}

	return pInstance;
}

Component* TradeInv::Clone()
{
	return new TradeInv(*this);
}

void TradeInv::Free()
{
	for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* src) {
		if (src)
			Safe_Release(src);
		});
	m_itemVec.clear();
	m_itemVec.shrink_to_fit();

	Super::Free();
}
