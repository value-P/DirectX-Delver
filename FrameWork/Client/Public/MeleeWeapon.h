#pragma once

#include "Weapon.h"

namespace Engine
{
	class Texture;
}

class MeleeWeapon : public Weapon
{
	using Super = Weapon;

private:
	explicit MeleeWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit MeleeWeapon(const MeleeWeapon& rhs);
	virtual ~MeleeWeapon();

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

protected:
	virtual void OnCollided() override;

private:
	HRESULT			Add_Component(wstring textureKey);
	void Refresh();

public:
	static MeleeWeapon* Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey, _vec3 colSize, COLLAYER colLayer, wstring itemName, ITEMID itemID, ITEMTYPE itemType, _uint itemPrice, _uint itemCount = 1, _bool isStackable = false, _bool isActivated = false, _uint attack = 0, _uint defense = 0);
	virtual ItemBase* Clone() override;

private:
	virtual void Free();

private:
	// temp
	_float m_fDeltaTime = 0.f;
	_float m_fMeleeTime = 0.f;

	bool m_bIsSoundPlaying = false;

	bool m_bIsDirUp = true;
	float m_fShakingTime = 0.f;
};
