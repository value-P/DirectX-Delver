#include "stdafx.h"

#include "UI_InvBG.h"
#include "UI_InvSlot.h"
#include "UI_EquipSlot.h"

#include "UI_Button.h"

#include "InventoryBase.h"
#include "ItemBase.h"
#include "EquipmentSlot.h"

UI_InvBG::UI_InvBG(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_InvBG::~UI_InvBG()
{
}

UI_InvBG* UI_InvBG::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_InvBG* pInstance = new UI_InvBG(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleParent::Create", MB_OK);
	}

	return pInstance;
}

HRESULT UI_InvBG::Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

	m_eUIReturn_Type = URT_SAVE;

	m_arrInvSlot.resize(18, nullptr);
	m_arrEquipSlot.resize(6, nullptr);

	if (FAILED(Add_UI()))
	{
		return E_FAIL;
	}
	return S_OK;
}

_int UI_InvBG::Update_UI(const _float& fTimeDelta)
{
	if (!(UIManager::GetInstance()->Key_Lock_UI(L"UI_InvBG")))
	{
		Key_Input(fTimeDelta);
	}

	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::Update_UI(fTimeDelta);
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���
	InventoryBase* pInventory = dynamic_cast<InventoryBase*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerInventory", ID_DYNAMIC));

	if (pInventory)
	{
		const vector<ItemBase*> itemVec = pInventory->GetItemVec();

		for (size_t i = 0; i < m_arrInvSlot.size(); ++i)
		{
			if (itemVec[i])
			{
				Texture* texture = itemVec[i]->Get_Texture();
				if (texture)
				{
					m_arrInvSlot[i]->Set_TextureItem(texture);
				}
			}
			else
				m_arrInvSlot[i]->Set_TextureItem(nullptr);
		}
	}
	
	EquipmentSlot* m_pEquip = dynamic_cast<EquipmentSlot*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerEquipmentSlot", ID_DYNAMIC));

	if (m_pEquip)
	{
		const vector<ItemBase*> itemVec = m_pEquip->GetItemVec();

		for (size_t i = 0; i < m_arrEquipSlot.size(); ++i)
		{
			if (itemVec[i])
			{
				Texture* texture = itemVec[i]->Get_Texture();
				if (texture)
				{
					m_arrEquipSlot[i]->Set_TextureItem(texture);
				}
			}
			else
				m_arrEquipSlot[i]->Set_TextureItem(nullptr);
		}
	}
	
	return 0;
}

void UI_InvBG::LateUpdate_UI()
{

	// �ڽ� UI�� �������� ������Ʈ �Ǳ����� ������� ������Ʈ�� ����
	UI::LateUpdate_UI();
	// �ڽ� UI�� �������� ������Ʈ ���Ŀ� ������� ������Ʈ�� �Ʒ���

}

void UI_InvBG::Render_UI()
{
	//UI::Render_UI();
	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [](UI* Temp)->void {Temp->Render_Icon(); });
}

void UI_InvBG::Update_Icon(const _float& fTimeDelta)
{
}

void UI_InvBG::LateUpdate_Icon()
{
}

void UI_InvBG::Render_Icon()
{
	UI::Render_Icon();
}

void UI_InvBG::OnMouseClicked(_uint index)
{
	UI::OnMouseClicked(index);
}

void UI_InvBG::OnIconClicked(_uint index)
{
	// �� Ŭ������ ���������� ǥ���ɶ� Ŭ�������� �Ҹ��� �ִٸ� ���⿡
}

void UI_InvBG::OnIconClicking(_uint index)
{
}

HRESULT UI_InvBG::Add_Component()
{
	return S_OK;
}

HRESULT UI_InvBG::Add_UI()
{
	
	// �κ��丮�� ��ü ���� ���� Ȯ��.
	float fRange_InvSlot = (sizeof(m_arrInvSlot) / sizeof(UI*));
	// �κ��丮�� ��� �� ����.
	int iInvRow = 3;
	int iInvColumn = 6;
	// �κ��丮�� ȭ�� �߾� ��ܿ� ��ġ ��Ű�� ���� ù��° ������ x��ǥ ���.
	float fFirstInvSlot_X = (float(WINCX) * 0.5f) - (float(iInvColumn) * 0.5f * float(INVENTORY_SIZE));
	float fFirstInvSlot_Y = INVENTORY_SIZE * 1.5f;


	// ��� â ��ü ���� ���� Ȯ��.
	// float fRange_EquipSlot = IB_END;
	// �κ��丮�� ��� �� ����.
	int iEquipRow = 3;
	int iEquipColumn = 2;
	// ��� â�� �κ��丮 ���� ���ʿ� ��ġ ��Ű�� ���� ù��° ������ x��ǥ ���.
	float fFirstEquipSlot_X = fFirstInvSlot_X - (float(INVENTORY_SIZE) * 2.2f);
	// ��� â �迭�� ��ȸ ������ �ʱⰪ ����.
	EQUIPSLOT_ID eEquipType = ES_HEAD;

	// ��� â ���� ��ȸ ���� ����.
	for (int iX = 0; iX < iEquipColumn; iX++)
	{
		for (int iY = 0; iY < iEquipRow; iY++)
		{
			m_arrEquipSlot[eEquipType] = UI_EquipSlot::Create(m_pGraphicDev, eEquipType, fFirstEquipSlot_X + (iX * INVENTORY_SIZE), fFirstInvSlot_Y + (iY * INVENTORY_SIZE), INVENTORY_SIZE, INVENTORY_SIZE);
			if (nullptr == m_arrEquipSlot[eEquipType])
			{
				return E_FAIL;
			}
			Add_ChildUI(m_arrEquipSlot[eEquipType]);
			eEquipType = EQUIPSLOT_ID(eEquipType + 1);
		}
	}
	Add_ChildUI_Group(&m_arrEquipSlot);
	
	if (!bStatusWindow_Activate)
	{
		// �κ��丮 �迭�� ��ȸ ������ �ʱⰪ ����.
		int iNum_InvSlot = 0;
		// �κ��丮 ���� ���� ����!
		for (int iY = 0; iY < iInvRow; iY++)
		{
			for (int iX = 0; iX < iInvColumn; iX++)
			{
				m_arrInvSlot[iNum_InvSlot] = UI_InvSlot::Create(m_pGraphicDev, fFirstInvSlot_X + (iX * INVENTORY_SIZE), fFirstInvSlot_Y + (iY * INVENTORY_SIZE), INVENTORY_SIZE, INVENTORY_SIZE);
				if (nullptr == m_arrInvSlot[iNum_InvSlot])
				{
					return E_FAIL;
				}
				Add_ChildUI(m_arrInvSlot[iNum_InvSlot]);
				iNum_InvSlot++;
			}
		}
	}
	Add_ChildUI_Group(&m_arrInvSlot);




	// �κ��丮 ��ü ��ư ���� Ȯ��.
	//float fRange_EquipSlot = ES_END;
	// �κ��丮�� ��� �� ����.
	// int iEquipRow = 3;
	// int iEquipColumn = 2;
	// �κ��丮 ��ư�� �κ��丮 ���� �����ʿ� ��ġ ��Ű�� ���� ù��° ������ x��ǥ ���.
	float fFirstInvButton_X = fFirstInvSlot_X + (INVENTORY_SIZE * iInvColumn);
	// �κ��丮 �迭�� ��ȸ ������ �ʱⰪ ����.
	//EQUIPSLOT_ID eEquipType = ES_HEAD;

	// [��ä] : �׷캰 �ڽ� UI�׷��� ����.(Picking ���� ����.)
	
	vector<wstring> strProtoKeyVec;

	strProtoKeyVec.clear();
	strProtoKeyVec.push_back(L"Proto_Inventory_Activate_Texture");
	strProtoKeyVec.push_back(L"Proto_Inventory_Inactivate_Texture");
	
	m_vecInventoryButton.push_back(UI_Button::Create(m_pGraphicDev, UBT_SWAP, strProtoKeyVec, fFirstInvButton_X, fFirstInvSlot_Y, 44, 30));
	if (nullptr == m_vecInventoryButton[0])
	{
		return E_FAIL;
	}
	Add_ChildUI(m_vecInventoryButton[0]);
	Add_ChildUI_Group(&m_vecInventoryButton);

	strProtoKeyVec.clear();
	strProtoKeyVec.push_back(L"Proto_Status_Activate_Texture");
	strProtoKeyVec.push_back(L"Proto_Status_Inactivate_Texture");
	
	m_vecStatusButton.push_back(UI_Button::Create(m_pGraphicDev, UBT_SWAP, strProtoKeyVec, fFirstInvButton_X, fFirstInvSlot_Y + 30, 44, 30));
	if (nullptr == m_vecStatusButton[0])
	{
		return E_FAIL;
	}
	Add_ChildUI(m_vecStatusButton[0]);
	Add_ChildUI_Group(&m_vecStatusButton);
	
	
	return S_OK;
}

void UI_InvBG::Free()
{
	UI::Free();
}

void UI_InvBG::Key_Input(const _float& fTimeDelta)
{
	if (Engine::Key_Down('I'))
	{
		UIManager::GetInstance()->Return_UI();
	}
}