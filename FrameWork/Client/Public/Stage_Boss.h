#pragma once
#include "Scene.h"

class CLoading;

namespace Engine
{
	class VIBuffer;
	class Texture;
}

class Stage_Boss :
    public Scene
{
public:
	explicit Stage_Boss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Stage_Boss();

public:
	HRESULT Init_Stage_Boss(GameObject** _ppPlayer);
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void LateUpdate_Scene() override;
	virtual void Render_Scene() override;

public:
	virtual void CheckLoading();
	void ClearStage() { m_bClear = true; }

private:

	HRESULT				Init_Layer_Environment(const wstring& pLayerTag);
	HRESULT				Init_Layer_SkyBox(const wstring& pLayerTag);
	HRESULT				Init_Layer_GameObject(const wstring& pLayerTag);
	HRESULT				Init_Layer_UI(const wstring& pLayerTag);
	HRESULT				Init_Layer_ColliderBox(const wstring& pLayerTag);

private:
	CLoading* m_pLoading = nullptr;

public:
	static		Stage_Boss* Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer);

private:
	int m_iCameraDirectionNum{ 0 };
	bool m_bClear{ false };
	VIBuffer* m_pBufferCom{ nullptr };
	Texture* m_pTextureCom{ nullptr };

private:
	virtual void Free() override;

};

