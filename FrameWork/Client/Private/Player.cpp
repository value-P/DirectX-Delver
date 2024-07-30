#include "stdafx.h"
#include "Player.h"
#include "NonPlayerCharacter.h"
#include "SingleCamera.h"
#include "MainApp.h"
#include "MouseSlot.h"

#include "ItemBase.h"
#include "InventoryBase.h"
#include "QuickSlot.h"
#include "EquipmentSlot.h"

#include "UI.h"
#include "UI_InvBG.h"
#include "UI_ShopBG.h"
#include "UI_LevelUpBG.h"
#include "UI_Button.h"

#include "MeleeWeapon.h"
#include "RangedWeapon.h"
#include "MagicWeapon.h"
#include "Armor.h"
#include "Shield.h"
#include "Accessory.h"
#include "Consumable.h"

#include "Rope.h"
#include "Magic.h"
#include "Arrow.h"
#include "InteractableBase.h"
#include "MovableWall.h"
#include "InteractionTrigger.h"
#include "AutoTrack.h"

#include "Export_Utility.h"
#include "Export_System.h"

Player::Player(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
	
}

Player::Player(const Player& rhs)
	: Super(rhs)
{
	
}

Player::~Player()
{
	
}

HRESULT Player::Add_Component(wstring textureKey)
{
	Component* pComponent = nullptr;

	// 플레이어 인벤토리 - 아이템 저장용
	pComponent = m_pInv = dynamic_cast<InventoryBase*>(Engine::Clone_Proto(L"Proto_Inventory"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PlayerInventory", pComponent });

	// 퀵슬롯 - 나중에 옮길지도???
	pComponent = m_pQuick = dynamic_cast<QuickSlot*>(Engine::Clone_Proto(L"Proto_QuickSlot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PlayerQuickSlot", pComponent });

	// 장비창
	pComponent = m_pEquip = dynamic_cast<EquipmentSlot*>(Engine::Clone_Proto(L"Proto_EquipmentSlot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Proto_PlayerEquipmentSlot", pComponent });

	m_pTextureCom = dynamic_cast<Texture*>(ProtoMgr::GetInstance()->Clone_Proto(L"Proto_PlayerHit"));
	if (nullptr == m_pTextureCom) { return E_FAIL; }
	m_mapComponent[ID_STATIC].insert({ L"Proto_PlayerHit",m_pTextureCom });


	return S_OK;
}

void Player::Key_Input(const _float& fTimeDelta)
{
	_vec3 vLook, vRight;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	D3DXVECTOR3 vForward{ vLook.x,0.0f,vLook.z };
	D3DXVec3Normalize(&vForward, &vForward);

	if (UIManager::GetInstance()->GetHUD() == UIManager::GetInstance()->GetCurUI())
	{
		if (m_bFix)
		{
			m_bFix = false;
		}

		if (m_bIsOnTrack == false)
		{
			// Temp
			if (Engine::Get_DIKeyState(DIK_W) & 0x80)
			{
				m_pTransformCom->Move_Forward(&vForward, fTimeDelta, m_creatureStat->iMove_Speed - m_fDebuffSpeed);
			}

			if (Engine::Get_DIKeyState(DIK_S) & 0x80)
			{
				m_pTransformCom->Move_Forward(&vForward, fTimeDelta, -(m_creatureStat->iMove_Speed - m_fDebuffSpeed));
			}

			if (Engine::Get_DIKeyState(DIK_D) & 0x80)
			{
				m_pTransformCom->Move_Forward(&vRight, fTimeDelta, m_creatureStat->iMove_Speed - m_fDebuffSpeed);
			}

			if (Engine::Get_DIKeyState(DIK_A) & 0x80)
			{
				m_pTransformCom->Move_Forward(&vRight, fTimeDelta, -(m_creatureStat->iMove_Speed - m_fDebuffSpeed));
			}

			if (Engine::Get_DIKeyState(DIK_W) & 0x80 || Engine::Get_DIKeyState(DIK_S) & 0x80 || Engine::Get_DIKeyState(DIK_D) & 0x80 || Engine::Get_DIKeyState(DIK_A) & 0x80)
			{
				m_iPlayerState |= PLAYER_MOVE;

				//SoundMgr::GetInstance()->Play_Sound(L"feet_default_01", SOUND_WALK, 0.6f);

			}
			else
			{
				if (m_iPlayerState & PLAYER_MOVE)
				{
					m_iPlayerState -= PLAYER_MOVE;

					//SoundMgr::GetInstance()->Stop_Sound(SOUND_WALK);
				}
			}
		}

		if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
		{
			if (m_bOnTerrain)
			{
				m_fDeltaTime = 0.0f;
				m_iPlayerState |= PLAYER_JUMP;
			}
		}

		if (Engine::Key_Down('I'))
		{
			UIManager::GetInstance()->Activate_UI(L"UI_InvBG");
			SoundMgr::GetInstance()->Play_Sound_NoInterval(L"open_inventory", SOUND_MISC, 0.6f);
			m_bFix = true;
		}

		if (Engine::Get_DIMouseState(DIM_LB) & 0x80)
		{
			ItemBase* item = m_pQuick->GetActivatedItem();

			if (item)
			{
				if (item->IsAttacking() == true && item->Get_ItemType() == ITEM_RANGEDWEAPON)
				{
					if (static_cast<RangedWeapon*>(item)->IsArrowReady())
					{
						UpdateStat();
						m_iPlayerState |= PLAYER_WEAKATTACK;
					}
				}

				else if (item->IsAttacking() == false)
				{
					UpdateStat();
					m_iPlayerState |= PLAYER_WEAKATTACK;
				}
				
			}
		}

		if (Engine::Get_DIMouseState(DIM_RB) & 0x80)
		{
			ItemBase* item = m_pQuick->GetActivatedItem();
			
			if (item)
			{
				if (item->IsAttacking() == false)
				{
					UpdateStat();
					m_iPlayerState |= PLAYER_STRONGATTACK;
				}
			}
		}
	}
	else
	{
		
		if (Engine::Get_DIKeyState(DIK_F1) & 0X80)
		{
			if (m_bIsPressed == false)
			{
				MeleeWeapon* weapon = MeleeWeapon::Create(m_pGraphicDev, L"Proto_SwordTexture", _vec3{0.3f, 0.3f, 0.3f}, COL_PLAYERATTACK, L"Sword",
					ITEMID_WEAPON, ITEM_MELEEWEAPON, 250, 1, false, false, 10, 0);
				if (weapon && m_pInv)
				{
					m_pInv->AddItem(14, weapon);

					m_bIsPressed = true;
				}

			}
		}

		if (Engine::Get_DIKeyState(DIK_F2) & 0x80)
		{
			if (m_bIsPressed1 == false)
			{
				Armor* armor = Armor::Create(m_pGraphicDev, L"Proto_LeatherChest", _vec3{0.3f, 0.3f, 0.3f}, COL_DROPITEM, L"LeatherChest",
					ITEMID_ARMOR, ITEM_CHEST, 300, 1, false, false, 0, 10);
				if (armor && m_pInv)
				{
					m_pInv->AddItem(8, armor);

					m_bIsPressed1 = true;
				}
			}
		}

		if (Engine::Get_DIKeyState(DIK_F3) & 0x80)
		{
			if (m_bIsPressed2 == false)
			{
				Accessory* acc = Accessory::Create(m_pGraphicDev, L"Proto_SilverNecklace", _vec3{0.3f, 0.3f, 0.3f}, COL_DROPITEM, L"SilverNecklace",
					ITEMID_ACCESSORY, ITEM_NECKLACE, 500, 1, false, false);
				if (acc && m_pInv)
				{
					m_pInv->AddItem(9, acc);

					m_bIsPressed2 = true;
				}
			}
		}

		if (Engine::Get_DIKeyState(DIK_F4) & 0x80)
		{
			if (m_bIsPressed3 == false)
			{
				Consumable* con = Consumable::Create(m_pGraphicDev, L"Proto_HealthPotion", _vec3{0.3f, 0.3f, 0.3f}, COL_DROPITEM, L"HealpthPotion",
					ITEMID_CONSUMABLE, ITEM_CONSUMABLE, 50, 1, true, false, 0, 0, 30);
				if (con && m_pInv)
				{
					m_pInv->AddItem(11, con);

					m_bIsPressed3 = true;
				}
			}
		}

		if (Engine::Get_DIKeyState(DIK_F5) & 0x80)
		{
			if (m_bIsPressed4 == false)
			{
				Shield* shield = Shield::Create(m_pGraphicDev, L"Proto_WoodShield", _vec3{0.3f, 0.3f, 0.3f}, COL_PLAYERATTACK, L"WoodShield",
					ITEMID_SHIELD, ITEM_LEFTHAND, 300, 1, false, false);
				if (shield && m_pInv)
				{
					m_pInv->AddItem(17, shield);

					m_bIsPressed4 = true;
				}
			}
		}

		if (Engine::Get_DIKeyState(DIK_F6) & 0x80)
		{
			if (m_bIsPressed5 == false)
			{
				MagicWeapon* magicWeapon = MagicWeapon::Create(m_pGraphicDev, L"Proto_MagicWeapon", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"MagicWeapon",
					ITEMID_WEAPON, ITEM_MAGICWEAPON, 300, MWT_MAGIC_1, 1, false, false, 10);
				if (magicWeapon && m_pInv)
				{
					m_pInv->AddItem(15, magicWeapon);

					m_bIsPressed5 = true;
				}
			}
		}

		if (Engine::Get_DIKeyState(DIK_F7) & 0x80)
		{
			if (m_bIsPressed6 == false)
			{
				RangedWeapon* rangedWeapon = RangedWeapon::Create(m_pGraphicDev, L"Proto_RangedWeapon", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"RangedWeapon",
					ITEMID_WEAPON, ITEM_RANGEDWEAPON, 300, 1, false, false, 10);
				if (rangedWeapon && m_pInv)
				{
					m_pInv->AddItem(16, rangedWeapon);

					m_bIsPressed6 = true;
				}
			}
		}

		if (Engine::Get_DIKeyState(DIK_F8) & 0x80)
		{
			if (m_bIsPressed7 == false)
			{
				MagicWeapon* magicWeapon = MagicWeapon::Create(m_pGraphicDev, L"Proto_MagicWeapon", _vec3{ 0.3f, 0.3f, 0.3f }, COL_PLAYERATTACK, L"MagicWeapon1",
					ITEMID_WEAPON, ITEM_MAGICWEAPON, 300, MWT_MAGIC_0, 1, false, false, 10);
				if (magicWeapon && m_pInv)
				{
					m_pInv->AddItem(7, magicWeapon);

					m_bIsPressed7 = true;
				}
			}
		}
	}

	if (Engine::Key_Down('R'))
	{
		if (m_iPlayerState & PLAYER_RIDEROPE)
		{
			m_iPlayerState -= PLAYER_RIDEROPE;
			m_fDeltaTime = 0.0f;
		}
		else
		{
			Throw_Rope();
		}
	}

	if (Engine::Get_DIKeyState(DIK_9) & 0x80)
	{
		m_iSealCount = 3;
	}
}

void Player::Mouse_Move()
{
	//_vec3 vRight, vUp, vLook;
	//m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	//m_pTransformCom->Get_Info(INFO_UP, &vUp);
	//m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

	_long		dwMouseMoveX{ 0 }, dwMouseMoveY{ 0 };

	if (dwMouseMoveX = Engine::Get_DIMouseMove(DIMS_X))
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMoveX / 10.f));
	}
	if (dwMouseMoveY = Engine::Get_DIMouseMove(DIMS_Y))
	{
		//if(m_pTransformCom->GetAngle().x)
		m_pTransformCom->Rotation(ROT_X, D3DXToRadian(dwMouseMoveY / 10.f));
	}
}

void Player::Mouse_Fix()
{
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void Player::UI_Picking()
{
	POINT ptMouse;
	::GetCursorPos(&ptMouse);
	::ScreenToClient(g_hWnd, &ptMouse);

	_matrix matOrthoProj;
	D3DXMatrixOrthoLH(&matOrthoProj, 800.f, 600.f, 0.0f, 1.0f);

	vector<UIBase*> vecUI = Engine::GetCurUIVec();
	for (auto it : vecUI)
	{
		vector<vector<UI*>*> _vecChildUI_Group = dynamic_cast<UI*>(it)->Get_ChildUI_Group();
		for (auto iter : _vecChildUI_Group)
		{
			vector<UI*> childUIVec = *iter;
			for (size_t j = 0; j < childUIVec.size(); ++j)
			{
				VIBuffer* uiBuffer = childUIVec[j]->Get_VIBuffer();
				if (uiBuffer == nullptr)
					continue;

				_vec3* pPos = uiBuffer->Get_VtxPos();
				vector<INDEX32> uiIndex = uiBuffer->GetIndex();
				_matrix matWorld = *(childUIVec[j]->Get_MatIconWorld());

				Ray* ray = Engine::RayAtLocalSpace(m_pGraphicDev, ptMouse.x, ptMouse.y, &matOrthoProj, &matWorld);

				_vec3 vPickedPos{ 0.f, 0.f, 0.f };

				for (size_t i = 0; i < uiIndex.size(); ++i)
				{
					_vec3 v0 = pPos[uiIndex[i]._0];
					_vec3 v1 = pPos[uiIndex[i]._1];
					_vec3 v2 = pPos[uiIndex[i]._2];

					// 1. 픽킹 대상이 있다면!
					if (Engine::RayPicking(v0, v1, v2, vPickedPos))
					{
						childUIVec[j]->OnIconClicking(j);
						return;
					}
				}
			}
		}
	}

	if (!(Engine::Get_DIMouseState(DIM_LB) & 0x80))
	{
		MouseSlot* _pMouseSlot = dynamic_cast<MouseSlot*>(Engine::Get_Component(L"GameObject", L"Mouse", L"Proto_MouseSlot", ID_DYNAMIC));
		//if (!(_pMouseSlot->IsSlotEmpty()))
		//	_pMouseSlot->Item_Slot_Swap();
		if (!(_pMouseSlot->IsSlotEmpty()))
		{
			ItemBase* dropItem = _pMouseSlot->RemoveItemFromSlot();
			if (dropItem)
			{
				dropItem->Get_Collider()->SetColLayer(COL_DROPITEM);
				dropItem->Get_Collider()->SetSize(0.5f, 0.5f, 0.5f);
				dropItem->Get_Transform()->Set_Pos(Get_Transform()->Get_Info(INFO_POS) + Get_Transform()->Get_Info(INFO_LOOK) * 1.5f);
				dropItem->Get_Transform()->Set_Scale(_vec3(0.3f, 0.3f, 0.3f));
				Engine::Get_CurScene()->Get_Layer(L"GameObject")->Add_GameObject(L"DroppedItem", dropItem);
			}
		}
	}
	
}

void Player::Throw_Rope()
{
	if (m_iPlayerState & PLAYER_SHOOTROPE) return;
	if (m_iPlayerState & PLAYER_RIDEROPE) return;
	
	_vec3 Pos, Look;
	m_pTransformCom->Get_Info(INFO_POS, &Pos);
	m_pTransformCom->Get_Info(INFO_LOOK, &Look);
	Ray* ray = Ray::Create(m_pGraphicDev, Pos, Look);
	GameObject* pPickedObj = nullptr;
	_vec3 pickedPos;
	_bool result = Engine::RayCast(ray, &pPickedObj, &pickedPos, 30.f);

	if (result)
	{
		SoundMgr::GetInstance()->Play_Sound_NoInterval(L"swing-whoosh-110410", SOUND_EFFECT_3, 1.0f);
		m_iPlayerState |= PLAYER_SHOOTROPE;
		m_vRopeTargetPos = pickedPos;
	}

	Safe_Release(ray);
}

void Player::PlayerAction()
{
	if (PLAYER_RIDEROPE & m_iPlayerState)
	{
		PlayerRideRope();
	}
	if (PLAYER_SHOOTROPE & m_iPlayerState)
	{
		PlayerShootRope();
	}
	ItemBase* Item = m_pQuick->GetActivatedItem();
	if (Item)
	{
		if (m_iPlayerState & PLAYER_WEAKATTACK)
		{
			dynamic_cast<Weapon*>(m_pQuick->GetActivatedItem())->Set_WeaponState(WEAPON_WEAK);
		}
		else if (m_iPlayerState & PLAYER_STRONGATTACK)
		{
			dynamic_cast<Weapon*>(m_pQuick->GetActivatedItem())->Set_WeaponState(WEAPON_STRONG);
		}
		else if (m_iPlayerState & PLAYER_MOVE)
		{
			dynamic_cast<Weapon*>(m_pQuick->GetActivatedItem())->Set_WeaponState(WEAPON_MOVE);
		}
		else if (m_iPlayerState & PLAYER_IDLE)
		{
			dynamic_cast<Weapon*>(m_pQuick->GetActivatedItem())->Set_WeaponState(WEAPON_IDLE);
		}
	}
	
	ItemBase* item = m_pEquip->GetLeftHandItem();
	if (item)
	{
		if (m_iPlayerState & PLAYER_WEAKATTACK)
		{
			dynamic_cast<Shield*>(item)->Set_ItemState(ItemState::ITEMSTATE_IDLE);
		}
		else if (m_iPlayerState & PLAYER_STRONGATTACK)
		{
			dynamic_cast<Shield*>(item)->Set_ItemState(ItemState::ITEMSTATE_IDLE);
		}
		else if (m_iPlayerState & PLAYER_MOVE)
		{
			dynamic_cast<Shield*>(item)->Set_ItemState(ItemState::ITEMSTATE_MOVE);
		}
		else if (m_iPlayerState & PLAYER_IDLE)
		{
			dynamic_cast<Shield*>(item)->Set_ItemState(ItemState::ITEMSTATE_IDLE);
		}
	}
}

void Player::UpdateStat()
{
	const vector<ItemBase*> itemVec = m_pQuick->GetItemVec();
	for (auto it = itemVec.begin(); it != itemVec.end(); ++it)
	{
		if (*it)
		{
			switch ((*it)->Get_ItemType())
			{
			case ITEM_MELEEWEAPON:
			{
				(*it)->Set_ItemAttack(m_creatureStat->iAttack);
			}
			break;
			case ITEM_RANGEDWEAPON:
			{
				(*it)->Set_ItemAttack(m_creatureStat->iRangedAttack);
			}
			break;
			case ITEM_MAGICWEAPON:
			{
				(*it)->Set_ItemAttack(m_creatureStat->iMagicAttack);
			}
			break;
			}
		}
	}
}

void Player::LevelUp()
{
	SoundMgr::GetInstance()->Play_Sound_NoInterval(L"levelup", SOUND_LVUP, 0.6f);

	// Level 관련 처리
	Set_PlayerLevel(Get_PlayerLevel() + 1);
	Set_PlayerExp(0);
	Set_PlayerMaxExp(Get_PlayerMaxExp() + 50);

	SingleCamera::GetInstance()->PlayCameraShake(0.f, 0.f);
	MainApp::SetGlobalTimeScale(0.001f);
	
	// Show UI_LevelUp
	UIManager::GetInstance()->Init_UI(L"UI_LevelUpBG", UI_LevelUpBG::Create(m_pGraphicDev, 0.f, 0.f, 1.f, 1.f));
	UIManager::GetInstance()->Activate_UI(L"UI_LevelUpBG");
	m_bIs_closed_LevelUpBG = false;
	m_bFix = true;

	// Stat 관련 처리
	Set_CreatureMaxHp(Get_CreatureMaxHp() + 30);
	Set_CreatureHp(Get_CreatureMaxHp());

}

void Player::PlayerRideRope()
{
	// 로프의 시작점 세팅
	_vec3 vPlayerPos, vOffset, vUp;
	m_pTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);
	vOffset = -(vUp * 0.2f);
	vPlayerPos = vPlayerPos + vOffset;

	// 로프 위치로부터 목표까지의 방향벡터
	_vec3 vDir = m_vRopeTargetPos - vPlayerPos;

	// 로프원점과 목표지점 사이의 거리
	_float fTotalLength = D3DXVec3Length(&vDir);

	if (fTotalLength < 3.f)
	{
		m_iPlayerState -= PLAYER_RIDEROPE;
		m_fRopeLength = 0.f;
		m_fDeltaTime = 0.15f;
		return;
	}

	// 정규화
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransformCom->Move_Forward(&vDir, m_fRealTimeDelta, 20.f);
	m_pRope->Rope_Update(&vPlayerPos,&m_vRopeTargetPos,0.5f);
}

void Player::PlayerShootRope()
{
	// 로프가 늘어늘어남
	m_fRopeLength += m_fRealTimeDelta * 40.f;
	
	// 로프의 시작점 세팅
	_vec3 RopePos, vPlayerPos, vOffset, vUp;
	m_pTransformCom->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	D3DXVec3Normalize(&vUp, &vUp);
	vOffset = -(vUp * 0.2f);
	RopePos = vPlayerPos + vOffset;

	// 로프 위치로부터 목표까지의 방향벡터
	_vec3 vDir = m_vRopeTargetPos - vPlayerPos;

	// 로프원점과 목표지점 사이의 거리
	_float fTotalLength = D3DXVec3Length(&vDir);
	
	// 만약 로프가 목표지점에 근접했다면 타고 날아가기
	if (fTotalLength - m_fRopeLength < 0.1f)
	{
		m_iPlayerState -= PLAYER_SHOOTROPE;
		m_iPlayerState |= PLAYER_RIDEROPE;
		return;
	}

	// 정규화
	D3DXVec3Normalize(&vDir, &vDir);
	_vec3 EndPos = vPlayerPos + (vDir * m_fRopeLength * 0.5f);

	m_pRope->Rope_Update(&RopePos, &EndPos, 0.5f);
}

Player* Player::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring textureKey)
{
	Player* pInstance = new Player(pGraphicDev);

	if (FAILED(pInstance->Init_GameObject()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	if (FAILED(pInstance->Add_Component(textureKey)))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	// 만들 때 default로 해줄 꺼라 따로 매개변수로 안받고 처리
	CreatureStat stat = CreatureStat(100, 100, 5, 0, 5000, 0, 100, 1);
	pInstance->Set_CreatureStat(&stat);
	pInstance->Set_CreatureMagicAttack(5);
	pInstance->Set_CreatureRangedAttack(5);
	pInstance->Set_CreatureMove_Speed(6.f);

	return pInstance;
}

void Player::OnCollided()
{
	if (m_pColliderCom == nullptr)
		return;

	if (m_pColliderCom->GetColOBB() & COL_DEBUFF)
	{
		m_fDebuffSpeed = m_creatureStat->iMove_Speed * 0.5f;
	}
	else
	{
		m_fDebuffSpeed = 0.f;
	}

	// 지형, 벽 충돌
	if (m_pColliderCom->GetColOBB() & COL_WALL)
	{
		PlaneCollision(&m_bOnTerrain);
	}

	if (m_pColliderCom->GetColSphere() & COL_DROPITEM)
	{
		if (Engine::Get_DIKeyState(DIK_E) & 0x80)
		{
			if (m_bIsPressed8 == false)
			{
				// 아이템 습득 코드 여기서 호출할 것 Or 구현할 것
				list<pair<CINFO, _vec3>> itemList = m_pColliderCom->GetColSphere_Info(COL_DROPITEM);
				//for (auto it = itemList.begin(); it != itemList.end(); ++it)
				//{
				//	ItemBase* item = static_cast<ItemBase*>(it->first.m_pThis);
				//	if (item)
				//	{
				//		m_pInv->AddItem(item->Clone());
				//		item->SetIsDeleted(true);
				//	}
				//}
				ItemBase* item = static_cast<ItemBase*>(itemList.front().first.m_pThis);
				if (item)
				{
					if (item->Get_ItemType() == ITEM_GOLD)
					{
						m_creatureStat->iMoney += item->Get_ItemPrice();
					}
					else
					{
						if (item->Get_ItemType() == ITEM_MELEEWEAPON)
						{
							item->Get_Collider()->SetColLayer(COL_PLAYERATTACK);
						}
						else if (item->Get_ItemType() == ITEM_RANGEDWEAPON)
						{
							item->Get_Collider()->SetColLayer(COL_PLAYERATTACK);
						}
						else if (item->Get_ItemType() == ITEM_MAGICWEAPON)
						{
							item->Get_Collider()->SetColLayer(COL_PLAYERATTACK);
						}

						m_pInv->AddItem(item->Clone());
					}

					item->SetIsDeleted(true);
				}
			}
			m_bIsPressed = true;
		}
		else
		{
			m_bIsPressed8 = false;
		}
	}

	if (m_pColliderCom->GetColOBB() & COL_MONSTERATTACK)
	{
		if (m_bPlayerHittable)
		{
			m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_MONSTERATTACK).front().first.m_iDamage;
		}
	}

	if (m_pColliderCom->GetColOBB() & COL_MONSTER_PROJ)
	{
		if (m_bPlayerHittable)
		{
			m_creatureStat->iHp -= m_pColliderCom->GetColOBB_Info(COL_MONSTER_PROJ).front().first.m_iDamage;
		}
	}

	if (m_pColliderCom->GetColSphere() & COL_EXPLOSION)
	{
		if (m_bPlayerHittable)
		{
			m_creatureStat->iHp -= m_pColliderCom->GetColSphere_Info(COL_EXPLOSION).front().first.m_iDamage;
		}
	}

	if (m_pColliderCom->GetColSphere() & COL_ENVIRONMENT)
	{
		// 풀이나 나무 상자 충돌 코드 여기서 호출 또는 구현
	}

	if (m_pColliderCom->GetColSphere() & COL_INTERACTION)
	{
		
	}

	if (m_pColliderCom->GetColSphere() & COL_NPC)
	{
		//if (Engine::Get_DIKeyState(DIK_F) & 0X80 && !m_bFix)
		if (Engine::Key_Down('F') && !m_bFix)
		{
			list<pair<CINFO, _vec3>> npcList = m_pColliderCom->GetColSphere_Info(COL_NPC);
			float Dist{ 0.0f };
			float Near{ 100.0f };
			void* pNpc{ nullptr };
			for (list<pair<CINFO, _vec3>>::iterator iter = npcList.begin(); iter != npcList.end(); iter++)
			{
				Dist = D3DXVec3Length(&(m_pTransformCom->Get_Info(INFO_POS) - (*iter).first.m_vInfo[INFO_POS]));
				if (Dist < Near)
				{
					Near = Dist;
					pNpc = (*iter).first.m_pThis;
				}
			}
			NonPlayerCharacter* npc = static_cast<NonPlayerCharacter*>(pNpc);
			
			npc->Talknpc(); // 이 코드 분석할 것
			m_bFix = true;
		}
	}
}

void Player::Free()
{
	
	Safe_Release(m_pRope);
	__super::Free();
}

HRESULT Player::Init_GameObject()
{
	Super::Init_GameObject(); // Creature의 Init_GameObject()호출(Creature의 Add_Component를 호출하기 위함)
	m_pColliderCom->SetSize(1.f, 1.f, 1.f);
	m_pColliderCom->SetColLayer(COL_PLAYER);
	m_pColliderCom->SetThisPointer(this);

	UIManager::GetInstance()->Init_UI(L"UI_InvBG", UI_InvBG::Create(m_pGraphicDev, 0.f, 0.f, 1.f, 1.f));

	m_pRope = Rope::Create(m_pGraphicDev);
	
	SingleCamera::GetInstance()->SetPlayerTransform(m_pTransformCom);

	return S_OK;
}

Engine::_int Player::Update_GameObject(const _float& fTimeDelta)
{
	if (m_fDeltaTime < 1.f)
	{
		m_fDeltaTime += fTimeDelta;
	}

	if (!m_bPlayerHittable)
	{
		m_fHittableDelay -= fTimeDelta;
		if (m_fHittableDelay < 0.0f)
		{
			m_bPlayerHittable = true;
		}
	}

	if (m_bPlayerShakable == false)
	{
		m_fShakableDelay += fTimeDelta;
		if (m_fShakableDelay >= 2.f)
		{
			m_bPlayerShakable = true;
			m_fShakableDelay = 0.f;
		}
	}

	if (m_iPastFrameHP > m_creatureStat->iHp)
	{
		m_iPastFrameHP = m_creatureStat->iHp;
		m_bPlayerHittable = false;
		if (m_bPlayerShakable)
		{
			SingleCamera::GetInstance()->PlayCameraShake(0.2f, 0.1f);
			m_bPlayerShakable = false;
		}
		m_fHittableDelay = 0.5f;
		if (m_iPastFrameHP <= 0)
		{
			m_creatureStat->iHp = m_creatureStat->iMaxHp;
		}
	}
	else if (m_iPastFrameHP < m_creatureStat->iHp)
	{
		m_iPastFrameHP = m_creatureStat->iHp;
	}
	
	if (m_bIs_closed_LevelUpBG)
		MainApp::SetGlobalTimeScale(1.f);

	UIManager::GetInstance()->SetPlayerHittable(m_bPlayerHittable);
	
	if (m_pQuick->GetHealAmount() != 0)
	{
		m_creatureStat->iHp += m_pQuick->GetHealAmount();
		if (m_creatureStat->iHp >= m_creatureStat->iMaxHp)
			m_creatureStat->iHp = m_creatureStat->iMaxHp;

		SoundMgr::GetInstance()->Play_Sound(L"cons_drink", SOUND_EFFECT_3, 0.6f);

		m_pQuick->ResetHealAmount();
	}

	ItemBase* item = m_pQuick->GetActivatedItem();
	if (item)
	{
		if (item->IsAttacking() == false)
		{
			if (m_iPlayerState & PLAYER_WEAKATTACK)
			{
				m_iPlayerState -= PLAYER_WEAKATTACK;
			}
			
			if (m_iPlayerState & PLAYER_STRONGATTACK)
			{
				m_iPlayerState -= PLAYER_STRONGATTACK;
			}
		}
	}

	m_fRealTimeDelta = fTimeDelta;

	Key_Input(fTimeDelta);

	UI_Picking();

	UpdateStat();

	PlayerAction(); 

	if (m_creatureStat->iExp >= m_creatureStat->iMaxExp)
	{
		LevelUp();
	}

	if (!m_bOnTerrain || m_iPlayerState & PLAYER_JUMP && !(m_iPlayerState & PLAYER_RIDEROPE))
	{
		if (!m_bIsOnTrack)
		{
			FallSpeed = 10.f - 20.9f * m_fDeltaTime; // 플레이어 점프 수정 10
			m_pTransformCom->Move_Forward(&D3DXVECTOR3{ 0.0f,1.0f,0.0f }, m_fRealTimeDelta, FallSpeed);
		}
	}

	if (m_pTransformCom->Get_Info(INFO_POS).y < -50.0f)
	{
		m_pTransformCom->Set_Pos(0.0f, 5.0f, 0.0f);
		PlayerDefaultSetting(); 
	}

	m_pRope->Update_GameObject(fTimeDelta);
	_int	iExit = Super::Update_GameObject(fTimeDelta);

	if (m_pQuick->GetActivatedItem())
	{
		m_pQuick->GetActivatedItem()->Update_GameObject(fTimeDelta);
	}

	if (m_pEquip->GetLeftHandItem())
	{
		m_pEquip->GetLeftHandItem()->Update_GameObject(fTimeDelta);
	}

	Engine::AddCollider(m_pColliderCom);

	return iExit;
}

void Player::LateUpdate_GameObject()
{
	// 투사체 생성 테스트 코드
	if (Engine::Get_DIKeyState(DIK_Z) & 0X80)
	{
		//SingleCamera::GetInstance()->PlayCameraShake(1.0f, 0.1f);
		Effect* pEffect = Magic::Create(m_pGraphicDev, MAGIC_1, MAGIC_EX_2, m_pTransformCom->Get_Info(INFO_POS) + m_pTransformCom->Get_Info(INFO_LOOK), m_pTransformCom->Get_Info(INFO_LOOK), 20.0f, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
		pEffect->SetColType(COL_PLAYER_PROJ);
		Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->Add_GameObject(L"Magic", pEffect);
		/*Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->
		   Add_GameObject(L"Arrow", Arrow::Create(m_pGraphicDev, m_pTransformCom->Get_Info(INFO_POS) + m_pTransformCom->Get_Info(INFO_LOOK), m_pTransformCom->Get_Info(INFO_LOOK), 45.f, 1));*/
	}
	if (Engine::Get_DIKeyState(DIK_X) & 0X80)
	{
		Effect* pEffect = Magic::Create(m_pGraphicDev, MAGIC_0, MAGIC_EX_0, m_pTransformCom->Get_Info(INFO_POS) + m_pTransformCom->Get_Info(INFO_LOOK), m_pTransformCom->Get_Info(INFO_LOOK), 40.0f, 5, D3DCOLOR_ARGB(255, 0, 0, 255));
		pEffect->SetColType(COL_PLAYER_PROJ);
		Management::GetInstance()->Get_CurScene()->Get_Layer(LAYER_GAMEOBJECT)->Add_GameObject(L"Magic", pEffect);
	}
	
	// 마우스 고정일 때는 
	if (!m_bFix)
	{
		Mouse_Move();
		Mouse_Fix();
		
	}
	
	m_bOnTerrain = false;
	OnCollided();
	m_pColliderCom->ReBuild();
	if (m_bOnTerrain)
	{
		m_fDeltaTime = 0.478f;
		if (m_iPlayerState & PLAYER_JUMP)
		{
			m_iPlayerState -= PLAYER_JUMP;
		}
	}
	

	_vec3 vPos{};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);

	if (m_pQuick->GetActivatedItem())
	{
		m_pQuick->GetActivatedItem()->LateUpdate_GameObject();
	}
	if (m_pEquip->GetLeftHandItem())
	{
		m_pEquip->GetLeftHandItem()->LateUpdate_GameObject();
	}
	m_pRope->LateUpdate_GameObject();
	Super::LateUpdate_GameObject();
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
}

void Player::Render_GameObject()
{
	if (!SingleCamera::GetInstance()->IsPlaying())
	{
		if (m_pQuick->GetActivatedItem())
		{
			D3DXMATRIX matIdentity, matPreView;

			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
			D3DXMatrixIdentity(&matIdentity);

			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &matIdentity);

			m_pQuick->GetActivatedItem()->Render_GameObject();

			m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		}
		
		if (m_pEquip->GetLeftHandItem())
		{
			if (m_pQuick->GetActivatedItem() && m_pQuick->GetActivatedItem()->Get_ItemType() == ITEM_RANGEDWEAPON)
			{
				
			}

			else
			{
				D3DXMATRIX matIdentity, matPreView;

				m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
				D3DXMatrixIdentity(&matIdentity);

				m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
				m_pGraphicDev->SetTransform(D3DTS_VIEW, &matIdentity);

				m_pEquip->GetLeftHandItem()->Render_GameObject();

				m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
			}
		}

		if (PLAYER_RIDEROPE & m_iPlayerState || PLAYER_SHOOTROPE & m_iPlayerState)
		{
			m_pRope->Render_GameObject();
		}

		if (m_pColliderCom->GetColSphere() & COL_DROPITEM)
			Engine::Render_Font(L"Font_LogoText", L"E : 습득", &_vec4(0, 0, WINCX, WINCY), DT_NOCLIP | DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	}


	// Test
	
}

void Player::PlayerDefaultSetting()
{
	m_iPlayerState = 1;
	m_fDeltaTime = 0.2f;
	m_pTransformCom->Set_Rotation(&D3DXVECTOR3{ 0.0f,0.0f,0.0f });
}

	

