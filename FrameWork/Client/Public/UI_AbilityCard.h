#pragma once
#include "UI.h"
class UI_AbilityCard : public UI
{
protected:
	explicit UI_AbilityCard(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~UI_AbilityCard();

public:
	static UI_AbilityCard* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, _int _iIdx_AbilityCard, AbilityCard_ID _eAbilityCard_ID, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);

public:
	virtual HRESULT Init_UI(_int _iIdx_AbilityCard, AbilityCard_ID _eAbilityCard_ID, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY);

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

	AbilityCard_ID m_eAbilityCard_ID = ACI_END;

	GameObject* pPlayer = nullptr;

	_float m_fSpeed = 10.f;

	_float m_fFirstDest_X = -254.5f;
	_float m_fLastDest_X;
	_float m_fOrigin_Y = -5.5f;
	_float m_fDest_Y = 7.5f;

	_int m_iCurStat = 0;

	wstring m_strProtoKey;
	wstring m_strName_AbilityCard;
	wstring m_strExplain_AbilityCard;
	wstring m_strCurStatTag;

	
	_int m_iIdx_AbilityCard = -1;

	bool m_bIs_X_FirstDest = false;
	bool m_bIs_X_LastDest = false;
	
	bool m_bIsClicking = false;
	bool m_bMouse_Lock = false;
	bool m_bShow_Explain_AbilityCard = false;
};

