#include "stdafx.h"
#include "OrbitBullet.h"
#include "MagicEx.h"

#include "Export_Utility.h"

OrbitBullet::OrbitBullet(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pDir, const _float& fSpeed, const _int& iDamage)
    :BossSkillObject(pGraphicDev)
{
    m_vStartPos = *pPos;
    m_vStartDirection = *pDir;
    m_fSpeed = fSpeed;
    m_iDamage = iDamage;
}

OrbitBullet::~OrbitBullet()
{
}

HRESULT OrbitBullet::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(m_vStartPos);

    m_pColliderCom->SetColLayer(COL_MONSTER_PROJ);
    m_pColliderCom->SetDamage(m_iDamage);
    m_pColliderCom->SetThisPointer(this);
    m_pColliderCom->SetSize(0.5f, 0.5f, 0.5f);
    m_pColliderCom->SetTransform(m_pTransformCom);

    m_pParticles = PTrace::Create(m_pGraphicDev);
    if (nullptr == m_pParticles) { return E_FAIL; }
    PSMgr::GetInstance()->AddParticles(m_pParticles);
    m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 0, 199, 90));

    return iResult;

}

_int OrbitBullet::Update_GameObject(const _float& fTimeDelta)
{
    m_fLifeTime += fTimeDelta;
    if (m_fLifeTime > 10.f)
        return -1;

    m_pTransformCom->Move_Forward(&m_vStartDirection, fTimeDelta, m_fSpeed);

    m_fParticleGenerateTime += fTimeDelta;
    if (m_fParticleGenerateTime > 0.1f)
    {
        m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 139, 0, 190));
        m_fParticleGenerateTime = 0.0f;
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

    return iResult;
}

void OrbitBullet::LateUpdate_GameObject()
{
    On_Collided();
    __super::LateUpdate_GameObject();
    Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void OrbitBullet::Render_GameObject()
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

HRESULT OrbitBullet::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL,
        L"../../Resource/Texture/Effect/Boss/Charge/Charge0.png");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

void OrbitBullet::On_Collided()
{
    if (m_pColliderCom->GetColType() & COL_ENVIRONMENT)
    {
        m_bAlive = false;
    }
       
    if (m_pColliderCom->GetColType() & COL_PLAYER)
    {
        m_bAlive = false;
        // ÆøÅº Á¦Á¶
        MagicEx* magic = MagicEx::Create(m_pGraphicDev, MAGIC_EX_2, m_pTransformCom->Get_Info(INFO_POS), m_iDamage);
        Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
            Add_GameObject(L"MagicEx", magic);
    }
}

OrbitBullet* OrbitBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos, _vec3* pDir, const _float& fSpeed, const _int& iDamage)
{
    OrbitBullet* pInstance = new OrbitBullet(pGraphicDev, pPos, pDir, fSpeed, iDamage);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("OrbitBullet Create Failed");
        return nullptr;
    }

    return pInstance;

}

void OrbitBullet::Free()
{
    __super::Free();
}
