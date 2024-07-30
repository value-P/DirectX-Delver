#include "stdafx.h"
#include "ShootingStar.h"

#include "Export_Utility.h"

ShootingStar::ShootingStar(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, const _float& fSpeed, const _int& iDamage)
    :BossSkillObject(pGraphicDev), m_pTarget(pTarget)
{
    m_fSpeed = fSpeed;
    m_iDamage = iDamage;
}

ShootingStar::ShootingStar(const ShootingStar& rhs)
    :BossSkillObject(rhs), m_pTarget(rhs.m_pTarget)
{
    m_fSpeed = rhs.m_fSpeed;
    m_iDamage = rhs.m_iDamage;
}

ShootingStar::~ShootingStar()
{
}

HRESULT ShootingStar::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetColLayer(COL_MONSTER_PROJ);
    m_pColliderCom->SetDamage(m_iDamage);
    m_pColliderCom->SetThisPointer(this);
    m_pColliderCom->SetSize(1.f, 1.f, 1.f);
    m_pColliderCom->SetTransform(m_pTransformCom);

    m_pParticles = PTrace::Create(m_pGraphicDev);
    if (nullptr == m_pParticles) { return E_FAIL; }
    PSMgr::GetInstance()->AddParticles(m_pParticles);
    m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 139, 0, 190));

    return iResult;
}

_int ShootingStar::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_pTarget)
        return -1;

    if (m_bOnShoot)
    {
        m_fLifeTime += fTimeDelta;

        if (m_fLifeTime > 10.f)
            return -1;
       
        m_pTransformCom->Move_Forward(&m_vStartDirection, fTimeDelta, m_fSpeed);
    }

    m_fParticleGenerateTime += fTimeDelta;
    if (m_fParticleGenerateTime > 0.1f)
    {
        m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 139, 0, 190));
        m_fParticleGenerateTime = 0.0f;
    }

    _int iResult = __super::Update_GameObject(fTimeDelta);
    if (iResult == -1) return iResult;

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

    Engine::Add_RenderGroup(RENDER_NONALPHA, this);

    return iResult;

}

void ShootingStar::LateUpdate_GameObject()
{
    On_Collided();
    __super::LateUpdate_GameObject();
}

void ShootingStar::Render_GameObject()
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

void ShootingStar::Shoot()
{
    if (m_pTarget && m_pTransformCom)
    {
        _vec3 TargetPos, myPos;
        m_pTarget->Get_Info(INFO_POS, &TargetPos);
        m_pTransformCom->Get_Info(INFO_POS, &myPos);
        m_vStartDirection = TargetPos - myPos;
        D3DXVec3Normalize(&m_vStartDirection, &m_vStartDirection);

        m_bOnShoot = true;
    }
    else
    {
        Destroy();
    }
}

HRESULT ShootingStar::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev,TEX_NORMAL,
        L"../../Resource/Texture/Effect/Boss/ShootingStar/ShootingStar_0.png");
    //pTex->SetFrameSpeed(5.f);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

void ShootingStar::On_Collided()
{
    if (m_pColliderCom->GetColType() & COL_ENVIRONMENT || m_pColliderCom->GetColOBB())
    {
        m_bAlive = false;
    }
}

ShootingStar* ShootingStar::Create(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, const _float& fSpeed, const _int& iDamage)
{
    ShootingStar* pInstance = new ShootingStar(pGraphicDev, pTarget, fSpeed, iDamage);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("ShootingStar Create Failed");
        return nullptr;
    }

    return pInstance;

}

void ShootingStar::Free()
{
    __super::Free();
}
