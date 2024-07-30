#pragma once

#include "Scene.h"

class CLoading;
class MoveWall;

class Stage2 : public Scene
{
private:
	explicit Stage2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~Stage2();

public:
	static Stage2* Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer);

public:
	HRESULT Init_Stage2(GameObject** _ppPlayer);

public:
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
	HRESULT CreateBalloon(Layer* _pLayer);
	HRESULT CreateMoveWall(Layer* _pLayer);
	HRESULT CreateFire(Layer* _pLayer);
	HRESULT CreateMonster(Layer* _pLayer);

private:
	CLoading* m_pLoading = nullptr;
	MoveWall* m_pStageDoor{ nullptr };
	bool m_bDoorCameraDirection{ true };
	int m_iCameraDirectionNum{ 0 };

private:
	virtual void Free() override;
};

