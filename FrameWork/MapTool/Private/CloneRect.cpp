#include "pch.h"
#include "CloneRect.h"
#include "Export_Utility.h"

CloneRect::CloneRect(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vInitPos, _vec2 vInitScale, const wstring& szTexturePath, const wstring& szOriginName)
    :GameObject(pGraphicDev), m_szTexturePath(szTexturePath), m_vInitPos(vInitPos), m_vInitScale(vInitScale)
{
    m_szOriginName = szOriginName;
}

CloneRect::CloneRect(const CloneRect& rhs)
    :GameObject(rhs), m_szTexturePath(rhs.m_szTexturePath), m_vInitPos(rhs.m_vInitPos), m_vInitScale(rhs.m_vInitScale)
{
    m_szOriginName = rhs.m_szOriginName;
}

CloneRect::~CloneRect()
{
}   

HRESULT CloneRect::Init_GameObject()
{
    HRESULT result = GameObject::Init_GameObject();
    m_pTransformCom->Set_Pos(m_vInitPos.x, m_vInitPos.y, m_vInitPos.z);

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return result;
}

_int CloneRect::Update_GameObject(const _float& fTimeDelta)
{
    return GameObject::Update_GameObject(fTimeDelta);
}

void CloneRect::LateUpdate_GameObject()
{
    GameObject::LateUpdate_GameObject();
}

void CloneRect::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CloneRect::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pBufferCom->Set_UVScale(m_vInitScale.x, m_vInitScale.y);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL, m_szTexturePath);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    return S_OK;
}

CloneRect* CloneRect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vInitPos, _vec2 vInitScale, const wstring& szTexturePath, const wstring& szOriginName)
{
    CloneRect* pInstance = new CloneRect(pGraphicDev, vInitPos, vInitScale,szTexturePath, szOriginName);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("CloneRect Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CloneRect::Free()
{
    __super::Free();
}
