#include "pch.h"
#include "ColliderBox.h"
#include "MapCollider.h"
#include "Transform.h"

ColliderBox::ColliderBox(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos)
    :GameObject(pGraphicDev),m_vInitPos(vInitPos)
{
    m_szOriginName = BOX_COLLIDER_NAME;
}

ColliderBox::ColliderBox(const ColliderBox& rhs)
    :GameObject(rhs)
{
    m_szOriginName = rhs.m_szOriginName;
}

ColliderBox::~ColliderBox()
{
}

HRESULT ColliderBox::Init_GameObject()
{
    HRESULT result = GameObject::Init_GameObject();
    m_pTransformCom->Set_Pos(m_vInitPos.x, m_vInitPos.y, m_vInitPos.z);

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return result;
}

_int ColliderBox::Update_GameObject(const _float& fTimeDelta)
{
    _int	iExit = GameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void ColliderBox::LateUpdate_GameObject()
{
    GameObject::LateUpdate_GameObject();
}

void ColliderBox::Render_GameObject()
{
    m_pTransformCom->SettingMatrix();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_pGraphicDev->SetTexture(0, nullptr);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

HRESULT ColliderBox::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pBufferCom = MapCollider::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

ColliderBox* ColliderBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vInitPos)
{
    ColliderBox* pInstance = new ColliderBox(pGraphicDev, vInitPos);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("ColliderBox Create Failed");
        return nullptr;
    }

    return pInstance;

}

void ColliderBox::Free()
{
    __super::Free();
}
