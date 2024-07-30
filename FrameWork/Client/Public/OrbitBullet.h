#pragma once
#include "BossSkillObject.h"

class OrbitBullet :
    public BossSkillObject
{
private:
	explicit OrbitBullet(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pDir, const _float& fSpeed, const _int& iDamage);
	virtual ~OrbitBullet();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;


protected:
	HRESULT	Add_Component();
	void	On_Collided();

private:
	_float			m_fLifeTime = 0.f;

public:
	static OrbitBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pDir, const _float& fSpeed, const _int& iDamage);

private:
	virtual void	Free();
};

