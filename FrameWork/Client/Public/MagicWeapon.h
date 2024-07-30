#pragma once

#include "Weapon.h"

enum MagicWeaponType
{
	MWT_MAGIC_0,
	MWT_MAGIC_1,

	MAGICWEAPONTYPE_END,
};

class MagicWeapon : public Weapon
{
	using Super = Weapon;

private:
	explicit MagicWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit MagicWeapon(const MagicWeapon& rhs);
	virtual ~MagicWeapon();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	MagicWeaponType GetMagicWeaponType() { return m_eType; }
	void SetMagicWeaponType(MagicWeaponType magicType) { m_eType = magicType; }

private:
	void StateIdle();
	void StateMove();
	void StateWeak();
	void StateStrong();

private:
	HRESULT			Add_Component(wstring textureKey);
	virtual void OnCollided() override;

public:
	static MagicWeapon* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType, _uint itemPrice, MagicWeaponType magicType, _uint itemCount = 1, _bool isStackable = false, _bool isActivated = false, _uint attack = 0, _uint defense = 0);
	virtual ItemBase* Clone() override;

private:
	virtual void Free();

private:
	// temp
	_float m_fDeltaTime = 0.f;
	_float m_fMagicTime = 0.f;

	_float m_fAngleX = 0.f;
	_float m_fAngleY = 0.f;
	_float m_fAngleZ = 0.f;

	MagicWeaponType m_eType = MAGICWEAPONTYPE_END;

	bool m_bIsDirUp = true;
	float m_fShakingTime = 0.f;
};
