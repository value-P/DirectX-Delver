#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL Renderer : public Base
{
	DECLARE_SINGLETON(Renderer)

private:
	explicit Renderer();
	virtual ~Renderer();

public:
	void		Add_RenderGroup(RENDERID eType, GameObject* pGameObject);
	void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Clear_RenderGroup();

public:
	void		Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Nonalpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev);
	void		Render_UI(LPDIRECT3DDEVICE9& pGraphicDev);

public:
	// Test
	_matrix&	Get_OrthoProjMat() { return m_orthoProj; }

private:
	list<GameObject*>			m_RenderGroup[RENDER_END];

	_matrix m_orthoProj;

	

private:
	virtual void		Free();
};

END