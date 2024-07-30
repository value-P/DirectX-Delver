#include "stdafx.h"
#include "UI_ShopCatalogue.h"
#include "TradeInv.h"
#include "ItemBase.h"
#include "MouseSlot.h"
#include "InventoryBase.h"
UI_ShopCatalogue::UI_ShopCatalogue(LPDIRECT3DDEVICE9 _pGraphicDevice)
	: UI(_pGraphicDevice)
{
}

UI_ShopCatalogue::~UI_ShopCatalogue()
{
}

UI_ShopCatalogue* UI_ShopCatalogue::Create(LPDIRECT3DDEVICE9 _pGraphicDevice, vector<wstring>& _strProtoKeyVec, TradeInv* _pTradeInv, ItemBase* _pItem, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY)
{
	UI_ShopCatalogue* pInstance = new UI_ShopCatalogue(_pGraphicDevice);

	if (FAILED(pInstance->Init_UI(_strProtoKeyVec, _pTradeInv, _pItem, _fX, _fY, _fSizeX, _fSizeY)))
	{
		Safe_Release(pInstance);
		MessageBox(nullptr, L"Init_UI Failed", L"ExampleChild::Create", MB_OK);
		return nullptr;
	}
	//pInstance->Set_ItemInfo(_tItem);
	return pInstance;
}

HRESULT UI_ShopCatalogue::Init_UI(vector<wstring>& _strProtoKeyVec, TradeInv* _pTradeInv, ItemBase* _pItem, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY)
{
	UI::Init_UI(_fX, _fY, _fSizeX, _fSizeY);

	//m_eUITYPE = UT_SLOT;
	m_strProtoKeyVec = _strProtoKeyVec;

	m_pTradeInv = _pTradeInv;
	m_pShopItem = _pItem;
	
	m_iPlayerMoney = m_pTradeInv->Get_PlayerMoney();

	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	if (FAILED(Add_UI()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int UI_ShopCatalogue::Update_UI(const _float& fTimeDelta)
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	
	UI::Update_UI(fTimeDelta);
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
	return 0;
}

void UI_ShopCatalogue::LateUpdate_UI()
{
	// 자식 UI의 아이콘이 업데이트 되기전에 해줘야할 업데이트는 위에
	UI::LateUpdate_UI();
	// 자식 UI의 아이콘이 업데이트 된후에 해줘야할 업데이트는 아래에
}

void UI_ShopCatalogue::Render_UI()
{
	// 이 클래스가 아이콘으로 표현될때 업데이트 해줘야할게 있다면 여기에
}

void UI_ShopCatalogue::Update_Icon(const _float& fTimeDelta)
{
	if (m_bIsClicking)
	{
		Safe_Release(m_pTextureIcon);
		m_pTextureIcon = dynamic_cast<Texture*>(Clone_Proto(m_strProtoKeyVec[0]));
	}
	else
	{
		Safe_Release(m_pTextureIcon);
		m_pTextureIcon = dynamic_cast<Texture*>(Clone_Proto(m_strProtoKeyVec[1]));
	}

	
	

	m_iPlayerMoney = m_pTradeInv->Get_PlayerMoney();

	if (m_iPlayerMoney < m_pShopItem->Get_ItemPrice())
	{
		if (m_bIsClicking) // 마우스 ON 강빨
		{
			m_tItemNameColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			m_tPriceColor = D3DXCOLOR(1.f, 0.600000024f, 0.600000024f, 1.f); // 강빨
		}
		else // 마우스 OFF 약빨
		{
			m_tItemNameColor = D3DXCOLOR(0.600000024f, 0.600000024f, 0.600000024f, 1.f);
			m_tPriceColor = D3DXCOLOR(0.654901981f, 0.411764711f, 0.411764711f, 1.f); // 약빨
		}
	}
	else
	{
		
		if (m_bIsClicking) // 마우스 ON 강녹
		{
			m_tItemNameColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			m_tPriceColor = D3DXCOLOR(0.600000024f, 1.f, 0.600000024f, 1.f); // 강녹
		}
		else // 마우스 OFF 약녹
		{
			m_tItemNameColor = D3DXCOLOR(0.600000024f, 0.600000024f, 0.600000024f, 1.f);
			m_tPriceColor = D3DXCOLOR(0.411764711f, 0.650980413f, 0.411764711f, 1.f); // 약녹
		}
	}
		
		
}

void UI_ShopCatalogue::LateUpdate_Icon()
{
	m_bIsClicking = false;
}

void UI_ShopCatalogue::Render_Icon()
{
	UI::Render_Icon();
	
	Engine::Render_Font(L"Font_RoundShape", m_pShopItem->Get_ItemName(), &_vec4(m_fX + 21, m_fY, m_fX + 150, m_fY + 16), DT_NOCLIP | DT_SINGLELINE | DT_VCENTER, m_tItemNameColor);
	Engine::Render_Font(L"Font_RoundShape_Number", to_wstring(m_pShopItem->Get_ItemPrice()), &_vec4(m_fX + 266, m_fY, m_fX + 338, m_fY + 16), DT_NOCLIP | DT_SINGLELINE | DT_VCENTER | DT_RIGHT, m_tPriceColor);
}

void UI_ShopCatalogue::OnIconClicking(_uint index)
{
	MouseSlot* mouseSlot = dynamic_cast<MouseSlot*>(Engine::Get_Component(L"GameObject", L"Mouse", L"Proto_MouseSlot", ID_DYNAMIC));
	bool bIsMouseSlot_Empty = mouseSlot->IsSlotEmpty();
	m_bIsClicking = true;

	if (Engine::Get_DIMouseState(DIM_LB) & 0x80) // 1) 왼쪽 마우스 클릭 상태!
	{
		// 마우스 슬롯 상태 구분
		if (bIsMouseSlot_Empty) // ① 현재 마우스 슬롯이 비어있다면!
		{
			if (m_bMouse_Lock == false)
			{
				m_bMouse_Lock = true;
				if (m_pShopItem->Get_ItemPrice() <= m_pTradeInv->Get_PlayerMoney())
				{
					m_iPlayerMoney = m_pTradeInv->Get_PlayerMoney() - m_pShopItem->Get_ItemPrice();
					m_pTradeInv->Set_PlayerMoney(m_iPlayerMoney);
					
					InventoryBase* inventory = dynamic_cast<InventoryBase*>(Engine::Get_Component(L"GameObject", L"Player", L"Proto_PlayerInventory", ID_DYNAMIC));
					inventory->AddItem(m_pTradeInv->MoveItemFromTradeInv(m_pShopItem));
				}
			}
		}

		else // ② 현재 마우스 슬롯이 차있다면!
		{
			;
		}
	}

	else if (Engine::Get_DIMouseState(DIM_RB) & 0x80) // 2) 오른쪽 마우스 클릭 상태!
	{
		; // 없는 기능입니다.
	}

	else // 3) 마우스 클릭이 없는 상태!
	{
		// 마우스 슬롯 상태 구분
		if (bIsMouseSlot_Empty)
			if (m_bMouse_Lock == true)
			{
				m_bMouse_Lock = false;
			}
			else // ② 현재 마우스 슬롯이 차있다면!
			{
				;
			}
	}
}

void UI_ShopCatalogue::OnMouseClicked(_uint index)
{
	// 이 클래스가 부모로서 표현될때 클릭했을때 불릴게 있다면 여기에
	UI::OnMouseClicked(index);
}

void UI_ShopCatalogue::OnIconClicked(_uint index)
{

}

HRESULT UI_ShopCatalogue::Add_Component()
{
	Component* pComponent = Clone_Proto(L"Proto_RcTex");
	if (nullptr == pComponent) { return E_FAIL; }
	m_pBufferCom = dynamic_cast<RcTex*>(pComponent);

	pComponent = Clone_Proto(m_strProtoKeyVec[1]);
	if (nullptr == pComponent) { return E_FAIL; }
	m_pTextureIcon = dynamic_cast<Texture*>(pComponent);

	return S_OK;
}

HRESULT UI_ShopCatalogue::Add_UI()
{
	return S_OK;
}

void UI_ShopCatalogue::Free()
{
	UI::Free();
}
