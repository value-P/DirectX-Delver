#include "stdafx.h"
#include "SpacialRend.h"
#include "Export_Utility.h"

SpacialRend::SpacialRend(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartDir, const _float& fSpeed)
    :GameObject(pGraphicDev), m_vStartDirection(*pStartDir), m_fSpeed(fSpeed)
{
}

SpacialRend::SpacialRend(const SpacialRend& rhs)
    :GameObject(rhs), m_vStartDirection(rhs.m_vStartDirection), m_fSpeed(rhs.m_fSpeed)
{
}

SpacialRend::~SpacialRend()
{
}

HRESULT SpacialRend::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return iResult;
}

_int SpacialRend::Update_GameObject(const _float& fTimeDelta)
{
    m_fLifeTime += fTimeDelta;
    if (m_fLifeTime > 10.f)
        return -1;

    m_pTransformCom->Move_Forward(&m_vStartDirection, fTimeDelta, m_fSpeed);

    _int iResult = __super::Update_GameObject(fTimeDelta);

    _matrix matWorld, matView, matBill;
    matWorld = *m_pTransformCom->Get_WorldMatrix();

    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._13 = matView._13;
    matBill._31 = matView._31;
    matBill._33 = matView._33;

    D3DXMatrixInverse(&matBill, 0, &matBill);

    // ºô * (½º * ÀÚ * ÀÌ)

    _matrix newMatWorld = matBill * matWorld;

    m_pTransformCom->Set_WorldMatrix(&newMatWorld);

    Engine::Add_RenderGroup(RENDER_NONALPHA, this);

    return iResult;
}

void SpacialRend::LateUpdate_GameObject()
{
    On_Collided();
    __super::LateUpdate_GameObject();
}

void SpacialRend::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 150);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT SpacialRend::Add_Component()
{
    Component* pComponent = nullptr;

    if(m_bIsHorizontal)
        pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Effect/Boss/Effect3.png");
    else
        pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Effect/Boss/Effect6.png");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

void SpacialRend::On_Collided()
{
}

SpacialRend* SpacialRend::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartDir, const _float& fSpeed, _bool isHorizontal)
{

    SpacialRend* pInstance = new SpacialRend(pGraphicDev, pStartDir, fSpeed);

    pInstance->m_bIsHorizontal = isHorizontal;

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("SpacialRend Create Failed");
        return nullptr;
    }

    return pInstance;
}

void SpacialRend::Free()
{
    __super::Free();
}
