#include "stdafx.h"
#include "BossPortal.h"

#include "Export_Utility.h"
#include "Export_System.h"

BossPortal::BossPortal(LPDIRECT3DDEVICE9 _pGraphicDevice, Transform* pPlayer, _vec3* pPos)
    :GameObject(_pGraphicDevice), m_pPlayer(pPlayer), m_StartPos(*pPos)
{
}

BossPortal::~BossPortal()
{
}

HRESULT BossPortal::Init_GameObject()
{
    __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    Engine::Play_Sound_NoInterval(L"PortalOpen", SOUND_INTERACTION, 1.f);
    
    m_pTransformCom->Set_Pos(m_StartPos);

    m_pColliderCom->SetSize(1.f, 1.0f, 0.2f);
    m_pColliderCom->SetColLayer(COL_INTERACTION);
    m_pColliderCom->SetThisPointer(this);
    m_pColliderCom->SetTransform(m_pTransformCom);

    m_pTransformCom->Set_Scale(2.f, 0.f, 2.f);

    return S_OK;
}

_int BossPortal::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bAlive) return -1;

    if (!m_bActivated)
    {
        m_fScaleY += fTimeDelta;
        
        if (m_fScaleY > 2.f)
        {
            m_fScaleY = 2.f;
            m_bActivated = true;
        }
        
        m_pTransformCom->Set_Scale(2.f, m_fScaleY, 2.f);
    }

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

    m_pColliderCom->ReBuild();
    Engine::AddCollider(m_pColliderCom);

    Compute_ViewZ(&m_pTransformCom->Get_Info(INFO_POS));
    Engine::Add_RenderGroup(RENDER_ALPHA, this);

    return iResult;

}

void BossPortal::LateUpdate_GameObject()
{
    if(m_bActivated) OnCollided();
    __super::LateUpdate_GameObject();
}

void BossPortal::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT BossPortal::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL,
        L"../../Resource/Texture/Effect/Boss/portal.png");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    pComponent = m_pColliderCom = Collider::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_COLLIDER,pComponent });

    return S_OK;

}

void BossPortal::Free()
{
    __super::Free();
}

void BossPortal::OnCollided()
{
    if (m_pColliderCom->GetColSphere() & COL_PLAYER)
    {
        if (m_pPlayer)
        {
            m_pPlayer->Set_Pos(-70.f, 24.f, 118.f);
            m_bAlive = false;
        }
    }
}

BossPortal* BossPortal::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, Transform* pPlayer, _vec3* pPos)
{
    BossPortal* pInstance = new BossPortal(_pGraphicDevice, pPlayer, pPos);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("BossPortal Create Failed");
        return nullptr;
    }

    return pInstance;
}
