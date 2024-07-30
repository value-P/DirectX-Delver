#include "stdafx.h"
#include "Orbit.h"
#include "OrbitBullet.h"

#include "Export_Utility.h"
#include "Export_System.h"

Orbit::Orbit(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, _vec3* pPos, const _float& fSpeed, const _int& iDamage)
    :BossSkillObject(pGraphicDev), m_pTarget(pTarget)
{
    m_fSpeed = fSpeed;
    m_iDamage = iDamage;
    m_vStartPos = *pPos;
}

Orbit::~Orbit()
{
}

HRESULT Orbit::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(m_vStartPos);
    m_pTransformCom->Set_Scale(2.f, 2.f, 2.f);

    return iResult;
}

_int Orbit::Update_GameObject(const _float& fTimeDelta)
{
    _vec3 myPos = m_pTransformCom->Get_Info(INFO_POS);
    _vec3 TargetPos = m_pTarget->Get_Info(INFO_POS);
    _vec3 vDir = TargetPos - myPos;
    D3DXVec3Normalize(&vDir, &vDir);
    
    _vec3 Pos = myPos + vDir * 0.5f;                           

    m_fShootTime += fTimeDelta;
    if (m_fShootTime > 0.8f)
    {
        Engine::Play_Sound_NoInterval(L"OribtBullet", SOUND_EFFECT_3, 0.5f);
        m_fShootTime = 0.f;

        OrbitBullet* pBullet = OrbitBullet::Create(m_pGraphicDev, &Pos, &vDir, 13.f, 1);
        Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
            Add_GameObject(L"OrbitBullet", pBullet);

        m_iShootCnt++;
    }
    
    if (m_iShootCnt > 5)
        m_bAlive = false;

    if (!m_bAlive) { return -1; }
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

    return iResult;
}

void Orbit::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
    Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void Orbit::Render_GameObject()
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

HRESULT Orbit::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev,TEX_NORMAL,
        L"../../Resource/Texture/Effect/Boss/EnergyHole.png");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

Orbit* Orbit::Create(LPDIRECT3DDEVICE9 pGraphicDev, Transform* pTarget, _vec3* pPos, const _float& fSpeed, const _int& iDamage)
{
    Orbit* pInstance = new Orbit(pGraphicDev, pTarget, pPos, fSpeed, iDamage);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("Orbit Create Failed");
        return nullptr;
    }

    return pInstance;

}

void Orbit::Free()
{
    __super::Free();
}
