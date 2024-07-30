#pragma once
#include "Scene.h"

class CLoading;

class Stage1 :
    public Scene
{
public:
	explicit Stage1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Stage1();

public:
	HRESULT Init_Stage1(GameObject** _ppPlayer);
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

public:
	virtual void CheckLoading();

public:
	void Test_ShopItem(); //[��ä] : UI Ȱ��ȭ�� ���� �ӽ� �ڵ�! ���� ������ ��!

private:

	HRESULT				Init_Layer_Environment(const wstring& pLayerTag);
	HRESULT				Init_Layer_SkyBox(const wstring& pLayerTag);
	HRESULT				Init_Layer_GameObject(const wstring& pLayerTag);
	HRESULT				Init_Layer_UI(const wstring& pLayerTag);
	HRESULT				Init_Layer_ColliderBox(const wstring& pLayerTag);

private:
	CLoading* m_pLoading = nullptr;

public:
	static		Stage1* Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer);

private:
	virtual void Free() override;

private:
	Engine::GameObject* pTestNPC = nullptr; //[��ä] : UI Ȱ��ȭ�� ���� �ӽ� �ڵ�! ���� ������ ��!

};

