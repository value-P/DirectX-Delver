#include "..\..\Header\Layer.h"

Layer::Layer()
{
}

Layer::~Layer()
{
}

Component * Layer::Get_Component(const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID)
{
	auto	iter = m_mapObject.find(pObjTag);

	if (iter == m_mapObject.end())
		return nullptr;
	
	return iter->second->Get_Component(pComponentTag, eID);
}

vector<GameObject*>* Layer::Get_GameObject(const wstring pObjTag)
{
	m_vecObject.clear();

	if (m_mapObject.count(pObjTag) == 1)
	{
		/*find_if(m_mapObject.begin(), m_mapObject.end(), [&pObjTag](auto pair)->bool{
			if (pair->first == pObjTag)
				return true;

			return false;
			});*/

		auto it = m_mapObject.find(pObjTag);
		if (it != m_mapObject.end())
		{
			m_vecObject.push_back(it->second);

			return &m_vecObject;
		}
	}

	else if (m_mapObject.count(pObjTag) > 1)
	{
		for (auto it = m_mapObject.lower_bound(pObjTag); it != m_mapObject.upper_bound(pObjTag); ++it)
		{
			m_vecObject.push_back(it->second);
		}

		return &m_vecObject;
	}

	return &m_vecObject;
}

HRESULT Layer::Add_GameObject(const wstring pObjTag, GameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_mapObject.insert({ pObjTag, pGameObject });

	return S_OK;
}

HRESULT Layer::Init_Layer()
{
	return S_OK;
}

_int Layer::Update_Layer(const _float & fTimeDelta)
{
	_int		iResult(0);

	for (multimap<const wstring, GameObject*>::iterator iter = m_mapObject.begin(); iter != m_mapObject.end(); )
	{
		iResult = (*iter).second->Update_GameObject(fTimeDelta);

		if (iResult & 0x80000000)
		{
			Safe_Release((*iter).second);
			iter = m_mapObject.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	return 0;
}

void Layer::LateUpdate_Layer()
{
	for (auto& iter : m_mapObject)
		iter.second->LateUpdate_GameObject();
}

Layer * Layer::Create()
{
	Layer *		pInstance = new Layer;

	if (FAILED(pInstance->Init_Layer()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Layer Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Layer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}
