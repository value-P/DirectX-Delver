#include "stdafx.h"
#include "Weapon.h"

#include "Export_Utility.h"

Weapon::Weapon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev), m_eState(WEAPON_IDLE)
{
}

Weapon::Weapon(const Weapon& rhs)
	: Super(rhs)
{
}

Weapon::~Weapon()
{
}

HRESULT Weapon::Init_GameObject()
{
	Super::Init_GameObject();


	return S_OK;
}

_int Weapon::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Super::Update_GameObject(fTimeDelta);

	return iExit;
}

void Weapon::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();
}

void Weapon::Render_GameObject()
{
	
}

void Weapon::Set_WeaponState(WEAPONSTATE _eWeaponState)
{
	m_eState = _eWeaponState;
}

void Weapon::Free()
{
	Super::Free();
}
