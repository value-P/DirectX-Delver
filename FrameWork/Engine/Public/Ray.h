#pragma once

#include "Base.h"

#include "Engine_Define.h"

class ENGINE_DLL Ray : public Base
{
private:
	explicit Ray(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit Ray(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOrigin, _vec3 vDir);
	~Ray();

public:
	HRESULT Init_Ray();

public:
	_vec3 Get_Origin() { return m_vOrigin; }
	void Set_Origin(_vec3 origin) { m_vOrigin = origin; }

	_vec3 Get_Dir() { return m_vDir; }
	void Set_Dir(_vec3 dir) { m_vDir = dir; }

public:
	static Ray* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static Ray* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vOrigin, _vec3 vDir);

public:
	virtual void Free();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
	_vec3 m_vOrigin;
	_vec3 m_vDir;
};
