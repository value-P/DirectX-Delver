#pragma once

#include "Weapon.h"

class RangedWeapon : public Weapon
{
	using Super = Weapon;

private:
	explicit RangedWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit RangedWeapon(const RangedWeapon& rhs);
	virtual ~RangedWeapon();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	void StateIdle();
	void StateMove();
	void StateWeak();
	void StateStrong();

	void Refresh();

private:
	HRESULT			Add_Component(wstring textureKey);
	virtual void OnCollided() override;

public:
	static RangedWeapon* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType, _uint itemPrice, _uint itemCount = 1, _bool isStackable = false, _bool isActivated = false, _uint attack = 0, _uint defense = 0);
	virtual ItemBase* Clone() override;

private:
	virtual void Free();

private:
	// temp
	_float m_fDeltaTime = 0.f;

	_float m_fAngleX = 0.f;
	_float m_fAngleY = 0.f;
	_float m_fAngleZ = 0.f;

	float m_fShakingTime = 0.f;
	bool m_bIsDirUp = true;
};
