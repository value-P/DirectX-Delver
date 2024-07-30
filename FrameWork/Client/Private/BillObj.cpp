#include "stdafx.h"
#include "BillObj.h"
#include "Export_Utility.h"

BillObj::BillObj(LPDIRECT3DDEVICE9 pGraphicDev)
    :GameObject(pGraphicDev)
{
}

BillObj::BillObj(const BillObj& rhs)
    :GameObject(rhs)
{
}

BillObj::~BillObj()
{
}

HRESULT BillObj::Init_GameObject()
{
    HRESULT result = GameObject::Init_GameObject();

    return result;
}

_int BillObj::Update_GameObject(const _float& fTimeDelta)
{
   _int iExit = GameObject::Update_GameObject(fTimeDelta);

    _matrix matWorld, matView, matBill;
    matWorld = *m_pTransformCom->Get_WorldMatrix();

    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._13 = matView._13;
    matBill._31 = matView._31;
    matBill._33 = matView._33;
    // 카메라의 y회전값
    D3DXMatrixInverse(&matBill, 0, &matBill);

    _matrix newMatWorld = matBill * matWorld;

    m_pTransformCom->Set_WorldMatrix(&newMatWorld);

    return iExit;
}

void BillObj::LateUpdate_GameObject()
{
    GameObject::LateUpdate_GameObject();

    Engine::Renderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

}

void BillObj::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT BillObj::Add_Component(const wstring& texturePath, _vec2 vInitScale)
{
    Component* pComponent = nullptr;

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL, texturePath);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    return S_OK;
}

BillObj* BillObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& texturePath, _vec2 vInitScale)
{
    BillObj* pInstance = new BillObj(pGraphicDev);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("BillObj Create Failed");
        return nullptr;
    }

    if (FAILED(pInstance->Add_Component(texturePath, vInitScale)))
    {
        Safe_Release(pInstance);

        MSG_BOX("BillObj Create Failed");
        return nullptr;
    }

    return pInstance;
}

void BillObj::Free()
{
    GameObject::Free();
}
