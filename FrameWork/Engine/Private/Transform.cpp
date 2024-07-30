#include "..\..\Header\Transform.h"

Transform::Transform() : m_vAngle(0.f, 0.f, 0.f), m_vScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

Transform::Transform(LPDIRECT3DDEVICE9 pGraphicDev)
	: Component(pGraphicDev), m_vAngle(0.f, 0.f, 0.f), m_vScale(1.f, 1.f, 1.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld);
}

Transform::Transform(const Transform & rhs)
	: Component(rhs)
{
	for (size_t i = 0; i < INFO_END; ++i)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_vAngle = rhs.m_vAngle;
	m_vScale = rhs.m_vScale;

	m_matWorld = rhs.m_matWorld;
}


Transform::~Transform()
{
}

void Transform::Set_PosX(const float& _fX)
{
	memcpy_s(&m_vInfo[INFO_POS].x, sizeof(float), &_fX, sizeof(float));
}

void Transform::Set_PosY(const float& _fY)
{
	memcpy_s(&m_vInfo[INFO_POS].y, sizeof(float), &_fY, sizeof(float));
}

void Transform::Set_PosZ(const float& _fZ)
{
	memcpy_s(&m_vInfo[INFO_POS].z, sizeof(float), &_fZ, sizeof(float));
}

void Transform::Move_PosX(const float& _fX)
{
	m_vInfo[INFO_POS].x += _fX;
}

void Transform::Move_PosY(const float& _fY)
{
	m_vInfo[INFO_POS].y += _fY;
}

void Transform::Move_PosZ(const float& _fZ)
{
	m_vInfo[INFO_POS].z += _fZ;
}

void Transform::Move_Pos(const D3DXVECTOR3& _vDir)
{
	m_vInfo[INFO_POS] += _vDir;
}

void Transform::Move_Pos(const float& _fX, const float& _fY, const float& _fZ)
{
	m_vInfo[INFO_POS].x += _fX;
	m_vInfo[INFO_POS].y += _fY;
	m_vInfo[INFO_POS].z += _fZ;
}

void Transform::Chase_Target(const _vec3 * pTargetPos, const _float & fTimeDelta, const _float & fSpeed)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fTimeDelta * fSpeed;

	_matrix		matScale, matTrans;
	_matrix		matRot = *Compute_Lookattarget(pTargetPos);

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POS].x, m_vInfo[INFO_POS].y, m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;
}

const _matrix * Transform::Compute_Lookattarget(const _vec3 * pTargetPos)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	/*_vec3		vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);
	_vec3		vUp;	_matrix		matRot;
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]);
	_float		fDot = D3DXVec3Dot(&vDir, &vUp);
	_float		fAngle = acos(fDot);
	D3DXMatrixRotationAxis(&matRot, &vAxis, fAngle);
	return &matRot;*/

	_vec3		vAxis, vUp;
	_matrix		matRot;

	return D3DXMatrixRotationAxis(&matRot, 
								D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir),
							acos(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
											 D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

void Transform::SettingMatrix()
{
	for (int i = 0; i < INFO_POS; i++)
	{
		memcpy_s(&m_matWorld.m[i][i], sizeof(float), ((float*)&m_vScale + i), sizeof(float));
	}
	memcpy_s(&m_matWorld.m[3][0], sizeof(D3DXVECTOR3), m_vInfo[INFO_POS], sizeof(D3DXVECTOR3));
}

HRESULT Transform::Init_Transform()
{
	D3DXMatrixIdentity(&m_matWorld);

	for (size_t i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	return S_OK;
}

// 월드 행렬 생성
_int Transform::Update_Component(const _float & fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기 변환

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}

	// 회전 변환
	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		for (_uint j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for(_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));

	return 0;
}

void Transform::LateUpdate_Component()
{
}

Transform * Transform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Transform *		pInstance = new Transform(pGraphicDev);

	if (FAILED(pInstance->Init_Transform()))
	{
		Safe_Release(pInstance);

		MSG_BOX("transform Create Failed");
		return nullptr;
	}

	return pInstance;
}

Component * Transform::Clone()
{
	return new Transform(*this);
}

void Transform::Free()
{
	Component::Free();
}
