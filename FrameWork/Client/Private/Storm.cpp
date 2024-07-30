#include "stdafx.h"
#include "Storm.h"

#include "Export_Utility.h"
#include "Export_System.h"

Storm::Storm(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fLifeTime, const _int& iDamage)
    :BossSkillObject(pGraphicDev), m_fLifeTime(fLifeTime)
{
    m_iDamage = iDamage;
}

Storm::~Storm()
{
}

HRESULT Storm::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Rotation(&_vec3(D3DXToRadian(90.f), 0.f, 0.f));
    m_pTransformCom->Set_Scale(7.f, 7.f, 1.f);

    m_pColliderCom->SetTransform(m_pTransformCom);
    m_pColliderCom->SetSize(7.f, 7.f,0.2f);
    m_pColliderCom->SetColLayer(COL_MONSTER_PROJ);
    m_pColliderCom->SetThisPointer(this);
    m_pColliderCom->SetDamage(m_iDamage);

    Engine::Play_Sound_NoInterval(L"Storm", SOUND_EFFECT_2, 0.5f);

    return iResult;
}

_int Storm::Update_GameObject(const _float& fTimeDelta)
{
    m_fLifeTime -= fTimeDelta;
    if (m_fLifeTime <= 0.f)
        m_bAlive = false;

    m_pTransformCom->Rotation(ROT_Y, fTimeDelta * 2.f);

    _int iResult = __super::Update_GameObject(fTimeDelta);

    m_pColliderCom->ReBuild();
    Engine::AddCollider(m_pColliderCom);
    return iResult;
}

void Storm::LateUpdate_GameObject()
{
    OnCollided();
    __super::LateUpdate_GameObject();
    Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void Storm::Render_GameObject()
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

HRESULT Storm::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL,
        L"../../Resource/Texture/Effect/Boss/Storm.png");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

void Storm::On_Collided()
{
}

Storm* Storm::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _float& fLifeTime, const _int& iDamage)
{
    Storm* pInstance = new Storm(pGraphicDev, fLifeTime, iDamage);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("ShootingStar Create Failed");
        return nullptr;
    }

    return pInstance;
}

void Storm::Free()
{
    __super::Free();
}
