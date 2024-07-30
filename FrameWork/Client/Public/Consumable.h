#pragma once

#include "ItemBase.h"

// 포션 등 소비 아이템
class Consumable : public ItemBase
{
	using Super = ItemBase;

protected:
	explicit Consumable(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Consumable(const Consumable& rhs);
	virtual ~Consumable();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	static Consumable* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType, _uint itemPrice, _uint itemCount = 1, _bool isStackable = false, _bool isActivated = false, _uint attack = 0, _uint defense = 0, _uint healAmount = 0);
	virtual ItemBase* Clone() override;

protected:
	HRESULT			Add_Component(wstring textureKey);

protected:
	virtual void Free();
};
