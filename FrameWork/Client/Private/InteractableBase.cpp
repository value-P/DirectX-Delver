#include "stdafx.h"
#include "InteractableBase.h"

#include "Export_Utility.h"

InteractableBase::InteractableBase(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

InteractableBase::InteractableBase(const InteractableBase& rhs)
	: Super(rhs)
{
}

InteractableBase::~InteractableBase()
{
}

HRESULT InteractableBase::Init_GameObject()
{
	Super::Init_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int InteractableBase::Update_GameObject(const _float& fTimeDelta)
{
	_int	iExit = Super::Update_GameObject(fTimeDelta);

	return iExit;
}

void InteractableBase::LateUpdate_GameObject()
{
	Super::LateUpdate_GameObject();
}

void InteractableBase::Render_GameObject()
{
}

HRESULT InteractableBase::Add_Component()
{
	Component* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<RcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Collider",pComponent });
	m_pColliderCom->SetTransform(m_pTransformCom);

	pComponent = m_pColliderWallCom = dynamic_cast<Collider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_WallCollider",pComponent });
	m_pColliderWallCom->SetTransform(m_pTransformCom);

	return S_OK;
}

void InteractableBase::Free()
{
	Super::Free();
}
