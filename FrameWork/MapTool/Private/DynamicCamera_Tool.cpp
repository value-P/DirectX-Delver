#include "pch.h"
#include "DynamicCamera_Tool.h"
#include "Export_System.h"

DynamicCamera_Tool::DynamicCamera_Tool(LPDIRECT3DDEVICE9 pGraphicDev)
	: Camera(pGraphicDev)
{
}

DynamicCamera_Tool::~DynamicCamera_Tool()
{
}

HRESULT DynamicCamera_Tool::Init_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(Camera::Init_GameObject(), E_FAIL);

	return S_OK;
}

_int DynamicCamera_Tool::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	if (false == m_bFix)
	{
		Mouse_Move();
		Mouse_Fix();
	}

	_int	iExit = Camera::Update_GameObject(fTimeDelta);

	return iExit;
}

void DynamicCamera_Tool::LateUpdate_GameObject()
{
	Camera::LateUpdate_GameObject();
}

void DynamicCamera_Tool::Set_Rotate(const _vec3* pRotate)
{
	m_vRotation = *pRotate;

	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	_vec3 vRight, vUp;
	vRight = { 1.f,0.f,0.f };
	vUp = { 0.f,1.f,0.f };

	_matrix matRotX, matRotY, matRot;
	D3DXMatrixRotationAxis(&matRotX, &vRight, D3DXToRadian(m_vRotation.y));
	D3DXMatrixRotationAxis(&matRotY, &vUp, D3DXToRadian(m_vRotation.x));

	matRot = matRotX * matRotY;

	_vec3 vLook = {0.f,0.f,1.f};

	D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

	m_vAt = m_vEye + vLook;
}

void DynamicCamera_Tool::Key_Input(const _float& fTimeDelta)
{
	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeed;
	
		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0X80)
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * m_fSpeed;

		m_vEye -= vLength;
		m_vAt -= vLength;

	}

	if (Engine::Get_DIKeyState(DIK_A) & 0X80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeed;

		m_vEye -= vLength;
		m_vAt -= vLength;

	}

	if (Engine::Get_DIKeyState(DIK_D) & 0X80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * m_fSpeed;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
		{
			ShowCursor(false);
			m_bFix = false;
		}
		else
		{
			ShowCursor(true);
			m_bFix = true;
		}

	}
	else
		m_bCheck = false;


	if (false == m_bFix)
	{
		return;
	}

}

void DynamicCamera_Tool::Mouse_Move()
{
	_long dwMouseMoveX(0);
	_long dwMouseMoveY(0);

	D3DXMATRIX matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (dwMouseMoveY = Engine::Get_DIMouseMove(DIMS_Y))
	{
		m_vRotation.y += dwMouseMoveY * 0.1f;
		Set_Rotate(&m_vRotation);
		//D3DXVECTOR3 vRight;
		//memcpy_s(&vRight, sizeof(D3DXVECTOR3), &matCamWorld.m[0][0], sizeof(D3DXVECTOR3));

		//D3DXVECTOR3 vLook = m_vAt - m_vEye;

		//D3DXMATRIX matRot;
		//D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian((float)dwMouseMoveY * 0.1f));
		//D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		//m_vAt = m_vEye + vLook;
	}

	if (dwMouseMoveX = Engine::Get_DIMouseMove(DIMS_X))
	{
		m_vRotation.x += dwMouseMoveX * 0.1f;
		Set_Rotate(&m_vRotation);
		//D3DXVECTOR3 vUp{ 0.0f,1.0f,0.0f };

		//D3DXVECTOR3 vLook = m_vAt - m_vEye;

		//D3DXMATRIX matRot;
		//D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian((float)dwMouseMoveX * 0.1f));
		//D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		//m_vAt = m_vEye + vLook;
	}
}

void DynamicCamera_Tool::Mouse_Fix()
{
	POINT ptMouse{ 1280 >> 1,800 >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

DynamicCamera_Tool* DynamicCamera_Tool::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	DynamicCamera_Tool* pInstance = new DynamicCamera_Tool(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);

		MSG_BOX("DynamicCamera_Tool Create Failed");
		return nullptr;
	}

	return pInstance;
}

void DynamicCamera_Tool::Free()
{
	Camera::Free();

}
