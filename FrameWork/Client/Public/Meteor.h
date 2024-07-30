#pragma once
#include "BossSkillObject.h"

class Meteor :
    public BossSkillObject
{
private:
	explicit Meteor(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartPos, const _float& fSpeed, const _int& iDamge);
	explicit Meteor(const Meteor& rhs);
	virtual ~Meteor();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();
	virtual void	On_Collided() override;

public:
	static Meteor* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartPos, const _float& fSpeed, const _int& iDamge);

private:
	virtual void	Free();
};

