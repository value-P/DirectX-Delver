#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL Layer : public Base
{
private:
	explicit Layer();
	virtual ~Layer();

public:
	Component*				Get_Component(const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID);
	vector<GameObject*>*	Get_GameObject(const wstring pObjTag);

public:
	HRESULT				Add_GameObject(const wstring pObjTag, GameObject*	pGameObject);

	HRESULT				Init_Layer();
	_int				Update_Layer(const _float& fTimeDelta);
	void				LateUpdate_Layer();

public:
	multimap<const wstring, GameObject*>* Get_ObjectMap() { return &m_mapObject; }

private:
	multimap<const wstring, GameObject*>			m_mapObject;
	vector<GameObject*> m_vecObject;

public:
	static		Layer*		Create();
	virtual		void		Free();
};

END