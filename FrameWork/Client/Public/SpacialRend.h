#pragma once
#include "GameObject.h"

class SpacialRend :
    public GameObject
{
private:
	explicit SpacialRend(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartDir, const _float& fSpeed);
	explicit SpacialRend(const SpacialRend& rhs);
	virtual ~SpacialRend();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

protected:
	HRESULT	Add_Component();
	void	On_Collided();

private:
	_vec3			m_vStartDirection;
	_float			m_fSpeed;
	_float			m_fLifeTime = 0.f;
	_bool			m_bIsHorizontal = true;

public:
	static SpacialRend* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartDir, const _float& fSpeed, _bool isHorizontal = true);

private:
	virtual void	Free();

};

