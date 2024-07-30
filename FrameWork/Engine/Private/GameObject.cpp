#include "..\..\Header\GameObject.h"
#include "Transform.h"

GameObject::GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_fViewZ(0.f)
{
	m_pGraphicDev->AddRef();
}

GameObject::GameObject(const GameObject & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_fViewZ(rhs.m_fViewZ)
{
	m_pGraphicDev->AddRef();
}

GameObject::~GameObject()
{
}

HRESULT GameObject::Init_GameObject()
{
	m_pTransformCom = Transform::Create(m_pGraphicDev);
	m_mapComponent[ID_DYNAMIC].emplace(TAG_TRANSFORM, m_pTransformCom);

	return S_OK;
}

_int GameObject::Update_GameObject(const _float & fTimeDelta)
{
	_int iExit = 0;
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iExit  = iter.second->Update_Component(fTimeDelta);

	return iExit;
}

void GameObject::LateUpdate_GameObject()
{
	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();
}

Component * GameObject::Find_Component(const wstring pComponentTag, COMPONENTID eID)
{
	auto	iter = m_mapComponent[eID].find(pComponentTag);

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

Component * GameObject::Get_Component(const wstring pComponentTag, COMPONENTID eID)
{
	Component *	pComponent = Find_Component(pComponentTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

void GameObject::Compute_ViewZ(const _vec3 * pPos)
{
	_matrix		matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, 0, &matCamWorld);

	_vec3	vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos - *pPos));

}

void GameObject::Free()
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}
