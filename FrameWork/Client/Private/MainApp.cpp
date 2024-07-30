#include "stdafx.h"
#include "MainApp.h"
#include <time.h>
#include "Export_System.h"
#include "Logo.h"
#include "RayPickingMgr.h"
#include "ParsingMgr.h"

#include "Villeage.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage_Boss.h" 

#include "SingleCamera.h"

float MainApp::m_fGlobalTimeScale = 1.f;
StageType MainApp::m_eStageType = STAGETYPE_END;
bool MainApp::m_bIsTriggered = false;

MainApp::MainApp() : m_pDeviceClass(nullptr), m_pGraphicDev(nullptr), m_pManagement(nullptr)
{
}


MainApp::~MainApp()
{
}

HRESULT MainApp::Init_MainApp()
{
	ShowCursor(false);
	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);

	FAILED_CHECK_RETURN(Init_Scene(m_pGraphicDev, &m_pManagement), E_FAIL);

	srand(UINT(time(NULL)));

	SingleCamera::GetInstance()->Init_Camera(m_pGraphicDev);

	return S_OK;
}

int MainApp::Update_MainApp(const float & fTimeDelta)
{
	// 프레임 보려고 넣어놨음 나중에 없애도 됨
	++m_iFPS;
	m_fTime += fTimeDelta;
	if (m_fTime >= 1.0f)
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		m_iFPS = 0;
		m_fTime = 0;
		SetWindowText(g_hWnd, m_szFPS);
	}

	Engine::Update_InputDev();

	KeyInput();

	ChangeStage();
	
	SingleCamera::GetInstance()->SetTimeDelta(fTimeDelta * m_fGlobalTimeScale);

	Engine::Update_Scene(fTimeDelta * m_fGlobalTimeScale, g_hWnd);

	return 0;
}

void MainApp::LateUpdate_MainApp()
{
	m_pManagement->LateUpdate_Scene();
}

void MainApp::Render_MainApp()
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	Engine::Render_Scene(m_pGraphicDev);

	Engine::Render_End();
}

void MainApp::KeyInput()
{
	if (Engine::Get_DIKeyState(DIK_H) & 0x80)
	{
		if (!m_bPress)
		{
			Engine::SoundMgr::GetInstance()->Stop_All();
			Engine::Set_Scene(Villeage::Create(m_pGraphicDev, &m_pPlayer));
		}
		m_bPress = true;
	}
	else if (Engine::Get_DIKeyState(DIK_J) & 0x80)
	{
		if (!m_bPress)
		{
			Engine::SoundMgr::GetInstance()->Stop_All();
			Engine::Set_Scene(Stage1::Create(m_pGraphicDev, &m_pPlayer));
		}
		m_bPress = true;
	}
	else if (Engine::Get_DIKeyState(DIK_K) & 0x80)
	{
		if (!m_bPress)
		{
			Engine::SoundMgr::GetInstance()->Stop_All();
			Engine::Set_Scene(Stage2::Create(m_pGraphicDev, &m_pPlayer));
		}
		m_bPress = true;
	}
	else if (Engine::Get_DIKeyState(DIK_L) & 0x80)
	{
		if (!m_bPress)
		{
			Engine::SoundMgr::GetInstance()->Stop_All();
			Engine::Set_Scene(Stage_Boss::Create(m_pGraphicDev, &m_pPlayer));
		}
		m_bPress = true;
	}
	else
	{
		m_bPress = false;
	}
}

void MainApp::ChangeStage()
{
	if (m_bIsTriggered)
	{
		switch (m_eStageType)
		{
		case STAGE_1:
		{
			Engine::SoundMgr::GetInstance()->Stop_All();
			Engine::Set_Scene(Stage1::Create(m_pGraphicDev, &m_pPlayer));

			m_bIsTriggered = false;
		}
		break;
		case STAGE_2:
		{
			Engine::SoundMgr::GetInstance()->Stop_All();
			Engine::Set_Scene(Stage2::Create(m_pGraphicDev, &m_pPlayer));

			m_bIsTriggered = false;
		}
		break;
		case STAGE_BOSS:
		{
			Engine::SoundMgr::GetInstance()->Stop_All();
			Engine::Set_Scene(Stage_Boss::Create(m_pGraphicDev, &m_pPlayer));

			m_bIsTriggered = false;
		}
		break;
		}
	}
}

HRESULT MainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Init_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	*ppGraphicDev = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z버퍼에 깊이 값을 무조건 기록은 하지만 자동 정렬을 할지 말지 결정
	(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z버퍼에 깊이 값을 기록할 지 결정

	// Font
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape", L"둥근모꼴", 9, 12, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape_NPC", L"둥근모꼴", 11, 11, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape_Button", L"둥근모꼴", 15, 15, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape_Number", L"픽셀로보로보체", 12, 12, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape_HP", L"픽셀로보로보체", 7, 7, FW_NORMAL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_LogoText", L"둥근모꼴", 23, 23, FW_HEAVY), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_LogoText", L"둥근모꼴", 23, 23, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_Jinji", L"궁서", 30, 30, FW_THIN), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape_LevelUp_Title", L"둥근모꼴", 16, 16, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape_LevelUp_Text", L"둥근모꼴", 12, 12, FW_HEAVY), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape_Name_AbilityCard", L"둥근모꼴", 16, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape_CurStat_AbilityCard", L"둥근모꼴", 13, 16, FW_HEAVY), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_RoundShape_Get_Technique", L"둥근모꼴", 16, 20, FW_HEAVY), E_FAIL);

	//test
	FAILED_CHECK_RETURN(Engine::Init_Font((*ppGraphicDev), L"Font_Test", L"맑은 고딕", 8, 20, FW_HEAVY), E_FAIL);

	// Dinput
	FAILED_CHECK_RETURN(Engine::Init_InputDev(g_hInst, g_hWnd), E_FAIL);

	// Sound
	FAILED_CHECK_RETURN(Engine::Init_SoundMgr(), E_FAIL);


	// Load Prefab Data
	for (int i = 0; i < SCENE_END; ++i)
	{
		ParsingMgr::GetInstance()->Load_Prefab((SCENETYPE)i);
	}

	//(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT MainApp::Init_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::Management ** ppManagement)
{
	Engine::Scene*		pScene = nullptr;

	pScene = CLogo::Create(pGraphicDev, &m_pPlayer);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(m_pGraphicDev, ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL);

	return S_OK;
}

MainApp * MainApp::Create()
{
	MainApp *		pInstance = new MainApp;

	if (FAILED(pInstance->Init_MainApp()))
	{
		delete pInstance;
		pInstance = nullptr;
	}
	
	return pInstance;
}

void MainApp::Free()
{
	Engine::Stop_All();

	Safe_Release(m_pPlayer);
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pManagement);

	Engine::Release_Utility();
	Engine::Release_System();
	ParsingMgr::GetInstance()->DestroyInstance();
}
