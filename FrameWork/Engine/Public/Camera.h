#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL Camera : public GameObject
{
protected:
	explicit Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Camera();

public:
	virtual HRESULT			Init_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual void			LateUpdate_GameObject();

public:
	void		Camera_Shaking();

protected:
	_matrix				m_matView, m_matProj;
	_vec3				m_vEye, m_vAt, m_vUp;
	_float				m_fFov, m_fAspect, m_fNear, m_fFar;

	_float		m_fShakingTime = 0.f;
	_bool		m_bOnShake = false;

protected:
	virtual void		Free();

};

END