#include "RayPickingMgr.h"

IMPLEMENT_SINGLETON(RayPickingMgr)

RayPickingMgr::RayPickingMgr()
{
	
}

RayPickingMgr::~RayPickingMgr()
{
	Free();
}

Ray* RayPickingMgr::RayAtViewSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj)
{
	// 따로 빼줄까??
	if (nullptr == m_pRay)
	{
		m_pRay = Ray::Create(pGraphicDev);
	}

	D3DVIEWPORT9 ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	_matrix Proj = *matProj;
	//pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	// 뷰포트 변환 -> 투영 변환
	_vec3 vMouse;
	vMouse.x = (static_cast<float>(iScreenX) / (ViewPort.Width * 0.5f)) - 1.f;
	vMouse.y = (static_cast<float>(iScreenY) / -(ViewPort.Height * 0.5f)) + 1.f;
	vMouse.z = 0.f;

	// 투영 -> 뷰스페이스
	D3DXMatrixInverse(&Proj, 0, &Proj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &Proj);

	_vec3 vRayPos, vRayDir;
	/*vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = vMouse - vRayPos;*/
	vRayPos = { vMouse.x, vMouse.y, vMouse.z };
	vRayDir = { 0.f, 0.f, 1.f };
	//D3DXVec3Normalize(&vRayDir, &vRayDir);

	m_pRay->Set_Origin(vRayPos);
	m_pRay->Set_Dir(vRayDir);

	return m_pRay;
}

Ray* RayPickingMgr::RayAtWorldSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj)
{
	m_pRay = RayAtViewSpace(pGraphicDev, iScreenX, iScreenY, matProj);

	_vec3 vRayPos, vRayDir;
	vRayPos = m_pRay->Get_Origin();
	vRayDir = m_pRay->Get_Dir();

	// 뷰스페이스 -> 월드
	_matrix matView;
	D3DXMatrixIdentity(&matView);
	/*pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);*/

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	m_pRay->Set_Origin(vRayPos);
	m_pRay->Set_Dir(vRayDir);

	return m_pRay;
}

Ray* RayPickingMgr::RayAtLocalSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj, _matrix* matWorld)
{
	m_pRay = RayAtWorldSpace(pGraphicDev, iScreenX, iScreenY, matProj);

	_vec3 vRayPos, vRayDir;
	vRayPos = m_pRay->Get_Origin();
	vRayDir = m_pRay->Get_Dir();

	_matrix world = *matWorld;
	D3DXMatrixInverse(&world, 0, &world);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &world);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &world);

	m_pRay->Set_Origin(vRayPos);
	m_pRay->Set_Dir(vRayDir);

	return m_pRay;
}

_bool RayPickingMgr::RayPicking(_vec3& v0, _vec3& v1, _vec3& v2, _vec3 vPickedPosition)
{
	float u, v, t;
	bool bPicked = D3DXIntersectTri(&v0, &v1, &v2, &m_pRay->Get_Origin(), &m_pRay->Get_Dir(), &u, &v, &t);
	vPickedPosition = m_pRay->Get_Origin() + (t * m_pRay->Get_Dir());

	return bPicked;
}

void RayPickingMgr::Free()
{
	Safe_Release(m_pRay);
}
