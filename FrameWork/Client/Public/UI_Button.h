#pragma once
#include "UI.h"

class UI_Button : public UI
{
	explicit UI_Button(LPDIRECT3DDEVICE9 _pGraphicDevice);
	virtual ~UI_Button();

public:
	static UI_Button* Create(LPDIRECT3DDEVICE9 _pGraphicDevice, UIBUTTON_TYPE _eUIButton_Type, vector<wstring>& _strProtoKeyVec, const _float& _fX, const _float& _fY, const _float& _fSizeX, const _float& _fSizeY);

public:
	virtual HRESULT Init_UI(UIBUTTON_TYPE _eUIButton_Type, vector<wstring>& _strProtoKeyVec, const float& _fX, const float& _fY, const float& _fSizeX, const float& _fSizeY);

public:
	virtual _int Update_UI(const _float& fTimeDelta);
	virtual void LateUpdate_UI();
	virtual void Render_UI();

public:
	UIBUTTON_TYPE Get_UIButton_Type() { return m_eUIButton_Type; }
	bool Is_ButtonClicked() { return m_bIs_ButtonClicked; }

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
	vector<wstring> m_strProtoKeyVec;
	UIBUTTON_TYPE m_eUIButton_Type = UBT_END;

	bool m_bIs_ButtonClicked = false;
};

