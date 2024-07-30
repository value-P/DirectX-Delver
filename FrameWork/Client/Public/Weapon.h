#pragma once

#include "ItemBase.h"

namespace Engine
{
	class Texture;
}

enum WEAPONSTATE
{
	WEAPON_IDLE,
	WEAPON_MOVE,
	WEAPON_WEAK,
	WEAPON_STRONG,
	WEAPON_END
};

class Weapon : public ItemBase
{
	using Super = ItemBase;

protected:
	explicit Weapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Weapon(const Weapon& rhs);
	virtual ~Weapon();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void Set_WeaponState(WEAPONSTATE _eWeaponState);
	WEAPONSTATE Get_WeaponState() { return m_eState; }

	_bool IsArrowReady() { return m_bArrowReady; }
	void SetIsArrowReady(bool arrowReady) { m_bArrowReady = arrowReady; }

	void RefreshRangedTime() { m_fRangedTime = 0.f; }
	void RefreshRangedFrame() { m_fFrame = 0.f; }

protected:
	WEAPONSTATE m_eState{ WEAPON_END };
	_bool m_bArrowReady = false;

	_float m_fRangedTime = 0.f;

	_float m_fFrame = 0.f;

protected:
	virtual void Free();
};
