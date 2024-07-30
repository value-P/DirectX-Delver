#include "stdafx.h"
#include "MapObject.h"
#include "Export_Utility.h"

MapObject::MapObject(LPDIRECT3DDEVICE9 pGraphicDev)
    :GameObject(pGraphicDev)
{
}

MapObject::MapObject(const MapObject& rhs)
    :GameObject(rhs)
{
}

MapObject::~MapObject()
{
}

HRESULT MapObject::Init_GameObject()
{
    HRESULT result = GameObject::Init_GameObject();

    return result;
}

_int MapObject::Update_GameObject(const _float& fTimeDelta)
{
    return GameObject::Update_GameObject(fTimeDelta);
}

void MapObject::LateUpdate_GameObject()
{
    GameObject::LateUpdate_GameObject();
    Engine::Renderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
}

void MapObject::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT MapObject::Add_Component(const wstring& texturePath, _vec2 vInitScale, PREFABTYPE eType)
{
    Component* pComponent = nullptr;

    switch (eType)
    {
    case Engine::PREFAB_RCTEX:    
        pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
        NULL_CHECK_RETURN(pComponent, E_FAIL);
        m_pBufferCom->Set_UVScale(vInitScale.x, vInitScale.y);
        m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

        pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL,texturePath);
        NULL_CHECK_RETURN(pComponent, E_FAIL);
        m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

        break;
    case Engine::PREFAB_CUBE:
        pComponent = m_pBufferCom = CubeTex::Create(m_pGraphicDev);
        NULL_CHECK_RETURN(pComponent, E_FAIL);
        m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

        pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_CUBE, texturePath);
        NULL_CHECK_RETURN(pComponent, E_FAIL);
        m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });
        break;
    }


    return S_OK;

}

MapObject* MapObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& texturePath, _vec2 vInitScale, PREFABTYPE eType)
{
    MapObject* pInstance = new MapObject(pGraphicDev);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("MapObject Create Failed");
        return nullptr;
    }

    if (FAILED(pInstance->Add_Component(texturePath, vInitScale, eType)))
    {
        Safe_Release(pInstance);

        MSG_BOX("MapObject Create Failed");
        return nullptr;
    }

    return pInstance;
}

void MapObject::Free()
{
    GameObject::Free();
}
