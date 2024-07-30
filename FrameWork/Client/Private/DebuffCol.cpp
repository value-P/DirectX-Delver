#include "stdafx.h"
#include "DebuffCol.h"

#include "Export_Utility.h"

DebuffCol::DebuffCol(LPDIRECT3DDEVICE9 _pGraphicDevice)
    :GameObject(_pGraphicDevice)
{
}

DebuffCol::~DebuffCol()
{
}

HRESULT DebuffCol::Init_GameObject()
{
    if (FAILED(Add_Component()))
    {
        return E_FAIL;
    }

    m_pColliderCom->SetSize(50.f, 1.0f, 50.f);
    m_pColliderCom->SelfBuilding(D3DXVECTOR3{ 1.0f,0.0f,0.0f }, D3DXVECTOR3{ 0.0f,1.0f,0.0f }, D3DXVECTOR3{ 0.0f,0.0f,1.0f }, D3DXVECTOR3{ -118.0f,21.0f,168.0f });
    m_pColliderCom->SetColLayer(COL_DEBUFF);
    m_pColliderCom->SetThisPointer(this);


    return S_OK;
}

_int DebuffCol::Update_GameObject(const _float& fTimeDelta)
{
    m_pColliderCom->ClearInfo();
    Engine::AddCollider(m_pColliderCom);
    
    return 0;
}

HRESULT DebuffCol::Add_Component()
{
    Component* pComponent = nullptr;
        
    pComponent = m_pColliderCom = Collider::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_COLLIDER,pComponent });

    return S_OK;
}

void DebuffCol::Free()
{
    __super::Free();
}

DebuffCol* DebuffCol::Create(LPDIRECT3DDEVICE9 _pGraphicDevice)
{
    DebuffCol* pInstance = new DebuffCol(_pGraphicDevice);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("DebuffCol Create Failed");
        return nullptr;
    }

    return pInstance;
}
