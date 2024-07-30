#pragma once
#include "BossSkillObject.h"
class Orbit :
    public BossSkillObject
{
private:
	explicit Orbit(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget,_vec3* pPos, const _float& fSpeed, const _int& iDamage);
	virtual ~Orbit();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();

private:
	Transform*		m_pTarget;
	_float			m_fShootTime = 0.f;
	int				m_iShootCnt = 0;

public:
	static Orbit* Create(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, _vec3* pPos, const _float& fSpeed, const _int& iDamage);

private:
	virtual void	Free();
};

