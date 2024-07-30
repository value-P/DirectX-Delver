#pragma once
#include "GameObject.h"

namespace Engine
{
	class MapCollider;
	class Transform;
}

class ColliderBox :
    public GameObject
{
private:
	explicit ColliderBox(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos);
	explicit ColliderBox(const ColliderBox& rhs);
	virtual ~ColliderBox();

public:
	virtual HRESULT Init_GameObject() override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject() override;
	virtual void Render_GameObject() override;

	virtual wstring Get_OriginName() override { return m_szOriginName; }

private:
	HRESULT			Add_Component();

private:	
	_vec3 m_vInitPos;

public:
	static ColliderBox* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos);

private:
	virtual void Free() override;
};

