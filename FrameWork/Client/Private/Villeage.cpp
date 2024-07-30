#include "stdafx.h"
#include "Villeage.h"
#include "SkyBox.h"
#include "Player.h"
#include "Loading.h"
#include "MouseCursor.h"

#include "NonPlayerCharacter.h"
#include "MyHUD.h"

#include "TradeInv.h"
#include "Monster.h"
#include "InteractionTrigger.h"
#include "VillageSkyBox.h"

#include "Export_Utility.h"
#include "Export_System.h"

// 아이템
#include "MeleeWeapon.h"
#include "RangedWeapon.h"
#include "MagicWeapon.h"
#include "Armor.h"
#include "Shield.h"
#include "Accessory.h"

#include "Consumable.h"

// 몬스터
#include "Slime.h"
#include "Bat.h"
#include "Warrior.h"
#include "Wizard.h"
#include "Zombi.h"
#include "Druid.h"
#include "Monk.h"


Villeage::Villeage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::Scene(pGraphicDev)
{
}

Villeage::~Villeage()
{
}

HRESULT Villeage::Init_Villeage(GameObject** _ppPlayer)
{
	m_pPlayer = *_ppPlayer;
	
	UIManager::GetInstance()->Init_UI(L"Proto_MyHUDTexture", MyHUD::Create(m_pGraphicDev));
	UIManager::GetInstance()->Activate_UI(UIManager::GetInstance()->GetHUD());

	FAILED_CHECK_RETURN(Init_Layer_Environment(LAYER_ENVI), E_FAIL);
	FAILED_CHECK_RETURN(Init_Layer_SkyBox(LAYER_SKYBOX), E_FAIL);
	FAILED_CHECK_RETURN(Init_Layer_GameObject(LAYER_GAMEOBJECT), E_FAIL);
	FAILED_CHECK_RETURN(Init_Layer_UI(LAYER_UI), E_FAIL);
	FAILED_CHECK_RETURN(Init_Layer_ColliderBox(LAYER_MAPCOL), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_VILLEAGE, this);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	if (!*_ppPlayer)
	{
		*_ppPlayer = m_pPlayer;
	}
	
	Engine::Play_BGM(L"05_Villeage", 1.f);
	return S_OK;
}

_int Villeage::Update_Scene(const _float& fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void Villeage::LateUpdate_Scene()
{
	__super::LateUpdate_Scene();
}

void Villeage::Render_Scene()
{
}

void Villeage::CheckLoading()
{
	if (m_pLoading)
	{
		m_pLoading->Get_Finish() ? m_bLoading = true : m_bLoading = false;
	}
}

HRESULT Villeage::Init_Layer_Environment(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::GameObject* pGameObject = nullptr;
	
	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Villeage::Init_Layer_SkyBox(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::GameObject* pGameObject = nullptr;

	//SkyBox
	pGameObject = VillageSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"VillageSkyBox", pGameObject), E_FAIL);

	

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Villeage::Init_Layer_GameObject(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::GameObject* pGameObject = nullptr;

	if (m_pPlayer)
	{
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
		m_pPlayer->Get_Transform()->Set_Pos(0.f, 15.f, 0.f);
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
		pPlayer->Get_Transform()->Set_Pos(0.f, 15.f, 0.f);
	}

	// Warp To Stage1
	pGameObject = InteractionTrigger::Create(m_pGraphicDev, _vec3(2.f, 1.f, 2.f), COL_INTERACTION, _vec3(122.f, -21.f, 12.f), ROOMTYPE_VILLAGE, INTTYPE_EVENT, TRITYPE_WARP);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"InteractionTriggerVillage", pGameObject), E_FAIL);

	CreatureStat monsterStat;

	// NPC
	// 1. NPC_Equipment
	pGameObject = NonPlayerCharacter::Create(m_pGraphicDev, L"Proto_NPC_Equipment_Texture", 4.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Equipment", pGameObject), E_FAIL);
	pGameObject->Get_Transform()->Set_Pos(63.f, -10.f, -13.0f);
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCType(NPC_TRADE);
	vector<vector<wstring>> pEquipmentDialogVec;
	pEquipmentDialogVec.push_back(
		{
			L"던전에 입장하시기 전 무장을 충분히 챙기시길 바랍니다",
			L"제한된 물량이지만, 필요하신 물품은 모두 준비되어 있습니다."
		});
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCDialog(pEquipmentDialogVec);
	// [우채] : NPC 아이템 추가!
	m_pNPC_Equipment = pGameObject;
	Equipment_ShopItem();
	

	// 2. NPC_Portion
	pGameObject = NonPlayerCharacter::Create(m_pGraphicDev, L"Proto_NPC_Portion_Texture", 4.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Portion", pGameObject), E_FAIL);
	pGameObject->Get_Transform()->Set_Pos(71.f, -10.f, -1.f);
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCType(NPC_TRADE);
	vector<vector<wstring>> pPortionDialogVec;
	pPortionDialogVec.push_back(
		{
			L"긴 여행을 떠나시는 건가요?",
			L"비상 상황을 대비해서 강력한 회복 물약을 추천해 드릴게요.",
			L"치명적인 상황에서 신속하게 체력을 회복할 수 있어요."
		});
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCDialog(pPortionDialogVec);
	
	m_pNPC_Portion = pGameObject;
	Portion_ShopItem();

	pGameObject = NonPlayerCharacter::Create(m_pGraphicDev, L"Proto_NPC_Scholar_Texture", 4.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Scholar", pGameObject), E_FAIL);
	pGameObject->Get_Transform()->Set_Pos(75.f, -10.f, -11.f);
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCType(NPC_DIALOG);
	vector<vector<wstring>> pScholarDialogVec;
	pScholarDialogVec.push_back(
		{ 
			L"새로운 모험가시군요. 반갑습니다.",
			L"저는 길드에서 파견나온 조사관입니다. "
		});
	pScholarDialogVec.push_back(
		{
			L"던전에 입장하시기 전 미리 경고드립니다."
		});
	pScholarDialogVec.push_back(
		{
			L"이 던전은 지극히 위험합니다."
		});
	pScholarDialogVec.push_back(
		{
			L"앞서 많은 모험가들이 던전에 도전했지만,",
			L"아직까지 무사히 빠져나오신 분은 없습니다."
		});
	pScholarDialogVec.push_back(
		{
			L"부디, 현명한 선택을 해주시길 부탁드리겠습니다."
		});
	pScholarDialogVec.push_back(
		{
			L"그럼에도 도전하시겠다면,",
			L"상인을 이용하여 장비를 충분히 갖추시고",
			L"도전하시길 권장드립니다."
		});
	pScholarDialogVec.push_back(
		{
			L"그럼 모험가님의 도전에 행운이 함께하길 빌겠습니다."
		});
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCDialog(pScholarDialogVec);

	
	pGameObject = NonPlayerCharacter::Create(m_pGraphicDev, L"Proto_NPC_Guitar_Texture", 4.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Guitar", pGameObject), E_FAIL);
	pGameObject->Get_Transform()->Set_Pos(61.f, -10.f, -1.f);
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCType(NPC_DIALOG);
	vector<vector<wstring>> pGuitarDialogVec;
	pGuitarDialogVec.push_back(
		{
			L"모험가님!혹시 던전에 들어가시는 길이신가요?"
		});
	pGuitarDialogVec.push_back(
		{
			L"그렇다면 모험가님께 한가지 부탁을 드리고 싶습니다.",
			L"던전 안에서 제 동료를 찾아보아주시겠어요 ?"
		});
	pGuitarDialogVec.push_back(
		{
			L"그렇다면 모험가님께 한가지 부탁을 드리고 싶습니다.",
			L"던전 안에서 제 동료를 찾아보아주시겠어요 ?"
		});
	pGuitarDialogVec.push_back(
		{
			L"얼마전 첫번째 던전을 막 넘겼다는 소식을 이후로",
			L"연락이 두절되어 걱정을 많이 하고 있던차였습니다."
		});
	pGuitarDialogVec.push_back(
		{
			L"그래서 부탁드립니다.",
			L"던전을 탐험하시며 제 동료를 찾아주세요."
		});
	pGuitarDialogVec.push_back(
		{
			L"하지만 무엇보다 탐험가님의 안전을 먼저 챙기시는 것이 우선입니다."
		});
	pGuitarDialogVec.push_back(
		{
			L"힘든 여정일 수 있곘지만, 부디 도움을 부탁드립니다."
		});
	pGuitarDialogVec.push_back(
		{
			L"행운을 빕니다.탐험가님!"
		});
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCDialog(pGuitarDialogVec);
	




	pGameObject = NonPlayerCharacter::Create(m_pGraphicDev, L"Proto_NPC_Vanguard_Texture", 4.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_Vanguard", pGameObject), E_FAIL);
	pGameObject->Get_Transform()->Set_Pos(68.0f, -10.0f, 11.0f);
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCType(NPC_DIALOG);
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_m_bQuest_NPC(true);
	vector<vector<wstring>> pVanguardDialogVec;
	pVanguardDialogVec.push_back(
		{
			L"그만. 더 이상 접근하면 공격하겠습니다.",
			L"분명 경고했습니다. 물러나세요."
		});
	pVanguardDialogVec.push_back(
		{
			L"앗? 제 동료의 이름을 어떻게 아시죠?",
			L"아... 제 동료가 보내셨군요. 정말 죄송합니다."
		});
	pVanguardDialogVec.push_back(
		{
			L"던전을 조사하던 중 연락망이 끊겨버려서",
			L"야영지로 소식을 전달할 수 없었습니다."
		});
	pVanguardDialogVec.push_back(
		{
			L"소식을 전해주셔서 정말 감사합니다."
		});
	pVanguardDialogVec.push_back(
		{
			L"모험가님과 함께 남은 여정을 마무리하고 싶지만,",
			L"현재 전투 휴유증으로 인해 잠시 이곳에 머물러야 될 것 같습니다.",
		});
	pVanguardDialogVec.push_back(
		{
			L"대신 제 로프를 드리겠습니다.",
			L"두번째 던전에서 아주 요긴하게 사용하실 수 있을 겁니다."
		});
	pVanguardDialogVec.push_back(
		{
			L"행운을 빕니다.탐험가님!"
		});
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCDialog(pVanguardDialogVec);






	// Mouse
	pGameObject = MouseCursor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Mouse", pGameObject), E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Villeage::Init_Layer_UI(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::GameObject* pUI = nullptr;

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT Villeage::Init_Layer_ColliderBox(const wstring& pLayerTag)
{
	Engine::Layer* pLayer = Engine::Layer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

Villeage* Villeage::Create(LPDIRECT3DDEVICE9 pGraphicDev, GameObject** _ppPlayer)
{
	Villeage* pInstance = new Villeage(pGraphicDev);

	if (FAILED(pInstance->Init_Villeage(_ppPlayer)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Villeage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void Villeage::Free()
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
void Villeage::Equipment_ShopItem()
{
	TradeInv* NPCInv = dynamic_cast<NonPlayerCharacter*>(m_pNPC_Equipment)->Get_TradeInv();
	MeleeWeapon* weapon = MeleeWeapon::Create(m_pGraphicDev, L"Proto_SwordTexture", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"어두운 그림자의 단검",
		ITEMID_WEAPON, ITEM_MELEEWEAPON, 250, 1, false, false, 10, 0);
	if (weapon && NPCInv)
	{
		NPCInv->AddItemToTradeInv(weapon);
	}
	RangedWeapon* rangedWeapon = RangedWeapon::Create(m_pGraphicDev, L"Proto_RangedWeapon", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"독수리뿔 장궁", ITEMID_WEAPON, ITEM_RANGEDWEAPON, 300, 1, false, false, 10);

	if (rangedWeapon && NPCInv)
	{
		NPCInv->AddItemToTradeInv(rangedWeapon);
	}

	MagicWeapon* magicWeapon = MagicWeapon::Create(m_pGraphicDev, L"Proto_MagicWeapon", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"영원한 빛의 마법봉",
		ITEMID_WEAPON, ITEM_MAGICWEAPON, 300, MWT_MAGIC_1, 1, false, false, 10);
	if (magicWeapon && NPCInv)
	{
		NPCInv->AddItemToTradeInv(magicWeapon);
	}

	MagicWeapon* magicWeapon2 = MagicWeapon::Create(m_pGraphicDev, L"Proto_MagicWeapon2", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"자연의 비전 마법봉",
		ITEMID_WEAPON, ITEM_MAGICWEAPON, 300, MWT_MAGIC_0, 1, false, false, 10);
	if (magicWeapon2 && NPCInv)
	{
		NPCInv->AddItemToTradeInv(magicWeapon2);
	}

	Armor* armor = Armor::Create(m_pGraphicDev, L"Proto_LeatherChest", _vec3{ 0.3f, 0.3f, 0.3f }, COL_DROPITEM, L"불에 달궈 만든 갑옷",
		ITEMID_ARMOR, ITEM_CHEST, 300, 1, false, false, 0, 10);
	if (armor && NPCInv)
	{
		NPCInv->AddItemToTradeInv(armor);
	}

	Shield* shield = Shield::Create(m_pGraphicDev, L"Proto_WoodShield", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"단단한 참나무 방패", ITEMID_SHIELD, ITEM_LEFTHAND, 300, 1, false, false);
	if (shield && NPCInv)
	{
		NPCInv->AddItemToTradeInv(shield);
	}

	Accessory* acc = Accessory::Create(m_pGraphicDev, L"Proto_SilverNecklace", _vec3{ 0.3f, 0.3f, 0.3f }, COL_DROPITEM, L"투명한 별빛 목걸이",
		ITEMID_ACCESSORY, ITEM_NECKLACE, 500, 1, false, false);
	if (acc && NPCInv)
	{
		NPCInv->AddItemToTradeInv(acc);
	}

}

void Villeage::Portion_ShopItem()
{
	TradeInv* NPCInv = dynamic_cast<NonPlayerCharacter*>(m_pNPC_Portion)->Get_TradeInv();
	
	Consumable* con = Consumable::Create(m_pGraphicDev, L"Proto_HealthPotion", _vec3{ 0.3f, 0.3f, 0.3f }, COL_DROPITEM, L"치유 물약",
		ITEMID_CONSUMABLE, ITEM_CONSUMABLE, 50, 1, true, false, 0, 0, 30);
	if (con && NPCInv)
	{
		NPCInv->AddItemToTradeInv(con);
	}
}
