#pragma once
#include "BossSkillObject.h"
class EnergyBall :
    public BossSkillObject
{
private:
	explicit EnergyBall(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartDir, Transform* pTargetTransform, const _int& iDamage, const _float& fSpeed);
	explicit EnergyBall(const EnergyBall& rhs);
	virtual ~EnergyBall();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();
	void	On_Collided();

private:
	void	ControlDir(const _float& fTimeDelta); // 물체의 속도를 제어하는 함수

private:
	Transform*		m_pTargetTransform;
	_float			m_fAccuracy = 0.95f;
	_float			m_fLifeTime = 0.f;
	_float			m_fControlTerm = 0.f;

	_bool			m_bIsModified = false;

public:
	static EnergyBall* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartDir, Transform* pTargetTransform, const _int& iDamage, const _float& fSpeed = 3.f);

private:
	virtual void	Free();

};

