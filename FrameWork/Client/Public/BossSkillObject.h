#pragma once
#include "GameObject.h"

namespace Engine
{
	class Collider;
    class PSystem;
}

class BossSkillObject :
    public GameObject
{
protected:
    explicit BossSkillObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit BossSkillObject(const BossSkillObject& rhs);
    virtual ~BossSkillObject();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT			Add_Component();
	virtual void	On_Collided();

protected:
	Collider*	m_pColliderCom{ nullptr };
	PSystem*	m_pParticles{ nullptr };
	float		m_fParticleGenerateTime{ 0.0f };

	_int			m_iDamage = 0;
	_vec3			m_vStartPos;
	_vec3			m_vStartDirection;
	_float			m_fSpeed = 0.f;
	_bool			m_bAlive = true;
	_bool			m_bAttacked = false; // 데미지가 프레임마다 들어가는 것 방지

protected:
	virtual void	Free();

};

