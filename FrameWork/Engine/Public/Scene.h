#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL Scene : public Base
{
protected:
	explicit Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Scene();

public:
	Component*				Get_Component(const wstring pLayerTag, const wstring pObjTag, const wstring pComponentTag, COMPONENTID eID);
	Layer*					Get_Layer(const wstring pLayerTag);
	vector<GameObject*>*	Get_GameObject(const wstring pLayerTag, const wstring pObjTag);

public:
	virtual HRESULT			Init_Scene();
	virtual _int			Update_Scene(const _float& fTimeDelta);
	virtual void			LateUpdate_Scene();
	virtual void			Render_Scene() PURE;

public:
	virtual void CheckLoading() abstract;
	bool IsFinishLoading() { return m_bLoading; }

protected:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	map<const wstring, Layer*>		m_mapLayer;
	bool							m_bLoading{ false };
	GameObject*						m_pPlayer{ nullptr };

public:
	virtual void		Free();
};

END