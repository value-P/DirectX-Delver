#include "stdafx.h"
#include "Stage1.h"
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
#include "Slime.h"
#include "MovableWall.h"
#include "InteractionTrigger.h"
#include "AutoTrack.h"
#include "MWLever.h"

#include "Export_Utility.h"
#include "Export_System.h"

Stage1::Stage1(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::Scene(pGraphicDev)
{
}

Stage1::~Stage1()
{
}

HRESULT Stage1::Init_Stage1(GameObject** _ppPlayer)
{
    m_pPlayer = *_ppPlayer;

    UIManager::GetInstance()->Init_UI(L"Proto_MyHUDTexture", MyHUD::Create(m_pGraphicDev));
    UIManager::GetInstance()->Activate_UI(UIManager::GetInstance()->GetHUD());

    FAILED_CHECK_RETURN(Init_Layer_Environment(LAYER_ENVI), E_FAIL);
    FAILED_CHECK_RETURN(Init_Layer_SkyBox(LAYER_SKYBOX), E_FAIL);
    FAILED_CHECK_RETURN(Init_Layer_GameObject(LAYER_GAMEOBJECT), E_FAIL);
    FAILED_CHECK_RETURN(Init_Layer_UI(LAYER_UI), E_FAIL);
    FAILED_CHECK_RETURN(Init_Layer_ColliderBox(LAYER_MAPCOL), E_FAIL);

    m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE1, this);
    NULL_CHECK_RETURN(m_pLoading, E_FAIL);

    if (!*_ppPlayer)
    {
        *_ppPlayer = m_pPlayer;
    }

    SoundMgr::GetInstance()->Play_BGM(L"03_shimmering_caves", 0.6f);

    return S_OK;
}

_int Stage1::Update_Scene(const _float& fTimeDelta)
{
    if (Engine::Key_Down('T'))
    {
        Engine::Play_Sound(L"mg_ice_shoot_03", SOUND_EFFECT_1,1.f);
    }

    return __super::Update_Scene(fTimeDelta);
}

void Stage1::LateUpdate_Scene()
{
    __super::LateUpdate_Scene();
}

void Stage1::Render_Scene()
{
}

void Stage1::CheckLoading()
{
    if (m_pLoading)
    {
        m_pLoading->Get_Finish() ? m_bLoading = true : m_bLoading = false;
    }
}

HRESULT Stage1::Init_Layer_Environment(const wstring& pLayerTag)
{
    Engine::Layer* pLayer = Engine::Layer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::GameObject* pGameObject = nullptr;

    // CombinedCamera

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT Stage1::Init_Layer_SkyBox(const wstring& pLayerTag)
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

HRESULT Stage1::Init_Layer_GameObject(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::GameObject* pGameObject = nullptr;


    if (m_pPlayer)
    {
        FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
        m_pPlayer->Get_Transform()->Set_Pos(6.f, 0.1f, 0.f);
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
        pPlayer->Get_Transform()->Set_Pos(6.f, 0.1f, 0.f);
    }

    // Mouse
    pGameObject = MouseCursor::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mouse", pGameObject), E_FAIL);

    // Interactable
    // FirstRoom
    pGameObject = MovableWall::Create(m_pGraphicDev, L"Proto_MovableWall", _vec3(2.f, 2.f, 0.5f), COL_INTERACTION, _vec3(7.f, -2.f, 14.99f), INTDIR_UP, INTTYPE_WALL, WALLTYPE_WALL, TRITYPE_WALL);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FirstRoomMovableWall", pGameObject), E_FAIL);
    pGameObject->Get_Transform()->Set_Scale(2.f, 2.f, 2.f);

    // ArcherRoom
    pGameObject = InteractionTrigger::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(7.f, 0.f, 72.5f), ROOMTYPE_ARCHER, INTTYPE_EVENT, TRITYPE_BLOCK | TRITYPE_SPAWN | TRITYPE_SEAL, 15);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ArcherRoomTrigger", pGameObject), E_FAIL);

	pGameObject = AutoTrack::Create(m_pGraphicDev, _vec3(0.5f, 0.5f, 6.f), COL_INTERACTION, _vec3(7.f, -1.f, 81.f), INTDIR_FRONT, INTTYPE_AUTOMOVE, _vec3(7.f, -1.f, 76.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ArcherRoomAutoTrack0", pGameObject), E_FAIL);

	pGameObject = AutoTrack::Create(m_pGraphicDev, _vec3(5.f, 0.5f, 0.5f), COL_INTERACTION, _vec3(11.f, -1.f, 88.f), INTDIR_RIGHT, INTTYPE_AUTOMOVE, _vec3(7.f, -1.f, 88.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ArcherRoomAutoTrack1", pGameObject), E_FAIL);

	pGameObject = AutoTrack::Create(m_pGraphicDev, _vec3(0.5f, 0.5f, 6.f), COL_INTERACTION, _vec3(17.f, -1.f, 93.f), INTDIR_FRONT, INTTYPE_AUTOMOVE,  _vec3(17.f, -1.f, 88.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ArcherRoomAutoTrack2", pGameObject), E_FAIL);

	pGameObject = AutoTrack::Create(m_pGraphicDev, _vec3(9.f, 0.5f, 0.5f), COL_INTERACTION, _vec3(9.f, -1.f, 100.f), INTDIR_LEFT, INTTYPE_AUTOMOVE, _vec3(17.f, -1.f, 100.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ArcherRoomAutoTrack3", pGameObject), E_FAIL);

	pGameObject = AutoTrack::Create(m_pGraphicDev, _vec3(0.5f, 0.5f, 12.f), COL_INTERACTION, _vec3(-1.f, -1.f, 88.f), INTDIR_BACK, INTTYPE_AUTOMOVE, _vec3(-1.f, -1.f, 100.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ArcherRoomAutoTrack4", pGameObject), E_FAIL);

	pGameObject = AutoTrack::Create(m_pGraphicDev, _vec3(0.5f, 0.5f, 0.5f), COL_INTERACTION, _vec3(-1.f, -1.f, 74.f), INTDIR_BACK, INTTYPE_AUTOMOVE, _vec3(-1.f, -1.f, 74.f), true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ArcherRoomAutoTrack5", pGameObject), E_FAIL);

    // SlimeRoom
	pGameObject = InteractionTrigger::Create(m_pGraphicDev, _vec3(5.f, 1.f, 5.f), COL_INTERACTION, _vec3(-46.f, -41.f, 39.f), ROOMTYPE_SLIME, INTTYPE_EVENT, TRITYPE_SPAWN | TRITYPE_SEAL, 12);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SlimeRoomTrigger", pGameObject), E_FAIL);

    // LadderRoom
    pGameObject = InteractionTrigger::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(78.f, -6.f, 50.f), ROOMTYPE_LADDER, INTTYPE_FLOATINGFLOOR, TRITYPE_BLOCK | TRITYPE_SPAWN, 7);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LadderRoomTriggerFF", pGameObject), E_FAIL);

    pGameObject = AutoTrack::Create(m_pGraphicDev, _vec3(1.f, 7.f, 1.f), COL_INTERACTION, _vec3(58.f, -3.f, 32.f), INTDIR_UP, INTTYPE_AUTOMOVE, _vec3(58.f, -7.f, 32.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LadderRoomAutoTrack0", pGameObject), E_FAIL);

    pGameObject = AutoTrack::Create(m_pGraphicDev, _vec3(0.5f, 0.5f, 0.5f), COL_INTERACTION, _vec3(58.f, 7.f, 32.f), INTDIR_UP, INTTYPE_AUTOMOVE, _vec3(58.f, 7.f, 32.f), true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LadderRoomAutoTrack1", pGameObject), E_FAIL);

    pGameObject = InteractionTrigger::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(78.f, 4.f, 32.f), ROOMTYPE_LADDER, INTTYPE_EVENT, TRITYPE_SEAL);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LadderRoomTrigerSeal", pGameObject), E_FAIL);

	pGameObject = MWLever::Create(m_pGraphicDev, D3DXVECTOR3{ 78.f, -8.f, 50.f }, D3DXVECTOR3{ -1.0f,0.0f,0.0f }, D3DXVECTOR3{ 0.0f,1.0f,0.0 });
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LadderRoomLever0", pGameObject), E_FAIL);

	pGameObject = MWLever::Create(m_pGraphicDev, D3DXVECTOR3{ 78.f, 3.f, 32.f }, D3DXVECTOR3{ -1.0f,0.0f,0.0f }, D3DXVECTOR3{ 0.0f,1.0f,0.0 });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LadderRoomLever0", pGameObject), E_FAIL);

    // CenterRoom
	pGameObject = InteractionTrigger::Create(m_pGraphicDev, _vec3(1.f, 1.f, 1.f), COL_INTERACTION, _vec3(7.f, -2.f, 32.f), ROOMTYPE_CENTER, INTTYPE_EVENT, TRITYPE_BLOCK | TRITYPE_SEAL);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CenterRoomTrigger", pGameObject), E_FAIL);

    pGameObject = InteractionTrigger::Create(m_pGraphicDev, _vec3(2.f, 0.1f, 2.f), COL_INTERACTION, _vec3(7.f, -2.f, 39.f), ROOMTYPE_CENTER, INTTYPE_EVENT, TRITYPE_WARP);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CenterRoomWarp", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Stage1::Init_Layer_UI(const wstring& pLayerTag)
{
    Engine::Layer* pLayer = Engine::Layer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::GameObject* pUI = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT Stage1::Init_Layer_ColliderBox(const wstring& pLayerTag)
{
    Engine::Layer* pLayer = Engine::Layer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);


    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

Stage1* Stage1::Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer)
{
    Stage1* pInstance = new Stage1(pGraphicDev);

    if (FAILED(pInstance->Init_Stage1(_ppPlayer)))
    {
        Safe_Release(pInstance);

        MSG_BOX("Stage1 Create Failed");
        return nullptr;
    }

    return pInstance;
}

void Stage1::Free()
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

// [우채] : UI 활성화를 위한 임시 코드!추후 제거할 것!
void Stage1::Test_ShopItem()
{
    TradeInv* NPCInv = dynamic_cast<NonPlayerCharacter*>(pTestNPC)->Get_TradeInv();
    MeleeWeapon* weapon = MeleeWeapon::Create(m_pGraphicDev, L"Proto_SwordTexture", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"바울의 검",
        ITEMID_WEAPON, ITEM_MELEEWEAPON, 250, 1, false, false, 10, 0);
    if (weapon && NPCInv)
    {
        NPCInv->AddItemToTradeInv(weapon);
    }

    Armor* armor = Armor::Create(m_pGraphicDev, L"Proto_LeatherChest", _vec3{0.3f, 0.3f, 0.3f}, COL_DROPITEM, L"관선의 갑옷", ITEMID_ARMOR,
        ITEM_CHEST, 750, 1, false, false);
    if (armor && NPCInv)
    {
        NPCInv->AddItemToTradeInv(armor);
    }
}
