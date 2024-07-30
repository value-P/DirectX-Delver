#include "stdafx.h"
#include "QuickSlot.h"
#include "ItemBase.h"
#include "InventoryBase.h"
#include "Weapon.h"

#include "Export_System.h"
#include "Export_Utility.h"

QuickSlot::QuickSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	: Super(pGraphicDev)
{
}

QuickSlot::QuickSlot(const QuickSlot& rhs)
	: Super(rhs)
{
	m_itemVec = rhs.m_itemVec;
}

QuickSlot::~QuickSlot()
{
}

HRESULT QuickSlot::Init_QuickSlot()
{
	m_itemVec.resize(5, nullptr);

	return S_OK;
}

_int QuickSlot::Update_Component(const _float& fTimeDelta)
{
	ActivateItem();

	return 0;
}

void QuickSlot::LateUpdate_Component()
{
}

ItemBase* QuickSlot::FindItemFromSlot(ItemBase* item)
{
	auto it = find_if(m_itemVec.begin(), m_itemVec.end(), [item](ItemBase* temp)->bool{
		if (temp)
		{
			if (item->Get_ItemID() == temp->Get_ItemID())
				return true;

			return false;
		}
		
		return false;
		});

	if (it != m_itemVec.end())
	{
		return *it;
	}

	return nullptr;
}

bool QuickSlot::AddItemToSlot(ItemBase* item)
{
	// Stackable 이면 Stack처리 나중에 TODO

	for (size_t i = 0; i < m_itemVec.size(); ++i)
	{
		if (m_itemVec[i] == nullptr)
		{
			m_itemVec[i] = item;

			return true;
		}
	}

	return false;
}

bool QuickSlot::AddItemToSlot(_uint index, ItemBase* item)
{
	if (m_itemVec[index] == nullptr)
	{
		m_itemVec[index] = item;

		return true;
	}

	return false;
}

ItemBase* QuickSlot::MoveItemFromSlot(_uint index)
{
	if (m_itemVec[index])
	{
		ItemBase* outItem = m_itemVec[index];
		m_itemVec[index] = nullptr;

		return outItem;
	}

	return nullptr;
}

QuickSlot* QuickSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	QuickSlot* pInstance = new QuickSlot(pGraphicDev);
	if (FAILED(pInstance->Init_QuickSlot()))
	{
		Safe_Release(pInstance);

		MSG_BOX("QuickSlot Create Faile");
		return nullptr;
	}

	return pInstance;
}

Component* QuickSlot::Clone()
{
	return new QuickSlot(*this);
}

void QuickSlot::ActivateItem()
{
	// 이걸로 퀵슬롯의 아이템을 활성화 할 것임
	if (Engine::Get_DIKeyState(DIKEYBOARD_1) & 0x80)
	{
		if (m_itemVec[0])
		{
			if ((m_itemVec[0]->Get_ItemType() == ITEM_CONSUMABLE))
			{
				m_iHealAmount = m_itemVec[0]->Get_ItemHealAmount();
				if (m_itemVec[0]->Decrease_ItemCount())
				{
					// 쓰고 남은 개수가 1이상

				}
				else
				{
					// 쓰고 남은 개수가 0이하
					Safe_Release(m_itemVec[0]);
					m_itemVec[0] = nullptr;
				}
			}

			else
			{
				for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* Temp)->void {Temp ? Temp->Set_IsActivated(false) : 0; });
				if (m_itemVec[0])
				{
					m_pActivatedItem = m_itemVec[0];
					m_pActivatedItem->Set_IsActivated(true);

					Weapon* weapon = dynamic_cast<Weapon*>(m_pActivatedItem);
					if (weapon)
					{
						weapon->SetIsAttacking(false);
						weapon->SetIsArrowReady(false);
						weapon->RefreshRangedFrame();
						weapon->RefreshRangedTime();
						if (weapon->Get_ItemType() == ITEM_RANGEDWEAPON)
						{
							weapon->Set_WeaponState(WEAPON_IDLE);
						}
					}

					SoundMgr::GetInstance()->Stop_Sound(SOUND_MISC);
					SoundMgr::GetInstance()->Play_Sound(L"ui_equip_weapon", SOUND_MISC, 0.6f);
				}
				else
				{
					m_pActivatedItem = nullptr;
				}
			}
		}
	}
	if (Engine::Get_DIKeyState(DIKEYBOARD_2) & 0x80)
	{
		if (m_itemVec[1])
		{
			if ((m_itemVec[1]->Get_ItemType() == ITEM_CONSUMABLE))
			{
				m_iHealAmount = m_itemVec[1]->Get_ItemHealAmount();
				if (m_itemVec[1]->Decrease_ItemCount())
				{
					// 쓰고 남은 개수가 1이상

				}
				else
				{
					// 쓰고 남은 개수가 0이하
					Safe_Release(m_itemVec[1]);
					m_itemVec[1] = nullptr;
				}
			}

			else
			{
				for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* Temp)->void {Temp ? Temp->Set_IsActivated(false) : 0; });
				if (m_itemVec[1])
				{
					m_pActivatedItem = m_itemVec[1];
					m_pActivatedItem->Set_IsActivated(true);

					Weapon* weapon = dynamic_cast<Weapon*>(m_pActivatedItem);
					if (weapon)
					{
						weapon->SetIsAttacking(false);
						weapon->SetIsArrowReady(false);
						weapon->RefreshRangedFrame();
						weapon->RefreshRangedTime();
						if (weapon->Get_ItemType() == ITEM_RANGEDWEAPON)
						{
							weapon->Set_WeaponState(WEAPON_IDLE);
						}
					}

					SoundMgr::GetInstance()->Stop_Sound(SOUND_MISC);
					SoundMgr::GetInstance()->Play_Sound(L"ui_equip_weapon", SOUND_MISC, 0.6f);
				}
				else
				{
					m_pActivatedItem = nullptr;
				}
			}
		}
	}
	if (Engine::Get_DIKeyState(DIKEYBOARD_3) & 0x80)
	{
		if (m_itemVec[2])
		{
			if ((m_itemVec[2]->Get_ItemType() == ITEM_CONSUMABLE))
			{
				m_iHealAmount = m_itemVec[2]->Get_ItemHealAmount();
				if (m_itemVec[2]->Decrease_ItemCount())
				{
					// 쓰고 남은 개수가 1이상

				}
				else
				{
					// 쓰고 남은 개수가 0이하
					Safe_Release(m_itemVec[2]);
					m_itemVec[2] = nullptr;
				}
			}

			else
			{
				for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* Temp)->void {Temp ? Temp->Set_IsActivated(false) : 0; });
				if (m_itemVec[2])
				{
					m_pActivatedItem = m_itemVec[2];
					m_pActivatedItem->Set_IsActivated(true);

					Weapon* weapon = dynamic_cast<Weapon*>(m_pActivatedItem);
					if (weapon)
					{
						weapon->SetIsAttacking(false);
						weapon->SetIsArrowReady(false);
						weapon->RefreshRangedFrame();
						weapon->RefreshRangedTime();
						if (weapon->Get_ItemType() == ITEM_RANGEDWEAPON)
						{
							weapon->Set_WeaponState(WEAPON_IDLE);
						}
					}

					SoundMgr::GetInstance()->Stop_Sound(SOUND_MISC);
					SoundMgr::GetInstance()->Play_Sound(L"ui_equip_weapon", SOUND_MISC, 0.6f);
				}
				else
				{
					m_pActivatedItem = nullptr;
				}
			}
		}
	}
	if (Engine::Get_DIKeyState(DIKEYBOARD_4) & 0x80)
	{
		if (m_itemVec[3])
		{
			if ((m_itemVec[3]->Get_ItemType() == ITEM_CONSUMABLE))
			{
				m_iHealAmount = m_itemVec[3]->Get_ItemHealAmount();
				if (m_itemVec[3]->Decrease_ItemCount())
				{
					// 쓰고 남은 개수가 1이상

				}
				else
				{
					// 쓰고 남은 개수가 0이하
					Safe_Release(m_itemVec[3]);
					m_itemVec[3] = nullptr;
				}
			}

			else
			{
				for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* Temp)->void {Temp ? Temp->Set_IsActivated(false) : 0; });
				if (m_itemVec[3])
				{
					m_pActivatedItem = m_itemVec[3];
					m_pActivatedItem->Set_IsActivated(true);

					Weapon* weapon = dynamic_cast<Weapon*>(m_pActivatedItem);
					if (weapon)
					{
						weapon->SetIsAttacking(false);
						weapon->SetIsArrowReady(false);
						weapon->RefreshRangedFrame();
						weapon->RefreshRangedTime();
						if (weapon->Get_ItemType() == ITEM_RANGEDWEAPON)
						{
							weapon->Set_WeaponState(WEAPON_IDLE);
						}
					}

					SoundMgr::GetInstance()->Stop_Sound(SOUND_MISC);
					SoundMgr::GetInstance()->Play_Sound(L"ui_equip_weapon", SOUND_MISC, 0.6f);
				}
				else
				{
					m_pActivatedItem = nullptr;
				}
			}
		}
	}
	if (Engine::Get_DIKeyState(DIKEYBOARD_5) & 0x80)
	{
		if (m_itemVec[4])
		{
			if ((m_itemVec[4]->Get_ItemType() == ITEM_CONSUMABLE))
			{
				m_iHealAmount = m_itemVec[4]->Get_ItemHealAmount();
				if (m_itemVec[4]->Decrease_ItemCount())
				{
					// 쓰고 남은 개수가 1이상

				}
				else
				{
					// 쓰고 남은 개수가 0이하
					Safe_Release(m_itemVec[4]);
					m_itemVec[4] = nullptr;
				}
			}

			else
			{
				for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* Temp)->void {Temp ? Temp->Set_IsActivated(false) : 0; });
				if (m_itemVec[4])
				{
					m_pActivatedItem = m_itemVec[4];
					m_pActivatedItem->Set_IsActivated(true);

					Weapon* weapon = dynamic_cast<Weapon*>(m_pActivatedItem);
					if (weapon)
					{
						weapon->SetIsAttacking(false);
						weapon->SetIsArrowReady(false);
						weapon->RefreshRangedFrame();
						weapon->RefreshRangedTime();
						if (weapon->Get_ItemType() == ITEM_RANGEDWEAPON)
						{
							weapon->Set_WeaponState(WEAPON_IDLE);
						}
					}

					SoundMgr::GetInstance()->Stop_Sound(SOUND_MISC);
					SoundMgr::GetInstance()->Play_Sound(L"ui_equip_weapon", SOUND_MISC, 0.6f);
				}
				else
				{
					m_pActivatedItem = nullptr;
				}
			}
		}
	}
}

void QuickSlot::Free()
{
	for_each(m_itemVec.begin(), m_itemVec.end(), [](ItemBase* src) {
		if (src)
			Safe_Release(src);
		});

	m_itemVec.clear();
	m_itemVec.shrink_to_fit();

	Super::Free();
}
