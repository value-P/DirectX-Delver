#include "..\..\Header\Camera.h"

#include "Export_Utility.h"

Camera::Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	: GameObject(pGraphicDev)
	/*, m_fFov(0.f), m_fAspect(0.f), m_fNear(0.f), m_fFar(0.f)
	, m_vEye(_vec3(0.f, 0.f, 0.f)) 
	, m_vAt(_vec3(0.f, 0.f, 0.f))
	, m_vUp(_vec3(0.f, 0.f, 0.f))*/
{
	//D3DXMatrixIdentity(&m_matView);
	//D3DXMatrixIdentity(&m_matProj);
}

Camera::~Camera()
{
}

HRESULT Camera::Init_GameObject()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//Pipeline::Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	//Pipeline::Make_ProjMatrix(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int Camera::Update_GameObject(const _float & fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	//Pipeline::Make_ViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	//Pipeline::Make_ProjMatrix(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	if (m_bOnShake)
	{
		m_fShakingTime += fTimeDelta;

		if (m_fShakingTime >= 0.1f)
		{
			m_bOnShake = false;
			m_fFov += D3DXToRadian(1.f);
			m_fShakingTime = 0.f;
		}
	}

	return 0;
}

void Camera::LateUpdate_GameObject()
{
	GameObject::LateUpdate_GameObject();
}

void Camera::Camera_Shaking()
{
	if (!m_bOnShake)
	{
		m_fFov -= D3DXToRadian(1.f);
		m_bOnShake = true;
	}
}

void Camera::Free()
{
	GameObject::Free();
}
