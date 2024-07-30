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

// ������
#include "MeleeWeapon.h"
#include "RangedWeapon.h"
#include "MagicWeapon.h"
#include "Armor.h"
#include "Shield.h"
#include "Accessory.h"

#include "Consumable.h"

// ����
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
			L"������ �����Ͻñ� �� ������ ����� ì��ñ� �ٶ��ϴ�",
			L"���ѵ� ����������, �ʿ��Ͻ� ��ǰ�� ��� �غ�Ǿ� �ֽ��ϴ�."
		});
	dynamic_cast<NonPlayerCharacter*>(pGameObject)->Set_NPCDialog(pEquipmentDialogVec);
	// [��ä] : NPC ������ �߰�!
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
			L"�� ������ �����ô� �ǰ���?",
			L"��� ��Ȳ�� ����ؼ� ������ ȸ�� ������ ��õ�� �帱�Կ�.",
			L"ġ������ ��Ȳ���� �ż��ϰ� ü���� ȸ���� �� �־��."
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
			L"���ο� ���谡�ñ���. �ݰ����ϴ�.",
			L"���� ��忡�� �İ߳��� ������Դϴ�. "
		});
	pScholarDialogVec.push_back(
		{
			L"������ �����Ͻñ� �� �̸� ���帳�ϴ�."
		});
	pScholarDialogVec.push_back(
		{
			L"�� ������ ������ �����մϴ�."
		});
	pScholarDialogVec.push_back(
		{
			L"�ռ� ���� ���谡���� ������ ����������,",
			L"�������� ������ ���������� ���� �����ϴ�."
		});
	pScholarDialogVec.push_back(
		{
			L"�ε�, ������ ������ ���ֽñ� ��Ź�帮�ڽ��ϴ�."
		});
	pScholarDialogVec.push_back(
		{
			L"�׷����� �����Ͻðڴٸ�,",
			L"������ �̿��Ͽ� ��� ����� ���߽ð�",
			L"�����Ͻñ� ����帳�ϴ�."
		});
	pScholarDialogVec.push_back(
		{
			L"�׷� ���谡���� ������ ����� �Բ��ϱ� ���ڽ��ϴ�."
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
			L"���谡��!Ȥ�� ������ ���ô� ���̽Ű���?"
		});
	pGuitarDialogVec.push_back(
		{
			L"�׷��ٸ� ���谡�Բ� �Ѱ��� ��Ź�� �帮�� �ͽ��ϴ�.",
			L"���� �ȿ��� �� ���Ḧ ã�ƺ����ֽðھ�� ?"
		});
	pGuitarDialogVec.push_back(
		{
			L"�׷��ٸ� ���谡�Բ� �Ѱ��� ��Ź�� �帮�� �ͽ��ϴ�.",
			L"���� �ȿ��� �� ���Ḧ ã�ƺ����ֽðھ�� ?"
		});
	pGuitarDialogVec.push_back(
		{
			L"���� ù��° ������ �� �Ѱ�ٴ� �ҽ��� ���ķ�",
			L"������ �����Ǿ� ������ ���� �ϰ� �ִ��������ϴ�."
		});
	pGuitarDialogVec.push_back(
		{
			L"�׷��� ��Ź�帳�ϴ�.",
			L"������ Ž���Ͻø� �� ���Ḧ ã���ּ���."
		});
	pGuitarDialogVec.push_back(
		{
			L"������ �������� Ž�谡���� ������ ���� ì��ô� ���� �켱�Դϴ�."
		});
	pGuitarDialogVec.push_back(
		{
			L"���� ������ �� �ց�����, �ε� ������ ��Ź�帳�ϴ�."
		});
	pGuitarDialogVec.push_back(
		{
			L"����� ���ϴ�.Ž�谡��!"
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
			L"�׸�. �� �̻� �����ϸ� �����ϰڽ��ϴ�.",
			L"�и� ����߽��ϴ�. ����������."
		});
	pVanguardDialogVec.push_back(
		{
			L"��? �� ������ �̸��� ��� �ƽ���?",
			L"��... �� ���ᰡ �����̱���. ���� �˼��մϴ�."
		});
	pVanguardDialogVec.push_back(
		{
			L"������ �����ϴ� �� �������� ���ܹ�����",
			L"�߿����� �ҽ��� ������ �� �������ϴ�."
		});
	pVanguardDialogVec.push_back(
		{
			L"�ҽ��� �����ּż� ���� �����մϴ�."
		});
	pVanguardDialogVec.push_back(
		{
			L"���谡�԰� �Բ� ���� ������ �������ϰ� ������,",
			L"���� ���� ���������� ���� ��� �̰��� �ӹ����� �� �� �����ϴ�.",
		});
	pVanguardDialogVec.push_back(
		{
			L"��� �� ������ �帮�ڽ��ϴ�.",
			L"�ι�° �������� ���� ����ϰ� ����Ͻ� �� ���� �̴ϴ�."
		});
	pVanguardDialogVec.push_back(
		{
			L"����� ���ϴ�.Ž�谡��!"
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

// [��ä] : UI Ȱ��ȭ�� ���� �ӽ� �ڵ�!���� ������ ��!
void Villeage::Equipment_ShopItem()
{
	TradeInv* NPCInv = dynamic_cast<NonPlayerCharacter*>(m_pNPC_Equipment)->Get_TradeInv();
	MeleeWeapon* weapon = MeleeWeapon::Create(m_pGraphicDev, L"Proto_SwordTexture", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"��ο� �׸����� �ܰ�",
		ITEMID_WEAPON, ITEM_MELEEWEAPON, 250, 1, false, false, 10, 0);
	if (weapon && NPCInv)
	{
		NPCInv->AddItemToTradeInv(weapon);
	}
	RangedWeapon* rangedWeapon = RangedWeapon::Create(m_pGraphicDev, L"Proto_RangedWeapon", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"�������� ���", ITEMID_WEAPON, ITEM_RANGEDWEAPON, 300, 1, false, false, 10);

	if (rangedWeapon && NPCInv)
	{
		NPCInv->AddItemToTradeInv(rangedWeapon);
	}

	MagicWeapon* magicWeapon = MagicWeapon::Create(m_pGraphicDev, L"Proto_MagicWeapon", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"������ ���� ������",
		ITEMID_WEAPON, ITEM_MAGICWEAPON, 300, MWT_MAGIC_1, 1, false, false, 10);
	if (magicWeapon && NPCInv)
	{
		NPCInv->AddItemToTradeInv(magicWeapon);
	}

	MagicWeapon* magicWeapon2 = MagicWeapon::Create(m_pGraphicDev, L"Proto_MagicWeapon2", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"�ڿ��� ���� ������",
		ITEMID_WEAPON, ITEM_MAGICWEAPON, 300, MWT_MAGIC_0, 1, false, false, 10);
	if (magicWeapon2 && NPCInv)
	{
		NPCInv->AddItemToTradeInv(magicWeapon2);
	}

	Armor* armor = Armor::Create(m_pGraphicDev, L"Proto_LeatherChest", _vec3{ 0.3f, 0.3f, 0.3f }, COL_DROPITEM, L"�ҿ� �ޱ� ���� ����",
		ITEMID_ARMOR, ITEM_CHEST, 300, 1, false, false, 0, 10);
	if (armor && NPCInv)
	{
		NPCInv->AddItemToTradeInv(armor);
	}

	Shield* shield = Shield::Create(m_pGraphicDev, L"Proto_WoodShield", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"�ܴ��� ������ ����", ITEMID_SHIELD, ITEM_LEFTHAND, 300, 1, false, false);
	if (shield && NPCInv)
	{
		NPCInv->AddItemToTradeInv(shield);
	}

	Accessory* acc = Accessory::Create(m_pGraphicDev, L"Proto_SilverNecklace", _vec3{ 0.3f, 0.3f, 0.3f }, COL_DROPITEM, L"������ ���� �����",
		ITEMID_ACCESSORY, ITEM_NECKLACE, 500, 1, false, false);
	if (acc && NPCInv)
	{
		NPCInv->AddItemToTradeInv(acc);
	}

}

void Villeage::Portion_ShopItem()
{
	TradeInv* NPCInv = dynamic_cast<NonPlayerCharacter*>(m_pNPC_Portion)->Get_TradeInv();
	
	Consumable* con = Consumable::Create(m_pGraphicDev, L"Proto_HealthPotion", _vec3{ 0.3f, 0.3f, 0.3f }, COL_DROPITEM, L"ġ�� ����",
		ITEMID_CONSUMABLE, ITEM_CONSUMABLE, 50, 1, true, false, 0, 0, 30);
	if (con && NPCInv)
	{
		NPCInv->AddItemToTradeInv(con);
	}
}
