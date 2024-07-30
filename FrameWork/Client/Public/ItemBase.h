#pragma once

#include "GameObject.h"
#include "Engine_Define.h"

namespace Engine
{
	class Collider;
}

// 아이템 정보는 여기에서 추가
struct ItemStat
{
	ItemStat() : iItemCount(1), iItemID(ITEMID_END), eItemType(ITEM_END), iItemPrice(0), bIsStackable(false), bIsActivated(false), iAttack(0), iDefense(0), iHealAmount(0) {}
	ItemStat(const wstring& itemName, ITEMID itemID, ITEMTYPE itemType, _uint itemPrice = 0, _uint itemCount = 1, _bool isStackable = false, _bool isActivated = false, _uint attack = 0, _uint defense = 0, _uint healAmount = 0)
		: wsItemName(itemName), iItemID(itemID), eItemType(itemType), iItemPrice(itemPrice), iItemCount(itemCount), bIsStackable(isStackable), bIsActivated(isActivated), iAttack(attack), iDefense(defense), iHealAmount(healAmount)
	{
		
	}

	wstring wsItemName;
	ITEMID iItemID; // Static으로 만들까? 아마 그래야 할듯? 귀찮다
	ITEMTYPE eItemType = ITEM_END;
	_uint iItemPrice = 0;
	_uint iItemCount = 1; // Default 개수는 1개
	_bool bIsStackable = false;
	_bool bIsActivated = false;

	////////////////////////////
	_uint iAttack = 0;
	_uint iTotalAttack = 0;
	_uint iDefense = 0;
	_uint iHealAmount = 0;
};

// 수정 필요
enum class ItemState
{
	ITEMSTATE_IDLE,
	ITEMSTATE_MOVE,
	ITEMSTATE_WEAK,
	ITEMSTATE_STRONG,

	ITEMSTATE_END,
};

// 모든 아이템(장비, 소비 등)의 부모 클래스
// 제작 목적 : 관리를 편하게 하기 위해서
// Creature나 ObjectBase와 마찬가지로 따로 Instance 하지 않는다
// 모든 아이템의 공통된 스텟, 기능들은 여기서 관리
class ItemBase : public GameObject
{
	using Super = GameObject;

protected:
	explicit ItemBase(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit ItemBase(const ItemBase& rhs);
	virtual ~ItemBase();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	// 경우에 따라 함수는 이동할 수 있음
	ItemStat* Get_ItemStat() { return m_ItemStat; }
	void Set_ItemStat(ItemStat* itemStat); // Item Create 할 때 부르면 될 듯?

	ITEMTYPE Get_ItemType() { return m_ItemStat->eItemType; }
	void Set_ItemType(ITEMTYPE itemType) { m_ItemStat->eItemType = itemType; }

	ItemState Get_ItemState() { return m_ItemState; }
	void Set_ItemState(ItemState itemState) { m_ItemState = itemState; }

	wstring Get_ItemName() { return m_ItemStat->wsItemName; }
	void Set_ItemName(const wstring& name) { m_ItemStat->wsItemName = name; }

	ITEMID Get_ItemID() { return m_ItemStat->iItemID; }
	void Set_ItemID(ITEMID itemID) { m_ItemStat->iItemID = itemID; }

	_uint Get_ItemPrice() { return m_ItemStat->iItemPrice; }
	void Set_ItemPrice(_uint itemPrice) { m_ItemStat->iItemPrice = itemPrice; }

	_uint Get_ItemCount() { return m_ItemStat->iItemCount; }
	void Set_ItemCount(_uint count) { m_ItemStat->iItemCount = count; }
	bool Increase_ItemCount(); // 추후 꼭 수정 필요
	bool Decrease_ItemCount(); // 추후 꼭 수정 필요

	bool IsStackable() { return m_ItemStat->bIsStackable; }
	void Set_IsStackable(bool isStackable) { m_ItemStat->bIsStackable = isStackable; }

	bool IsActivated() { return m_ItemStat->bIsActivated; }
	void Set_IsActivated(bool isActivated) { m_ItemStat->bIsActivated = isActivated; }

	wstring Get_ItemTextureKey() { return m_textureKey; }
	void Set_ItemTextureKey(const wstring& textureKey) { m_textureKey = textureKey; }

	_uint Get_ItemAttack() { return m_ItemStat->iAttack; }
	void Set_ItemAttack(_uint attack) { m_ItemStat->iAttack = attack; }

	_uint Get_ItemTotalAttack() { return m_ItemStat->iTotalAttack; }
	void Set_ItemTotalAttack(_uint totalAttack) { m_ItemStat->iTotalAttack = totalAttack; }

	_uint Get_ItemDefense() { return m_ItemStat->iDefense; }
	void Set_ItemDefense(_uint defense) { m_ItemStat->iDefense = defense; }

	_uint Get_ItemHealAmount() { return m_ItemStat->iHealAmount; }
	void Set_ItemHealAmount(_uint healAmount) { m_ItemStat->iHealAmount = healAmount; }

	Collider* Get_Collider() { return m_pColliderCom; }

	_bool IsAttacking() { return m_bIsAttacking; }
	void SetIsAttacking(_bool isAttacking) { m_bIsAttacking = isAttacking; }

	bool IsDeleted() { return m_bIsDeleted; }
	void SetIsDeleted(bool isDeleted) { m_bIsDeleted = isDeleted; }

public:
	virtual ItemBase* Clone() abstract;

protected:
	HRESULT Add_Component();
	void PlaneCollision();
	void TrackingPlayer();

protected:
	virtual void Free();

protected:
	Collider* m_pColliderCom{ nullptr };

	ItemStat* m_ItemStat = nullptr;
	ItemState m_ItemState = ItemState::ITEMSTATE_END;
	
	D3DXVECTOR3 m_vInfo[INFO_END];
	D3DXVECTOR3 m_vAngle{ 0.0f,0.0f,0.0f };
	D3DXMATRIX m_matWorld;

	// Temp 임시 코드
	wstring m_textureKey;
	_bool m_bIsAttacking = false;
	bool m_bIsDeleted = false;
};
