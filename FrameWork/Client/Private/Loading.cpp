#include "stdafx.h"
#include "..\Header\Loading.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "InventoryBase.h"
#include "QuickSlot.h"
#include "MouseSlot.h"
#include "ParsingMgr.h"
#include "TradeInv.h"
#include "EquipmentSlot.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
	, m_eLoading(LOADING_END)
{
	ZeroMemory(m_szString, sizeof(m_szString));
	m_pGraphicDev->AddRef();
}


CLoading::~CLoading()
{
}

HRESULT CLoading::Init_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eLoading = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForStage()
{
	// Texture
	{
		// NPC
		{
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_NPC_Equipment_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/NPC/NPC_Equipment/NPC_Equipment_%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_NPC_Scholar_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/NPC/NPC_Scholar/NPC_Scholar_%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_NPC_Portion_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/NPC/NPC_Portion/NPC_Portion_%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_NPC_Guitar_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/NPC/NPC_Guitar/NPC_Guitar_%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_NPC_Vanguard_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/NPC/NPC_Vanguard/NPC_Vanguard_%d.png", 4)), E_FAIL);
		}
		// Monster
		{
			// Slime
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Slime_Idle_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Slime/Slime_Idle/Slime_Idle_%d.png", 5)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Slime_Attack_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Slime/Slime_Attack/Slime_Attack_%d.png", 7)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Slime_Hit_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Slime/Slime_Hit/Slime_Hit_%d.png", 8)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Slime_Death_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Slime/Slime_Death/Slime_Death_%d.png", 4)), E_FAIL);
			// Bat
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Bat_Idle_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Bat/Bat_Idle/Bat_Idle_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Bat_Attack_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Bat/Bat_Attack/Bat_Attack_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Bat_Hit_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Bat/Bat_Hit/Bat_Hit_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Bat_Death_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Bat/Bat_Death/Bat_Death_%d.png", 4)), E_FAIL);
			// Warrior
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Warrior_Idle_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Warrior/Warrior_Idle/Warrior_Idle_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Warrior_Attack_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Warrior/Warrior_Attack/Warrior_Attack_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Warrior_Hit_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Warrior/Warrior_Hit/Warrior_Hit_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Warrior_Death_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Warrior/Warrior_Death/Warrior_Death_%d.png", 4)), E_FAIL);
			// Wizard
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Wizard_Idle_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Wizard/Wizard_Idle/Wizard_Idle_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Wizard_Attack_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Wizard/Wizard_Attack/Wizard_Attack_%d.png", 7)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Wizard_Hit_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Wizard/Wizard_Hit/Wizard_Hit_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Wizard_Death_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Wizard/Wizard_Death/Wizard_Death_%d.png", 4)), E_FAIL);
			// Zombi
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Zombi_Idle_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Zombi/Zombi_Idle/Zombi_Idle_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Zombi_Attack_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Zombi/Zombi_Attack/Zombi_Attack_%d.png", 6)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Zombi_Hit_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Zombi/Zombi_Hit/Zombi_Hit_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Zombi_Death_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Zombi/Zombi_Death/Zombi_Death_%d.png", 4)), E_FAIL);

			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Druid_Idle_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Druid/Druid_Idle/Druid_Idle_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Druid_Attack_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Druid/Druid_Attack/Druid_Attack_%d.png", 5)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Druid_Hit_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Druid/Druid_Hit/Druid_Hit_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Druid_Death_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Druid/Druid_Death/Druid_Death_%d.png", 4)), E_FAIL);

			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Monk_Idle_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Monk/Monk_Idle/Monk_Idle_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Monk_Attack_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Monk/Monk_Attack/Monk_Attack_%d.png", 9)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Monk_Hit_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Monk/Monk_Hit/Monk_Hit_%d.png", 4)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Monk_Death_Texture", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Monster/Monk/Monk_Death/Monk_Death_%d.png", 4)), E_FAIL);

		}
		
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_SkyBoxTexture", Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/SkyBox/Sky_Space.dds", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_SkyBox_Village_Texture", Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/SkyBox/MidNight.dds", 1)), E_FAIL);
		//FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_SkyBoxTexture", Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/SkyBox/Villeage%d.dds", 1)), E_FAIL);

		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_GrassTexture", Texture::Create(m_pGraphicDev, TEX_NORMAL,	L"../../Resource/Texture/Explosion/Grass.png", 1)), E_FAIL);
		
		// Effect
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Fragment", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Effect/Fragment/Fragment_%d.png", 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_PlayerHit", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Effect/PlayerHit.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Fire", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Effect/Object/Fire_%d.png", 7)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Magic0", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Effect/Projectile/Magic0_%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Magic1", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Effect/Projectile/Magic1_%d.png", 8)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Magic_Ex_0", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Effect/MagicEx/Magic_Ex_0_%d.png", 4)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Magic_Ex_1", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Effect/MagicEx/Magic_Ex_1_%d.png", 3)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Magic_Ex_2", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Effect/MagicEx/Magic_Ex_2_%d.png", 9)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Arrow", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Effect/Projectile/Arrow.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Effect0", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Effect/EffectBundle/Effect0_%d.png", 14)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Effect1", AnimationTexture::Create(m_pGraphicDev, L"../../Resource/Texture/Effect/EffectBundle/Effect1_%d.png", 8)), E_FAIL);
		
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Balloon", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Effect/Object/Balloon.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Cube_Under_Lava", Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/Cube/Stage2_Cube_Under_Lava.dds", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Elevator", Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/Cube/Elevator.dds", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_MoveStone", Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/Cube/MoveStone.dds", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Stage2_Door", Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/Cube/Stage2_Door.dds", 1)), E_FAIL);
		
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_LeverStone", Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/Cube/LeverStone.dds", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_LeverWood", Texture::Create(m_pGraphicDev, TEX_CUBE, L"../../Resource/Texture/Cube/LeverWood.dds", 1)), E_FAIL);
		
		// UI
		{
			// HUD_HpBar
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_HpBar_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/HpBar.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_HpBar_Frame_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/HpBar_Frame.png", 1)), E_FAIL);

			// Slot UI
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_ItemSlot_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/ItemSlot.png", 1)), E_FAIL);

			// Window_Element UI
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Window_0_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/Window/Window_0.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Window_1_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/Window/Window_1.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Window_2_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/Window/Window_2.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Window_3_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/Window/Window_3.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Window_4_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/Window/Window_4.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Window_5_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/Window/Window_5.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Window_6_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/Window/Window_6.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Window_7_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/Window/Window_7.png", 1)), E_FAIL);
			
			// Element UI
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_DefaultButton_Up_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Button/DefaultButton_Up.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_DefaultButton_Down_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Button/DefaultButton_Down.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_CoinPile_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/CoinPile.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Continue_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/Continue.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_MouseCursor_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/MouseCursor.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_CrossCursor_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/CrossCursor.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_EmptyCursor_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Element/EmptyCursor.png", 1)), E_FAIL);

			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Inventory_Activate_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Button/Inventory_Activate.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Inventory_Inactivate_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Button/Inventory_Inactivate.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Status_Activate_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Button/Status_Activate.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Status_Inactivate_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_Button/Status_Inactivate.png", 1)), E_FAIL);

			// UI_EquipSlot
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_HeadSlot_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/HeadSlot.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_ChestSlot_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/ChestSlot.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_PantsSlot_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/PantsSlot.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_HandsSlot_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/HandsSlot.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_RingSlot_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/RingSlot.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_AmuletSlot_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/AmuletSlot.png", 1)), E_FAIL);

			// UI_EquipSlot_Able
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_HeadSlot_Able_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/HeadSlot_Able.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_ChestSlot_Able_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/ChestSlot_Able.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_PantsSlot_Able_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/PantsSlot_Able.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_HandsSlot_Able_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/HandsSlot_Able.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_RingSlot_Able_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/RingSlot_Able.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_AmuletSlot_Able_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_EquipSlot/AmuletSlot_Able.png", 1)), E_FAIL);

			// UI_levelup
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_LevelUpBG_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_LevelUp/LevelUpBG.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Attack_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_LevelUp/Attack.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Speed_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_LevelUp/Speed.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Agility_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_LevelUp/Agility.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Defense_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_LevelUp/Defense.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Health_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_LevelUp/Health.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Magic_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/UI_LevelUp/Magic.png", 1)), E_FAIL);

			// UI_Filter
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Table_Hover_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/Table_Hover.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Table_NoHover_Texture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/UI/Table_NoHover.png", 1)), E_FAIL);
		}

		// Item
		{
			// Sword
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_SwordTexture", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Weapon/Sword1_01.png", 1)), E_FAIL);
			// Armor
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_LeatherChest", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Armor/LeatherArmor_1.png", 1)), E_FAIL);
			// Accessory
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_SilverNecklace", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Accessory/SilverNecklace_0.png", 1)), E_FAIL);
			// Consumable
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_HealthPotion", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Consumable/Consumable_1.png", 1)), E_FAIL);
			// Shield
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_WoodShield", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Shield/WoodShield_%d.png", 2)), E_FAIL);
			// MagicWeapon
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_MagicWeapon", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/MagicWeapon/MagicWeapon_2.png", 1)), E_FAIL);
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_MagicWeapon2", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/MagicWeapon/MagicWeapon_1.png", 1)), E_FAIL);
			// RangedWeapon
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_RangedWeapon", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/RangedWeapon/Bow_%d.png", 4)), E_FAIL);

			// Gold
			FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Gold", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Gold/Gold_0.png", 1)), E_FAIL);

		}
		// Interactable
		// MovableWall
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_MovableWall", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Rect/Tile_Wall_7.png", 1)), E_FAIL);
		
		// FloatFloor
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_FloatingFloor", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Rect/Tile_Floor_20.png", 1)), E_FAIL);

		// Ladder
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Ladder", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Ladder/Ladder_0.png", 1)), E_FAIL);

		// Seal
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Seal0", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Rect/Tile_Wall_9.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Seal1", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Rect/Tile_Wall_15.png", 1)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Seal2", Texture::Create(m_pGraphicDev, TEX_NORMAL, L"../../Resource/Texture/Rect/Tile_Wall_14.png", 1)), E_FAIL);

	}
	// VI
	{
		//FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_TerrainTex", TerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_CubeTex", CubeTex::Create(m_pGraphicDev)), E_FAIL);

	}
	// Function
	{
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Transform", Transform::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Collider", Collider::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_Inventory", InventoryBase::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_QuickSlot", QuickSlot::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_MouseSlot", MouseSlot::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_EquipmentSlot", EquipmentSlot::Create(m_pGraphicDev)), E_FAIL);
		FAILED_CHECK_RETURN(Engine::Init_Proto(L"Proto_TradeInv", TradeInv::Create(m_pGraphicDev)), E_FAIL);

	}

	int soundCnt = 0;

	Engine::LoadSoundFile(&soundCnt);
	lstrcpy(m_szString, L"게임을 시작하려면 아무 키나 누르세요.");

	m_bFinish = true;

	return 0;
}

_uint CLoading::Loading_ForStage(LOADINGID eLoading)
{
	
	switch (eLoading)
	{
	case CLoading::LOADING_VILLEAGE:
		ParsingMgr::GetInstance()->Setting_MapObjects(SCENE_VILLEAGE,m_pCurScene);
		break;
	case CLoading::LOADING_STAGE1:
		ParsingMgr::GetInstance()->Setting_MapObjects(SCENE_STAGE1,m_pCurScene);
		break;
	case CLoading::LOADING_STAGE2:
		ParsingMgr::GetInstance()->Setting_MapObjects(SCENE_STAGE2,m_pCurScene);
		break;
	case CLoading::LOADING_BOSS:
		ParsingMgr::GetInstance()->Setting_MapObjects(SCENE_BOSS,m_pCurScene);
		break;
	}

	lstrcpy(m_szString, L"잠시 후 다음 스테이지로 이동됩니다.");

	m_bFinish = true;

	return 0;
}

unsigned int CLoading::Thread_Main(void * pArg)
{
	CLoading*		pLoading = reinterpret_cast<CLoading*>(pArg);
	_uint			iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	case LOADING_VILLEAGE:
		iFlag = pLoading->Loading_ForStage(LOADING_VILLEAGE);
		break;
	case LOADING_STAGE1:
		iFlag = pLoading->Loading_ForStage(LOADING_STAGE1);
		break;
	case LOADING_STAGE2:
		iFlag = pLoading->Loading_ForStage(LOADING_STAGE2);
		break;
	case LOADING_STAGE3:
		iFlag = pLoading->Loading_ForStage(LOADING_STAGE3);
		break;
	case LOADING_STAGE4:
		iFlag = pLoading->Loading_ForStage(LOADING_STAGE4);
		break;

	case LOADING_BOSS:
		iFlag = pLoading->Loading_ForStage(LOADING_BOSS);
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading, Scene* pCurScene)
{
	CLoading*		pInstance = new CLoading(pGraphicDev);

	if (pCurScene)
	{
		pInstance->m_pCurScene = pCurScene;
	}

	if (FAILED(pInstance->Init_Loading(eLoading)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Loading Create Failed");
		return nullptr;
	}
	
	return pInstance;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
