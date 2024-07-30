#pragma once
#include "Scene.h"
#include "BackGround.h"
#include "Loading.h"

class CLogo : public Engine::Scene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo();

public:
	HRESULT Init_Logo(GameObject** _ppPlayer);
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

public:
	virtual void CheckLoading();

private:
	HRESULT				Init_Prototype();

private:
	CLoading*			m_pLoading;
	GameObject*			m_pLoadingImg{ nullptr };
	GameObject**		m_ppPlayer;

public:
	static		CLogo*	Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer);

private:
	virtual void Free() override;
};

