#include "Ray.h"

Ray::Ray(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Ray::Ray(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOrigin, _vec3 vDir)
	: m_pGraphicDev(pGraphicDev), m_vOrigin(vOrigin), m_vDir(vDir)
{
	m_pGraphicDev->AddRef();
}

Ray::~Ray()
{

}

HRESULT Ray::Init_Ray()
{

	return S_OK;
}

Ray* Ray::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Ray* pInstance = new Ray(pGraphicDev);
	if (FAILED(pInstance->Init_Ray()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Ray Create Failed");
		return nullptr;
	}

	return pInstance;;
}

Ray* Ray::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOrigin, _vec3 vDir)
{
	Ray* pInstance = new Ray(pGraphicDev, vOrigin, vDir);
	if (FAILED(pInstance->Init_Ray()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Ray Create Failed");
		return nullptr;
	}

	return pInstance;;
}

void Ray::Free()
{
	Safe_Release(m_pGraphicDev);
}
