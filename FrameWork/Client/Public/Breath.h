#pragma once
#include "BossSkillObject.h"
class BossMonster2;

class Breath :
    public BossSkillObject
{
private:
	explicit Breath(LPDIRECT3DDEVICE9 pGraphicDev, BossMonster2* pOwner, Transform* pTargetTransform, const _float& fSpeed, const _int& iDamage);
	explicit Breath(const Breath& rhs);
	virtual ~Breath();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();

	void	LengthUpdate(const _float& fTimeDelta);	

private:
	BossMonster2* m_pOwner;
	Transform* m_pTargetTransform;

	VTXTEX m_tVertices[6];
	_vec3  m_vPoints[4];
	_vec3  m_vPrevEndPos;
	_vec3  m_vEndPos;
	_float m_fThickness = 2.f;
	_float m_fLength = 0.f;
	_float m_fUVLength = 0.f;

	_float m_fTrackingTime = 0.f;

public:
	static Breath* Create(LPDIRECT3DDEVICE9 pGraphicDev, BossMonster2* pOwner, Transform* pTargetTransform, const _float& fSpeed, const _int& iDamage);

private:
	virtual void	Free();

};

