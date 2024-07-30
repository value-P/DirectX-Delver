#pragma once

#include "Camera.h"

class DynamicCamera_Tool : public Camera
{
private:
	explicit DynamicCamera_Tool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~DynamicCamera_Tool();

public:
	HRESULT			Init_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
		const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar);

	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual void			LateUpdate_GameObject();

public:
	void		Set_Eye(const _vec3* pEye) { m_vEye = *pEye; }
	const _vec3* Get_Eye() { return &m_vEye; }
	void		Set_Rotate(const _vec3* pRotate);
	const _vec3* Get_Rotate() { return &m_vRotation; }

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move();
	void		Mouse_Fix();

public:
	_float		m_fSpeed = 20.f;

private:
	_bool		m_bFix = false;
	_bool		m_bCheck = false;

	_vec3		m_vRotation = { 0.f,0.f,0.f };

public:
	static DynamicCamera_Tool* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFov = D3DXToRadian(60.f),
		const _float& fAspect = (_float)WINCX / WINCY,
		const _float& fNear = 0.1f,
		const _float& fFar = 1000.f);

private:
	virtual void Free();

};

