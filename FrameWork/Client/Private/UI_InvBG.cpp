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

	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
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

	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에

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
	// 이 클래스가 아이콘으로 표현될때 클릭했을때 불릴게 있다면 여기에
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
	
	// 인벤토리의 전체 슬롯 개수 확인.
	float fRange_InvSlot = (sizeof(m_arrInvSlot) / sizeof(UI*));
	// 인벤토리의 행과 열 설정.
	int iInvRow = 3;
	int iInvColumn = 6;
	// 인벤토리를 화면 중앙 상단에 위치 시키기 위해 첫번째 슬롯의 x좌표 계산.
	float fFirstInvSlot_X = (float(WINCX) * 0.5f) - (float(iInvColumn) * 0.5f * float(INVENTORY_SIZE));
	float fFirstInvSlot_Y = INVENTORY_SIZE * 1.5f;


	// 장비 창 전체 슬롯 개수 확인.
	// float fRange_EquipSlot = IB_END;
	// 인벤토리의 행과 열 설정.
	int iEquipRow = 3;
	int iEquipColumn = 2;
	// 장비 창을 인벤토리 기준 왼쪽에 위치 시키기 위해 첫번째 슬롯의 x좌표 계산.
	float fFirstEquipSlot_X = fFirstInvSlot_X - (float(INVENTORY_SIZE) * 2.2f);
	// 장비 창 배열읠 순회 생성할 초기값 설정.
	EQUIPSLOT_ID eEquipType = ES_HEAD;

	// 장비 창 슬롯 순회 생성 시작.
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
		// 인벤토리 배열읠 순회 생성할 초기값 설정.
		int iNum_InvSlot = 0;
		// 인벤토리 슬롯 생성 시작!
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




	// 인벤토리 전체 버튼 개수 확인.
	//float fRange_EquipSlot = ES_END;
	// 인벤토리의 행과 열 설정.
	// int iEquipRow = 3;
	// int iEquipColumn = 2;
	// 인벤토리 버튼을 인벤토리 기준 오른쪽에 위치 시키기 위해 첫번째 슬롯의 x좌표 계산.
	float fFirstInvButton_X = fFirstInvSlot_X + (INVENTORY_SIZE * iInvColumn);
	// 인벤토리 배열읠 순회 생성할 초기값 설정.
	//EQUIPSLOT_ID eEquipType = ES_HEAD;

	// [우채] : 그룹별 자식 UI그룹을 보관.(Picking 관련 도움.)
	
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