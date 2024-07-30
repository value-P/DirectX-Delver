#pragma once
#include "BossSkillObject.h"
class MeteorLine :
    public BossSkillObject
{
private:
	explicit MeteorLine(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, const _float& fSpeed, const _int& iDamage);
	explicit MeteorLine(const MeteorLine& rhs);
	virtual ~MeteorLine();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();
	void	Create_Meteor();

private:
	_float m_fTime = 0.f;

public:
	static MeteorLine* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, const _float& fSpeed, const _int& iDamage);

private:
	virtual void	Free();
};

