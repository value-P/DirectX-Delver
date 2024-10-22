#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE {	DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	// 사운드 용
	enum CHANNELID {
		SOUND_EFFECT_1, 
		SOUND_EFFECT_2, 
		SOUND_EFFECT_3, 
		SOUND_MONSTER_HIT, 
		SOUND_UI_1, 
		SOUND_UI_2, 
		SOUND_BGM, 
		SOUND_LVUP, 
		SOUND_WALK, 
		SOUND_ATK_SWORD, 
		SOUND_ATK_BOW, 
		SOUND_ATK_MAGIC, 
		SOUND_INTERACTION, 
		SOUND_MISC, 
		SOUND_SLIME, 
		SOUND_BAT, 
		SOUND_WARRIOR, SOUND_WIZARD, 
		SOUND_ZOMBI, SOUND_DRUID, SOUND_MONK, MAXCHANNEL };

	// 바울 추가
	enum SCENETYPE {SCENE_LOGO,SCENE_VILLEAGE, SCENE_STAGE1, SCENE_STAGE2, SCENE_STAGE3,SCENE_STAGE4, SCENE_BOSS, SCENE_END};
	enum PREFABTYPE { PREFAB_BILLBOARD, PREFAB_RCTEX, PREFAB_CUBE, PREFAB_END };
	enum OBJECTTYPE { OBJ_MONSTER, OBJ_ITEM, OBJ_ENVI, OBJ_END };

	// 우채 추가
	enum UIRETURN_TYPE { URT_SAVE, URT_DELETE, URT_END };
	enum UIBUTTON_TYPE { UBT_CLOSE, UBT_SWAP, UBT_END };
	enum BUTTONSTATUS_ID { BS_UP, BS_DOWN, BS_END };
	enum EQUIPSLOT_ID { ES_HEAD, ES_CHEST, ES_PANTS, ES_HANDS, ES_RING, ES_AMULET,  ES_END };
	enum SLOTTYPE_ID { ST_QUICK, ST_INVENTORY, ST_EQUIP, ST_END };
	enum MOUSE_BUTTONSTATUS_ID { MBS_UP, MBS_DOWN, MBS_END };
	enum MONSTER_ID { MI_SLIME, MI_BAT, MI_WARRIOR, MI_WIZARD, MI_ZOMBI, MI_DRUID, MI_MONK, MI_END };
	enum AbilityCard_ID { ACI_ATTACK, ACI_SPEED, ACI_AGILITY, ACI_DEFENSE, ACI_HEALTH, ACI_MAGIC, ACI_END };

	enum COLLAYER
	{
		COL_PLAYER = 1 << 0,
		COL_MONSTER = 1 << 1,
		COL_WALL = 1 << 2,
		COL_DROPITEM = 1 << 3,
		COL_PLAYERATTACK = 1 << 4,
		COL_MONSTERATTACK = 1 << 5,
		COL_EXPLOSION = 1 << 6,
		COL_ENVIRONMENT = 1 << 7,
		COL_FRAGMENT = 1 << 8,
		COL_NPC = 1 << 9,
		COL_PLAYER_PROJ = 1 << 10,
		COL_MONSTER_PROJ = 1 << 11,
		COL_INTERACTION = 1 << 12,
		COL_DEBUFF = 1 << 13,
		// COL_END에 총 레이어의 갯수를 넣었음
		COL_END = 14
	};

	enum ITEMTYPE
	{
		ITEM_HEAD,
		ITEM_CHEST,
		ITEM_LEG,
		ITEM_LEFTHAND,
		ITEM_RING,
		ITEM_NECKLACE,
		

		/// /////////////////

		ITEM_MELEEWEAPON,
		ITEM_RANGEDWEAPON,
		ITEM_MAGICWEAPON,
		ITEM_CONSUMABLE,
		ITEM_SHIELD,
		ITEM_PROJECTILE,
		ITEM_GOLD,

		ITEM_END,
	};

	// 모든 객체를 통합 ID로 관리할까??
	enum ITEMID
	{
		ITEMID_WEAPON = 1,
		ITEMID_ARMOR = 2,
		ITEMID_ACCESSORY = 3,
		ITEMID_CONSUMABLE = 4,
		ITEMID_SHIELD = 5,
		ITEMID_PROJECTILE = 6,

		ITEMID_END,
	};

	enum EQUIPSLOTIDX
	{
		EQUIP_HEAD,
		EQUIP_CHEST,
		EUQIP_LEG,
		EQUIP_LEFTHAND,
		EQUIP_NECKLACE,
		EQUIP_RING,

		EQUIPSLOTEND,
	};

	enum NPCTYPE
	{
		NPC_TRADE,
		NPC_DIALOG,

		NPC_END,
	};

	enum BossPhase
	{
		PHASE1,	// 보스 방 입장시 초기 상태
		PHASE2, // 여기서부터 패턴시작
	};

	enum BossState
	{
		BOSS_SLEEP,
		BOSS_AWAKE,
		BOSS_IDLE,
		BOSS_PATTERN1,
		BOSS_PATTERN2,
		BOSS_PATTERN3,
		BOSS_GROGGY,
		BOSS_HIT,
		BOSS_DEAD,

		BOSS_END
	};

}
#endif // Engine_Enum_h__
