#include "stdafx.h"
#include "MapColObject.h"
#include "Transform.h"
#include "Collider.h"
#include "Export_Utility.h"

MapColObject::MapColObject(LPDIRECT3DDEVICE9 pGraphicDev)
    :GameObject(pGraphicDev)
{
}

MapColObject::MapColObject(const MapColObject& rhs)
    :GameObject(rhs)
{
}

MapColObject::~MapColObject()
{
}

HRESULT MapColObject::Init_GameObject()
{
    HRESULT result = __super::Init_GameObject();

	return result;
}

_int MapColObject::Update_GameObject(const _float& fTimeDelta)
{
	int i = GameObject::Update_GameObject(fTimeDelta);

	Engine::AddCollider(m_pColliderCom);

	return i;
}

void MapColObject::LateUpdate_GameObject()
{
	GameObject::LateUpdate_GameObject();
}

void MapColObject::Render_GameObject()
{
}

HRESULT MapColObject::Add_Component(const _vec3& vInitPos, const _vec3& vInitScale)
{
	Component* pComponent = nullptr;

	m_pTransformCom->Set_Pos(vInitPos);
	m_pTransformCom->Set_Scale(vInitScale.x, vInitScale.y, vInitScale.z);

	pComponent = m_pColliderCom = Collider::Create(m_pGraphicDev);
	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetColLayer(COL_WALL);
	m_pColliderCom->SetSize(vInitScale.x, vInitScale.y, vInitScale.z);
	m_pColliderCom->SettingCInfo();
	m_pColliderCom->SetThisPointer(this);


	m_mapComponent[ID_DYNAMIC].emplace(TAG_COLLIDER, pComponent);

	pComponent = m_pBufferCom = CubeTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

	return S_OK;
}

MapColObject* MapColObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos, const _vec3& vInitScale)
{
    MapColObject* pInstance = new MapColObject(pGraphicDev);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("MapObjectCollider Create Failed");
        return nullptr;
    }

    if (FAILED(pInstance->Add_Component(vInitPos, vInitScale)))
    {
        Safe_Release(pInstance);

        MSG_BOX("MapObjectCollider Create Failed");
        return nullptr;
    }

    return pInstance;
}

void MapColObject::Free()
{
	__super::Free();
}
