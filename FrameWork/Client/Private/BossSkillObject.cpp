#include "stdafx.h"
#include "BossSkillObject.h"
#include "Export_Utility.h"

BossSkillObject::BossSkillObject(LPDIRECT3DDEVICE9 pGraphicDev)
    :GameObject(pGraphicDev)
{
}

BossSkillObject::BossSkillObject(const BossSkillObject& rhs)
    :GameObject(rhs)
{
}

BossSkillObject::~BossSkillObject()
{
}

HRESULT BossSkillObject::Init_GameObject()
{
    HRESULT iResult = __super::Init_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return iResult;
}

_int BossSkillObject::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bAlive) return -1;

    _int iResult = __super::Update_GameObject(fTimeDelta);

    return iResult;
}

void BossSkillObject::LateUpdate_GameObject()
{
    __super::LateUpdate_GameObject();
}

void BossSkillObject::Render_GameObject()
{
}

HRESULT BossSkillObject::Add_Component()
{
    Component* pComponent = nullptr;

    pComponent = m_pColliderCom = Collider::Create(m_pGraphicDev);
    m_pColliderCom->SetTransform(m_pTransformCom);
    m_mapComponent[ID_STATIC].insert({ TAG_COLLIDER, pComponent });

    return S_OK;
}

void BossSkillObject::On_Collided()
{
}

void BossSkillObject::Free()
{
    __super::Free();
}
