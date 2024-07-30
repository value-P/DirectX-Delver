#pragma once
#include "Scene.h"

class CLoading;

class Villeage : public Scene
{

public:
	explicit Villeage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Villeage();

public:
	HRESULT Init_Villeage(GameObject** _ppPlayer);
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

public:
	virtual void CheckLoading();

private:

	HRESULT				Init_Layer_Environment(const wstring& pLayerTag);
	HRESULT				Init_Layer_SkyBox(const wstring& pLayerTag);
	HRESULT				Init_Layer_GameObject(const wstring& pLayerTag);
	HRESULT				Init_Layer_UI(const wstring& pLayerTag);
	HRESULT				Init_Layer_ColliderBox(const wstring& pLayerTag);

private:
	CLoading* m_pLoading = nullptr;

public:
	static		Villeage* Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer);

private:
	virtual void Free() override;

public:
	void Equipment_ShopItem(); //[우채] : UI 활성화를 위한 임시 코드! 추후 제거할 것!
	void Portion_ShopItem(); //[우채] : UI 활성화를 위한 임시 코드! 추후 제거할 것!

private:
	Engine::GameObject* m_pNPC_Equipment = nullptr; //[우채] : UI 활성화를 위한 임시 코드! 추후 제거할 것!
	Engine::GameObject* m_pNPC_Portion = nullptr; //[우채] : UI 활성화를 위한 임시 코드! 추후 제거할 것!

};

