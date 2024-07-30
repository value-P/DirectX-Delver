#pragma once
#include "BossSkillObject.h"
class JumpCrack :
    public BossSkillObject
{
private:
	explicit JumpCrack(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _int& iDamage);
	explicit JumpCrack(const JumpCrack& rhs);
	virtual ~JumpCrack();

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
	static JumpCrack* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _int& iDamage);

private:
	virtual void	Free();

};

