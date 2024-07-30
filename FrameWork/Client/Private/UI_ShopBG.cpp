#include "stdafx.h"
#include "UI_ShopBG.h"
#include "UI_ShopCatalogue.h"
#include "TradeInv.h"
#include "ItemBase.h"
#include "DecorIMG.h"
#include "UI_Button.h"
#include "InventoryBase.h"
UI_ShopBG::UI_ShopBG(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_ShopBG::~UI_ShopBG()
{
}

UI_ShopBG* UI_ShopBG::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, TradeInv* _pTradeInv, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_ShopBG* pInstance = new UI_ShopBG(_pGraphicDevice);
	
	if (FAILED(pInstance->Init_UI(_pTradeInv, _fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleParent::Create", MB_OK);
	}
	
	return pInstance;
}

HRESULT UI_ShopBG::Init_UI(TradeInv* _pTradeInv, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

	m_eUIReturn_Type = URT_DELETE;

	m_pTradeInv = _pTradeInv;
	// 아 음...
	m_pShopItemList = m_pTradeInv->Get_ShopItemList();
	m_iPlayerMoney = m_pTradeInv->Get_PlayerMoney();
	//m_vecItemList = _vecItem;
	//m_iPlayerMoney = _iPlayer_Price;
	/*
	m_matBG._11 = 256.f;
	m_matBG._22 = 192.f;

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	*/
	if (FAILED(Add_UI()))
	{
		return E_FAIL;
	}
	return S_OK;
}

_int UI_ShopBG::Update_UI(const _float& fTimeDelta)
{
	
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	if (!(UIManager::GetInstance()->Key_Lock_UI(L"UI_ShopBG")))
	{
		Key_Input(fTimeDelta);
	}
	return 0;
}

void UI_ShopBG::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();

	if (dynamic_cast<UI_Button*>(m_pClose_Button)->Is_ButtonClicked())
	{
		UIManager::GetInstance()->Return_UI();
		UIManager::GetInstance()->SelectDelete_UI(L"UI_ShopBG");
	}
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void UI_ShopBG::Render_UI()
{
	//UI::Render_UI();
	
	for_each(m_vecChildUI.begin(), m_vecChildUI.end(), [](UI* Temp)->void {Temp->Render_Icon(); });
	Engine::Render_Font(L"Font_RoundShape_Number", to_wstring(m_pTradeInv->Get_PlayerMoney()), &_vec4(520, fButtonFonst_Y, 569, fButtonFonst_Y+10), DT_NOCLIP | DT_SINGLELINE | DT_VCENTER | DT_RIGHT, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	Engine::Render_Font(L"Font_RoundShape_Button", L"나가기", &_vec4(232, fButtonFonst_Y, 298, fButtonFonst_Y+23), DT_NOCLIP| DT_CENTER | DT_SINGLELINE | DT_VCENTER, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

void UI_ShopBG::Update_Icon(const _float& fTimeDelta)
{

}

void UI_ShopBG::LateUpdate_Icon()
{
}

void UI_ShopBG::Render_Icon()
{
	UI::Render_Icon();
	
}

void UI_ShopBG::OnMouseClicked(_uint index)
{
	UI::OnMouseClicked(index);
}

void UI_ShopBG::OnIconClicked(_uint index)
{
	// 이 클래스가 아이콘으로 표현될때 클릭했을때 불릴게 있다면 여기에
}

void UI_ShopBG::OnIconClicking(_uint index)
{
}

HRESULT UI_ShopBG::Add_Component()
{
	/*
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	pComponent = Clone_Proto(L"Proto_Window_Texture");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureBG = dynamic_cast<Texture*>(pComponent);
	return S_OK;
	*/
	return S_OK;
}

HRESULT UI_ShopBG::Add_UI()
{
	// 동전더미, 판매 아이템들,버튼
	vector<wstring> strProtoKeyVec;
	// 해당 상점의 아이템 개수 확인.
	int iRange_ShopItem = (*m_pShopItemList).size();
	// 첫번째 아이템 시작 위치 계산.
	float fFirstShopItem_Y = (float(WINCY) * 0.5f) - (float(iRange_ShopItem) * float(SHOPITEM_SIZE) * 0.5f);


	// Window
	UI* pUI_InputCheck = nullptr;
	wstring strWindowProtoKey = L"Proto_Window_" + to_wstring(iRange_ShopItem-1) + L"_Texture";
	pUI_InputCheck = DecorIMG::Create(m_pGraphicDev, strWindowProtoKey, 144, 44, 512, 512);
	if (nullptr == pUI_InputCheck)
	{
		return E_FAIL;
	}
	Add_ChildUI(pUI_InputCheck);

	

	float ShopfButton_Y;
	float CoinPile_Y;
	// Shop Catalogue

	strProtoKeyVec.clear();
	strProtoKeyVec.push_back(L"Proto_Table_Hover_Texture");
	strProtoKeyVec.push_back(L"Proto_Table_NoHover_Texture");
	for (int i = 0; i < iRange_ShopItem; i++)
	{
		
		pUI_InputCheck = DecorIMG::Create(m_pGraphicDev, (*m_pShopItemList)[i]->Get_ItemTextureKey(), 232, fFirstShopItem_Y + (SHOPITEM_SIZE * i), SHOPITEM_SIZE, SHOPITEM_SIZE);
		if (nullptr == pUI_InputCheck)
		{
			return E_FAIL;
		}
		Add_ChildUI(pUI_InputCheck);
		
		m_vecShopItem.push_back(UI_ShopCatalogue::Create(m_pGraphicDev, strProtoKeyVec, m_pTradeInv, (*m_pShopItemList)[i], 232, fFirstShopItem_Y + (SHOPITEM_SIZE * i), 337, SHOPITEM_SIZE));
		if (nullptr == m_vecShopItem[i])
		{
			return E_FAIL;
		}
		Add_ChildUI(m_vecShopItem[i]);

		if (i == iRange_ShopItem - 1)
		{
			ShopfButton_Y = fFirstShopItem_Y + (SHOPITEM_SIZE * i) + 10;
			fButtonFonst_Y = ShopfButton_Y + 28;
			//fButtonFonst_Y = fFirstShopItem_Y + (SHOPITEM_SIZE * i) + SHOPITEM_SIZE+ 10;
		}
	}
	Add_ChildUI_Group(&m_vecShopItem);



	// Coin Pile
	// [우채] : 추후 X, Y 위치값 상대 위치로 변경!
	pUI_InputCheck = DecorIMG::Create(m_pGraphicDev, L"Proto_CoinPile_Texture", 520, fButtonFonst_Y+5, SHOPITEM_SIZE, SHOPITEM_SIZE);
	if (nullptr == pUI_InputCheck)
	{
		return E_FAIL;
	}
	Add_ChildUI(pUI_InputCheck);


	// [우채] : 추후 X, Y 위치값 상대 위치로 변경!
	
	strProtoKeyVec.clear();
	strProtoKeyVec.push_back(L"Proto_DefaultButton_Up_Texture");
	strProtoKeyVec.push_back(L"Proto_DefaultButton_Down_Texture");

	pUI_InputCheck = UI_Button::Create(m_pGraphicDev, UBT_CLOSE, strProtoKeyVec, 201, ShopfButton_Y - 23, 128, 128);
	m_pClose_Button = pUI_InputCheck;
	m_vecShopButton.push_back(pUI_InputCheck);
	if (nullptr == m_vecShopButton[0])
	{
		return E_FAIL;
	}
	Add_ChildUI(m_vecShopButton[0]);
	Add_ChildUI_Group(&m_vecShopButton);
	
	return S_OK;
}

void UI_ShopBG::Free()
{
	UI::Free();
}


void UI_ShopBG::Key_Input(const _float& fTimeDelta)
{
	if (Engine::Key_Down('F'))
	{
		UIManager::GetInstance()->Return_UI();

		UIManager::GetInstance()->SelectDelete_UI(L"UI_ShopBG");
	}
}