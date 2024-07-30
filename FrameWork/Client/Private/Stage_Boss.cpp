#include "stdafx.h"
#include "Stage_Boss.h"
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
#include "BossMonster1.h"
#include "BossMonster2.h"
#include "DebuffCol.h"
#include "VillageSkyBox.h"

#include "Export_Utility.h"
#include "Export_System.h"

Stage_Boss::Stage_Boss(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::Scene(pGraphicDev)
{
}

Stage_Boss::~Stage_Boss()
{
}

HRESULT Stage_Boss::Init_Stage_Boss(GameObject** _ppPlayer)
{
    m_pPlayer = *_ppPlayer;

    UIManager::GetInstance()->Init_UI(L"Proto_MyHUDTexture", MyHUD::Create(m_pGraphicDev));
    UIManager::GetInstance()->Activate_UI(UIManager::GetInstance()->GetHUD());

    m_pTextureCom = Texture::Create(m_pGraphicDev, TEX_NORMAL,
        L"../../Resource/Texture/UI/Ending.png");
    m_pBufferCom = dynamic_cast<RcTex*>(ProtoMgr::GetInstance()->Clone_Proto(L"Proto_RcTex"));

    
    FAILED_CHECK_RETURN(Init_Layer_Environment(LAYER_ENVI), E_FAIL);
    FAILED_CHECK_RETURN(Init_Layer_SkyBox(LAYER_SKYBOX), E_FAIL);
    FAILED_CHECK_RETURN(Init_Layer_GameObject(LAYER_GAMEOBJECT), E_FAIL);
    FAILED_CHECK_RETURN(Init_Layer_UI(LAYER_UI), E_FAIL);
    FAILED_CHECK_RETURN(Init_Layer_ColliderBox(LAYER_MAPCOL), E_FAIL);

    m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_BOSS, this);
    NULL_CHECK_RETURN(m_pLoading, E_FAIL);

    if (!*_ppPlayer)
    {
        *_ppPlayer = m_pPlayer;
    }

    Engine::Play_BGM(L"Boss", 0.5f);

    return S_OK;
}

_int Stage_Boss::Update_Scene(const _float& fTimeDelta)
{
	if (m_iCameraDirectionNum < 3 && !SingleCamera::GetInstance()->IsPlaying())
	{
		switch (m_iCameraDirectionNum)
		{
		case 0:
		{
			SingleCamera::GetInstance()->PlayCameraDirection2(D3DXVECTOR3{ -120.0f,55.0f,20.0f }, D3DXVECTOR3{ 115.0f,55.0f,20.0f }, D3DXVECTOR3{ 0.0f,23.0f,210.0f }, 5.0f, D3DXToRadian(75.0f));
			m_iCameraDirectionNum++;
		}
		break;
		case 1:
		{
			SingleCamera::GetInstance()->PlayCameraDirection2(D3DXVECTOR3{ 68.0f,30.0f,118.0f }, D3DXVECTOR3{ 98.0f,39.0f,148.0f }, D3DXVECTOR3{ 118.0f,31.f,168.0f }, 4.5f);
			m_iCameraDirectionNum++;
		}
		break;
		case 2:
		{
			SingleCamera::GetInstance()->PlayCameraDirection2(D3DXVECTOR3{ -60.0f,45.0f,110.0f }, D3DXVECTOR3{ -98.0f,29.0f,148.0f }, D3DXVECTOR3{ -118.0f,26.f,168.0f }, 4.5f);
			m_iCameraDirectionNum++;
        }
        break;
        }
    }
	int iExit{ 0 };
    if (!m_bClear)
    {
        iExit = __super::Update_Scene(fTimeDelta);
    }

    return iExit;
}

void Stage_Boss::LateUpdate_Scene()
{
    if (!m_bClear)
    {
		__super::LateUpdate_Scene();
    }
}

void Stage_Boss::Render_Scene()
{
    if (m_bClear)
    {
        D3DXMATRIX matWorld, matView, matProj;
        D3DXMatrixIdentity(&matWorld);
        D3DXMatrixIdentity(&matView);
        D3DXMatrixOrthoLH(&matProj, 2.0f, 2.0f, 0.0f, 1.0f);

        m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
        m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
        m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

        m_pTextureCom->Set_Texture();
        m_pBufferCom->Render_Buffer();
    }
}

void Stage_Boss::CheckLoading()
{
    if (m_pLoading)
    {
        m_pLoading->Get_Finish() ? m_bLoading = true : m_bLoading = false;
    }
}

HRESULT Stage_Boss::Init_Layer_Environment(const wstring& pLayerTag)
{
    Engine::Layer* pLayer = Engine::Layer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::GameObject* pGameObject = nullptr;
    
    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT Stage_Boss::Init_Layer_SkyBox(const wstring& pLayerTag)
{
    Engine::Layer* pLayer = Engine::Layer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::GameObject* pGameObject = nullptr;

    //SkyBox
    //pGameObject = CSkyBox::Create(m_pGraphicDev);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

    pGameObject = VillageSkyBox::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VillageSkyBox", pGameObject), E_FAIL);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT Stage_Boss::Init_Layer_GameObject(const wstring& pLayerTag)
{
    Engine::Layer* pLayer = Engine::Layer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::GameObject* pGameObject = nullptr;
    BossMonster1* pBoss1 = nullptr;
    BossMonster2* pBoss2 = nullptr;

    if (m_pPlayer)
    {
        FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
        m_pPlayer->Get_Transform()->Set_Pos(0.f, 9.f, 0.f);
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
        //pPlayer->Get_Transform()->Set_Pos(118.f, 45.f, 178.f);
        pPlayer->Get_Transform()->Set_Pos(-118.f, 45.f, 178.f);
    }

    // Mouse
    pGameObject = MouseCursor::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mouse", pGameObject), E_FAIL);

    // Palkia
    pGameObject = pBoss1 = BossMonster1::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    CreatureStat Boss1Stat = CreatureStat(200, 7, 5, 0);
    pBoss1->Get_Transform()->Set_Scale(5.f, 5.f, 5.f);
    pBoss1->Get_Transform()->Set_Pos(118.f,35.f,168.f);
    pBoss1->Set_Target(m_pPlayer);
    pBoss1->Set_CreatureStat(&Boss1Stat);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss1", pGameObject), E_FAIL);

    // Dialga
    pGameObject = pBoss2 = BossMonster2::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    CreatureStat Boss2Stat = CreatureStat(200, 7, 5, 0);
    pBoss2->Get_Transform()->Set_Scale(5.f, 5.f, 5.f);
    pBoss2->Get_Transform()->Set_Pos(-118.f,35.f,168.f);
    pBoss2->Set_Target(m_pPlayer);
    pBoss2->Set_CreatureStat(&Boss2Stat);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss2", pGameObject), E_FAIL);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT Stage_Boss::Init_Layer_UI(const wstring& pLayerTag)
{
    Engine::Layer* pLayer = Engine::Layer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    Engine::GameObject* pUI = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT Stage_Boss::Init_Layer_ColliderBox(const wstring& pLayerTag)
{
    Engine::Layer* pLayer = Engine::Layer::Create();
    NULL_CHECK_RETURN(pLayer, E_FAIL);

    GameObject* pColBox = DebuffCol::Create(m_pGraphicDev);
    NULL_CHECK_RETURN(pColBox, E_FAIL);

    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DebuffCol", pColBox), E_FAIL);

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

Stage_Boss* Stage_Boss::Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer)
{
    Stage_Boss* pInstance = new Stage_Boss(pGraphicDev);

    if (FAILED(pInstance->Init_Stage_Boss(_ppPlayer)))
    {
        Safe_Release(pInstance);

        MSG_BOX("Stage_Boss Create Failed");
        return nullptr;
    }

    return pInstance;
}

void Stage_Boss::Free()
{
    Safe_Release(m_pLoading);

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pBufferCom);

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
