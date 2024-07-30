#include "stdafx.h"
#include "Breath.h"
#include "BossMonster2.h"

#include "Export_Utility.h"

Breath::Breath(LPDIRECT3DDEVICE9 pGraphicDev, BossMonster2* pOwner, Transform* pTargetTransform, const _float& fSpeed, const _int& iDamage)
    :BossSkillObject(pGraphicDev), m_pTargetTransform(pTargetTransform), m_pOwner(pOwner)
{
    m_fSpeed = fSpeed;
    m_iDamage = iDamage;
}

Breath::Breath(const Breath& rhs)
    :BossSkillObject(rhs), m_pTargetTransform(rhs.m_pTargetTransform), m_pOwner(rhs.m_pOwner)
{
    m_fSpeed = rhs.m_fSpeed;
    m_iDamage = rhs.m_iDamage;
}

Breath::~Breath()
{
}

HRESULT Breath::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetColLayer(COL_MONSTER_PROJ);
    m_pColliderCom->SetDamage(m_iDamage);

    m_tVertices[0].vTexUV = { 0.f,0.f };
    m_tVertices[1].vTexUV = { 1.f,0.f };
    m_tVertices[2].vTexUV = { 1.f,1.f };

    m_tVertices[3].vTexUV = { 0.f,0.f };
    m_tVertices[4].vTexUV = { 1.f,1.f };
    m_tVertices[5].vTexUV = { 0.f,1.f };

    _vec3 vTarget = m_pTargetTransform->Get_Info(INFO_POS);
    vTarget.y -= 0.9f;
    m_vPrevEndPos = vTarget;
    
    return iResult;
}

_int Breath::Update_GameObject(const _float& fTimeDelta)
{
    if (m_pOwner->IsLaserEnd())
        m_bAlive = false;

    LengthUpdate(fTimeDelta);

    _int iResult = __super::Update_GameObject(fTimeDelta);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
    Engine::Add_RenderGroup(RENDER_NONALPHA, this);

    return iResult;
}

void Breath::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
}

void Breath::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 200);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetFVF(FVF_TEX);
    m_pTextureCom->Set_Texture();
    m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, m_tVertices, sizeof(VTXTEX));

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

HRESULT Breath::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Effect/Boss/Laser.png");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = RcTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });


    return S_OK;
}

void Breath::LengthUpdate(const _float& fTimeDelta)
{
    // 길이 업데이트
    m_vStartPos = m_pOwner->Get_Transform()->Get_Info(INFO_POS);
    _vec3 ownerLook = m_pOwner->Get_Transform()->Get_Info(INFO_LOOK);
    m_vStartPos.y += 0.5f;
    m_vStartPos -= m_vStartDirection * 0.2f;

    _vec3 vTarget = m_pTargetTransform->Get_Info(INFO_POS);
    vTarget.y -= 0.9f;

    _vec3 lerpEndPos = vTarget * (1.f - 0.95f) + m_vPrevEndPos * 0.95f;

    m_vStartDirection = lerpEndPos - m_vStartPos;
    D3DXVec3Normalize(&m_vStartDirection, &m_vStartDirection);

    m_vPrevEndPos = lerpEndPos;

    m_fUVLength += fTimeDelta * m_fSpeed * 0.6f;
    if (m_fLength < 50.f) m_fLength += fTimeDelta * m_fSpeed;

    m_vEndPos = m_vStartPos + m_vStartDirection * m_fLength;

    _vec3 Normal = { m_vStartDirection.z, 0.f, -m_vStartDirection.x }; // y축과 수직으로 평평하게
    D3DXVec3Normalize(&Normal, &Normal);

    m_vPoints[0] = m_vStartPos + m_fThickness * 0.5f * Normal;
    m_vPoints[1] = m_vEndPos + m_fThickness * 0.5f * Normal;
    m_vPoints[2] = m_vStartPos - m_fThickness * 0.5f * Normal;
    m_vPoints[3] = m_vEndPos - m_fThickness * 0.5f * Normal;

    m_tVertices[0].vPosition = m_vPoints[3];
    m_tVertices[1].vPosition = m_vPoints[1];
    m_tVertices[2].vPosition = m_vPoints[0];

    m_tVertices[3].vPosition = m_vPoints[3];
    m_tVertices[4].vPosition = m_vPoints[0];
    m_tVertices[5].vPosition = m_vPoints[2];

    // UV업데이트
    m_tVertices[0].vTexUV = { 0.f,0.f };
    m_tVertices[1].vTexUV = { 1.f,0.f };
    m_tVertices[2].vTexUV = { 1.f,m_fUVLength };

    m_tVertices[3].vTexUV = { 0.f,0.f };
    m_tVertices[4].vTexUV = { 1.f,m_fUVLength };
    m_tVertices[5].vTexUV = { 0.f,m_fUVLength };

    // 충돌체 업데이트
    _vec3 vLook = m_vStartDirection;
    _vec3 vPos = m_vStartPos + vLook * m_fLength * 0.5f;      // 중점
    _vec3 vRight, vUp;
    D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
    D3DXVec3Cross(&vUp, &vLook, &vRight);

    m_pColliderCom->SetSize(0.25f, 0.25f, m_fLength * 0.5f);
    m_pColliderCom->SelfBuilding(vRight, vUp, vLook, vPos);
    Engine::AddCollider(m_pColliderCom);
}

Breath* Breath::Create(LPDIRECT3DDEVICE9 pGraphicDev, BossMonster2* pOwner, Transform* pTargetTransform, const _float& fSpeed, const _int& iDamage)
{
    Breath* pInstance = new Breath(pGraphicDev, pOwner, pTargetTransform, fSpeed, iDamage);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("Breath Create Failed");
        return nullptr;
    }

    return pInstance;
}

void Breath::Free()
{
    __super::Free();
}
