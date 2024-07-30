#include "stdafx.h"
#include "Meteor.h"
#include "MagicEx.h"
#include "Export_Utility.h"

Meteor::Meteor(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartPos, const _float& fSpeed, const _int& iDamge)
    :BossSkillObject(pGraphicDev)
{
    m_vStartPos = *pStartPos;
    m_fSpeed = fSpeed;
    m_iDamage = iDamge;
}

Meteor::Meteor(const Meteor& rhs)
    :BossSkillObject(rhs)
{
    m_vStartPos = rhs.m_vStartPos;
    m_fSpeed    = rhs.m_fSpeed;
    m_iDamage   = rhs.m_iDamage;
}

Meteor::~Meteor()
{
}

HRESULT Meteor::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(m_vStartPos);
    m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);
    m_pColliderCom->SetColLayer(COL_MONSTER_PROJ);

    m_pColliderCom->SetDamage(m_iDamage);

    m_pParticles = PTrace::Create(m_pGraphicDev);
    if (nullptr == m_pParticles) { return E_FAIL; }
    PSMgr::GetInstance()->AddParticles(m_pParticles);
    m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255,250,0,0));

    return iResult;
}

_int Meteor::Update_GameObject(const _float& fTimeDelta)
{
    m_pTransformCom->Move_Forward(&_vec3(0.f, -1.f, 0.f), fTimeDelta, m_fSpeed);

    m_pTransformCom->Rotation(ROT_X, fTimeDelta * 2.f);
    m_pTransformCom->Rotation(ROT_Y, fTimeDelta * 2.f);
    //m_pTransformCom->Rotation(ROT_Z, fTimeDelta * 2.f);

    m_fParticleGenerateTime += fTimeDelta;
    if (m_fParticleGenerateTime > 0.1f)
    {
        m_pParticles->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 250, 0, 0));
        m_fParticleGenerateTime = 0.0f;
    }

    _int iResult = __super::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_NONALPHA, this);

    m_pColliderCom->ReBuild();

    Engine::AddCollider(m_pColliderCom);

    return iResult;
}

void Meteor::LateUpdate_GameObject()
{
    On_Collided();
    __super::LateUpdate_GameObject();
}

void Meteor::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT Meteor::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Cube/Meteor.dds");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = CubeTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

void Meteor::On_Collided()
{
    if (m_pColliderCom->GetColType() & COL_ENVIRONMENT || m_pColliderCom->GetColOBB())
    {
        m_bAlive = false;
        
        // ÆøÅº Á¦Á¶
        MagicEx* magic = MagicEx::Create(m_pGraphicDev, MAGIC_EX_2, m_pTransformCom->Get_Info(INFO_POS), m_iDamage);
        magic->Get_Transform()->Set_Scale(3.f, 3.f, 3.f);
        Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
            Add_GameObject(L"MagicEx", magic);
    }
}

Meteor* Meteor::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pStartDir, const _float& fSpeed, const _int& iDaamge)
{
    Meteor* pInstance = new Meteor(pGraphicDev, pStartDir, fSpeed, iDaamge);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("SpacialRend Create Failed");
        return nullptr;
    }

    return pInstance;
}

void Meteor::Free()
{
    __super::Free();
}
