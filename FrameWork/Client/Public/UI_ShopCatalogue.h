#pragma once
#include "UI.h"

class ItemBase;
class TradeInv;

class UI_ShopCatalogue : public UI
{
protected:
	explicit UI_ShopCatalogue(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~UI_ShopCatalogue();

public:
	static UI_ShopCatalogue* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, vector<wstring>& _strProtoKeyVec, TradeInv* _pTradeInv, ItemBase* _pItem, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);

public:
	virtual HRESULT Init_UI(vector<wstring>& _strProtoKeyVec, TradeInv* _pTradeInv, ItemBase* _pItem, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY);

public:
	virtual _int Update_UI(const _float& fTimeDelta);
	virtual void LateUpdate_UI();
	virtual void Render_UI();

protected:
	virtual void Update_Icon(const _float& fTimeDelta);
	virtual void LateUpdate_Icon();
	virtual void Render_Icon();

protected:
	virtual void OnMouseClicked(_uint index);
	virtual void OnIconClicked(_uint index);
	virtual void OnIconClicking(_uint index);

public:
	//void Set_ItemInfo(ITEM tItemInfo) { m_tItemInfo = tItemInfo; }

private:
	HRESULT Add_Component();
	HRESULT Add_UI();

private:
	virtual void Free();
	
private:
	TradeInv* m_pTradeInv = nullptr;
	ItemBase* m_pShopItem = nullptr;
	_int m_iPlayerMoney;
	D3DXCOLOR m_tItemNameColor;
	D3DXCOLOR m_tPriceColor;
	vector<wstring> m_strProtoKeyVec;

	bool m_bIsClicking = false;
	bool m_bMouse_Lock = false;
};

