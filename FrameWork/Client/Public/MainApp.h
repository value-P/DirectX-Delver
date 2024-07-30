#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Export_System.h"
#include "Export_Utility.h"

BEGIN(Engine)

class GraphicDev;
class Management;

END

enum StageType
{
	STAGE_1,
	STAGE_2,
	STAGE_BOSS,

	STAGETYPE_END,
};

class MainApp : public Base
{
private:
	explicit MainApp();
	virtual ~MainApp();

public:
	HRESULT		Init_MainApp();
	int			Update_MainApp(const float& fTimeDelta);
	void		LateUpdate_MainApp();
	void		Render_MainApp();

public:
	static void SetGlobalTimeScale(float globalTimeScale) { m_fGlobalTimeScale = globalTimeScale; }
	static void SetStageType(StageType stageType) { m_eStageType = stageType; }
	static void SetIsTriggered(bool isTriggered) { m_bIsTriggered = isTriggered; }

private:
	void		KeyInput();
	void		ChangeStage();

private:
	HRESULT			SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Init_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::Management** ppManagement);

private:
	Engine::GraphicDev*			m_pDeviceClass;
	Engine::Management*			m_pManagement;
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	float m_fTime{ 0.0f };
	int m_iFPS{ 0 };
	TCHAR m_szFPS[16]{ L"" };

	bool m_bPress{ false };
	GameObject* m_pPlayer{ nullptr };

	static float m_fGlobalTimeScale;

	static StageType m_eStageType;
	static bool m_bIsTriggered;

	// 생성, 소멸 함수
public:
	static	MainApp*		Create();

private:
	virtual void			Free();
};

