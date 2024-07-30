#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Scene.h"
#include "Ray.h"

BEGIN(Engine)

class ENGINE_DLL Management :	public Base
{
	DECLARE_SINGLETON(Management)
	
private:
	explicit Management();
	virtual ~Management();

public:
	_int			Update_Scene(const _float& fTimeDelta, HWND hWnd);
	void			LateUpdate_Scene();
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	Component*				Get_Component(const wstring pLayerTag, const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID);
	Layer*					Get_Layer(const wstring pLayerTag);
	vector<GameObject*>*	Get_GameObject(const wstring pLayerTag, const wstring pObjTag);

public:
	// 일단은 아이템 버리기 할 때 Spawn하려고 추가
	Scene*			Get_CurScene() { return m_pScene; }
	HRESULT			Set_Scene(Scene* pScene);

public:
	void			SetLoadingImg(GameObject* _pLoadingImg);

public:
	bool			RayCast(Ray* ray, GameObject** pPickedObj, _vec3* pPickedPos, _float maxDist = 100.f, const wstring& layerTag = LAYER_MAPCOL);

private:
	Scene*			m_pScene;
	GameObject*		m_pLoadingImg{ nullptr };

public:
	virtual void		Free();
};

END