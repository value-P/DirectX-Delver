#pragma once

#include "ItemBase.h"

class Armor : public ItemBase
{
	using Super = ItemBase;

protected:
	explicit Armor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Armor(const Armor& rhs);
	virtual ~Armor();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;
	
public:
	// Collider 정보 채워줄 필요 없음
	static Armor* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType, _uint itemPrice, _uint itemCount = 1, _bool isStackable = false, _bool isActivated = false, _uint attack = 0, _uint defense = 0);
	virtual ItemBase* Clone() override;

protected:
	HRESULT			Add_Component(wstring textureKey);
	virtual void OnCollided() override;

protected:
	virtual void Free();
};
