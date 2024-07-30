#pragma once
#include "BossSkillObject.h"
class DragonClow :
    public BossSkillObject
{
private:
	explicit DragonClow(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pDir , const _float& fSpeed, const _int& iDamage);
	virtual ~DragonClow();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();
	void	On_Collided();

private:
	_float m_fAliveTime = 0.f;

public:
	static DragonClow* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pDir, const _float& fSpeed, const _int& iDamage);

private:
	virtual void	Free();
};

