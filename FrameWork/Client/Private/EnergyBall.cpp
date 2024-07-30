#include "stdafx.h"
#include "EnergyBall.h"
#include "Export_Utility.h"

EnergyBall::EnergyBall(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartDir, Transform* pTargetTransform, const _int& iDamage, const _float& fSpeed)
    :BossSkillObject(pGraphicDev), m_pTargetTransform(pTargetTransform)
{
    m_vStartDirection = *pStartDir;
    m_fSpeed = fSpeed;
    m_iDamage = iDamage;
}

EnergyBall::EnergyBall(const EnergyBall& rhs)
    :BossSkillObject(rhs), m_pTargetTransform(rhs.m_pTargetTransform)
{
    m_vStartDirection = rhs.m_vStartDirection;
    m_fSpeed = rhs.m_fSpeed;
    m_iDamage = rhs.m_iDamage;
}

EnergyBall::~EnergyBall()
{
}

HRESULT EnergyBall::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pColliderCom->SetColLayer(COL_MONSTER_PROJ);
    m_pColliderCom->SetDamage(m_iDamage);
    m_pColliderCom->SetSize(0.5f, 0.5f, 0.5f);

    m_pParticles = PTrace::Create(m_pGraphicDev);
    if (nullptr == m_pParticles) { return E_FAIL; }
    PSMgr::GetInstance()->AddParticles(m_pParticles);
    m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 139, 0, 190));

    return iResult;
}

_int EnergyBall::Update_GameObject(const _float& fTimeDelta)
{
    m_fLifeTime += fTimeDelta;
    if (m_fLifeTime > 10.f)
        return -1;

    ControlDir(fTimeDelta);

    m_pTransformCom->Move_Forward(&m_vStartDirection, fTimeDelta, m_fSpeed);

    m_pTransformCom->Rotation(ROT_X, fTimeDelta);
    m_pTransformCom->Rotation(ROT_Y, fTimeDelta);

    m_fParticleGenerateTime += fTimeDelta;
    if (m_fParticleGenerateTime > 0.1f)
    {
        m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 250, 0, 0));
        m_fParticleGenerateTime = 0.0f;
    }

    _int iResult =  __super::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_NONALPHA, this);

    m_pColliderCom->ReBuild();

    Engine::AddCollider(m_pColliderCom);

    return iResult;
}

void EnergyBall::LateUpdate_GameObject()
{
    On_Collided();
    __super::LateUpdate_GameObject();
}

void EnergyBall::Render_GameObject()
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

HRESULT EnergyBall::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/Cube/EnergyBall1.dds");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = CubeTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

void EnergyBall::On_Collided()
{
    if (m_pColliderCom->GetColType() & COL_ENVIRONMENT || m_pColliderCom->GetColOBB())
    {
        m_bAlive = false;
    }
}

void EnergyBall::ControlDir(const _float& fTimeDelta)
{
    if (m_pTargetTransform && !m_bIsModified)
    {
        m_fControlTerm += fTimeDelta;
        if (m_fControlTerm > 1.f)
        {

            _vec3 TargetPos, myPos;
            m_pTargetTransform->Get_Info(INFO_POS, &TargetPos);
            m_pTransformCom->Get_Info(INFO_POS, &myPos);
            _vec3 TargetDir = TargetPos - myPos;
            D3DXVec3Normalize(&TargetDir, &TargetDir);

            m_vStartDirection = TargetDir;

            m_fSpeed *= 7.f;

            m_bIsModified = true;

            m_fControlTerm = 0.f;
        }
    }

}

EnergyBall* EnergyBall::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartDir, Transform* pTargetTransform, const _int& iDamage, const _float& fSpeed)
{
    EnergyBall* pInstance = new EnergyBall(pGraphicDev, pStartDir, pTargetTransform, iDamage, fSpeed);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("EnergyBall Create Failed");
        return nullptr;
    }

    return pInstance;
}

void EnergyBall::Free()
{
    __super::Free();
}
