#include "stdafx.h"
#include "Stage2.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "SkyBox.h"
#include "Player.h"
#include "Loading.h"
#include "MouseCursor.h"
#include "MeleeWeapon.h"
#include "NonPlayerCharacter.h"
#include "MyHUD.h"
#include "Armor.h"
#include "TradeInv.h"
#include "Monster.h"
#include "Balloon.h"
#include "MoveWall.h"
#include "MWLever.h"
#include "Fire.h"
#include "Slime.h"
#include "Bat.h"
#include "Warrior.h"
#include "Wizard.h"
#include "Zombi.h"
#include "InteractionTrigger.h"


Stage2::Stage2(LPDIRECT3DDEVICE9 pGraphicDev)
	: Scene(pGraphicDev)
{
}

Stage2::~Stage2()
{
}

Stage2* Stage2::Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer)
{
	Stage2* pInstance = new Stage2(pGraphicDev);

	if (FAILED(pInstance->Init_Stage2(_ppPlayer)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_Scene Failed", L"Stage2::Create", MB_OK);
		return nullptr;
	}

	return pInstance;
}

HRESULT Stage2::Init_Stage2(GameObject** _ppPlayer)
{
	m_pPlayer = *_ppPlayer;

	UIManager::GetInstance()->Init_UI(L"Proto_MyHUDTexture", MyHUD::Create(m_pGraphicDev));
	UIManager::GetInstance()->Activate_UI(UIManager::GetInstance()->GetHUD());

	FAILED_CHECK_RETURN(Init_Layer_Environment(LAYER_ENVI), E_FAIL);
	FAILED_CHECK_RETURN(Init_Layer_ColliderBox(LAYER_MAPCOL), E_FAIL);
	FAILED_CHECK_RETURN(Init_Layer_SkyBox(LAYER_SKYBOX), E_FAIL);
	FAILED_CHECK_RETURN(Init_Layer_GameObject(LAYER_GAMEOBJECT), E_FAIL);
	FAILED_CHECK_RETURN(Init_Layer_UI(LAYER_UI), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE2, this);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	if (!*_ppPlayer)
	{
		*_ppPlayer = m_pPlayer;
	}

	SoundMgr::GetInstance()->Play_BGM(L"Stage2BGM", 1.0f);

	return S_OK;
}

_int Stage2::Update_Scene(const _float& fTimeDelta)
{
	/*if (!SingleCamera::GetInstance()->IsPlaying())
	{
		SingleCamera::GetInstance()->PlayCameraDirection4(D3DXVECTOR3{ -16.0f,15.0f,94.0f }, D3DXVECTOR3{ -43.0f,-3.0f,127.0f }, false, D3DX_PI / 4.0f, 4.0f);
	}*/

	if (m_iCameraDirectionNum < 3 && !SingleCamera::GetInstance()->IsPlaying())
	{
		switch (m_iCameraDirectionNum)
		{
		case 0:
		{
			SingleCamera::GetInstance()->PlayCameraDirectionR3(D3DXVECTOR3{ 10.0f,19.3f,126.0f }, D3DXVECTOR3{ 1.0f,19.3f,120.0f }, D3DX_PI * 0.9f, -D3DX_PI * 0.1f, 4.0f);
			m_iCameraDirectionNum++;
			//SingleCamera::GetInstance()->PlayCameraShake(4.0f, 0.1f);
		}
		break;
		case 1:
		{
			SingleCamera::GetInstance()->PlayCameraDirection4(D3DXVECTOR3{ -16.0f,15.0f,94.0f }, D3DXVECTOR3{ -43.0f,-3.0f,127.0f }, false, D3DX_PI * 0.6f, 4.0f);
			m_iCameraDirectionNum++;
			//SingleCamera::GetInstance()->PlayCameraShake(4.0f, 0.1f);
		}
		break;
		case 2:
		{
			SingleCamera::GetInstance()->PlayCameraDirection3(D3DXVECTOR3{ -39.4f,16.4f,119.5f }, D3DXVECTOR3{ -46.9f,16.4f,122.1f }, D3DXVECTOR3{ -1.0f,-1.0f,-3.0f }, 4.0f);
			m_iCameraDirectionNum++;
		}
		break;
		}
	}

	if (m_pStageDoor->GetMove() && m_bDoorCameraDirection)
	{
		SoundMgr::GetInstance()->Play_Sound_NoInterval(L"earthquake1", SOUND_EFFECT_1, 0.7f);
		SingleCamera::GetInstance()->PlayCameraDirection1(D3DXVECTOR3{ -57.0f,5.8f,61.0f }, D3DXVECTOR3{ -57.0f,5.8f,60.0f }, 2.6f);
		SingleCamera::GetInstance()->PlayCameraShake(2.6f, 0.1f);
		m_bDoorCameraDirection = false;
	}

	return __super::Update_Scene(fTimeDelta);
}

void Stage2::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void Stage2::Render_Scene()
{
}

void Stage2::CheckLoading()
{
	if (m_pLoading)
	{
		m_pLoading->Get_Finish() ? m_bLoading = true : m_bLoading = false;
	}
}

HRESULT Stage2::Init_Layer_Environment(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::GameObject* pGameObject = nullptr;

	if (FAILED(CreateBalloon(pLayer))) { return E_FAIL; }
	if (FAILED(CreateMoveWall(pLayer))) { return E_FAIL; }
	if (FAILED(CreateFire(pLayer))) { return E_FAIL; }

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Stage2::Init_Layer_SkyBox(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::GameObject* pGameObject = nullptr;

	//SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Stage2::Init_Layer_GameObject(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::GameObject* pGameObject = nullptr;

	if (m_pPlayer)
	{
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
		m_pPlayer->Get_Transform()->Set_Pos(0.f, 3.5f, 0.f);
		dynamic_cast<Player*>(m_pPlayer)->PlayerDefaultSetting();
	}
	else
	{
		Player* pPlayer = nullptr;
		//Player
		
		m_pPlayer = Player::Create(m_pGraphicDev, L"Proto_PlayerTexture");
		NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
		pPlayer = dynamic_cast<Player*>(m_pPlayer);
		pPlayer->Get_Transform()->Set_Pos(0.f, 3.5f, 0.f);
	}

	// Mouse
	pGameObject = MouseCursor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mouse", pGameObject), E_FAIL);

	// Warp To Boss
	pGameObject = InteractionTrigger::Create(m_pGraphicDev, _vec3(2.f, 1.f, 2.f), COL_INTERACTION, _vec3(-57.f, 7.f, -6.5f), ROOMTYPE_STAGE2, INTTYPE_EVENT, TRITYPE_WARP);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"InteractionTriggerStage2", pGameObject), E_FAIL);

	CreateMonster(pLayer);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Stage2::Init_Layer_UI(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::GameObject* pUI = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Stage2::Init_Layer_ColliderBox(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Stage2::CreateBalloon(Layer* _pLayer)
{
	Engine::GameObject* pGameObject{ nullptr };

	D3DXCOLOR dwColor[3]{ D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f } ,D3DXCOLOR{ 1.0f,0.0f,0.0f,1.0f } ,D3DXCOLOR{ 0.0f,0.0f,1.0f,1.0f } };
	D3DXVECTOR3 vDir[3]{ { float(rand() % 101 - 50) * 0.1f, float(rand() % 101 - 50) * 0.1f, float(rand() % 101 - 50) * 0.1f } ,
							{ float(rand() % 101 - 50) * 0.1f, float(rand() % 101 - 50) * 0.1f, float(rand() % 101 - 50) * 0.1f } ,
							{ float(rand() % 101 - 50) * 0.1f, float(rand() % 101 - 50) * 0.1f, float(rand() % 101 - 50) * 0.1f } };
	D3DXVec3Normalize(&vDir[0], &vDir[0]);
	D3DXVec3Normalize(&vDir[1], &vDir[1]);
	D3DXVec3Normalize(&vDir[2], &vDir[2]);
	D3DXVECTOR3 vMinDot{ -34.0f,4.0f,93.0f };
	D3DXVECTOR3 vMaxDot{ -20.0f,17.0f,125.0f };
	pGameObject = Balloon::Create(m_pGraphicDev, D3DXVECTOR3{ -28.5f,10.5f,106.5f }, vDir[0], 5.0f, dwColor[0], vMinDot, vMaxDot,
		MoveWall::Create(m_pGraphicDev, true, false, L"Proto_Cube_Under_Lava", D3DXVECTOR3{ -25.0f,-13.0f,118.0f }, D3DXVECTOR3{ -25.0f,-1.0f,118.0f }, D3DXVECTOR3{ 9.0f,1.0f,3.0f }, 2.0f, 0.0f));
	if (nullptr == pGameObject) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Balloon", pGameObject))) { return E_FAIL; }

	pGameObject = Balloon::Create(m_pGraphicDev, D3DXVECTOR3{ -28.5f,10.5f,106.5f }, vDir[1], 5.0f, dwColor[1], vMinDot, vMaxDot,
		MoveWall::Create(m_pGraphicDev, true, false, L"Proto_Cube_Under_Lava", D3DXVECTOR3{ -37.0f,-13.0f,114.0f }, D3DXVECTOR3{ -37.0f,-1.0f,114.0f }, D3DXVECTOR3{ 3.0f,1.0f,7.0f }, 2.0f, 0.0f));
	if (nullptr == pGameObject) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Balloon", pGameObject))) { return E_FAIL; }

	pGameObject = Balloon::Create(m_pGraphicDev, D3DXVECTOR3{ -28.5f,10.5f,106.5f }, vDir[2], 5.0f, dwColor[2], vMinDot, vMaxDot,
		MoveWall::Create(m_pGraphicDev, true, false, L"Proto_Cube_Under_Lava", D3DXVECTOR3{ -49.0f,-13.0f,110.0f }, D3DXVECTOR3{ -49.0f,-1.0f,110.0f }, D3DXVECTOR3{ 9.0f,1.0f,3.0f }, 2.0f, 0.0f));
	if (nullptr == pGameObject) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Balloon", pGameObject))) { return E_FAIL; }

	return S_OK;
}

HRESULT Stage2::CreateMoveWall(Layer* _pLayer)
{
	MoveWall* pElevator1{ nullptr };
	MoveWall* pElevator2{ nullptr };
	MoveWall* pDoor{ nullptr };
	MoveWall* pGameObject{ nullptr };
	GameObject* pGameObject2{ nullptr };

	pElevator1 = MoveWall::Create(m_pGraphicDev, false, true, L"Proto_Elevator", D3DXVECTOR3{ -65.0f,-1.0f,100.0f }, D3DXVECTOR3{ -65.0f,13.0f,100.0f }, D3DXVECTOR3{ 3.0f,1.0f,3.0f }, 5.0f, 1.5f);
	if (nullptr == pElevator1) { return E_FAIL; }

	pElevator2 = MoveWall::Create(m_pGraphicDev, false, true, L"Proto_Elevator", D3DXVECTOR3{ -65.0f,-11.0f,100.0f }, D3DXVECTOR3{ -65.0f,-1.0f,100.0f }, D3DXVECTOR3{ 3.0f,1.0f,3.0f }, 5.0f, 1.5f);
	if (nullptr == pElevator2) { return E_FAIL; }

	pElevator1->SetLinkedWall(pElevator2);
	pElevator2->SetLinkedWall(pElevator1);

	if (FAILED(_pLayer->Add_GameObject(L"LinkedElevator", pElevator1))) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"LinkedElevator", pElevator2))) { return E_FAIL; }

	pGameObject2 = MWLever::Create(m_pGraphicDev, D3DXVECTOR3{ -59.7f,-7.5f,96.0f }, D3DXVECTOR3{ 0.0f,-1.0f,0.0f }, D3DXVECTOR3{ 1.0f,0.0f,0.0 }, pElevator2);
	if (nullptr == pGameObject2) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Lever", pGameObject2))) { return E_FAIL; }

	pGameObject2 = MWLever::Create(m_pGraphicDev, D3DXVECTOR3{ -67.0f,0.3f,105.0f }, D3DXVECTOR3{ 0.0f,0.0f,1.0f }, D3DXVECTOR3{ 0.0f,1.0f,0.0 }, pElevator1);
	if (nullptr == pGameObject2) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Lever", pGameObject2))) { return E_FAIL; }

	pGameObject2 = MWLever::Create(m_pGraphicDev, D3DXVECTOR3{ -67.7f,16.0f,96.0f }, D3DXVECTOR3{ 0.0f,-1.0f,0.0f }, D3DXVECTOR3{ 1.0f,0.0f,0.0 }, pElevator1);
	if (nullptr == pGameObject2) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Lever", pGameObject2))) { return E_FAIL; }

	pGameObject = MoveWall::Create(m_pGraphicDev, true, true, L"Proto_Elevator", D3DXVECTOR3{ -69.0f,0.0f,48.0f }, D3DXVECTOR3{ -69.0f,13.0f,48.0f }, D3DXVECTOR3{ 3.0f,1.0f,3.0f }, 5.0f, 1.5f);
	if (nullptr == pGameObject) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Elevator", pGameObject))) { return E_FAIL; }

	pGameObject = MoveWall::Create(m_pGraphicDev, true, true, L"Proto_MoveStone", D3DXVECTOR3{ -71.0f,-7.0f,55.0f }, D3DXVECTOR3{ -43.0f,-7.0f,55.0f }, D3DXVECTOR3{ 1.0f,3.0f,2.0f }, 16.0f, 0.8f);
	if (nullptr == pGameObject) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"MoveStone", pGameObject))) { return E_FAIL; }

	pGameObject = MoveWall::Create(m_pGraphicDev, true, true, L"Proto_MoveStone", D3DXVECTOR3{ -71.0f,-7.0f,65.0f }, D3DXVECTOR3{ -43.0f,-7.0f,65.0f }, D3DXVECTOR3{ 1.0f,3.0f,2.0f }, 13.0f, 0.8f);
	if (nullptr == pGameObject) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"MoveStone", pGameObject))) { return E_FAIL; }

	pGameObject = MoveWall::Create(m_pGraphicDev, true, true, L"Proto_MoveStone", D3DXVECTOR3{ -71.0f,-7.0f,74.0f }, D3DXVECTOR3{ -43.0f,-7.0f,74.0f }, D3DXVECTOR3{ 1.0f,3.0f,2.0f }, 10.0f, 0.8f);
	if (nullptr == pGameObject) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"MoveStone", pGameObject))) { return E_FAIL; }

	pGameObject = MoveWall::Create(m_pGraphicDev, true, true, L"Proto_MoveStone", D3DXVECTOR3{ -71.0f,-7.0f,86.0f }, D3DXVECTOR3{ -43.0f,-7.0f,86.0f }, D3DXVECTOR3{ 1.0f,3.0f,2.0f }, 8.0f, 0.8f);
	if (nullptr == pGameObject) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"MoveStone", pGameObject))) { return E_FAIL; }

	pDoor = MoveWall::Create(m_pGraphicDev, false, false, L"Proto_Stage2_Door", D3DXVECTOR3{ -57.0f,5.0f,40.0f }, D3DXVECTOR3{ -57.0f,12.0f,40.0f }, D3DXVECTOR3{ 3.0f,4.0f,0.5f }, 1.5f, 100.0f);
	if (nullptr == pDoor) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Proto_Stage2_Door", pDoor))) { return E_FAIL; }

	m_pStageDoor = pDoor;

	pGameObject2 = MWLever::Create(m_pGraphicDev, D3DXVECTOR3{ -57.0f,-9.5f,16.0f }, D3DXVECTOR3{ -1.0f,0.0f,0.0f }, D3DXVECTOR3{ 0.0f,1.0f,0.0 }, pDoor, D3DXVECTOR3{ 1.0f,0.5f,1.0f });
	if (nullptr == pGameObject2) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Lever", pGameObject2))) { return E_FAIL; }

	return S_OK;
}

HRESULT Stage2::CreateFire(Layer* _pLayer)
{
	GameObject* pFire{ nullptr };

	pFire = Fire::Create(m_pGraphicDev, D3DXVECTOR3{ -65.0f,9.2f,14.0f });
	if (nullptr == pFire) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Fire", pFire))) { return E_FAIL; }

	pFire = Fire::Create(m_pGraphicDev, D3DXVECTOR3{ -49.0f,9.2f,14.0f });
	if (nullptr == pFire) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Fire", pFire))) { return E_FAIL; }

	pFire = Fire::Create(m_pGraphicDev, D3DXVECTOR3{ -65.0f,9.2f,22.0f });
	if (nullptr == pFire) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Fire", pFire))) { return E_FAIL; }

	pFire = Fire::Create(m_pGraphicDev, D3DXVECTOR3{ -49.0f,9.2f,22.0f });
	if (nullptr == pFire) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Fire", pFire))) { return E_FAIL; }

	pFire = Fire::Create(m_pGraphicDev, D3DXVECTOR3{ -65.0f,9.2f,30.0f });
	if (nullptr == pFire) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Fire", pFire))) { return E_FAIL; }

	pFire = Fire::Create(m_pGraphicDev, D3DXVECTOR3{ -49.0f,9.2f,30.0f });
	if (nullptr == pFire) { return E_FAIL; }
	if (FAILED(_pLayer->Add_GameObject(L"Fire", pFire))) { return E_FAIL; }

	return S_OK;
}

HRESULT Stage2::CreateMonster(Layer* _pLayer)
{
	Monster* pMonster{ nullptr };
	CreatureStat monsterStat;

	pMonster = Wizard::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(15, 15, 4, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(35.0f, 35.0f, 4.0f, 0.0f);
	pMonster->Get_Transform()->Set_Pos(1.2f, 2.0f, 61.0f);
	if (FAILED(_pLayer->Add_GameObject(L"Wizard", pMonster))) { return E_FAIL; }

	pMonster = Wizard::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(15, 15, 4, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(35.0f, 35.0f, 4.0f, 0.0f);
	pMonster->Get_Transform()->Set_Pos(1.2f, 4.0f, 69.0f);
	if (FAILED(_pLayer->Add_GameObject(L"Wizard", pMonster))) { return E_FAIL; }

	pMonster = Wizard::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(15, 15, 4, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(18.0f, 18.0f, 4.0f, 3.0f);
	pMonster->Get_Transform()->Set_Pos(-7.5f, 4.0f, 118.0f);
	if (FAILED(_pLayer->Add_GameObject(L"Wizard", pMonster))) { return E_FAIL; }
	
	pMonster = Wizard::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(15, 15, 4, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(25.0f, 25.0f, 4.0f, 0.0f);
	pMonster->Get_Transform()->Set_Pos(-65.0f, 11.5f, 124.3f);
	if (FAILED(_pLayer->Add_GameObject(L"Wizard", pMonster))) { return E_FAIL; }

	pMonster = Wizard::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(15, 15, 4, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(26.0f, 26.0f, 4.0f, 0.0f);
	pMonster->Get_Transform()->Set_Pos(-54.5f, 18.0f, 52.3f);
	if (FAILED(_pLayer->Add_GameObject(L"Wizard", pMonster))) { return E_FAIL; }

	pMonster = Wizard::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(15, 15, 4, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(20.0f, 15.0f, 4.0f, 3.0f);
	pMonster->Get_Transform()->Set_Pos(-53.7f, -6.0f, 17.1f);
	if (FAILED(_pLayer->Add_GameObject(L"Wizard", pMonster))) { return E_FAIL; }


	pMonster = Warrior::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(20, 20, 5, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(17.0f, 2.0f, 3.0f, 4.0f);
	pMonster->Get_Transform()->Set_Pos(-2.1f, 4.0f, 123.6f);
	if (FAILED(_pLayer->Add_GameObject(L"Warrior", pMonster))) { return E_FAIL; }

	pMonster = Warrior::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(20, 20, 5, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(17.0f, 2.0f, 3.0f, 4.0f);
	pMonster->Get_Transform()->Set_Pos(-3.2f, 4.0f, 111.2f);
	if (FAILED(_pLayer->Add_GameObject(L"Warrior", pMonster))) { return E_FAIL; }

	pMonster = Warrior::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(20, 20, 5, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(15.0f, 2.0f, 3.0f, 4.0f);
	pMonster->Get_Transform()->Set_Pos(-65.8f, 4.0f, 109.f);
	if (FAILED(_pLayer->Add_GameObject(L"Warrior", pMonster))) { return E_FAIL; }

	pMonster = Warrior::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(20, 20, 5, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(15.0f, 2.0f, 3.0f, 4.0f);
	pMonster->Get_Transform()->Set_Pos(-59.5f, 18.0f, 70.1f);
	if (FAILED(_pLayer->Add_GameObject(L"Warrior", pMonster))) { return E_FAIL; }


	pMonster = Zombi::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(40, 40, 7, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(10.0f, 1.4f, 4.0f, 2.0f);
	pMonster->Get_Transform()->Set_Pos(-60.0f, -6.0f, 24.7f);
	if (FAILED(_pLayer->Add_GameObject(L"Zombie", pMonster))) { return E_FAIL; }

	pMonster = Zombi::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(40, 40, 7, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(10.0f, 1.4f, 4.0f, 2.0f);
	pMonster->Get_Transform()->Set_Pos(-53.5f, -6.0f, 31.0f);
	if (FAILED(_pLayer->Add_GameObject(L"Zombie", pMonster))) { return E_FAIL; }

	pMonster = Zombi::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(40, 40, 7, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(10.0f, 1.4f, 4.0f, 2.0f);
	pMonster->Get_Transform()->Set_Pos(-57.0f, -6.0f, 28.7f);
	if (FAILED(_pLayer->Add_GameObject(L"Zombie", pMonster))) { return E_FAIL; }


	pMonster = Bat::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(10, 10, 2, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(25.0f, 1.4f, 3.0f, 5.0f);
	pMonster->Get_Transform()->Set_Pos(-44.0f, 17.0f, 62.0f);
	if (FAILED(_pLayer->Add_GameObject(L"Bat", pMonster))) { return E_FAIL; }

	pMonster = Bat::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(10, 10, 2, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(25.0f, 1.4f, 3.0f, 5.0f);
	pMonster->Get_Transform()->Set_Pos(-70.0f, 17.0f, 70.0f);
	if (FAILED(_pLayer->Add_GameObject(L"Bat", pMonster))) { return E_FAIL; }

	pMonster = Bat::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(10, 10, 2, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(25.0f, 1.4f, 3.0f, 5.0f);
	pMonster->Get_Transform()->Set_Pos(-50.0f, -7.0f, 77.0f);
	if (FAILED(_pLayer->Add_GameObject(L"Bat", pMonster))) { return E_FAIL; }

	pMonster = Bat::Create(m_pGraphicDev);
	if (nullptr == pMonster) { return E_FAIL; }
	monsterStat = CreatureStat(10, 10, 2, 0);
	pMonster->Set_CreatureStat(&monsterStat);
	pMonster->Set_BehaviorStat(25.0f, 1.4f, 3.0f, 5.0f);
	pMonster->Get_Transform()->Set_Pos(-68.0f, -7.0f, 70.0f);
	if (FAILED(_pLayer->Add_GameObject(L"Bat", pMonster))) { return E_FAIL; }


	return S_OK;
}

void Stage2::Free()
{
	Safe_Release(m_pLoading);

	Layer* pGameObjectLayer = m_mapLayer.at(L"GameObject");
	if (pGameObjectLayer)
	{
		multimap<const wstring, GameObject*>::iterator iter = (*pGameObjectLayer->Get_ObjectMap()).begin();
		for (; iter != (*pGameObjectLayer->Get_ObjectMap()).end(); iter++)
		{
			if ((*iter).first == L"Player")
			{
				pGameObjectLayer->Get_ObjectMap()->erase(iter);
				break;
			}
		}
	}
	
	Scene::Free();
}
