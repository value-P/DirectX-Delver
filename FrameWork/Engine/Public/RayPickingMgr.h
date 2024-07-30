#pragma once

#include "Base.h"
#include "Ray.h"

#include "Engine_Define.h"

class Ray;

class ENGINE_DLL RayPickingMgr : public Base
{
	DECLARE_SINGLETON(RayPickingMgr)

private:
	explicit RayPickingMgr();
	~RayPickingMgr();

public:
	Ray* RayAtViewSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj);
	Ray* RayAtWorldSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj);
	Ray* RayAtLocalSpace(LPDIRECT3DDEVICE9 pGraphicDev, int iScreenX, int iScreenY, _matrix* matProj, _matrix* matWorld);

public:
	_bool RayPicking(_vec3& v0, _vec3& v1, _vec3& v2, _vec3 vPickedPosition);

public:
	virtual void Free();

private:
	Ray* m_pRay = nullptr;
};
