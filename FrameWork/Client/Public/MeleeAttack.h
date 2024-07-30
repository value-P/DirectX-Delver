#pragma once

#include "Effect.h"

namespace Engine
{
	class Collider;
}

class MeleeAttack : public Effect
{
private:
	explicit MeleeAttack(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~MeleeAttack();

public:
	static MeleeAttack* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vLook, const float& _fColSize, const int& _iDamage = 1);

public:
	HRESULT Init_MeleeAttack(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vLook, const float& _fColSize, const int& _iDamage);

public:
	virtual int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

public:
	int GetDamage() { return m_iDamage; }
	virtual void SetColType(COLLAYER _eType);

private:
	HRESULT	Add_Component();

private:
	Collider* m_pColliderCom{ nullptr };
	int m_iDamage{ 1 };
	
	bool m_bAttack{ false };

private:
	virtual void Free();
};

