#pragma once
#include "UI.h"

class HUD_HpBar : public UI
{
	explicit HUD_HpBar(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~HUD_HpBar();

public:
	static HUD_HpBar* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);

public:
	virtual HRESULT Init_UI(const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY);

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

private:
	HRESULT Add_Component();
	HRESULT Add_UI();

private:
	virtual void Free();

private:
	GameObject* pPlayer = nullptr;
	//D3DXCOLOR m_tColor = D3DXCOLOR(0.501960814f, 0.501960814f, 0.501960814f, 1.f);
	D3DXCOLOR m_tColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	wstring m_strCurHP_Status;
	_int m_iPlayer_MaxHp = 0;
	_int m_iPlayer_CurHp = 0;
};

