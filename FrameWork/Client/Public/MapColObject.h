#pragma once
#include "GameObject.h"

namespace Engine
{
	class Collider;
	class Transform;
}

class MapColObject :
    public GameObject
{
private:
	explicit MapColObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit MapColObject(const MapColObject& rhs);
	virtual ~MapColObject();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

private:
	HRESULT			Add_Component(const _vec3& vInitPos, const _vec3& vInitScale);

private:
	Collider*		m_pColliderCom = nullptr;

public:
	static MapColObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos, const _vec3& vInitScale);

private:
	virtual void Free() override;
};

