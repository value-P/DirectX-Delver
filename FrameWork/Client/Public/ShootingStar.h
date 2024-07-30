#pragma once
#include "BossSkillObject.h"

namespace Engine
{
	class Transform;
}

class ShootingStar :
    public BossSkillObject
{
private:
	explicit ShootingStar(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, const _float& fSpeed, const _int& iDamage);
	explicit ShootingStar(const ShootingStar& rhs);
	virtual ~ShootingStar();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	void	Shoot();
	void	Destroy() { m_bAlive = false; }

protected:
	HRESULT	Add_Component();
	void	On_Collided();

private:
	_float			m_fLifeTime = 0.f;
	Transform*		m_pTarget;
	_bool			m_bOnShoot = false;

public:
	static ShootingStar* Create(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, const _float& fSpeed, const _int& iDamage);

private:
	virtual void	Free();
};

