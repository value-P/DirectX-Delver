#pragma once
#include "BossSkillObject.h"
class Storm :
    public BossSkillObject
{
private:
	explicit Storm(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fLifeTime, const _int& iDamage);
	virtual ~Storm();

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
	static Storm* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fLifeTime, const _int& iDamage);

private:
	virtual void	Free();
};

