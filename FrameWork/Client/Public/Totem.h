#pragma once
#include "BossSkillObject.h"

namespace Engine
{
	class Texture;
}

class Totem :
    public BossSkillObject
{
private:
	explicit Totem(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, Transform* pTarget);
	virtual ~Totem();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

public:
	_bool	IsActivated() { return m_bActivated; }
	void	Destroy();

protected:
	HRESULT	Add_Component();
	void	On_Collided();

private:
	_float			m_fLifeTime = 0.f;
	Transform*		m_pTarget;
	_bool			m_bActivated = true;

	Texture*		m_pActiveTex = nullptr;
	Texture*		m_pInActiveTex = nullptr;

public:
	static Totem* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, Transform* pTarget);

private:
	virtual void	Free();

};

