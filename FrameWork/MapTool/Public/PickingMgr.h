#pragma once

#include "Engine_Define.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "ColliderBox.h"

class PickingMgr
{
public:
	static PickingMgr* GetInstance()
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new PickingMgr();
		}
		return m_pInstance;
	}
	static void DestroyInstance()
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	explicit PickingMgr();
	virtual ~PickingMgr();

public:
	bool Picking(LPDIRECT3DDEVICE9 _pGraphicDevice, map<const wstring, list<GameObject*>>* _pMap, GameObject** _ppGameObject, D3DXVECTOR3* _pVec3);

	bool Picking_ColBox(LPDIRECT3DDEVICE9 _pGraphicDevice, list<ColliderBox*>* _pList, ColliderBox** _ppColBox, D3DXVECTOR3* _pVec3);

	void RenderPickingMgr(LPDIRECT3DDEVICE9 _pGraphicDevice);

	_vec3 Get_PickingPos() { 
		_vec3 result = { 0.f,0.f,0.f };
		if (m_pTransform)
		{
			m_pTransform->Get_Info(INFO_POS, &result);
		}
		
		return result;
	}

private:
	static PickingMgr* m_pInstance;

private:
	PickCube* m_pCubeCom{ nullptr };
	Transform* m_pTransform{ nullptr };

private:
	virtual void Free();
};

