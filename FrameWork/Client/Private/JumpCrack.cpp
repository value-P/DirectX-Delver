#include "stdafx.h"
#include "JumpCrack.h"
#include "Export_Utility.h"

JumpCrack::JumpCrack(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _int& iDamage)
    :BossSkillObject(pGraphicDev)
{
    m_vStartPos = *pPos;
    m_iDamage = iDamage;
}

JumpCrack::JumpCrack(const JumpCrack& rhs)
    :BossSkillObject(rhs)
{
    m_vStartPos = rhs.m_vStartPos;
    m_iDamage = rhs.m_iDamage;
}

JumpCrack::~JumpCrack()
{
}

HRESULT JumpCrack::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(m_vStartPos);
    m_pColliderCom->SetColLayer(COL_MONSTER_PROJ);
    m_pColliderCom->SetSize(3.5f, 3.5f, 0.3f);

    m_pColliderCom->SetDamage(m_iDamage);

    return iResult;
}

_int JumpCrack::Update_GameObject(const _float& fTimeDelta)
{
    m_fLifeTime += fTimeDelta;
    if (m_fLifeTime > 5.f)
        return -1;
    _int iResult = __super::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_NONALPHA, this);

    // 0.2초만 충돌 활성화
    if (m_fLifeTime > 0.2f)
    {
        m_pColliderCom->ReBuild();
        Engine::AddCollider(m_pColliderCom);
    }

    return iResult;
}

void JumpCrack::LateUpdate_GameObject()
{
    On_Collided();
    __super::LateUpdate_GameObject();
}

void JumpCrack::Render_GameObject()
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

HRESULT JumpCrack::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Effect/Boss/Crack.png");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

void JumpCrack::On_Collided()
{
}

JumpCrack* JumpCrack::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _int& iDamage)
{
    JumpCrack* pInstance = new JumpCrack(pGraphicDev, pPos, iDamage);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("JumpCrack Create Failed");
        return nullptr;
    }

    return pInstance;
}

void JumpCrack::Free()
{
    __super::Free();
}
