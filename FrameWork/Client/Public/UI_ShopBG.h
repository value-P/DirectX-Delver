#pragma once
#include "UI.h"

class TradeInv;
class ItemBase;

class UI_ShopBG : public UI
{
	protected:
		explicit UI_ShopBG(LPDIRECT3DDEVICE9 _pGraphicDevice);
		virtual ~UI_ShopBG();

	public:
		static UI_ShopBG* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, TradeInv* _pTradeInv, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);
	public:
		virtual HRESULT Init_UI(TradeInv* _pTradeInv, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY);

	public:
		virtual _int Update_UI(const _float & fTimeDelta);
		virtual void LateUpdate_UI();
		virtual void Render_UI();

	protected:
		virtual void Update_Icon(const _float & fTimeDelta);
		virtual void LateUpdate_Icon();
		virtual void Render_Icon();

	protected:
		virtual void OnMouseClicked(_uint index);
		virtual void OnIconClicked(_uint index);
		virtual void OnIconClicking(_uint index);

	private:
		HRESULT Add_Component();
		HRESULT Add_UI();

	private:
		void Key_Input(const _float & fTimeDelta);

	private:
		virtual void Free();

	private:
		_int m_iPlayerMoney;

		TradeInv* m_pTradeInv = nullptr;
		vector<ItemBase*>* m_pShopItemList = nullptr;

		vector<UI*> m_vecShopItem;
		vector<UI*> m_vecShopButton;
		UI* m_pClose_Button = nullptr;

		float fButtonFonst_Y;
};