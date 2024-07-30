#include "SingleCamera.h"
#include "Export_System.h"
#include "Export_Utility.h"

SingleCamera* SingleCamera::m_pInstance{ nullptr };

SingleCamera::SingleCamera()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

SingleCamera::~SingleCamera()
{
	Free();
}

void SingleCamera::Init_Camera(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
	if (nullptr == m_pGraphicDevice && _pGraphicDevice)
	{
		m_pGraphicDevice = _pGraphicDevice;
		m_pGraphicDevice->AddRef();
	}
}

void SingleCamera::SetPlayerTransform(Transform* _pPlayerTransform)
{
	if (nullptr == m_pPlayerTransform && _pPlayerTransform)
	{
		m_pPlayerTransform = _pPlayerTransform;
	}
}

void SingleCamera::SetTimeDelta(const float& _fTimeDelta)
{
	memcpy_s(&m_fDeltaTime, sizeof(float), &_fTimeDelta, sizeof(float));
}

void SingleCamera::PlayerViewpoint()
{
	if (m_pPlayerTransform)
	{
		memcpy_s(&m_vEye, sizeof(D3DXVECTOR3), &m_pPlayerTransform->Get_Info(INFO_POS), sizeof(D3DXVECTOR3));
		memcpy_s(&m_vAt, sizeof(D3DXVECTOR3), &m_pPlayerTransform->Get_Info(INFO_LOOK), sizeof(D3DXVECTOR3));
		memcpy_s(&m_vUp, sizeof(D3DXVECTOR3), &m_pPlayerTransform->Get_Info(INFO_UP), sizeof(D3DXVECTOR3));
		
		m_vEye += D3DXVECTOR3{ 0.0f,0.5f,0.0f };
		m_vAt += m_vEye;

		DefaultSetting();
		
		if (m_bShake)
		{
			PlayShake(m_vEye, m_vAt);
			m_fTimeS -= m_fDeltaTime;

			if (0.0f > m_fTimeS)
			{
				m_bShake = false;
			}
		}
		else
		{
			D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
		}

		D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

		m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);
		m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	}
}

void SingleCamera::PlayCameraDirection()
{
	switch (m_iPlayNum)
	{
	case 1:
	{
		PlayDirection1();
	}
	break;
	case 2:
	{
		PlayDirection2();
	}
	break;
	case 3:
	{
		PlayDirection3();
	}
	break;
	case 4:
	{
		PlayDirection4();
	}
	break;
	}

	if (m_bShake)
	{
		m_fTimeS -= m_fDeltaTime;

		if (0.0f > m_fTimeS)
		{
			m_bShake = false;
		}
	}

	m_fTimeD -= m_fDeltaTime;

	if (0.0f > m_fTimeD)
	{
		m_iPlayNum = 0;
		m_bPlay = false;
	}
}

void Engine::SingleCamera::StopCameraDirection()
{
	m_bPlay = false;
	m_bShake = false;
}

void SingleCamera::PlayCameraDirection1(const D3DXVECTOR3& _vEye, const D3DXVECTOR3& _vAt, const float& _fSecond, const float& _fFov)
{
	m_iPlayNum = 1;
	m_vEye = _vEye;
	m_vAt = _vAt;
	m_vUp = D3DXVECTOR3{ 0.0f,1.0f,0.0f };
	m_fFov = _fFov;
	m_fTimeD = _fSecond;
	m_bPlay = true;
}

void SingleCamera::PlayCameraDirectionR1(const D3DXVECTOR3& _vEye, const float& _fRadianX, const float& _fRadianY, const float& _fSecond, const float& _fFov)
{
	m_iPlayNum = 1;
	m_vEye = _vEye;
	m_vAt = _vEye + D3DXVECTOR3{ sin(_fRadianX),sin(_fRadianY),cos(_fRadianX) };
	m_vUp = D3DXVECTOR3{ 0.0f,1.0f,0.0f };
	m_fFov = _fFov;
	m_fTimeD = _fSecond;
	m_bPlay = true;
}

void SingleCamera::PlayCameraDirection2(const D3DXVECTOR3& _vEyeBegin, const D3DXVECTOR3& _vEyeEnd, const D3DXVECTOR3& _vAt, const float& _fSecond, const float& _fFov)
{
	m_iPlayNum = 2;
	m_vEye = _vEyeBegin;
	m_vAt = _vAt;
	m_vUp = D3DXVECTOR3{ 0.0f,1.0f,0.0f };
	m_fFov = _fFov;
	m_fTimeD = _fSecond;
	m_vMoveDir = (_vEyeEnd - _vEyeBegin) / _fSecond;
	m_bPlay = true;
}

void SingleCamera::PlayCameraDirection3(const D3DXVECTOR3& _vEyeBegin, const D3DXVECTOR3& _vEyeEnd, const D3DXVECTOR3& _vDir, const float& _fSecond, const float& _fFov)
{
	m_iPlayNum = 3;
	m_vEye = _vEyeBegin;
	m_vUp = D3DXVECTOR3{ 0.0f,1.0f,0.0f };
	m_vLookDir = _vDir;
	m_fFov = _fFov;
	m_fTimeD = _fSecond;
	m_vMoveDir = (_vEyeEnd - _vEyeBegin) / _fSecond;
	m_bPlay = true;
}

void SingleCamera::PlayCameraDirectionR3(const D3DXVECTOR3& _vEyeBegin, const D3DXVECTOR3& _vEyeEnd, const float& _fRadianX, const float& _fRadianY, const float& _fSecond, const float& _fFov)
{
	m_iPlayNum = 3;
	m_vEye = _vEyeBegin;
	m_vUp = D3DXVECTOR3{ 0.0f,1.0f,0.0f };
	m_vLookDir = D3DXVECTOR3{ sin(_fRadianX),sin(_fRadianY),cos(_fRadianX) };
	m_fFov = _fFov;
	m_fTimeD = _fSecond;
	m_vMoveDir = (_vEyeEnd - _vEyeBegin) / _fSecond;
	m_bPlay = true;
}

void SingleCamera::PlayCameraDirection4(const D3DXVECTOR3& _vEye, const D3DXVECTOR3& _vAt, bool _bLeftRot, const float& _fRadian, const float& _fSecond, const float& _fFov)
{
	m_iPlayNum = 4;
	m_vEye = _vEye;
	D3DXVec3Normalize(&m_vLookDir, &(_vAt - _vEye));
	m_vAt = m_vEye + m_vLookDir;
	m_vUp = D3DXVECTOR3{ 0.0f,1.0f,0.0f };
	m_fFov = _fFov;
	m_bLeft = _bLeftRot;
	m_fRadian = _fRadian;
	m_fCurRadian = 0.0f;
	m_fRotSpeed = _fRadian / _fSecond;
	m_fTimeD = _fSecond;
	m_bPlay = true;
}

void SingleCamera::PlayCameraDirectionR4(const D3DXVECTOR3& _vEye, const float& _fRadianX, const float& _fRadianY, bool _bLeftRot, const float& _fRadian, const float& _fSecond, const float& _fFov)
{
	m_iPlayNum = 4;
	m_vEye = _vEye;
	m_vAt = _vEye + D3DXVECTOR3{ sin(_fRadianX),sin(_fRadianY),cos(_fRadianX) };
	m_vUp = D3DXVECTOR3{ 0.0f,1.0f,0.0f };
	m_fFov = _fFov;
	m_bLeft = _bLeftRot;
	m_fRadian = _fRadian;
	m_fCurRadian = 0.0f;
	m_fRotSpeed = _fRadian / _fSecond;
	m_fTimeD = _fSecond;
	m_bPlay = true;
}

void Engine::SingleCamera::PlayCameraShake(const float& _fSecond, const float& _fShakePower)
{
	m_bShake = true;
	m_fTimeS = _fSecond;
	m_fShakePower = _fShakePower;
}

void SingleCamera::DefaultSetting()
{
	m_fFov = 1.0471976f;	// D3DX_PI / 3.0f
	m_fAspect = 1.3333334f;	// 4.0f / 3.0f
	m_fNear = 0.1f;
	m_fFar = 1000.0f;
}

void SingleCamera::PlayDirection1()
{
	if (m_bShake)
	{
		PlayShake(m_vEye, m_vAt);
	}
	else
	{
		D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	}
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void SingleCamera::PlayDirection2()
{
	m_vEye += m_vMoveDir * m_fDeltaTime;
	if (m_bShake)
	{
		PlayShake(m_vEye, m_vAt);
	}
	else
	{
		D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	}
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void SingleCamera::PlayDirection3()
{
	m_vEye += m_vMoveDir * m_fDeltaTime;
	m_vAt = m_vEye + m_vLookDir;

	if (m_bShake)
	{
		PlayShake(m_vEye, m_vAt);
	}
	else
	{
		D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	}

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void SingleCamera::PlayDirection4()
{
	D3DXMATRIX matRotY;
	D3DXVECTOR3 _vRotDir;
	if (m_bLeft)
	{
		D3DXMatrixRotationY(&matRotY, -m_fCurRadian);
	}
	else
	{
		D3DXMatrixRotationY(&matRotY, m_fCurRadian);
	}
	m_fCurRadian += m_fRotSpeed * m_fDeltaTime;
	//memcpy_s(&matRotY.m[3][0], sizeof(D3DXVECTOR3), m_vEye, sizeof(D3DXVECTOR3));

	D3DXVec3TransformNormal(&_vRotDir, &m_vLookDir, &matRotY);

	m_vAt = m_vEye + _vRotDir;

	if (m_bShake)
	{
		PlayShake(m_vEye, m_vAt);
	}
	else
	{
		D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	}

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void Engine::SingleCamera::PlayShake(const D3DXVECTOR3& _vEye, const D3DXVECTOR3& _vAt)
{
	D3DXVECTOR3 vRandom;
	D3DXVECTOR3 vMin{ -m_fShakePower,-m_fShakePower,-m_fShakePower }, vMax{ m_fShakePower,m_fShakePower,m_fShakePower };
	GetRandomVector(&vRandom, &vMin, &vMax);
	D3DXVECTOR3 vShakeEye{ _vEye + vRandom };
	D3DXVECTOR3 vShakeAt{ _vAt + vRandom };

	D3DXMatrixLookAtLH(&m_matView, &vShakeEye, &vShakeAt, &m_vUp);
	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void SingleCamera::Free()
{
	Safe_Release(m_pGraphicDevice);
}
