#include "Scene.h"

Scene::Scene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Scene::~Scene()
{
}

Component * Scene::Get_Component(const wstring pLayerTag, const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID)
{
	auto	iter = m_mapLayer.find(pLayerTag);

	if (iter == m_mapLayer.end())
		return nullptr;
	
	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

Layer* Scene::Get_Layer(const wstring pLayerTag)
{
	auto it = m_mapLayer.find(pLayerTag);

	if (it == m_mapLayer.end())
		return nullptr;

	return it->second;
}

vector<GameObject*>* Scene::Get_GameObject(const wstring pLayerTag, const wstring pObjTag)
{
	auto it = m_mapLayer.find(pLayerTag);

	if (it == m_mapLayer.end())
		return nullptr;

	return it->second->Get_GameObject(pObjTag);
}

HRESULT Scene::Init_Scene()
{
	return S_OK;
}

_int Scene::Update_Scene(const _float & fTimeDelta)
{
	_int		iResult(0);

	for (auto& iter : m_mapLayer)
	{
		iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void Scene::LateUpdate_Scene()
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate_Layer();
}

void Scene::Free()
{
	//for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	for (map<const wstring, Layer*>::iterator iter = m_mapLayer.begin(); iter != m_mapLayer.end(); iter++)
	{
		Safe_Release(iter->second);
	}

	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}
