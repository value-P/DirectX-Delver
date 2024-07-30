#include "stdafx.h"
#include "ToTemBomb.h"

#include "Export_Utility.h"

ToTemBomb::ToTemBomb(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos)
    :BossSkillObject(pGraphicDev)
{
    m_vStartPos = *pPos;
}

ToTemBomb::~ToTemBomb()
{
}

HRESULT ToTemBomb::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Pos(m_vStartPos);

    return iResult;
}

_int ToTemBomb::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bAlive) { return -1; }
    m_pTransformCom->Rotation(ROT_X, fTimeDelta * 3.f);
    m_pTransformCom->Rotation(ROT_Y, fTimeDelta * 3.f);

    if (InitScale < 7.f)
    {
        InitScale += fTimeDelta;
        m_pTransformCom->Set_Scale(InitScale, InitScale, InitScale);
    }
    

    _int iResult = __super::Update_GameObject(fTimeDelta);

    return iResult;
}

void ToTemBomb::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
    Engine::Add_RenderGroup(RENDER_NONALPHA, this);
}

void ToTemBomb::Render_GameObject()
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

void ToTemBomb::Bomb()
{
    m_bAlive = false;

    // 파티클 제조
    PSystem* pPSystem = PSpread::Create(m_pGraphicDev);
    if (nullptr == pPSystem) { return; }
    PSMgr::GetInstance()->AddParticles(pPSystem);
    for (int i = 0; i < 16; i++)
    {
        pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 211, 0, 0));
    }
}

void ToTemBomb::Destroy_Bomb()
{
    m_bAlive = false;

    // 파티클 제조
    PSystem* pPSystem = PSpread::Create(m_pGraphicDev);
    if (nullptr == pPSystem) { return; }
    PSMgr::GetInstance()->AddParticles(pPSystem);
    for (int i = 0; i < 16; i++)
    {
        pPSystem->AddParticle(m_pTransformCom->Get_Info(INFO_POS), D3DCOLOR_ARGB(255, 0, 25, 150));
    }
}

HRESULT ToTemBomb::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_CUBE,
        L"../../Resource/Texture/Cube/SpaceEye.dds");
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_TEXTURE, pComponent });

    pComponent = m_pBufferCom = CubeTex::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ TAG_VIBUFFER, pComponent });

    return S_OK;
}

ToTemBomb* ToTemBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* pPos)
{
    ToTemBomb* pInstance = new ToTemBomb(pGraphicDev, pPos);

    if (FAILED(pInstance->Init_GameObject()))
    {
        Safe_Release(pInstance);

        MSG_BOX("ToTemBomb Create Failed");
        return nullptr;
    }

    return pInstance;
}

void ToTemBomb::Free()
{
    __super::Free();
}
