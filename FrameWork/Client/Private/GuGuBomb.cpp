#include "stdafx.h"
#include "GuGuBomb.h"
#include "MagicEx.h"

#include "Export_Utility.h"
#include "Export_System.h"

GuGuBomb::GuGuBomb(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, const _float& fSpeed, const _int& iDamage)
    :BossSkillObject(pGraphicDev), m_pTarget(pTarget)
{
    m_fSpeed = fSpeed;
    m_iDamage = iDamage;
}

GuGuBomb::~GuGuBomb()
{
}

HRESULT GuGuBomb::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetColLayer(COL_MONSTER);
    m_pColliderCom->SetDamage(m_iDamage);
    m_pColliderCom->SetThisPointer(this);
    m_pColliderCom->SetSize(1.f, 1.f, 1.f);
    m_pColliderCom->SetTransform(m_pTransformCom);

    Engine::Play_Sound_NoInterval(L"GuGu", SOUND_EFFECT_3, 0.5f);

    return iResult;
}

_int GuGuBomb::Update_GameObject(const _float& fTimeDelta)
{
    _vec3 myPos = m_pTransformCom->Get_Info(INFO_POS);
    _vec3 TargetPos = m_pTarget->Get_Info(INFO_POS);

    _vec3 TargetDir = TargetPos - myPos;
    _float Dist = D3DXVec3Length(&TargetDir);

    D3DXVec3Normalize(&TargetDir, &TargetDir);
    _vec3 vRight, vUp;
    D3DXVec3Cross(&vRight, &TargetDir, &_vec3(0.f, 1.f, 0.f));
    D3DXVec3Cross(&vUp, &TargetDir, &vRight);

    m_pTransformCom->Move_Forward(&TargetDir, fTimeDelta, m_fSpeed);

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

void GuGuBomb::LateUpdate_GameObject()
{
    On_Collided();
    __super::LateUpdate_GameObject();
    Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void GuGuBomb::Render_GameObject()
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

HRESULT GuGuBomb::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = AnimationTexture::Create(m_pGraphicDev,
        L"../../Resource/Texture/Effect/Boss/GuGu/GuGu_%d.png",2,2.f);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

void GuGuBomb::On_Collided()
{
    _vec3 myPos = m_pTransformCom->Get_Info(INFO_POS);
    _vec3 TargetPos = m_pTarget->Get_Info(INFO_POS);
    _vec3 TargetDir = TargetPos - myPos;
    _float Dist = D3DXVec3Length(&TargetDir);


    if (m_pColliderCom->GetColOBB() & COL_PLAYERATTACK ||
        m_pColliderCom->GetColOBB() & COL_PLAYER_PROJ ||
        m_pColliderCom->GetColSphere() & COL_EXPLOSION ||
        Dist < 1.f)
    {
        // »ç¸Á
        m_bAlive = false;

        // ÆÄÆ¼Å¬ Á¦Á¶
        PSystem* pPSystem = PSpread::Create(m_pGraphicDev);
        if (nullptr == pPSystem) { return; }
        PSMgr::GetInstance()->AddParticles(pPSystem);
        for (int i = 0; i < 16; i++)
        {
            pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 211, 0, 0));
        }
        
        // ÆøÅº Á¦Á¶
        MagicEx* magic = MagicEx::Create(m_pGraphicDev, MAGIC_EX_2, m_pTransformCom->Get_Info(INFO_POS), m_iDamage);
        Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
            Add_GameObject(L"MagicEx", magic);
    }

}

GuGuBomb* GuGuBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, const _float& fSpeed, const _int& iDamage)
{
    GuGuBomb* pInstance = new GuGuBomb(pGraphicDev, pTarget, fSpeed, iDamage);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("GuGuBomb Create Failed");
        return nullptr;
    }

    return pInstance;
}

void GuGuBomb::Free()
{
    __super::Free();
}
